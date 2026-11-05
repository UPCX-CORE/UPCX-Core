# Deploy runbook: upcx.bridge, upcx.msg, upcx.lp

Draft for the person who holds the mainnet keys. Nothing here has been run against mainnet. Every `<PLACEHOLDER>` must be filled in from a source you trust, and each step should be checked on the testnet first.

## 0. Before you start

| Item | Value |
|---|---|
| RPC | `https://shard-a-node1.upcx.org:9000` (read) |
| Chain CLI | `clupcx -u $RPC` |
| Build image | `032959621317.dkr.ecr.ap-northeast-1.amazonaws.com/upcx-testnet/upcx-cdt:latest` |
| Bridge account | `<BRIDGE_ACCOUNT>` (the `UPCX_BRIDGE_ACCOUNT_NAME` in the bridge backend) |
| Msg account | `<MSG_ACCOUNT>` |
| LP account | `liquidity` is the name used by the DEX frontend (`LIQUIDITY_CONTRACT`); it returned "unspecified" on the public RPC when probed, so confirm the real name |

The mainnet account names for these three were not found in any repository or by RPC probing. Confirm them first.

### Fees
Every non-system transaction on this chain needs a native `upcx.token::fee` action. Contract deploys are charged by size. Put `upcx@active` as the first authorizer so the system exemption applies, as the bridge test showed (a deploy without it was refused with `Required:43037`).

### Reproduce the artifacts
Build from the exact commit and compare hashes before deploying. The wasm below was built from `origin/main` in the CDT image.

| Contract | Repo @ commit | Expected wasm sha256 |
|---|---|---|
| `upcx.bridge` | UPCX-Bridge-Contract-UPCX @3258a98 | `01e2ec0e72533a48d40fbb556a6d767ef6536ed8d483cd75499a43a66d96726e` |
| `upcx.msg` | UPCX-Messager-SmartContract @aa8accf | `3cb1d39300bed30642e4d9f32cb3c8afa43c1ec6c275e29fb0a83acbbbbb0a42` |
| `upcx.lp` | UPCX-DEX-SmartContract @cf7a323 | `2dc4e947e7aef465468765743efc4fbb602576f44a0e38bb985d04b8c698fbfd` |

```bash
sha256sum build/<contract>.wasm   # must equal the table
```
Note the build output is named `<contract>.wasm.wasm` and `<contract>.wasm.abi`; `clupcx set contract` wants the wasm and abi paths directly.

### Record what is live now (rollback material)
```bash
for a in <BRIDGE_ACCOUNT> <MSG_ACCOUNT> <LP_ACCOUNT>; do
  clupcx -u $RPC get code $a -c rollback-$a.wasm -a rollback-$a.abi
  sha256sum rollback-$a.wasm
done
```
Keep these files. Redeploying them is the rollback.

## 1. upcx.msg (smallest change, do first)

Change: `update` now finds the right channel. No table or ABI change.

```bash
# 1. dry run: confirm the account and its current hash
clupcx -u $RPC get code <MSG_ACCOUNT>

# 2. deploy the wasm only (abi unchanged, sha256 d9894bb0...)
clupcx -u $RPC set code <MSG_ACCOUNT> upcx.msg.wasm.wasm \
   -p upcx@active -p <MSG_ACCOUNT>@active

# 3. verify
clupcx -u $RPC get code <MSG_ACCOUNT>     # must print the expected sha256
```
Do not deploy the capped build `fc6a64f9...` from the earlier PR.

Rollback: `set code` the saved `rollback-<MSG_ACCOUNT>.wasm`.

## 2. upcx.lp (ABI change: token-contract whitelist)

Change: `initpool` needs `creator` authority and only accepts token contracts on a whitelist. The ABI gains `addtokcont`, `rmtokcont`, `addcreator`, `rmcreator`.

```bash
# 1. deploy wasm and abi
clupcx -u $RPC set contract <LP_ACCOUNT> ./lp-build upcx.lp.wasm upcx.lp.abi \
   -p upcx@active -p <LP_ACCOUNT>@active
#   or: set code, then set abi, with the same -p list

# 2. whitelist the token contracts that pools may use
clupcx -u $RPC push action <LP_ACCOUNT> addtokcont '["upcx.token"]' -p <LP_ACCOUNT>@active
#   add every other token contract that existing pools use, then verify:
clupcx -u $RPC get table <LP_ACCOUNT> <LP_ACCOUNT> tokencont

# 3. add pool creators that should be allowed
clupcx -u $RPC push action <LP_ACCOUNT> addcreator '["<CREATOR>"]' -p <LP_ACCOUNT>@active
```
Before step 1: list the token contracts in use by existing pools (the `pool` table) so step 2 does not block live pools. The harness did not check how existing pools are affected, so read the table first.

Each push action also needs the fee action; if `clupcx push action` is refused with "Transaction Fee is not enough", build the transaction with `clupcx push transaction`, as the system session did.

Rollback: redeploy the saved wasm and abi. The whitelist tables are then unused.

## 3. upcx.bridge (largest change, needs the relayer first)

Change: pause switch, per-transaction cap, deposit records, source-bound replay protection (`withdrawsrc`). The wasm is 63,325 bytes (previous: 42,639).

### Preconditions (do these before the deploy)
1. Ship the Bridge-Backend relayer that understands pause/cap errors and uses `withdrawsrc` (Bridge-Backend #13, #14). Keep the Helm flag **off** for now (it renders `"false"`).
2. Fund the bridge account for the larger code (RAM) and for the deploy fee.
3. Decide the cap value and who may flip the pause.

### Deploy
```bash
# upcx@active first: system actions are fee-exempt, deploys are charged by size
clupcx -u $RPC set contract <BRIDGE_ACCOUNT> ./bridge-build upcx.bridge.wasm upcx.bridge.abi \
   -p upcx@active -p <BRIDGE_ACCOUNT>@active

# verify tables and actions
clupcx -u $RPC get abi <BRIDGE_ACCOUNT> | grep -E 'bridgecfg|deposits|bwsrc|bwtxns|setcap|setpaused|setreqsrc|withdrawsrc'
```

### Configure (all start permissive; tighten after the relayer is live)
```bash
# safe default first: pause until the relayer is confirmed working
clupcx -u $RPC push action <BRIDGE_ACCOUNT> setpaused '[true]'  -p <BRIDGE_ACCOUNT>@active
# set the per-transaction cap (asset symbol must match the bridge token, e.g. "100000.00000 UPC")
clupcx -u $RPC push action <BRIDGE_ACCOUNT> setcap '["<CAP> UPC"]' -p <BRIDGE_ACCOUNT>@active
# when the relayer sends source tx hashes, require them
clupcx -u $RPC push action <BRIDGE_ACCOUNT> setreqsrc '[true]' -p <BRIDGE_ACCOUNT>@active
# go live
clupcx -u $RPC push action <BRIDGE_ACCOUNT> setpaused '[false]' -p <BRIDGE_ACCOUNT>@active
```
Pausing blocks `withdraw` and `withdrawsrc` only. Verify the deposit path still accepts a small test transfer.

### Verify with a small test
1. Deposit a small amount from a test account whose `active` grants the bridge `upcx.code`. Confirm a `deposits` row appears.
2. Run one relayer-style `withdrawsrc` for a small amount. Confirm payout, then repeat the same `src_tx_hash` with a new `txns_id` and expect "Source deposit already withdrawn".
3. Send a withdrawal above the cap and expect rejection.

Relayer fee note: with the new contract the fee for `withdrawsrc` is action size plus 4 (68 bytes of data needed 72; 60 was refused).

Rollback: `set contract` the saved wasm and abi. Rows written by the new tables are ignored by the old code; replay rows from the old contract remain enforced by the new one.

## 4. After deploying

- Re-read `get code` for all three accounts and compare with the table.
- Update the audit doc deployed-hash table (section 2) and fix PR status (section 6).
- Keep the rollback files and the transaction ids.
