# Mainnet node packaging

Systemd units for the `UPCX_Main*` producer nodes, which until now ran a hand-started
`./nodupcx` with no supervision: nothing brought them back after a reboot or an unclean
exit.

## Layout

| File | Installed as |
|---|---|
| `nodupcx.service` | `/etc/systemd/system/nodupcx.service` |
| `nodupcx-run.sh` | `/usr/local/bin/nodupcx-run.sh` |
| `nodupcx-watchdog.{service,timer}` | `/etc/systemd/system/` |
| `nodupcx-watchdog.sh` | `/usr/local/bin/nodupcx-watchdog.sh` |
| `nodupcx.env.example` | `/etc/default/nodupcx` (per node, not overwritten) |

```
sudo ./install.sh
sudo vi /etc/default/nodupcx     # NODUPCX_PRODUCER_NAME is required
sudo systemctl start nodupcx
```

## What the run script does

* Builds the command line from `/etc/default/nodupcx`. The plugin set, CORS, HTTP and
  producer-thread flags match the invocation the fleet has been running.
* Passes `--data-dir` / `--config-dir` explicitly rather than relying on `$HOME`, which
  systemd does not always export.
* Refuses to start when the data dir holds no chain state, unless
  `NODUPCX_ALLOW_FRESH_CHAIN=1`. Bare `nodupcx` on an empty data dir bootstraps a new
  chain from the compiled-in genesis and starts a sync from block 1.
* On a non-zero exit, retries once with `--hard-replay-blockchain` — the usual cure for
  the chainbase dirty flag. Guarded: only when `blocks.log` starts at genesis (a
  snapshot-restored node has a truncated log and must be restored again instead), and at
  most once per `NODUPCX_REPLAY_COOLDOWN_SEC`.

`config.ini` is read, never written: the block-signing key and peer list live there and
this packaging does not touch them.

## Deliberate omissions

* **No `-e` / `--enable-stale-production`.** It appears in the fleet's shell history. On
  a live multi-producer chain it lets a lagging node produce off the tip.
* **No genesis handling.** These nodes join an existing chain; `--genesis-json` against
  existing state makes nodupcx refuse to start.
