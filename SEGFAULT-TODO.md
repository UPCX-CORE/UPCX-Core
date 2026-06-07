# TODO: investigate nodupcx SIGSEGV under RPC/transaction load

**Status:** open — needs investigation by the core team.

## Summary
A `nodupcx` built from the `develop` branch (commit `7f254b4`, reported version
`v2.1.0-7f254b4ec87012669680404f296a27b739ee1f5d-dirty`) **segfaults under RPC /
transaction load**. Production runs the older, stable commit
`62f809344c6a90d34841ddc1145b3e8689ef9a8b`, which does not exhibit this. This
implies a regression introduced on `develop` **after** `62f8093`.

## Symptom
- The node boots and produces blocks normally while **idle**.
- It **crashes within seconds** once it starts handling API/transaction load —
  reproduced twice during a standard bios-boot sequence, each time right after
  `POST /v1/producer/schedule_protocol_feature_activations` followed by the first
  `setcode` (deploying `upcx.boot`).
- Crash is a SIGSEGV inside libc (a bad pointer handed to libc — heap corruption
  or use-after-free), not an illegal instruction:

  ```
  nodupcx[<pid>]: segfault at 7f38fcec7df0 ip 00007f3efe6819a8 sp 00007ffc99a7a4e8 \
      error 4 in libc-2.31.so[7f3efe518000+178000]
  /usr/local/bin/nodupcx-run.sh: line 25:  <pid> Segmentation fault (core dumped) ...
  ```
  `error 4` = user-mode read of an unmapped page.

## Environment
- AWS EC2 `t3a.large` (AMD EPYC 7571: avx, avx2, bmi1/2, sse4_1/2; **no avx512**),
  Ubuntu 20.04 (focal), glibc 2.31, run under systemd as user `ubuntu`.
- Built via `scripts/upcx_build.sh` (system gcc-9 / llvm-7, non-pinned, default
  Release; **no `-march=native`** in the build, so not a CPU-instruction mismatch).
- **Not reproduced** in local Docker `ubuntu:20.04` containers running the same
  `.deb` (incl. with `--cpus=2 --memory=8g`) under light load — only on the EC2
  host under bootstrap load.

## Suggested next steps
1. `git log 62f809344c6a90d34841ddc1145b3e8689ef9a8b..7f254b4` — review changes
   landed on `develop` after the production commit; bisect for the regression.
2. Build `RelWithDebInfo` (symbols) and capture a backtrace via
   `coredumpctl gdb nodupcx` / the core dump to pin the faulting call site
   (likely in the chain_api / producer_api / net request path).
3. Run under ASan/Valgrind against the bios-boot + `setcode` sequence to surface
   the heap corruption / use-after-free.

## Workaround in use
The IaC testnet pins the chain AMI to a `.deb` built from the production commit
`62f8093` (see `upcx-iac/packer`). The chain AMI also ships a health watchdog
(`nodupcx-watchdog.timer`) and dirty-DB auto-replay (`nodupcx-run.sh`) that
recover the node if it exits uncleanly.
