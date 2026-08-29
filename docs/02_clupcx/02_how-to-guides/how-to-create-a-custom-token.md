# Creating Wallets and Tokens on UPCX-Core

This guide documents the end-to-end process for creating wallets, accounts, and
**custom tokens** (e.g. a `DAN` token) on a UPCX-Core blockchain.

UPCX-Core is an EOSIO-derived chain. The bundled tools are renamed but behave
like their EOSIO counterparts:

| UPCX tool  | EOSIO equivalent | Role                                  |
|------------|------------------|---------------------------------------|
| `nodupcx`  | `nodeos`         | Block-producing / chain node daemon   |
| `kupcxd`   | `keosd`          | Wallet daemon (stores/encrypts keys)  |
| `clupcx`   | `cleos`          | Command-line client (RPC + wallet)    |

---

## Key concept: the core token (`UPC`) vs. custom tokens (`DAN`)

There are **two fundamentally different kinds of token** on this chain, and the
distinction matters for your question.

### 1. The core / system token — `UPC`

`UPC` is the **native chain token**. Its symbol is *compiled into the binaries*,
not created at runtime:

- Defined in `CMakeLists.txt:148` — `set( CORE_SYMBOL_NAME "UPC" )` (default,
  overridable at build time with `-DCORE_SYMBOL_NAME=...`, max 7 chars).
- Precision is fixed at **4 decimals** (`4,UPC`).
- It is the token the **`upcx.system`** contract uses for staking, RAM purchase,
  CPU/NET bandwidth, producer voting, and REX.

You **cannot add a second "core" token at runtime** — that symbol is baked into
the build and the genesis/system contract. Changing the core symbol means
recompiling the chain. So a `DAN` token is *never* a second core token.

### 2. Custom tokens — `DAN` (what you actually create)

A `DAN` token is an ordinary fungible token managed by the **`upcx.token`**
contract (`contracts/contracts/upcx.token/`). It is just rows in that contract's
tables — created entirely at runtime, no recompile, no genesis change.

The `upcx.token` contract exposes these actions
(`upcx.token/include/upcx.token/upcx.token.hpp`):

| Action     | Purpose                                                        |
|------------|----------------------------------------------------------------|
| `create`   | Register a new token symbol with a maximum supply              |
| `issue`    | Mint tokens (up to max supply) to the issuer account           |
| `transfer` | Move tokens between accounts                                   |
| `retire`   | Burn tokens, reducing supply                                   |
| `open`     | Create a zero-balance row for an account (RAM paid by another) |
| `close`    | Remove a zero-balance row                                      |

> **Important:** A custom token created this way has **no system powers**. It
> cannot be staked for CPU/NET or used to buy RAM unless the system contract is
> specifically built/configured to recognize it. It is a standalone asset.

> **Note on token ownership / namespacing:** A token is identified by the pair
> *(contract account, symbol)*. The standard `DAN` token lives under the
> `upcx.token` contract account. If a *different* account deploys its own copy
> of the token contract, it can also have a `DAN` symbol — they are distinct
> tokens. `clupcx get currency balance <contract> <account> DAN` always takes
> the contract account as the first argument for this reason.

---

## Prerequisites

- A running `nodupcx` node you can reach (default RPC `http://127.0.0.1:8888`).
- `clupcx` and `kupcxd` installed (bundled with UPCX).
- The `upcx.token` contract deployed on the chain (it is, on any standard
  network — see *Appendix* if you are bootstrapping from scratch).
- An existing account with authority to create accounts and pay RAM (on a fresh
  chain this is the `upcx` system account).

`clupcx` talks to the node with `-u/--url` and to the wallet with
`--wallet-url`. Examples below assume local defaults.

---

## Step 1 — Create a wallet

The wallet (managed by `kupcxd`) holds your private keys. `clupcx` auto-launches
`kupcxd` if it isn't running.

```sh
# Default wallet, password saved to a file
clupcx wallet create -f default_wallet.pwd

# Or a named wallet
clupcx wallet create -n dan_wallet -f dan_wallet.pwd
```

Save the printed password — it is required to unlock the wallet:

```sh
clupcx wallet unlock -n dan_wallet --password $(cat dan_wallet.pwd)
```

---

## Step 2 — Create a key pair

Every account needs an **owner** and (recommended) an **active** key. Generate
keys with:

```sh
clupcx create key --to-console
# Private key: 5K...
# Public key:  UPCX...
```

Public keys on this chain are prefixed `UPCX`.

Import the private key into the wallet so `clupcx` can sign with it:

```sh
clupcx wallet import          # prompts for the private key
# or non-interactively:
clupcx wallet import -n dan_wallet --private-key 5K...
```

Repeat / reuse keys for the owner and active permissions as you prefer.

---

## Step 3 — Create the account that will own/issue the token

You need an account to act as the token issuer. Create it from an existing
(creator) account whose key is in the unlocked wallet.

On a chain **without** the system contract restricting resources (e.g. a dev
chain), the simple form works:

```sh
clupcx create account upcx danissuer UPCX<ownerPubKey> UPCX<activePubKey>
```

On a **production / system-contract** chain you must also provision RAM and
stake CPU/NET, so use `system newaccount`:

```sh
clupcx system newaccount upcx danissuer UPCX<ownerPubKey> UPCX<activePubKey> \
  --stake-net "1.0000 UPC" \
  --stake-cpu "1.0000 UPC" \
  --buy-ram-kbytes 8
```

(`upcx` is the creator/payer here; replace with whichever funded account you
control. Resource amounts are denominated in the core token `UPC`.)

---

## Step 4 — Create the `DAN` token

Call the `create` action on the `upcx.token` contract. This registers the symbol
and its maximum supply. The `issuer` is the account allowed to later mint it.

```sh
clupcx push action upcx.token create \
  '["danissuer", "1000000000.0000 DAN"]' \
  -p upcx.token@active
```

- `"danissuer"` — the account that will be allowed to `issue` the token.
- `"1000000000.0000 DAN"` — maximum supply **and** precision. The `4` decimal
  places (`.0000`) define the token's precision; choose what you need.
- The action must be authorized by the **token contract account** (`upcx.token`),
  because `create` writes to a table scoped under that contract.

Validation rules (from `upcx.token.cpp`): symbol must be valid and not already
exist, and max supply must be positive.

---

## Step 5 — Issue (mint) `DAN` into circulation

`issue` mints tokens up to the maximum supply, crediting the **issuer** account.
It must be authorized by the issuer named in `create`:

```sh
clupcx push action upcx.token issue \
  '["danissuer", "1000000.0000 DAN", "initial issuance"]' \
  -p danissuer@active
```

- `"danissuer"` — recipient; **must be the issuer** for `issue`.
- `"1000000.0000 DAN"` — amount to mint (≤ remaining max supply).
- `"initial issuance"` — free-form memo.

To distribute further, use `transfer` (below). To put tokens directly into
another account at issuance time, `issue` to yourself then `transfer`.

---

## Step 6 — Transfer `DAN` to other accounts

```sh
clupcx push action upcx.token transfer \
  '["danissuer", "bob", "100.0000 DAN", "welcome"]' \
  -p danissuer@active
```

Or, if `DAN` were the core/system token, the shorthand `clupcx transfer` works;
for custom tokens prefer the explicit `push action` form above so the correct
contract is targeted.

---

## Step 7 — Verify

```sh
# Token definition / supply (scoped to the upcx.token contract)
clupcx get currency stats upcx.token DAN

# A specific account's balance
clupcx get currency balance upcx.token danissuer DAN
clupcx get currency balance upcx.token bob DAN

# Raw tables
clupcx get table upcx.token DAN stat        # currency_stats: supply/max/issuer
clupcx get table upcx.token danissuer accounts   # balances for that account
```

---

## Quick reference — full `DAN` token in one block

```sh
# wallet + key
clupcx wallet create -n dan_wallet -f dan_wallet.pwd
clupcx wallet import -n dan_wallet               # paste issuer private key

# issuer account (dev-chain form)
clupcx create account upcx danissuer UPCX<owner> UPCX<active>

# token lifecycle
clupcx push action upcx.token create '["danissuer", "1000000000.0000 DAN"]' -p upcx.token@active
clupcx push action upcx.token issue  '["danissuer", "1000000.0000 DAN", "init"]' -p danissuer@active
clupcx push action upcx.token transfer '["danissuer", "bob", "100.0000 DAN", "hi"]' -p danissuer@active

# verify
clupcx get currency stats   upcx.token DAN
clupcx get currency balance upcx.token bob DAN
```

---

## Appendix — bootstrapping the token contract on a fresh chain

If `upcx.token` is not yet deployed (brand-new chain), deploy it before Step 4.
This mirrors `tutorials/bios-boot-tutorial/bios-boot-tutorial.py`:

```sh
# deploy the token contract to the upcx.token account
clupcx set contract upcx.token <contracts_dir>/upcx.token/

# create + issue the CORE token (UPC), done once at genesis
clupcx push action upcx.token create '["upcx", "10000000000.0000 UPC"]' -p upcx.token
clupcx push action upcx.token issue  '["upcx", "<amount>.0000 UPC", "memo"]' -p upcx
```

After that, custom tokens like `DAN` follow Steps 4–7 above. Note the core token
`UPC` and a custom token `DAN` are created with the *same* `upcx.token` actions —
the only thing special about `UPC` is that the rest of the system software was
compiled to treat that symbol as the chain's native resource token.

---

## Managing this with HTTP APIs (outside the CLI / containers)

Everything above is doable over HTTP without `clupcx` — `clupcx` itself is just a
client over these REST endpoints. There are **two separate API surfaces**, served
by two different daemons:

### A. Chain API — served by `nodupcx`

- Default endpoint: **`http://127.0.0.1:8888/`**
- Enabled by loading the `chain_api_plugin` (`plugins/chain_api_plugin/`).
- Documented as OpenAPI/Swagger: `plugins/chain_api_plugin/chain.swagger.yaml`.

Endpoints registered in `chain_api_plugin.cpp`. Relevant ones for this workflow:

| Endpoint                          | Use                                                        |
|-----------------------------------|------------------------------------------------------------|
| `POST /v1/chain/get_info`         | Chain id + head block (needed as TAPOS for any tx)         |
| `POST /v1/chain/get_account`      | Inspect an account                                          |
| `POST /v1/chain/get_abi`          | Fetch a contract's ABI (e.g. `upcx.token`)                  |
| `POST /v1/chain/abi_json_to_bin`  | Serialize action args (e.g. `create`/`issue`) to binary    |
| `POST /v1/chain/get_required_keys`| Determine which keys must sign a transaction               |
| `POST /v1/chain/get_currency_stats`   | Read a token's supply/max/issuer                        |
| `POST /v1/chain/get_currency_balance` | Read an account's token balance                        |
| `POST /v1/chain/get_table_rows`   | Read `accounts` / `stat` tables directly                   |
| `POST /v1/chain/push_transaction` | **Submit** a signed transaction (newaccount, create, issue…)|
| `POST /v1/chain/send_transaction` | Newer submit endpoint (preferred over push)                |

There is **no single "create token" or "create account" endpoint**. Those are
*actions inside a signed transaction*. The flow over HTTP is:

1. `get_info` → grab `chain_id`, `head_block_id` (TAPOS reference).
2. Build the action(s) — e.g. `upcx.token::create` / `issue`, or
   `upcx::newaccount` — and serialize args with `abi_json_to_bin` (or do it
   client-side with an SDK).
3. Sign the transaction (see the Wallet API, or sign client-side with the
   private key).
4. `push_transaction` / `send_transaction` to broadcast.

In practice you do **not** hand-roll this — a client library (any `eosjs`-style
SDK pointed at the `UPCX`/`nodupcx` endpoint) performs steps 1–4 for you. Since
UPCX is EOSIO-compatible at the protocol/RPC level, standard EOSIO JS/Python
SDKs work against these endpoints; `clupcx` is simply the reference client.

### B. Wallet API — served by `kupcxd`

- Default endpoint: a **unix socket** (`unix:///<home>/upcx-wallet/kupcxd.sock`),
  *not* a network port — deliberately, for safety.
- Defined in `plugins/wallet_api_plugin/wallet_api_plugin.cpp`.

Endpoints (`/v1/wallet/...`): `create`, `open`, `unlock`, `lock`, `lock_all`,
`import_key`, `create_key`, `remove_key`, `list_wallets`, `list_keys`,
`get_public_keys`, `set_timeout`, `sign_transaction`, `sign_digest`.

So wallet/key creation and signing are *also* fully API-driven — the `clupcx
wallet ...` commands in Steps 1–2 map one-to-one onto these calls.

> **Security warning:** `kupcxd`'s wallet API exposes key material and signing.
> It defaults to a local unix socket for a reason. **Do not bind it to a public
> interface.** If you need signing in a service/non-container context, prefer
> signing **client-side** with an SDK (keys never leave your app) and use only
> the `nodupcx` chain API to broadcast — rather than exposing `kupcxd` over the
> network. See `docs/03_kupcxd/20_security.md`.

### Example: create + issue `DAN` purely over HTTP (sign client-side)

```sh
# 1. chain context
curl -s http://127.0.0.1:8888/v1/chain/get_info

# 2..4. build/sign/broadcast — done by an SDK; the final broadcast is:
curl -s -X POST http://127.0.0.1:8888/v1/chain/send_transaction \
  -d '{ "signatures": ["SIG_K1_..."], "compression": false,
        "packed_context_free_data": "",
        "packed_trx": "<hex-serialized signed transaction>" }'
```

The `packed_trx` contains the `upcx.token::create` (or `issue`, `newaccount`,
`transfer`) action(s) — identical in effect to the `clupcx push action`
commands above, just assembled and signed by your own code.

---

## Source references

- Core symbol: `CMakeLists.txt:147-157` (`CORE_SYMBOL_NAME`, default `UPC`)
- Token contract: `contracts/contracts/upcx.token/`
  (`upcx.token.hpp`, `src/upcx.token.cpp`, `bin/upcx.token.abi`)
- CLI how-tos: `docs/02_clupcx/02_how-to-guides/`
- Command reference: `docs/02_clupcx/03_command-reference/`
- Bootstrap example: `tutorials/bios-boot-tutorial/bios-boot-tutorial.py`
- Chain HTTP API: `plugins/chain_api_plugin/chain_api_plugin.cpp`,
  `plugins/chain_api_plugin/chain.swagger.yaml`
- Wallet HTTP API: `plugins/wallet_api_plugin/wallet_api_plugin.cpp`
- Wallet security: `docs/03_kupcxd/20_security.md`
