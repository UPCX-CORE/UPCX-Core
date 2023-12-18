## Overview

This guide provides instructions on how to connect to specifc wallet when using `clupcx`. `Clupcx` can connect to a specific wallet by using the `--wallet-url` optional argument, followed by the http address and port number.

The example uses the `wallet-url` optional arguments to request data from the the specified `kupcxd` instance.

[[info | Default address:port]]
| If no optional arguments are used (i.e. no `--wallet-url`), `clupcx` attempts to connect to a local `nodupcx` or `kupcxd` running at localhost or `127.0.0.1` and default port `8900`. Use the `kupcxd` command line arguments or [config.ini](../../03_kupcxd/10_usage.md/#launching-kupcxd-manually) file to specify a different address.

## Before you Begin

Make sure you meet the following requirements:

- Install the currently supported version of `clupcx` and `kupcxd`.
  [[info | Note]]
  | The `clupcx` tool and `kupcxd` are bundled with the UPCXIO software. [Installing UPCXIO](../../00_install/index.md) will install the `clupcx` and `kupcxd` command line tools.
- You have access to an UPCXIO blockchain and the http address and port number of a `nodupcx` instance.

## Reference

See the following reference guides for command line usage and related options:

- [clupcx](../index.md) command

## Example

1. Add the `--wallet-url` option to specify the `kupcxd` instance

```shell
clupcx --wallet-url http://kupcxd-host:8900 COMMAND
```

**where**

- `--wallet-url http://kupcxd-host:8900` = The http address and port number of the `kupcxd` instance to connect to
- COMMAND = The `clupcx`command.

## Summary

In conclusion, by following these instructions you are able to connect to a specified `kupcxd` instance.
