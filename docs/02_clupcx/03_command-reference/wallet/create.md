## Description

Creates a wallet with the specified name. If no name is given, the wallet will be created with the name 'default'

## Positionals

None

## Options

- `-n, --name` _TEXT_ - The name of the new wallet
- `-f, --file` _TEXT_ - Name of file to write wallet password output to. (Must be set, unless "--to-console" is passed
- `--to-console` - Print password to console

## Command

clupcx wallet create [OPTIONS]

**Where**

- [OPTIONS] = See Options in Command Usage section below.

**Note**: The arguments and options enclosed in square brackets are optional.

## Description

Create a new wallet locally.

## Command Usage

The following information shows the different positionals and options you can use with the `clupcx wallet create` command:

### Positionals:

- none

### Options

`-h`, `--help` - Print this help message and exit
`-n`, `--name` _TEXT_ - The name of the new wallet, if blank the name is `default`
`-f`, `--file` _TEXT_ - Name of file to write wallet password output to. Must be set, unless "--to-console" is passed
`--to-console` - Print password to console.

## Requirements

- Install the currently supported version of `clupcx` and `kupcxd`.
  [[info | Note]]
  | `Clupcx` and `kupcxd` are bundled with the UPCX software. [Installing UPCX](../../00_install/index.md) will also install the `clupcx` and `kupcxd` command line tools.

## Examples

1. Create a new wallet called `default` and output the wallet password to the screen

```shell
clupcx wallet create  --to-console
```

**Where**
`--to-console` = Tells the `clupcx wallet create` command to print the wallet password to the console.

**Example Output**

```shell
Creating wallet: default
Save password to use in the future to unlock this wallet.
Without password imported keys will not be retrievable.
"PW5HuN7hkCBdMNQAa8z3NuThp3LSDtjv77odQYs5t2wgghJq4MUxo"
```

2. Create a new wallet called `my_wallet` and output the wallet password to a file called `my_wallet_password.txt`

```shell
clupcx wallet create --name my_wallet --file my_wallet_passwords.txt
```

**Where**
`--name` my_wallet = Tells the `clupcx wallet create` command to create a wallet called `my_wallet_password.txt`
`--file` my_wallet_passwords.txt = Tells the `clupcx wallet create` command to store the password in a file called `my_wallet_password.txt`.

**Example Output**

```shell
Creating wallet: my_wallet
Save password to use in the future to unlock this wallet.
Without password imported keys will not be retrievable.
saving password to my_wallet_passwords.txt
```

```shell
cat my_wallet_passwords.txt
```

**Example Output**

```shell
PW5JZaEQXoTKWZRg313aTYS7iNh2jMH4Xs7zWS3vrTZ4p5hCSFGFH(base)
```

```shell
clupcx wallet list
```

**Example Output**

```shell
Wallets:
[
  "default *",
  "my_wallet *"
]
```
