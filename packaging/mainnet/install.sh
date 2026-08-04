#!/usr/bin/env bash
# Install the nodupcx systemd units on a mainnet node. Enables but does not start
# anything — starting a producer is an operator decision.
set -euo pipefail

SRC="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

[[ ${EUID} -eq 0 ]] || { echo "run as root" >&2; exit 1; }

install -m 0755 "${SRC}/nodupcx-run.sh"      /usr/local/bin/nodupcx-run.sh
install -m 0755 "${SRC}/nodupcx-watchdog.sh" /usr/local/bin/nodupcx-watchdog.sh

for unit in nodupcx.service nodupcx-watchdog.service nodupcx-watchdog.timer; do
  install -m 0644 "${SRC}/${unit}" "/etc/systemd/system/${unit}"
done

# Never overwrite an existing per-node config.
if [[ ! -e /etc/default/nodupcx ]]; then
  install -m 0640 "${SRC}/nodupcx.env.example" /etc/default/nodupcx
  echo "wrote /etc/default/nodupcx — set NODUPCX_PRODUCER_NAME before starting"
fi

systemctl daemon-reload
systemctl enable nodupcx.service nodupcx-watchdog.timer

cat <<'EOF'

Installed. Next:
  1. edit /etc/default/nodupcx (producer name, peers)
  2. stop any hand-started nodupcx cleanly (SIGTERM, wait for exit) so the
     chainbase is flushed and the unit does not inherit a dirty DB
  3. systemctl start nodupcx && journalctl -fu nodupcx
  4. curl -s localhost:8888/v1/chain/get_info  # head_block_num advancing
EOF
