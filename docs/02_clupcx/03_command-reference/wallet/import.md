## Command

clupcx wallet import [OPTIONS]

**Where**

- [OPTIONS] = See Options in Command Usage section below.

**Note**: The arguments and options enclosed in square brackets are optional.

## Description

Imports private key into wallet. This command will launch `kupcxd` if it is not already running.

## Command Usage

The following information shows the different positionals and options you can use with the `clupcx wallet import` command:

### Positionals:

- none

### Options

- `-h,--help` - Print this help message and exit
- `-n, --name` _TEXT_ - The name of the wallet to import key into.
- `--private-key` _TEXT_ - Private key in WIF format to import.

## Requirements

- Install the currently supported version of `clupcx` and `kupcxd`.
  [[info | Note]]
  | `Clupcx` and `kupcxd` are bundled with the UPCXIO software. [Installing UPCXIO](../../00_install/index.md) will also install the `clupcx` and `kupcxd` command line tools.

## Examples

1. Import a private key to the default wallet. The wallet must be **open** and **unlocked**.

```shell
clupcx wallet import
```

The command asks for the private key. Enter it.

```shell
5KDNWQvY2seBPVUz7MiiaEDGTwACfuXu78bwZu7w2UDM9A3u3Fs
```

**Example Output**

```shell
private key: imported private key for: UPCX5zG7PsdtzQ9achTdRtXwHieL7yyigBFiJDRAQonqBsfKyL3XhC
```

2. Import a private key to a named wallet. The wallet must be `open` and `unlocked.`

```shell
clupcx wallet import --name my_wallet --private-key 5KDNWQvY2seBPVUz7MiiaEDGTwACfuXu78bwZu7w2UDM9A3u3Fs
```

**Where**
`--name` my_wallet = Tells the `clupcx wallet import` command to import the key to `my_wallet`
`--private-key` 5KDNWQvY2seBPVUz7MiiaEDGTwACfuXu78bwZu7w2UDM9A3u3Fs = Tells the `clupcx wallet import` command the private key to import

**Example Output**

```shell
imported private key for: UPCX5zG7PsdtzQ9achTdRtXwHieL7yyigBFiJDRAQonqBsfKyL3XhC
```
