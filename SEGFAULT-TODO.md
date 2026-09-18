# TODO: investigate nodupcx SIGSEGV under RPC/transaction load

**Status:** open, escalated — live mainnet incident (2026-09-18). All 7 shard-a
producers (`UPCX_Main1-7`, ap-northeast-1) independently segfaulted (rc=139,
SIGSEGV in libc) within ~10h of sustained RPC/tx load, each triggering a
12h+ `--hard-replay-blockchain`. Confirmed via SSM journalctl:
`nodupcx-run.sh: ... Segmentation fault (core dumped)`. Mainnet runs commit
`e7ac36b8460f6d9165e0a8012e1407d1c625f33d` — **not** the `7f254b4` build
originally implicated below. See "Bisection findings" for why this matters.

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

## Bisection findings (2026-09-18)

**Scope note:** a real `git bisect` (build RelWithDebInfo at each candidate,
reproduce under load, capture a backtrace) was not achievable in this
sandboxed worktree — there's no EC2-class host or production-like RPC/tx
generator available here, and this doc's own reproduction notes say the crash
does **not** manifest in local Docker even under light load, only on a real
EC2 host under real load. A rebuild-and-run here would not have reproduced
anything to bisect against. Instead this pass is a full commit-by-commit code
review of `62f809344c6a90d34841ddc1145b3e8689ef9a8b..7f254b4ec87012669680404f296a27b739ee1f5d`
(12 commits). Dynamic confirmation (coredumpctl/gdb backtrace) is still the
required next step — see below.

### The range, oldest to newest

```
c99642c  update: upcx.system contract updated       (contract .wasm/.abi binary bump)
0148195  update: name inner db changed to id         (native, upcx_contract.cpp)
1282603  update: tx action list api added to history api
926eca2  update: history by account detailed
c1cf51a  fix: syntax error
81cbea0  fix: invalid indexing
00cc626  update: revert current code
e7ac36b  fix: syntax error                            <-- mainnet's current build
2b50c26  update: oracle added as a free transaction
b94ee7c  update: net usage check banned
c6c0eea  update: CPU check banned
7f254b4  fix: remove assert for resource checking     <-- originally-implicated build
```

### Finding 1 — the history_plugin churn (1282603..e7ac36b) is a functional no-op

`git diff c99642c e7ac36b -- plugins/history_plugin/ plugins/history_api_plugin/`
shows **zero** net change beyond two blank lines. Commits `1282603` and
`926eca2` added a tx-action-list/by-account history API (+~80 lines);
`c1cf51a`/`81cbea0` were same-day hotfixes to it; `00cc626` ("revert current
code") and `e7ac36b` ("fix: syntax error") then removed all of it again. So
functionally, **mainnet's `e7ac36b` == `62f8093` plus exactly two changes**:
`c99642c` (contract binary) and `0148195` (one line in `upcx_contract.cpp`).

### Finding 2 — those two remaining changes don't show an obvious memory-safety bug

- `c99642c` only touches the compiled `upcx.system` contract `.wasm`/`.abi`
  (no native source in this repo for that contract). It runs sandboxed inside
  the WASM VM; nothing in this diff touches the wasm interface/intrinsics, so
  it's a weak candidate for a native libc-level SIGSEGV.
- `0148195` changes `apply_upcx_newaccount` in `libraries/chain/upcx_contract.cpp`
  to store `name(create.id).to_string()` (canonical, ≤12-char, derived from the
  already-uniqueness-checked account id) into `account_object::real_name`
  instead of the raw user-supplied `create.name` string. `real_name` is
  indexed by `ordered_unique<by_real_name>` in `undo_index` — checked
  `undo_index::emplace()` in `libraries/chainbase/include/chainbase/undo_index.hpp`
  and confirmed it throws `std::logic_error` cleanly on a uniqueness-constraint
  violation (strong exception safety), not UB — so a real_name collision, old
  or new code, cannot itself produce a segfault. The new value is actually
  *more* collision-resistant than the old (derived from an already-unique key
  vs. arbitrary user input). No dangling-reference issue either: the lambda
  passed to `db.create()` runs synchronously in the same call.

**Net effect: nothing in `62f8093..e7ac36b` stood out under code review as a
plausible source of heap corruption.** That's a significant finding in its
own right — see "What this means for mainnet" below.

### Finding 3 — the 4 commits *after* e7ac36b remove real safety checks (best suspect for the originally-reported dev-branch crash)

`git diff e7ac36b 7f254b4 -- libraries/chain/transaction_context.cpp libraries/chain/resource_limits.cpp` :

- `b94ee7c`/`c6c0eea` comment out `check_net_usage()` (both call sites) and
  `validate_cpu_usage_to_bill(...)` in `transaction_context.cpp`.
- `7f254b4` comments out the `UPCX_ASSERT` bounds checks in
  `resource_limits_manager::add_transaction_usage()` for **both** CPU and NET
  usage against `max_user_use_in_window`.
- `2b50c26` adds an unconditional fee bypass for `upcx.oracle` actions.

Commenting out the checks that used to *throw* when a transaction's billed
CPU/NET usage exceeded its account's window limit means usage values that
were previously guaranteed bounded can now flow downstream unbounded. The
originally-reported crash happens specifically right after a `setcode` (a
large, resource-heavy tx) under load — exactly the scenario these checks
existed to gate. This remains the leading, highest-confidence suspect for the
`7f254b4` bootstrap/setcode crash reported earlier in this doc.

### What this means for mainnet

Mainnet runs `e7ac36b`, which per Finding 1/2 **does not contain** the
`2b50c26..7f254b4` assert-removal commits — yet it still segfaulted 7/7
independently under real load. Two explanations, not mutually exclusive:

1. **Two different bugs.** The dev-branch bootstrap/setcode crash (Finding 3)
   and the mainnet sustained-load crash may be separate regressions that
   happen to share a symptom (SIGSEGV inside libc). Avoiding the 4 assert-
   removal commits would fix the former but says nothing about the latter.
2. **The bug (or its trigger conditions) predates `62f8093`.** Since the
   diff between `62f8093` and `e7ac36b` is functionally trivial (Finding 1/2),
   and `62f8093` was only ever validated by shorter testnet bootstraps, not
   ~10h of real mainnet RPC/tx volume, it's plausible `62f8093` carries the
   same latent issue and simply hadn't been exercised long/hard enough to
   surface it. **A plain rollback to `62f8093` is not confirmed to fix the
   live incident** — treat it as the most conservative available option, not
   a proven fix.

### Recommendation

- **Do not build the next mainnet AMI from `7f254b4`** (or any commit at/after
  `2b50c26`) — those commits demonstrably strip resource-usage bounds
  checking and should be avoided regardless of whether they explain the
  mainnet incident.
- Short term, keep mainnet's AMI pinned to `e7ac36b` (current) or `62f8093`
  (most conservative) — neither is proven safe under sustained real load, but
  both are strictly safer than anything from `2b50c26` onward.
- **The next action has to be dynamic, not more code review**: recover the
  core dumps already produced by the 7 crashes (via `coredumpctl` on the
  affected instances once recovery completes, or wherever cores are shipped)
  and get a `gdb` backtrace with symbols. That backtrace is what actually
  distinguishes "same bug as Finding 3" from "a different, older bug" — code
  review alone cannot resolve that with confidence given how small the
  `62f8093..e7ac36b` diff is.
- Once a backtrace points at a specific subsystem, a targeted dynamic bisect
  (RelWithDebInfo + real RPC/tx load, ideally on a staging host matching
  mainnet's instance type/traffic) can confirm the introducing commit — this
  requires infrastructure outside this worktree/session.
