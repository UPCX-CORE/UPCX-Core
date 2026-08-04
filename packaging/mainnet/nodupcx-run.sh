#!/usr/bin/env bash
# Wrapper around nodupcx for the mainnet producer nodes. Assembles the command line
# from /etc/default/nodupcx, refuses to bootstrap a new chain by accident, and
# recovers a "dirty" chainbase left by an unclean shutdown.
#
# Peers and the block-signing key come from config.ini in NODUPCX_CONFIG_DIR; this
# script never writes that file.
set -uo pipefail

# shellcheck disable=SC1091
[[ -r /etc/default/nodupcx ]] && . /etc/default/nodupcx

BIN_DIR="${NODUPCX_BIN_DIR:-/home/ubuntu/UPCX-Core/build/bin}"
DATA_DIR="${NODUPCX_DATA_DIR:-/root/.local/share/upcx/nodupcx/data}"
CONFIG_DIR="${NODUPCX_CONFIG_DIR:-/root/.local/share/upcx/nodupcx/config}"
PRODUCER_NAME="${NODUPCX_PRODUCER_NAME:-}"
PEERS="${NODUPCX_P2P_PEERS:-}"
HTTP_ADDR="${NODUPCX_HTTP_SERVER_ADDRESS:-0.0.0.0:8888}"
EXTRA_ARGS="${NODUPCX_EXTRA_ARGS:-}"
AUTO_REPLAY="${NODUPCX_AUTO_REPLAY:-1}"
REPLAY_COOLDOWN_SEC="${NODUPCX_REPLAY_COOLDOWN_SEC:-21600}"
ALLOW_FRESH_CHAIN="${NODUPCX_ALLOW_FRESH_CHAIN:-0}"

BIN="${BIN_DIR}/nodupcx"
BLOCKS_LOG="${DATA_DIR}/blocks/blocks.log"
REPLAY_STAMP="${DATA_DIR}/.last-auto-replay"

die() { echo "nodupcx-run: $*" >&2; exit 1; }

[[ -x "${BIN}" ]] || die "${BIN} is not executable"
[[ -n "${PRODUCER_NAME}" ]] || die "NODUPCX_PRODUCER_NAME is unset; each node signs under its own producer name"

# An empty data dir makes nodupcx silently bootstrap a fresh chain from the compiled-in
# default genesis and begin a multi-day sync from block 1. On a production producer that
# is always a mistake, so it has to be asked for.
if [[ ! -e "${BLOCKS_LOG}" && ! -e "${DATA_DIR}/state/shared_memory.bin" ]]; then
  [[ "${ALLOW_FRESH_CHAIN}" == "1" ]] \
    || die "no chain state under ${DATA_DIR}; restore a snapshot or set NODUPCX_ALLOW_FRESH_CHAIN=1"
fi

[[ -r "${CONFIG_DIR}/config.ini" ]] \
  || echo "nodupcx-run: WARNING no ${CONFIG_DIR}/config.ini — peers and signature-provider may be missing" >&2

ARGS=(
  --data-dir "${DATA_DIR}"
  --config-dir "${CONFIG_DIR}"
  -p "${PRODUCER_NAME}"
  --plugin upcx::chain_plugin
  --plugin upcx::chain_api_plugin
  --plugin upcx::history_api_plugin
  --plugin upcx::net_api_plugin
  --plugin upcx::producer_plugin
  --plugin upcx::producer_api_plugin
  --access-control-allow-origin='*'
  --contracts-console
  --http-validate-host=false
  --verbose-http-errors
  --http-max-response-time-ms 300
  --producer-threads=10
  --http-server-address "${HTTP_ADDR}"
)

# Appended to whatever config.ini already lists; leave NODUPCX_P2P_PEERS empty if the
# peers are configured there, or the node dials each of them twice.
for peer in ${PEERS}; do
  ARGS+=(--p2p-peer-address "${peer}")
done

# Unquoted on purpose: NODUPCX_EXTRA_ARGS is a word list.
# shellcheck disable=SC2206
[[ -n "${EXTRA_ARGS}" ]] && ARGS+=(${EXTRA_ARGS})

"${BIN}" "${ARGS[@]}"
rc=$?
[[ ${rc} -eq 0 ]] && exit 0

# Non-zero exit is most often the chainbase dirty flag after an unclean stop, which needs
# a replay. --hard-replay-blockchain salvages blocks.log and rebuilds all state (plain
# --replay-blockchain can still trip the dirty flag on the reversible DB).
echo "nodupcx-run: exited rc=${rc}" >&2

[[ "${AUTO_REPLAY}" == "1" ]] || exit "${rc}"
[[ -r "${BLOCKS_LOG}" ]] || { echo "nodupcx-run: no blocks.log, not replaying" >&2; exit "${rc}"; }

# A hard replay rebuilds from the start of blocks.log, so it only works when the log runs
# from genesis. A snapshot-restored node has a truncated log and must be restored again.
hdr="$(od -An -tu4 -N8 "${BLOCKS_LOG}" 2>/dev/null | tr -s ' ' | sed 's/^ //;s/ $//')"
log_version="${hdr%% *}"
first_block="${hdr##* }"
[[ "${log_version}" == "1" ]] && first_block=1
if [[ "${first_block}" != "1" ]]; then
  echo "nodupcx-run: blocks.log starts at block ${first_block}, not genesis — restore from a snapshot instead" >&2
  exit "${rc}"
fi

# One replay per cooldown window, so a failure that a replay cannot fix does not put the
# node into a loop of multi-hour replays.
now="$(date +%s)"
last="$(cat "${REPLAY_STAMP}" 2>/dev/null)"
[[ "${last}" =~ ^[0-9]+$ ]] || last=0
if (( now - last < REPLAY_COOLDOWN_SEC )); then
  echo "nodupcx-run: auto-replay already attempted $(( now - last ))s ago, giving up" >&2
  exit "${rc}"
fi
echo "${now}" > "${REPLAY_STAMP}"

# Same ARGS: the replay run continues serving and producing once it catches up, and on
# this fleet the plugins and producer name come from the command line, not config.ini.
echo "nodupcx-run: attempting --hard-replay-blockchain recovery" >&2
exec "${BIN}" "${ARGS[@]}" --hard-replay-blockchain
