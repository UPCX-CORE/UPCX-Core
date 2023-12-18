## Goal

Create a keypair consisting of a public and a private key for signing transactions in the UPCXIO blockchain.

## Before you begin

Before you follow the steps to create a new key pair, make sure the following items are fulfilled:

- Install the currently supported version of `clupcx`

[[info | Note]]
| The clupcx tool is bundled with the UPCXIO software. [Installing UPCXIO](../../00_install/index.md) will also install the clupcx tool.

- Acquire functional understanding of asymmetric cryptography (public and private keypair) in the context of blockchain

## Steps

To create a keypair and print the result to the console:

```sh
clupcx create key --to-console
```

**Example Output**

```sh
clupcx create key --to-console
Private key: 5KPzrqNMJdr6AX6abKg*******************************cH
Public key: UPCX4wSiQ2jbYGrqiiKCm8oWR88NYoqnmK4nNL1RCtSQeSFkGtqsNc
```

To create a keypair and save it to a file:

```sh
clupcx create key --file FILE_TO_SAVEKEY
```

Where: FILE_TO_SAVEKEY = name of the file

**Example Output**

```sh
clupcx create key --file pw.txt
saving keys to pw.txt
```

To view the saved keypair in the file:

```sh
cat pw.txt
Private key: 5K7************************************************
Public key: UPCX71k3WdpLDeqeyqVRAAxwpz6TqXwDo9Brik5dQhdvvpeTKdNT59
```
