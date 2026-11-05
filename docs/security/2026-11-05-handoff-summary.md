# Handoff summary: 2026-11-05 contract authorization sweep

For a parent session consolidating the work. Branch `audit/contract-auth-sweep` of `UPCX-CORE/UPCX-Core`, PR #15 (base `develop`), docs only. Written 2026-11-05 ~05:00 UTC. Rules held throughout: read-only against mainnet, nothing signed or broadcast, no attacker URLs fetched, no secrets read.

## Documents in this PR (all under `docs/security/`)
| File | Purpose |
|---|---|
| `2026-11-05-contract-auth-sweep.md` | Main audit: exploit summary, deployed code vs source, findings A1-J4, per-action auth tables (section 4), work order, fix PR status (6), builds and hash parity (7, 7.1, 7.2) |
| `2026-11-05-deploy-runbook-bridge-msg-lp.md` | Deploy commands for `upcx.msg`, `upcx.lp`, `upcx.bridge`. The bridge deploy (section 3) was run on `myjuczx34urx`; its configuration steps and the other two contracts were not |
| `2026-11-05-evm-contracts-stakeholder-brief.md` | Plain-language brief on the Ethereum and Arbitrum contracts and decisions needed |
| this file | Handoff |

## The incident
Attacker `nfa.test` (with `nfa.test.a`, `.b`) abused missing `require_auth` on `update` and `settle` of the MPA token contract on `paya.token` and `demo.token` (16 and 3 `update` calls, 1 `settle` each; payloads were external URLs, `javascript:` and `data:` URIs). Nothing else on chain was touched.

## Closed on chain (verified by RPC)
- `paya.token` and `demo.token` both run wasm `542c73ef7891521fedb636f5ceae2855d812a37bd972ec8c6db2ca97c354a182` (redeployed 03:32 UTC). It is `UPCX-MPA-SmartContract` main at #4: the auth fix (`34f5b393…`, first deployed 00:04 UTC) plus `increase` needing `upcx.token` and `removebalance` limited to whitelisted tokens.
- Their active and owner keys were rotated 01:30-01:34 UTC. The owner confirmed this was expected.
- `upcx.token` was never affected; it runs `30b5740c…` and reproduces from source.
- The ETH bridge `myjuczx34urx` was upgraded on 2026-11-05 (tx `cf7e3b5d…`) from `17e69dc0…` to `01e2ec0e…` (`upcx.bridge` @3258a98). State and the 4,119.30144 UPC balance were unchanged. It runs unpaused with no cap, and the relayer change (Bridge-Backend #13, #14) is not yet shipped.
- `nfa.test.a` and `nfa.test.b` had their code and ABI cleared the same day (attacker wasm `cc952a6d…` archived).

## Fixed in source, merged, not on mainnet (bridge excepted)
| Contract | Findings | Repo PRs | Final main hash (wasm sha256, CDT image) |
|---|---|---|---|
| `voting` | C1-C4, deposit redesign, 1 UPC minimum | UPCX-SmartContract #5, #6, #8, #9 | `4f8a8010d1710f986fc9ceb27c8f6d101d1614fc92b8469bd67a7bd1d76508af` |
| `upcx.mta` | D1, D2 | #5, #6 | `640225bc7398fc4127f77b32933a51f7f3f80891d46e6cf1f12d656b4b4d15ae` |
| `upcx.system` | F1-F5 | #4, #7 | `3e3801bde90427bb1cdca875c92527d1fc2a7f65ebd75ad24e58d7341dc5443c` |
| `upcx.evm` | G1 (TESTING off build only) | #5, #6 | `ef8c035981536fc3274f29f316c891f8fcb0f417d6bde8318f2f17cf802d788b` |
| `upcx.msg` | E1 (E2 caps dropped by decision) | Messager #2, #3 | `3cb1d39300bed30642e4d9f32cb3c8afa43c1ec6c275e29fb0a83acbbbbb0a42` |
| `upcx.lp` | H1, H2 | DEX #19, #20 | `2dc4e947e7aef465468765743efc4fbb602576f44a0e38bb985d04b8c698fbfd` |
| `upcx.bridge` | I1, I2 | Bridge-Contract-UPCX #2 | `01e2ec0e72533a48d40fbb556a6d767ef6536ed8d483cd75499a43a66d96726e` (deployed on `myjuczx34urx`) |
| MPA `upcx.token` / `upcx.mpa` | A1-A4 | MPA #3, #4, #5 | `542c73ef…` / `90a27b920956f284fc68b3e35beb8ecf86d4e45e95571ed180554be02e473c4c` |
| EVM | B1, J1-J4 | Stake-B #4, #5, #6; Arbitrum #1; Bridge-Contract-ETH #7; Stake-SmartContract #1 | Hardhat, not wasm |
| Backend, frontend | relayer handling, Helm flag | Bridge-Backend #13, #14; Bridge-Frontend #3 | Frontend #3 test never ran (no CodeArtifact token) |

Test evidence on throwaway `nodupcx` (chain-node image, deployed-hash `upcx.token`): voting 52/52 with the minimum, `upcx.mta` 14/14, system 35/35 (pre-fix build reproduces F1 variants and the F4 collision), `upcx.bridge` 83/83 (16 fail on a deliberately broken build), `upcx.msg` fixed vs vulnerable side by side, lp harness (not run against the pre-fix wasm). Arbitrum 31 Hardhat tests.

## Key facts learned
- Live `upcx` (system) = source commit `35d2e90` (2025-02-18) plus the `60318fd` build-file shim, hash `4daa36fa1f6c4a6afed05c1b1a21e5abf2286fb89b1cc0ffb5a4326a93ea6cb8`. Main is that plus about nine months of undeployed changes plus F1-F5, so a system deploy is a large review, not a patch. Live ABI has no `bidclaim` table (21 tables, 61 actions), so there is nothing stranded to dump.
- F1 is present in the live system code (`enterbid` escrows nothing; `claimbid` pays from `upcx.names`). Impact today is negligible: `upcx.names` holds 0.00864 UPC and `namebids` is empty. Not reproduced on a chain; the session task was blocked by a safeguard prompt. Deferred by decision.
- Chain behaviour that cost time: every non-system tx needs an `upcx.token::fee` action (put `upcx@active` first to be exempt); deploys are charged by size; inline actions inherit authority only for self-sends, so depositors must grant the contract's `upcx.code` permission; code permission is `upcx.code`, not `eosio.code`.
- `voting::deposit` is now an ABI break (send `upcx.token::transfer` to `voting`). No client in any tracked repo calls it. Whether a voting UI exists elsewhere is unknown; voting deploy is deferred.
- EVM: 3 of 6 Stake-B proxies are upgradable by one EOA; the other 3 and the Stake-SmartContract proxy have renounced admins and cannot take any fix. ETH bridge proxy upgradable by a different EOA. Stake-B proxy `0x14e4…cf51` runs implementation `0x1a43…a3da`, which is unreviewed.

## Pending, in suggested order
1. Bridge configuration on `myjuczx34urx`: choose and set the withdrawal cap (past withdrawals: median about 10 UPC, max 2,785 UPC), ship the relayer, then `setreqsrc`, then run the runbook's small tests. Confirm the live bridge frontend sends valid `0x`+40-hex destinations, since the new contract rejects anything else.
2. `upcx.msg`, `upcx.lp`, `upcx.mta`, `voting`, `upcx.evm` and `upcx.mpa` are not on mainnet, so there is nothing to deploy; their fixes ship at first launch. Deferred by the owner.
3. Stakeholder decisions in the EVM brief: multisig/timelock for the owner EOAs (J2), migrate or accept the non-upgradable staking copies, authorize upgrades, approve the Arbitrum redeploy and fund-migration plan (payouts then need two approvers; backend rework required), review of `0x1a43…a3da`.
4. `upcx.system`: decide whether to deploy main (review `git diff 35d2e90 origin/main -- contracts/upcx.system`) or keep a stopgap (keep `upcx.names` unfunded, no auctions). F1 deferred.
5. Smaller: `upcx.evm` TESTING=ON host tests never run; `upcx.mpa` deployed hash not compared; vulnerable-token scan needs an indexer; Frontend #3 test; six stale Stake-B tests failing on main; MPA findings A3, A5, A6 open or accepted.
6. Not verified: how much value sits in each EVM contract; the state of `zzjtestzz` (`get_account` errors).

## Sessions and housekeeping
- Nine `aoe` sessions ("Auth fix: ...") did the fixes; all PRs they opened are merged and the sessions are archived (`aoe session archive`; unarchive with `aoe session unarchive <id>`). They also sit in an `Archive` group I created earlier. Two (ETH bridge, Stake-SmartContract) point at worktrees their agents deleted.
- Scratch material (builds, RPC dumps, harnesses) lives under `/tmp/claude-1000/-home-dandy-github-com-upcx-core-UPCX-Core-worktrees-contract-auth-audit/4f4572c0-bcc9-4381-a2fa-65a469051855/scratchpad/` (`final/`, `parity/`, `chain/`, `run_final.sh`, `build_in_cdt.sh`). It is temporary.
- Unsent text found in several session inputs (delete branches, remove worktrees, merge PRs) was never typed by me and was cleared or ignored, not acted on.
