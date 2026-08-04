#!/usr/bin/env bash
# Health watchdog for nodupcx. The node can stall during startup (http_plugin never binds
# its port) — a hang, not a crash, so systemd's Restart=on-failure does not catch it. If
# the unit is active but the chain RPC is not answering after a grace period, restart it.
set -uo pipefail

# shellcheck disable=SC1091
[[ -r /etc/default/nodupcx ]] && . /etc/default/nodupcx

PROBE_URL="${NODUPCX_WATCHDOG_URL:-http://127.0.0.1:8888/v1/chain/get_info}"
# A replay after an unclean stop can run for hours on mainnet with the RPC unbound the
# whole time. Restarting through that would restart it forever.
GRACE_SEC="${NODUPCX_WATCHDOG_GRACE_SEC:-3600}"

systemctl is-active --quiet nodupcx || exit 0   # only act when it should be up

START_TS="$(systemctl show -p ActiveEnterTimestamp --value nodupcx 2>/dev/null)"
START_EPOCH="$(date -d "${START_TS}" +%s 2>/dev/null || echo 0)"
ACTIVE_SEC=$(( $(date +%s) - START_EPOCH ))
(( ACTIVE_SEC < GRACE_SEC )) && exit 0

curl -fsS --max-time 8 "${PROBE_URL}" >/dev/null 2>&1 && exit 0

logger -t nodupcx-watchdog "RPC unresponsive after ${ACTIVE_SEC}s active — restarting nodupcx"
systemctl restart nodupcx
