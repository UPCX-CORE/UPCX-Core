# UPCX Ethereum-side contracts: plain-language brief for stakeholders

Date: 2026-11-05. Prepared from the contract-authorization audit that followed the 2026-11-05 token incident. This is a summary for decision makers. The technical detail and evidence are in `2026-11-05-contract-auth-sweep.md`.

## The short version

- The incident on 5 November was on the UPCX chain (the token contracts `paya.token` and `demo.token`), not on Ethereum or Arbitrum. It is fixed on chain.
- We then reviewed the Ethereum and Arbitrum contracts for the same kind of mistake (an action anyone could call). We found one real flaw of that kind, in the staking contracts, and fixed it in the code.
- **A fix in the code does not protect money until it is installed.** Of the eight staking and bridge contracts on Ethereum, **four can be upgraded with one signature, and four can never be changed**, because their upgrade rights were given up. Money in those four can only be protected by moving it to new contracts.
- The bigger, lasting risk is not a bug: **a single private key controls upgrades and, for several contracts, fund movements.** Nothing needs a hack to go wrong if that key is stolen or lost.

## What each contract is for

| Contract | What it does, simply | Where it lives |
|---|---|---|
| **Staking (Stake-B)** | Holds users' staked UPC and pays rewards on a schedule. Six copies are deployed. | Ethereum mainnet |
| **Staking (original)** | The earlier staking contract. One copy is deployed. | Ethereum mainnet |
| **ETH bridge** | Locks UPC on Ethereum and releases it when UPC is sent back from the UPCX chain. | Ethereum mainnet |
| **Arbitrum bridge** | The same job on Arbitrum. Payouts are approved by a small group of signers. | Arbitrum |

## What we found, and where it stands

| # | Plain-language finding | How serious | Fixed in code? | Protects users today? |
|---|---|---|---|---|
| 1 | In the Stake-B contracts, anyone who knew a staker's record number could ask the contract to pay that record's money to themselves, once withdrawals were open. | High | Yes (merged) | **Only on the 3 upgradable copies.** The other 3 copies still have the flaw. |
| 2 | The owner key can withdraw funds from staking and can change the payout address, with no delay and no second approver. | Medium by design | Partly. A new rule stops the owner withdrawing money owed to stakers. | Same as above. The owner can still repoint the payout address. |
| 3 | The Arbitrum bridge sent funds on one operator's say-so. It now needs two of three approvers. A way to cancel a stuck request was added. | Medium | Yes (merged) | **Not until redeployed.** These contracts are not upgradable, so a new copy must be deployed and funds moved. |
| 4 | The Arbitrum approver group can be changed by the owner plus one approver, without the third approver agreeing. | Medium | No. Accepted by the owners. | Open. |
| 5 | The newer bridge version splits roles so that anyone with the admin role can hand out the payout role. | Medium | Yes (merged) | Not on mainnet according to the deployment manifest. |
| 6 | Minor hardening in the staking contracts (setup functions left open, a check that could never fail, referral codes that could be overwritten). | Low | Yes (merged) | Same limits as #1. |

## The four contracts that can never be changed

| Contract | Upgrade rights | Consequence |
|---|---|---|
| 3 of the 6 Stake-B copies | Given up (set to the zero address) | The flaw in #1 stays. Any remaining funds can only be protected by moving them to a new contract. |
| The original Staking contract | Given up | The minor fixes in #6 never apply. |

One of the three non-upgradable Stake-B copies runs a version of the code that is **not in our records and has not been reviewed** (address ending `…cf51`). Review it before relying on it.

## Who holds the power today

- The upgrade rights of the three upgradable Stake-B copies belong to **one ordinary wallet** (one key).
- The ETH bridge is controlled by **a different single wallet**.
- None of these is a multi-person wallet. Anyone who obtains that key can replace the contract's code, in one transaction, with code that sends funds anywhere.

## Decisions requested from stakeholders

1. **Owner keys.** Approve moving the owner and upgrade rights of the upgradable contracts to a multi-signature wallet (for example 2 of 3), optionally with a waiting period before upgrades take effect. This is the single biggest reduction in risk and needs no code change.
2. **The non-upgradable staking copies.** Choose between (a) leaving them as they are and accepting the risk, or (b) deploying new copies and migrating stakers. Migration needs staker action or an owner-run process and has a cost.
3. **Install the fixes.** Authorize the upgrade of the 3 upgradable Stake-B copies and the ETH bridge. These are one-signature upgrades. Each should be tested on a copy first.
4. **Arbitrum bridge.** Approve the redeploy and fund-migration plan (in the contract repository). Note that routine payouts will then need two approvers, so people must be named and available. The payout service also needs updating before the cut-over.
5. **Unreviewed copy.** Commission a review of the version at address `…cf51` before more money goes in.

## What has not been checked

- We did not look at on-chain balances for each contract, so we cannot state how much money is exposed.
- We did not re-test the live contracts. Findings come from reading the code, the deployment records, and running tests on copies.
- The mainnet state was read through public interfaces only. Nothing was signed or sent.

## Glossary

- **Proxy / upgradable:** a contract whose code can be swapped by its owner without changing its address.
- **Renounced:** the owner gave up the right to upgrade for good. Safe from a stolen key, but also impossible to fix.
- **Multi-signature (multisig):** a wallet that needs several people to approve each action.
- **Timelock:** a rule that delays an action so others can react before it takes effect.
