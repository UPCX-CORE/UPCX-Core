## Overview

This guide provides instructions on how to buy RAM for an UPCX blockchain account using the clupcx CLI tool. RAM is a system resource used to store blockchain state such as smart contract data and account information.

The example uses `clupcx` to buy RAM for the alice account. The alice account pays for the RAM and the alice@active permisssion authorizes the transaction.

## Before you Begin

Make sure you meet the following requirements:

- Install the currently supported version of `clupcx.`
  [[info | Note]]
  | `Clupcx` is bundled with the UPCX software. [Installing UPCX](../../00_install/index.md) will install the `clupcx` and `kupcxd` command line tools.
- You have access to an UPCX blockchain and the `upcx.system` reference contract from [`upcx.contracts`](https://github.com/UPCX/upcx.contracts) repository is deployed and used to manage system resources.
- You have an UPCX account and access to the account's private key.
- You have sufficient [tokens allocated](how-to-transfer-an-upcx.token-token.md) to your account.

## Reference

See the following reference guides for command line usage and related options:

- [clupcx system buyram](../03_command-reference/system/system-buyram.md) command

## Buy Ram Procedure

The following step shows you how to buy RAM

1. Run the following command to buy RAM worth 0.1 of `SYS` tokens for the alice account:

```shell
clupcx system buyram alice alice "0.1 SYS" -p alice@active
```

**Where**

- `alice` = payer, the account paying for RAM.
- `alice` = receiver, the account receiving bought RAM.
- `0.1 SYS` = The amount of tokens to pay for RAM.
- `-p alice@active` = The permission used to authorize the payment, in this case the active permission.

**Example Output**

```sh
executed transaction: aa243c30571a5ecc8458cb971fa366e763682d89b636fe9dbe7d28327d1cc4e9  128 bytes  283 us
#         upcx <= upcx::buyram                {"payer":"alice","receiver":"alice","quant":"0.1000 SYS"}
#   upcx.token <= upcx.token::transfer        {"from":"alice","to":"upcx.ram","quantity":"0.0995 SYS","memo":"buy ram"}
#   upcx.token <= upcx.token::transfer        {"from":"alice","to":"upcx.ramfee","quantity":"0.0005 SYS","memo":"ram fee"}
#         alice <= upcx.token::transfer        {"from":"alice","to":"upcx.ram","quantity":"0.0995 SYS","memo":"buy ram"}
#     upcx.ram <= upcx.token::transfer        {"from":"alice","to":"upcx.ram","quantity":"0.0995 SYS","memo":"buy ram"}
#         alice <= upcx.token::transfer        {"from":"alice","to":"upcx.ramfee","quantity":"0.0005 SYS","memo":"ram fee"}
#  upcx.ramfee <= upcx.token::transfer        {"from":"alice","to":"upcx.ramfee","quantity":"0.0005 SYS","memo":"ram fee"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## Summary

In conclusion, by following these instructions you are able to purchase RAM, with a specified amount of tokens, for the specified accounts.
