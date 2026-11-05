# Contract authorization sweep (2026-11-05)

Trigger: mainnet incident where account `nfa.test` called `update` and `settle` on token contracts that had no `require_auth`.
Scope: every action in the UPCX contract repos, a comparison with mainnet code hashes, and the on-chain footprint of `nfa.test`, `nfa.test.a` and `nfa.test.b`.
Method: static reading of source, plus read-only RPC (`get_info`, `get_code_hash`, `get_raw_code_and_abi`, `get_abi`, `get_account`, `history/get_actions`) against one mainnet node. Nothing was signed or broadcast. No attacker URL was fetched. No secrets were read.

"Verified" means the finding was re-read in source by the author of this doc. "Reported" means a static-review pass reported it and it was not independently re-read.

## 1. What was actually exploited

The exploited accounts were `paya.token` and `demo.token`, not `upcx.token`. Both run the same 17-action token contract, whose source is `UPCX-MPA-SmartContract/contracts/upcx.token`. In that source on `main`, `update` and `settle` have no `require_auth`.

| Item | Value |
|---|---|
| Window | 2026-11-02 05:33 to 2026-11-05 00:16 UTC |
| Actions on the three accounts | 179 |
| `paya.token::update` / `demo.token::update` | 16 / 3, all authorized only by `nfa.test@active`. Payloads: external URLs, `javascript:` URIs, `data:image/svg+xml`, `data:text/html`, and an attribute-breakout string. |
| `settle` | 1 on `paya.token`, 1 on `demo.token` |
| `upcx.token::fee` | 126 notifications. Paid from `nfa.test.a`, 1000 units each (0.01 UPC at precision 5, 1.26 UPC in total), to `upcx.fee`. |
| `upcx.token::transfer` | 1, 0.05 UPC from `nfa.test.b` to `nfa.test.a` |
| `setcode` / `setabi` on `nfa.test` | 3 / 8, all with empty or minimal payloads |
| Self-permission changes | temporary permissions created and deleted on `nfa.test`; at the end all three accounts were set to `upcx@active` and `upcx@owner` |
| Other contracts touched | none. Every action in the history is on `upcx` (native), `upcx.token`, `paya.token`, `demo.token` or the three accounts themselves. |

Remediation already on chain: at 2026-11-05 00:04 UTC `paya.token` and `demo.token` were redeployed with the fix-branch wasm (hash below) and `unsettle` was called on each. `paya.token` also had `update` called by `upcx@active` and `paya.token@active`.

`nfa.test.a` and `nfa.test.b` still hold contract code (hash below, 63,056 bytes, identical on both). `nfa.test.b` exposes an NFA-style ABI (`create`, `mint`, `transfer`, `burn`, `batch*`, `setapprove`). The wasm imports include `set_proposed_producers`, `set_blockchain_parameters_packed` and `get_active_producers`, which are privileged intrinsics. Neither account is flagged `privileged`, so those imports cannot succeed today. The code was inspected statically only and never run. Both accounts are locked to `upcx@active`, and the public key that was used controls no accounts (`get_key_accounts` is empty).

## 2. Deployed code versus source

| Account | Deployed code hash (sha256 of wasm) | Matches |
|---|---|---|
| `upcx` (system) | `4daa36fa1f6c4a6afed05c1b1a21e5abf2286fb89b1cc0ffb5a4326a93ea6cb8` | A mainnet dump dated 2026-10-03 under `artifacts/`. It does **not** match `upcx-iac/packer/artifacts/upcx.system` (`ef069252...`), so the deployed system contract cannot be tied to a source build. |
| `upcx.token` | `30b5740c7a46ffd696f4b7e5feebe329396ab2e9dc950f3ca9ada26a53873ed1` | `UPCX-Core/contracts/contracts/upcx.token/bin/upcx.token.wasm`. It has 7 actions, including `fee`, and has no `update` or `settle`. |
| `upcx.msig` | `8874b87c263a760e73a926a35f3a5f33290d0142000dcd5b2e3ea4bd1cb43f93` | `upcx-iac/packer/artifacts/upcx.msig` |
| `upcx.nfa` | `0d60357e71ba271ced6ac7759ec465a3962fb50bb958aa328f675229a312196b` | `upcx-iac/packer/artifacts/upcx.nfa` |
| `paya.token`, `demo.token` | `542c73ef7891521fedb636f5ceae2855d812a37bd972ec8c6db2ca97c354a182` | `UPCX-MPA-SmartContract` main at #4 (c5234a6). Redeployed at 03:32 UTC (`paya.token` block 127,477,904, `demo.token` four seconds later), each authorised by its own `active` only. It is the #3 auth fix (`34f5b393…`, which was deployed at 00:04 UTC) plus two restrictions: `increase` also needs `upcx.token`, and `removebalance` only works on whitelisted tokens. The `fix/token-update-settle-auth` branch (b5e5787) never merged. |
| `upcx.mpa` | zero hash (no code) | Nothing is deployed, although the MPA token source references it. |
| `nfa.test` | zero hash (no code) | n/a |
| `nfa.test.a`, `nfa.test.b` | `cc952a6d0e7742323e7e97c4134fabe0c397e15f4662a47468d0a4ecfaefcd55` | No source found in the workspace. |

Reference: the vulnerable MPA `main` wasm hashes to `ea9ba64298f9697b...` (prefix only was recorded). Any account that still reports that hash is vulnerable.

Gaps in this comparison:
- RPC cannot list every account that holds contract code. Other accounts on the vulnerable `main` build of the MPA token may exist. Check by querying the indexer for accounts whose `code_hash` is `ea9ba642...`.
- The account names for `upcx.mta`, `upcx.oracle`, `upcx.evm`, `upcx.lp`, `upcx.msg`, `upcx.bridge`, `voting` and `upcx.bios` were guessed and are not deployed under those names. Whether the contracts in section 4 are live, and under which accounts, is unknown.
- `upcx.system`'s deployed bytes could not be diffed against source, so section 3 and 4 findings on it are source-level only.

### 2.1 EVM proxies (Ethereum mainnet, read-only RPC, 2026-11-05)

Admin and implementation come from the EIP-1967 slots; the ProxyAdmin owner from `owner()`. The Stake-B, Stake-SmartContract and ETH bridge addresses are the proxies listed in each repo's `.openzeppelin/mainnet.json` or deploy scripts.

| Proxy | Contract | ProxyAdmin owner | Upgradable |
|---|---|---|---|
| `0xBbe5dF4f...28Be`, `0x3b46570e...842e`, `0x8f01Ce43...EBF8` | Stake-B | one shared EOA | yes, one signature |
| `0x3BEF3105...48E9`, `0xd1e5D321...9246`, `0x14e46e1B...cf51` | Stake-B | zero address | **no** (renounced) |
| `0x7b41eebC...422C` | Stake-SmartContract | zero address | **no** (renounced) |
| `0xEbCB31A8...1D8A` | ETH bridge `UpcxBridge` | one EOA (different key) | yes, one signature |

- All eight proxies run the implementation recorded in the manifests except Stake-B `0x14e46e1B...cf51`, which points at `0x1a43be55...a3da`. That implementation is not in `.openzeppelin/mainnet.json` and has not been reviewed.
- Consequence for B1: the fix reaches only the first three Stake-B proxies. The other three (and the Stake-SmartContract proxy for J4) cannot be upgraded, so funds on them move only by migrating to new contracts.
- Owners are plain EOAs, not multisigs, so an upgrade is one signature and a compromise of that key is an upgrade of the contract.

## 3. Findings

Severity is the author's judgment on source alone. Where a contract's deployment is unconfirmed, that is stated.

| ID | Sev | Repo / contract | Finding | Status |
|---|---|---|---|---|
| A1 | Critical | `UPCX-MPA-SmartContract` `upcx.token` `update`, `settle` (`main`) | No auth. `update` writes an unbounded, unvalidated `img` string (stored-XSS vector; RAM paid by the contract). `settle` sets the per-token flag that restricts transfers. | Verified. Exploited. Code fixed on `fix/token-update-settle-auth` (unmerged). Mainnet `paya.token` and `demo.token` run the fix. |
| A2 | High | `UPCX-MPA-SmartContract` `upcx.mpa` `create` (`main`) | No auth. Anyone can register an MPA symbol with a chosen `owner` and `base`, paid by the contract. | Reported. Fixed on the same branch. No `upcx.mpa` code is deployed. |
| A3 | Low | `upcx.mpa` `settle` (`main` and fix branch) | No `require_auth`, and the comment claims it is restricted. Not exploitable today because the inline transfer from `owner` needs `owner`'s signature or an `eosio.code` grant. | Reported. Still open on the fix branch. |
| A4 | Medium | MPA `upcx.token` `removebalance` (`main`) | Issuer erases a balance without reducing supply. Auth is correct. | Reported. Fixed on the fix branch. |
| A5 | Low | MPA `upcx.token` `whiteaccount` | Whitelist vector is unbounded; RAM stays with the contract. Issuer-gated. | Reported. |
| A6 | Low | Fix branch `check_img` | Accepts any `https://` host, so a tracking pixel is still possible. Script and `data:` URIs are rejected. | Reported. |
| B1 | High | `Stake-B-Smart-Contract` `StakingContractV1/V3/V4.withdraw(id)` | No check that the caller owns `records[id]`; the payout goes to `msg.sender`. Any caller can take a record's withdrawable principal and reward once the schedule opens. | Verified (V3). Fixed on `main` in Stake-B-Smart-Contract #4 and #5 (merged 2026-11-05); deployed proxies are unchanged until an upgrade is signed. Mainnet manifest lists V1 and V3 implementations, so an upgrade is needed. Only 3 of the 6 mainnet proxies can be upgraded (section 2.1). |
| C1 | Critical | `UPCX-SmartContract` `voting` `sendtip` | No `require_auth(sender)`; `sub_balance(sender, ...)` debits any depositor. | Verified (no auth call in the action). Deployment unknown. |
| C2 | Critical | `voting` `newcontract` | `creator` is unauthenticated (the `require_auth(creator)` line is commented out) and `feerate` is unbounded, so `sendtip` can credit a creator more than the sender pays. | Reported. |
| C3 | High | `voting` `subscribe`, `newcontract`, `add_balance`, `receivesub` | Unbounded rows paid by the contract; one bad subscription reverts the whole batch. | Reported. |
| C4 | Medium | `voting` `claimfee` | Admin's own balance row is not reduced when the fee is paid, so it can be repeated. | Reported. |
| D1 | Critical | `UPCX-SmartContract` `upcx.mta` `transfer`, `batchtrans` | No positivity, validity or symbol check on `quantity`. A negative amount passes `sub_balance` and credits the sender. | Verified. Deployment unknown. |
| D2 | Low | `upcx.mta` `mint` | No sign check on `quantity`. Issuer only. | Reported. |
| E1 | High | `UPCX-Messager-SmartContract` `upcx.msg` `update` | Looks up the channel with the message id (`_channels.find(id)`) instead of `itr->channel_id`, so authority is checked against an unrelated channel. Also bills the growth to the original author. | Verified. Deployment unknown. |
| E2 | Low / Medium | `upcx.msg` `create`, `send` | No per-account channel cap; message content is unbounded and not sanitized. | Reported. |
| F1 | Critical (source) | `UPCX-SmartContract` `upcx.system` `enterbid`, `claimbid` | `enterbid` escrows nothing and `claimbid` pays `names_account` funds to `bid->owner` without requiring a real bid. A displaced owner can claim against another account's row. | Reported. The payout path was read; the exploit was not reproduced. Requires the deployed system contract to match source, which is unconfirmed. |
| F2 | Low | `upcx.system` `bidrefund` | No auth. Pays only the stored bidder. | Reported. |
| F3 | Medium | `upcx.system` `regproducer` | `url` stored with a length check only; rendered by explorers and wallets. | Reported. |
| F4 | Low / Medium | `upcx.system` name hash | `string_to_uint64` overflows for names of 11 or more characters, so distinct names can collide. | Reported. |
| F5 | Info | `upcx.bios` / system `setabi` | No auth in the contract body; relies on the chain's native handler. Add `require_auth(account)` as defense in depth. | Reported. |
| G1 | Medium | `upcx.evm` | Built with `TESTING true`. `devnewacct` mints an EVM balance that `withdraw` converts to UPC, and `clearall` wipes state. All are gated by the contract key. | Reported. |
| G2 | Info | `upcx.evm` | Signed transactions carry no chain-id binding (cross-chain replay). | Reported. |
| H1 | High | `UPCX-DEX-SmartContract` `upcx.lp` `initpool` (branch `fix/oracle-bounded-rate-tables`, old code) | No auth, and caller-supplied token contracts flow into inline actions authorized by `upcx.lp@active`. `main` adds `require_auth(creator)` and a creator gate. | Reported. The checked-out branch is behind `main`. Confirm the old code is not deployed. |
| H2 | Medium | DEX `upcx.lp` on `main` | Token contracts are validated only by "a stat row exists"; an approved creator can still supply a custom contract. | Reported. |
| I1 | High | `UPCX-Bridge-Contract-UPCX` `upcx.bridge` `withdraw` | Single authority (`require_auth(get_self())`) releases any amount. Replay set keyed on a relayer-chosen `uint64`. No cap or pause. The bridge account's permission setup is not visible from source. | Reported. |
| I2 | Medium | `upcx.bridge` `deposit` | Destination and memo are not recorded or validated on chain; plain token transfers to the bridge are silently accepted. | Reported. |
| J1 | Medium | `UPCX-Arbitrum-Bridge-Contract` approvals | The owner, a new approver and one existing approver reach the 2-of-3 threshold for an approver change. A pending request cannot be cancelled. | Reported. |
| J2 | Medium (by design) | Arbitrum bridge `sendUPC`; ETH bridge `receiveFromUPCX`; staking `withdraw` / `withdrawByAdmin` / `setUPCAddress` | Single operator or owner key can move funds. No timelock or multisig in the contracts. | Reported. |
| J3 | Medium | `UpcxBridgeV2` | Roles are split from the `Ownable` owner; any `DEFAULT_ADMIN_ROLE` holder can grant the receiver role. Not on mainnet per the manifest. | Reported. |
| J4 | Low | Stake contracts | Implementations lack `_disableInitializers()`; `require(amount >= 0)` is always true; referral codes can be overwritten. | Reported. |

No missing-authorization pattern was found in: `upcx.token` as deployed (7 actions), `upcx.msig`, `upcx.nfa`, `upcx.bios`, `upcx.boot`, `upcx.oracle` (both copies), or the REX, delegatebw and voting-on-producers actions of `upcx.system`.

Not auditable: `UPCX-Blind-SmartContract` and `UPCX-Link-SmartContract` contain only a README, with no contract source. `*-Audit` twins were treated as snapshots.

## 4. Per-contract action tables

Legend: "self" = `require_auth(get_self())`. "chain" = the native handler enforces auth and the contract body has none. "FINDING" refers to the IDs in section 3. Line numbers are against the repo ref named in each heading. Sources are static review; rows marked in section 3 as "Verified" were re-read by hand.

### 4.1 `upcx.token` as deployed (`UPCX-Core` `contracts/contracts/upcx.token`, 7 actions)

| Action | State mutated | Auth | Verdict |
|---|---|---|---|
| create | stat | self (:8) | OK |
| issue | stat, accounts | `st.issuer` from stat row (:39); `to == issuer` enforced | OK |
| retire | stat, accounts | `st.issuer` (:64) | OK |
| transfer | accounts | `from` (:83); RAM payer `has_auth(to) ? to : from` (:97) is safe because `from` is always authorized | OK |
| open | accounts | `ram_payer` (:131) | OK |
| close | accounts | `owner` (:151) | OK |
| fee | accounts, via internal `transfer(from, upcx.fee, ...)` | `from`, through `transfer` | OK |

### 4.2 MPA `upcx.token` (`UPCX-MPA-SmartContract`, 17 actions; M = `main`, F = `fix/token-update-settle-auth`)

| Action | State mutated | Auth (M / F) | Verdict |
|---|---|---|---|
| create | stat | self (:8) | OK |
| createv2 | stat, permissions, statsV2 | self (:30); F adds `check_img` | OK |
| createv3 | stat, permissions, statsV2, whitelist | self (:68); F adds `check_img` | OK |
| update | statsV2.token_img | M: **none** (:113-133). F: `has_auth(self)` or `st.issuer` (:123) plus `check_img` | M: FINDING A1. F: OK |
| settle | permissions.settled | M: **none** (:303-311). F: `require_auth(settler)` (:323), `settler` = constant `upcx.mpa` | M: FINDING A1. F: OK |
| unsettle | permissions.settled | F only: self (:335) | OK |
| issue | stat, accounts | `st.issuer` (:162) | OK |
| retire | stat, accounts | `st.issuer` (:187) | OK |
| increase | stat.max_supply | `st.issuer` (:143) | OK |
| transfer | accounts | `from` (:206) | OK |
| open | accounts | `ram_payer` (:270) | OK |
| close | accounts | `owner` (:290) | OK |
| fee | accounts, via `transfer` | `from`, through `transfer` | OK |
| whiteaccount | whitelist.accounts | `currency->issuer` (:323) | OK; FINDING A5 |
| removewhite | whitelist | `currency->issuer` (:351) | OK |
| updatelimit | whitelist.limit | `currency->issuer` (:366) | OK |
| removebalance | accounts erase | `currency->issuer` (:383); M does not reduce supply | OK; FINDING A4 (M) |

### 4.3 `upcx.mpa` (`UPCX-MPA-SmartContract`)

| Action | State mutated | Auth | Verdict |
|---|---|---|---|
| create | symbols.v2 (contract-paid RAM) | M: **none** (:8-24). F: self (:9) | M: FINDING A2. F: OK |
| setcoll | collateral, inline transfer from `owner` | `owner` (:32) and `symItr->owner == owner` (:38) | OK |
| rmcoll | collateral, inline transfer from contract | `owner` (:103) and `symItr->owner == owner` (:109) | OK |
| settle | reads owner's balance, two inline transfers | **none** on M and F (:65); inline transfer needs `owner`'s signature or `eosio.code` grant | FINDING A3 |
| onprice | may send inline `token::settle` as `upcx.mpa@active` | `on_notify("upcx.oracle::update")`; CDT pins first receiver | OK; never fires because the oracle does not notify |

### 4.4 `upcx.system` (`UPCX-SmartContract` `main`, `ba6ddc9`)

| Action | State mutated | Auth | Verdict |
|---|---|---|---|
| newaccount | user_resources, resource limits, name_bid | chain; creator must be highest bidder unless self or social (:320-322) | OK |
| setabi | abihash | chain | Info, FINDING F5 |
| updateauth, deleteauth, linkauth, unlinkauth, canceldelay, setcode | stubs | chain | OK |
| onerror | none | always fails | OK |
| init | rammarket, inline `open` | self (:361) | OK |
| onblock | gstate, producers, name_bid, rex | self | OK |
| setram | rammarket, gstate | self (:49) | OK |
| setramrate | gstate2 | self (:87) | OK |
| setparams | gstate, chain params | self (:94) | OK |
| setpriv | `set_privileged` | self (:101) | OK |
| setalimits | resource limits | self (:106) | OK |
| setacctram, setacctnet, setacctcpu | voters, resource limits | self (:124, :169, :213) | OK |
| activate | `preactivate_feature` | self (:257) | OK |
| rmvproducer | producers | self (:262) | OK |
| updtrevision | gstate2 | self (:271) | OK |
| swapname | user_resources names, inline transfer | `client` and `host` (:386-387) | OK |
| buyram | rammarket, userres, inline transfers | `payer` (dbw:48) | OK |
| buyrambytes | via buyram | `payer` | OK |
| sellram | userres, rammarket, transfers | `account` (:115) | OK |
| delegatebw, undelegatebw | via changebw | `from` (:175) | OK; receiver pays RAM when `transfer == true` (Info) |
| refund | refunds, inline transfer | `owner` (:403) | OK |
| regproducer | producers, producers2 (url, key, location) | `producer` (voting:26) | Auth OK; FINDING F3 |
| unregprod | producers | `producer` (:69) | OK |
| voteproducer | voters, producers | `voter_name` (:165) | OK |
| regproxy | voters | `proxy` (:300) | OK |
| claimrewards | gstate, inline issue and transfers | `owner` (producer_pay:70) | OK |
| bidname | inline transfer, bids, refunds, deferred trx | `bidder` (:13) | OK |
| bidrefund | inline transfer from `names_account` to bidder | **none** (:78-87) | FINDING F2 (Low) |
| enterbid | name_bid, claim_bid, userres, name_list | `bidder` (:90); no escrow | FINDING F1 |
| claimbid | inline transfer from `names_account`, bids, name_list, userres | `owner` (:158); mutates and pays `bid->owner`, not necessarily the caller | FINDING F1 |
| syncname | name_list | self (:247) | OK |
| cancelbid | bids, userres, name_list | `owner` (:264) and `current->owner == owner` | OK |
| updateconfig (bids) | bidconfig2 | self if unset, else `config->admin` (:298, :300) | OK |
| claimbidfee | inline transfer to admin | `config->admin` (:327) | OK |
| deposit, withdraw (rex) | rex funds | `owner` (rex:12, :26) | OK |
| buyrex | rex pool | `from` (:41) | OK |
| unstaketorex | delbw, rex | `owner` (:58) | OK |
| sellrex | rex pool | `from` (:98) | OK |
| cnclrexorder | rex orders | `owner` (:148) | OK |
| rentcpu, rentnet | loans | `from` (:157, :166) | OK |
| fundcpuloan, fundnetloan, defcpuloan, defnetloan | loans | `from` (:175-199) and `itr->from == from` | OK |
| updaterex | rex balance | `owner` (:207) | OK |
| setrex | rex pool | `require_auth("upcx"_n)` hard-coded (:233) | OK if the system account is `upcx` |
| rexexec | rex queues | `user` (:245); user-chosen loop count (CPU only) | OK |
| consolidate, mvtosavings, mvfrsavings, closerex | rex balance | `owner` (:252, :263, :297, :320) | OK |
| buyresult, sellresult, orderresult, rentresult | none (trace only) | none needed | OK |

### 4.5 `upcx.bios`, `upcx.boot`, `upcx.msig`, `voting`, `upcx.evm` (`UPCX-SmartContract` `main`)

| Contract::action | State mutated | Auth | Verdict |
|---|---|---|---|
| bios::setabi | abihash, payer = `account` | chain only (:11-24) | Info, FINDING F5 |
| bios::onerror, boot::onerror | none | always fails | OK |
| bios::setpriv | `set_privileged` | self (:31) | OK |
| bios::setalimits | `set_resource_limits` | self (:36) | OK |
| bios::setprods | `set_proposed_producers` | self (:41) | OK |
| bios::setparams | `set_blockchain_parameters` | self (:46) | OK |
| bios::setpparams | `set_parameters_packed` | self (:51) | OK |
| bios::setkvparams | `set_kv_parameters` | self (:56) | OK |
| bios::reqauth | none | `from` | OK |
| bios::activate, boot::activate | `preactivate_feature` | self | OK |
| bios::reqactivated, boot::reqactivated | none | none needed | OK |
| bios and boot native stubs | none | chain | OK |
| msig::propose | proposals, approvals | `proposer` (:25) and `check_transaction_authorization` (:33) | OK |
| msig::approve | approvals | `level` (:62) | OK |
| msig::unapprove | approvals | `level` (:95) | OK |
| msig::cancel | proposals, approvals | `canceler` (:119) | OK |
| msig::exec | erase, deferred trx | `executer` (:143) and `check_transaction_authorization` | OK |
| msig::invalidate | invalidations | `account` (:192) | OK |
| voting::deposit | balances, inline transfer | `sender` (:12) | OK |
| voting::sendtip | balances | **none** (:26-70) | FINDING C1 |
| voting::newcontract | contracts (contract-paid RAM) | `player` only (:105); `creator` auth commented out (:106); `feerate` unbounded | FINDING C2, C3 |
| voting::withdraw | balances, inline transfer from contract | `owner` (:124) | OK |
| voting::updateconfig | config | self if unset, else `config->admin` (:141, :143) | OK |
| voting::claimfee | balances, inline transfer to admin | `config->admin` (:166) | FINDING C4 |
| voting::subscribe | subscriptions (contract-paid RAM) | `sender` (:186); `quantity` and `recipient` unvalidated | FINDING C3 |
| voting::receivesub | loops `sendtip` | admin or self (:215, :218) | Auth OK; FINDING C3 |
| voting::cancelsub | subscriptions | `sender` (:234) | Auth OK; logic bug (F7 in working notes) |
| evm::create | accounts | `account` (:10) | OK |
| evm::raw | accounts, state, balances | ctor `require_auth(ram_payer)` (transaction.hpp:195); unsigned path adds caller account; signed path via ECDSA | OK; FINDING G2 |
| evm::call | none (always reverts) | ctor `require_auth(ram_payer)` | OK; inverted `end()` check (Low) |
| evm::withdraw | balances, inline transfer | `to` (transfer.cpp:24); no symbol check | OK (Low) |
| evm on_notify `upcx.token::transfer` | balances | dispatcher pinned to `upcx.token`; body checks `get_first_receiver()`, symbol, `to == self` | OK |
| evm TESTING build: clearall, devnewacct, devnewstore, teststatetx, testtx, printtx | mutate or mint EVM state | self | FINDING G1 |
| evm TESTING build: printstate, printaccount | read-only | none | Info |

### 4.6 `upcx.mta`, `upcx.nfa`, `upcx.oracle` (`UPCX-SmartContract` `main`)

| Contract::action | State mutated | Auth | Verdict |
|---|---|---|---|
| mta::create | tokens | self (:6) | OK |
| mta::mint | tokens.current_supply, accounts | `token_itr->issuer` (:31); no sign check | FINDING D2 |
| mta::batchmint | loops `mint` | self (:43) plus per-token issuer | OK |
| mta::transfer | accounts | `from` (:55); no positivity, validity or symbol check | FINDING D1 |
| mta::batchtrans | loops `transfer` | `from` (:65) | FINDING D1 |
| mta::setapprove | approvals | `owner` (:77) | OK; table is never used |
| mta::burn, batchburn | accounts, supply | `owner` (:128, :135); not in `UPCX_DISPATCH` so uncallable | OK (dead code) |
| nfa::create | stat | self (:6) | OK |
| nfa::mint | tokens, accounts, supply | `st.issuer` (:51); `uris` unvalidated | OK; Info |
| nfa::transferid | tokens.owner | `from` (:78) and `owner == from` (:91) | OK |
| nfa::transfer | inline `transferid` as `{from, active}` | `from` (:115) | OK |
| nfa::setrampayer | tokens (RAM payer) | `payer` (:164) and owner == payer (:171) | OK |
| nfa::burn | tokens, supply | `owner` (:202) and owner check (:207) | OK |
| oracle::create (UPCX-SmartContract) | symbollist | `oracle_owner` constant `upcx` (:11) | OK |
| oracle::update (UPCX-SmartContract) | rates, untrimmed | `symb->owner` (:31) | Auth OK; unbounded growth (Low, working-notes F6) |

### 4.7 DEX (`UPCX-DEX-SmartContract`; checked-out branch `fix/oracle-bounded-rate-tables`, `ef6e530`, which is behind `main` at `7ab31d3`)

| Contract::action | State mutated | Auth | Verdict |
|---|---|---|---|
| oracle::create | symbollist | `oracle_owner` (:21) | OK |
| oracle::update | ratesbase, rates (trimmed) | `symb->owner` (:71); rate and weight validated | OK |
| oracle::prune | erases old rows | self (:156) | OK |
| lp::initpool (branch, old code) | pools (contract-paid RAM), inline `evcreate` | **none** (:5-65); `conta`, `contb` unvalidated | FINDING H1 |
| lp::initpool (`main`) | pools | `creator`, which must be admin or in creators table | FINDING H2 |
| lp::add | pools, history, inline transfers | `owner` (:68) | OK; pool contracts come from the pool row |
| lp::remove | pools, history, inline transfers | `owner` (:181) | OK; same caveat |
| lp::swap | pools, fees, inline transfers | `user` (:313) | OK; same caveat |
| lp::claimfee | pools, inline transfers | `owner` (:460) | OK |
| lp::evrate, evcreate (branch) | print only | **none** (:514, :518) | Info; `main` adds self |
| lp::updateconfig | config | self if unset or admin empty, else `config->admin` (:529-533) | OK |
| lp::claimadmin | inline transfer from contract | `admin` (:550) and admin match (:555) | OK |
| lp::claimall | loops fee rows | `admin` (:584) and admin match (:589) | OK |
| lp (`main`): updpoolfee, setpartner, addcreator, rmcreator, prunehist, seedvolume, claimpartner | config, pools, creators | `config->admin` or the stored partner | OK (auth gates only skimmed) |

### 4.8 `upcx.msg` (`UPCX-Messager-SmartContract` `main`, `aacbb01`)

| Action | State mutated | Auth | Verdict |
|---|---|---|---|
| create | channels (sender-paid RAM) | `sender` (:10); `receiver` arbitrary | OK; FINDING E2 |
| send | accounts, messages, optional fee transfer | `auth_user` taken from the channel row by the `from` flag (:43-44) | OK; FINDING E2 |
| update | messages.content (`same_payer`) | `sender` or `receiver` of `_channels.find(id)`, with `id` = **message** id (:117-120) | FINDING E1 |

### 4.9 `upcx.bridge` (`UPCX-Bridge-Contract-UPCX` `main`, `43294da`)

| Action | State mutated | Auth | Verdict |
|---|---|---|---|
| deposit | inline `upcx.token::transfer` from `from` to the bridge; no row written | `from` (:49) | Auth OK; FINDING I2 |
| withdraw | bwtxns replay set, inline transfer from the bridge to `to` | self (:69) only | FINDING I1 |

### 4.10 Solidity: access control by function

Roles: Arbitrum bridge `owner` is the deployer and `approverA/B/C` are constructor arguments; `operator` exists on `AssetManagement` only. ETH bridge and staking contracts use an `OwnableUpgradeable` owner; Stake-B also has `adminContractAddress`. Upgrade authority is the Transparent proxy's ProxyAdmin, which is not in the repos.

**`UPCX-Arbitrum-Bridge-Contract`** (`UPCXArbitrumBridge.sol`, `AssetManagement.sol`; same functions unless noted)

| Function | Moves | Access control | Verdict |
|---|---|---|---|
| requestUPCAddressChange | opens request | `onlyOwner` (:39) | OK |
| approveUPCAddressChange | sets `UPC_Address` at 2 approvals | `onlyApprovers` (:44) | OK |
| requestWithdraw | opens UPC withdraw | `onlyOwner` | OK; `to` may be zero (J4/Low) |
| approveWithdraw | transfers UPC at 2 approvals | `onlyApprovers` | OK |
| convert | pulls caller's own UPC | `transferFrom(msg.sender)` | OK |
| requestOwnershipTransfer, approveOwnershipTransfer | changes owner | `onlyOwner`, `onlyApprovers` | OK |
| requestApproverChange | opens request | `onlyOwner` | OK |
| approveApproverChange | replaces approver A, B or C | `onlyApproversOrPending` | FINDING J1 |
| requestOperatorChange, approveOperatorChange (AssetManagement) | changes operator | `onlyOwner`, `onlyApprovers` | OK |
| requestEthWithdraw (AssetManagement :283) | operator deposits ETH | `onlyOperator` | OK; stuck-ETH risk (Low) |
| approveEthWithdraw (:300) | sends ETH at 2 approvals | `onlyApprovers` | OK |
| sendUPC (:322) | sends UPC with no approval | `onlyOperator` | FINDING J2 |
| `Lock.sol` withdraw | sample timelock | owner and timelock | OK |

**`UPCX-Bridge-Contract-ETH`**

| Contract::function | Moves | Access control | Verdict |
|---|---|---|---|
| Erc20UPC::mint | mints | `onlyOwner` | OK; uncapped (Info) |
| UpcxBridge::initialize (:68) | sets owner, token, fee | `initializer` | FINDING J4 |
| UpcxBridge::sendToUPCX | pulls caller's UPC and fee | `whenNotPaused`, `nonReentrant`, `msg.sender` | OK |
| UpcxBridge::finalizeSendToUPCX | sets status | `onlyOwner` | OK |
| UpcxBridge::receiveFromUPCX (:125) | pays UPC to `to` | `onlyOwner`, `whenNotPaused` | FINDING J2 |
| UpcxBridge::withdrawFee, withdrawUPC | ETH or UPC to owner | `onlyOwner` | FINDING J2 |
| UpcxBridge::setFee, pause, unpause | config | `onlyOwner` | OK |
| UpcxBridge receive/fallback (:203, :208) | accepts ETH | open | Info |
| UpcxBridgeV2::initialize (:277) | also grants owner admin and receiver roles | `initializer` | FINDING J3, J4 |
| UpcxBridgeV2::receiveFromUPCX (:338) | pays UPC | `hasRole(BRIDGE_RECEIVER_ROLE)` | FINDING J3 |
| UpcxBridgeV2::setAdminRole, removeAdminRole | grants and revokes admin | `onlyOwner` | FINDING J3 |
| UpcxBridgeV2::setReceiver, removeReceiver | grants and revokes receiver | `onlyOwner` and caller must hold the role admin | FINDING J3 |
| UpcxBridgeV2::grantRole, revokeRole, renounceRole | roles | `DEFAULT_ADMIN_ROLE` | FINDING J3 |
| UpcxBridgeV2 other functions | as V1 | as V1 | OK |

**`Stake-SmartContract`** (`StakingContract.sol`, `V1`, `V2`)

| Function | Moves | Access control | Verdict |
|---|---|---|---|
| constructor / `initialvalue(owner)` | sets owner | `initializer` | FINDING J4 |
| stake | pulls caller's UPC | `nonReentrant`, `isStakablePeriod` | FINDING J4 (`amount >= 0`) |
| claim(id) | pays principal and reward | `msg.sender == records[id].staker` | OK |
| claimReferralFee(id) | pays referral fee | `msg.sender == referralCodes[code]` | OK; FINDING J4 |
| generateReferralCode | writes `referralCodes[code]` | `staked[msg.sender]` | FINDING J4 (weak randomness, overwrite) |
| deposit | owner funds contract | `onlyOwner` | OK |
| withdraw | any amount to owner | `onlyOwner`, no solvency check | FINDING J2 |
| setUPCAddress | swaps token | `onlyOwner` | FINDING J2 |
| setStakingPeriod, setStakingLimit, setReferralFee | config | `onlyOwner` | OK |
| pushRecord(s), pushReferralCodes | writes records | `onlyOwner` | FINDING J2 |

**`Stake-B-Smart-Contract`**

| Contract::function | Moves | Access control | Verdict |
|---|---|---|---|
| StakingAdminContract::setContracts | staking list | `onlyOwner` | OK |
| StakingAdminContract::setAdminDate | calls `setAdminDate` per contract | `onlyOwner` | OK |
| V1/V3/V4::initialvalue | owner, APY, token | `initializer` | FINDING J4 |
| V1/V3/V4::setAdminDate | `listDate`, `startDate` | `onlyAdminContract` | OK |
| V1/V3/V4::stake | pulls caller's UPC | `nonReentrant` | OK |
| V1/V3/V4::withdraw(id) | pays `msg.sender` | **none** before 2026-11-05; now `records[id].staker == msg.sender` on `main` (#4); deployed proxies still run the old code | FINDING B1 |
| V1/V3/V4::setUPCAddress | swaps token | `onlyOwner` | FINDING J2 |
| V1/V3/V4::setAdminContractAddress | admin contract | `onlyOwner` | OK |
| V1/V3/V4::withdrawByAdmin | any amount to owner | `onlyOwner`, no solvency check | FINDING J2 |

## 5. Recommended order of work

Each fix belongs in its own repo's PR.

1. Merge `UPCX-MPA-SmartContract` `fix/token-update-settle-auth`, and find any other account running the vulnerable hash `ea9ba642...` through the indexer.
2. Decide whether `voting`, `upcx.mta`, `upcx.msg`, `upcx.lp` (old branch) and `upcx.bridge` are deployed, and under which accounts. C1, D1 and E1 are one-line fixes and should land regardless.
3. Merge Stake-B `fix/withdraw-access-control` and upgrade the V1 and V3 proxies that hold funds.
4. Review `upcx.system` name bidding (F1) against a build that matches the deployed hash.
5. Confirm on-chain owners for the bridge account permissions and the EVM ProxyAdmin, which decide the real impact of I1, J1 and J2.

## 6. Fix PR status (2026-11-05)

Nothing in this table is deployed. "Built" means compiled with the real CDT image; "chain-tested" means run on a throwaway `nodupcx`. Rows marked unbuilt have not been through either.

| Repo | PR | Findings | State | Notes |
|---|---|---|---|---|
| Stake-B-Smart-Contract | #4, #5 | B1, J2/J4 hardening | merged | Hardhat tests pass. 3 of 6 mainnet proxies can be upgraded (section 2.1). #3 closed unmerged. Old remote branches deleted. 6 tests in `StakingV1_v1.js` and `StakingV1_v2.js` fail on main and need updating. |
| Stake-B-Smart-Contract | #6 | J2 (`withdrawByAdmin` solvency floor) | merged | Not part of the original owner decision (J2 was note-only); added at the owner's request. Stricter than today: the owner cannot withdraw principal owed to stakers. Reaches only upgradable proxies. |
| UPCX-Bridge-Backend | #13, #14 | I1/I2 relayer side, Helm flag | merged | Relayer handles pause/cap errors as pending and uses `withdrawsrc` behind a flag (off by default). Helm flag and frontend address check still to do. |
| UPCX-SmartContract | #5, #6, #8 | C1-C4, D1-D2, G1, `cancelsub` | merged, built, chain-tested | #6 adds the CDT 1.8.1 / cmake 3.10 build fixes. On a throwaway `nodupcx`: voting 39/39 (12/39 before), `upcx.mta` 14/14 (3/14 before). `upcx.evm` built with TESTING off only; the TESTING=ON host tests were not run. #8 moves `voting::deposit` to a `upcx.token::transfer` notification, because inline transfers authorised by the depositor fail on this chain. **ABI break:** clients must send `upcx.token::transfer` to `voting` (memo unused). The contract pays RAM per new depositor row, with no minimum deposit or row cap yet. `voting@active` still needs its own `upcx.code` permission for `withdraw` and `claimfee`. Client PRs are in progress and must ship with the redeploy. |
| UPCX-SmartContract | #4, #7 | F1-F5, `onblock` double-pay and related name-bidding issues | merged, built, chain-tested | #7 adds the regression script: 35/35 on main, and the pre-fix build `ba6ddc9` reproduces F1 variants and the F4 id collision. Not testable locally: `onblock` auction close (14 days of chain time) and the `setabi` check. The `bidrefund` auth change is harmless (the baseline refund also works because `upcx` skips inline auth checks). Main builds to `3e3801bd…`, not the deployed `4daa36fa…`; see section 7. |
| UPCX-Messager-SmartContract | #2, #3 | E1 | merged, built, chain-tested | #3 drops the E2 caps and adds `tests/local-chain`. Deploy `upcx.msg.wasm` sha256 `3cb1d393…0a42` (ABI unchanged, `setcode` only). The capped build from #2 (`fc6a64f9…`) must not be deployed; anyone who did needs the same redeploy. |
| UPCX-DEX-SmartContract | #19, #20 | H2 | merged, built, chain-tested | #20 adds the `upcx.lp` local-chain harness for `initpool` and the token whitelist (not run against the pre-fix wasm). H1 did not reproduce on `main`. Adds a token-contract whitelist (ABI change, redeploy); whitelist any non-`upcx.token` contracts that need pools. |
| UPCX-Bridge-Contract-UPCX | #2 | I1, I2 | merged, built, chain-tested | Compile fix is the `upcx/crypto.hpp` include. 83/83 checks on a throwaway `nodupcx` (replay, cap, pause, `deposits` rows, in-place upgrade over main, relayer-shaped `withdrawsrc`); 16 checks fail on a deliberately broken build. Wasm is 63,325 bytes (main 42,639), so the deploy needs a size-based fee authorised by `upcx@active`. Relayer changes must ship before the cap or pause is enabled. RAM cost per row still an estimate. |
| UPCX-Arbitrum-Bridge-Contract | #1 | J1 (accepted risk), J2, J4 | merged | `sendUPC` now needs 2-of-3 approval; cancel added for other request types; redeploy and fund-migration plan in the PR. |
| UPCX-Bridge-Contract-ETH | #7 | J3, J4 | merged | Storage layout unchanged (`validateUpgrade` passes). Mainnet proxy needs a new implementation and a ProxyAdmin upgrade. Mainnet proxy still needs a new implementation and ProxyAdmin upgrade. |
| Stake-SmartContract | #1 | J4 | merged | Non-breaking fixes only. The one mainnet proxy has a renounced ProxyAdmin, so fixes apply to new deployments only. |
| UPCX-MPA-SmartContract | existing branch and sessions | A1-A6 | not tracked here | Main at #4 (`542c73ef…`) is deployed on `paya.token` and `demo.token`; the fix branch never merged. A2/A3 are with the existing MPA sessions. |

**Items to confirm (found 2026-11-05 ~03:50 UTC, read-only RPC):**

- `paya.token` and `demo.token` had both their `active` and `owner` keys replaced between 01:30 and 01:34 UTC, each signed by the account's previous `owner`, and were redeployed at 03:32 UTC. None of this came from the audit sweep. The new `demo.token` public keys appear in a `mainnet-key-separation` session's scratchpad (public key files only), which suggests an intentional key-separation job. Confirm with whoever runs it.
- No client of `voting::deposit` exists in any tracked repo under the workspace (the voting `deposit` change is an ABI break). A voting UI whose source is outside these repos would not show up, so check before the voting redeploy.

## 7. Builds with the CDT image (2026-11-05)

Toolchain: `upcx-testnet/upcx-cdt:latest` (upcx.cdt 1.8.1, CMake 3.10), built in a clean container from `git archive` of each ref. Hashes are sha256 of `<name>.wasm.wasm`.

| Contract | Ref | Result | sha256 |
|---|---|---|---|
| `upcx.token` | UPCX-SmartContract `main` | built | `30b5740c7a46ffd696f4b7e5feebe329396ab2e9dc950f3ca9ada26a53873ed1` (equals the code deployed on mainnet `upcx.token`) |
| `upcx.msg` | Messager `main` (`d308660`) | built | `fc6a64f9357441906b19e395d30682a25e2d24549643c2fedc3576c4e73d4cf5` |
| `upcx.mta` | UPCX-SmartContract `main` (`beed171`) | built | `640225bc7398fc4127f77b32933a51f7f3f80891d46e6cf1f12d656b4b4d15ae` |
| `upcx.system` | UPCX-SmartContract `main` (`beed171`) | built | `4aadbdf1a01a78e8169805b6b01e857ef1e5ebdb1954cab8b437358601ef8f7f` |
| `upcx.system` | `8acf669` (last commit before the fixes; source as `ba6ddc9` plus the build-target shim) | built | `823254a3c1be420ee0db7cb021d59ee0f6d6d58fa2aad2c141d9ad9653618d72` |
| `upcx.lp` (`liquidity`) | DEX `main` (`153f3c8`) | built | `2dc4e947e7aef465468765743efc4fbb602576f44a0e38bb985d04b8c698fbfd` |
| `upcx.oracle` | DEX `main` (`153f3c8`) | built | `0b3eeec5f25ac2b266b8aef7168e2287bf89ea9c86ed95d270ba18f6e022f5f1` |
| `voting` | UPCX-SmartContract `main` | **build fails** | CMake names the target `voting`, the toolchain names it `voting.wasm`; needs the same target-name shim the other contracts have |
| `upcx.evm` | UPCX-SmartContract `main` | **build fails** | Uses `target_link_options`, which needs CMake 3.13; the image has 3.10 |
| `upcx.bridge` | `UPCX-Bridge-Contract-UPCX` #2 branch (`67896e9`) | **compile error** | `upcx.bridge.cpp:191`: `use of undeclared identifier 'sha256'` |

Parity with mainnet:
- `upcx.token` is reproducible: the image builds the deployed bytes exactly.
- `upcx.system` is not: the pre-fix source builds to `823254a3...`, deployed is `4daa36fa...`, and the packer artifact is `ef069252...`. The deployed system contract was built from different source, flags or toolchain, so a `setcode` of the fixed build cannot be reviewed as a diff against what is live. Resolve this before any system-contract deploy.
