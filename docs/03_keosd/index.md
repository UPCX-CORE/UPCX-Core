---
content_title: Kupcxd
---

## Introduction

`kupcxd` is a key manager service daemon for storing private keys and signing digital messages. It provides a secure key storage medium for keys to be encrypted at rest in the associated wallet file. `kupcxd` also defines a secure enclave for signing transaction created by `clupcx` or a third part library.

## Installation

`kupcxd` is distributed as part of the [UPCX software suite](https://github.com/UPCX/upcx/blob/master/README.md). To install `kupcxd` just visit the [UPCX Software Installation](../00_install/index.md) section.

## Operation

When a wallet is unlocked with the corresponding password, `clupcx` can request `kupcxd` to sign a transaction with the appropriate private keys. Also, `kupcxd` provides support for hardware-based wallets such as Secure Encalve and YubiHSM.

[[info | Audience]]
| `kupcxd` is intended to be used by UPCX developers only.
