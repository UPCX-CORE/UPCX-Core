## Overview

This how-to guide provides instructions on how to vote for block producers.

## Before you begin

- Install the latest version of `clupcx`.

- Ensure the [reference system contracts](https://developers.upcx.io/manuals/upcxio.contracts/v1.9/build-and-deploy) are deployed and used to manage system resources.

- Understand the following:

  - What a [block producer](https://developers.upcx.io/welcome/v2.1/protocol-guides/consensus_protocol/#11-block-producers) is.
  - How [voting](https://developers.upcx.io/manuals/upcxio.contracts/v1.9/key-concepts/vote) works.

- Unlock your wallet.

## Command Reference

See the following reference guides for command line usage and related options for the `clupcx` command:

- The [clupcx system voteproducer prods](https://developers.upcx.io/manuals/upcx/v2.1/clupcx/command-reference/system/system-voteproducer-prods) reference.

## Procedure

The following steps show:

1. How to vote for blockproducer1 and blockproducer2 from an account called `upcxiotestts2`:

```sh
clupcx system voteproducer prods upcxiotestts2 blockproducer1 blockproducer2
```

Where:

- `upcxiotestts2` = the account that votes.
- `blockproducer1` and `blockproducer2` = the accounts receiving the votes. The number of accounts receiving the votes can vary from one to multiple. Maximum default number of block producers one account can vote for is 30.

Example output:

```console
executed transaction: 2d8b58f7387aef52a1746d7a22d304bbbe0304481d7751fc4a50b619df62676d  128 bytes  374 us
#         upcxio <= upcxio::voteproducer          {"voter":"upcxiotestts2","proxy":"","producers":["blockproducer1","blockproducer2"]}
```

## Summary

In conclusion, the above instructions show how to vote for block producers.
