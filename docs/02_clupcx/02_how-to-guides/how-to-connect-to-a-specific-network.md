## Overview

This guide provides instructions on how to connect to specifc UPCX blockchain when using `clupcx`. `Clupcx` can connect to a specific node by using the `--url` optional argument, followed by the http address and port number.

The examples use the `--url`optional argument to send commands to the specified blockchain.

[[info | Default address:port]]
| If no optional arguments are used (i.e. no `--url`), `clupcx` attempts to connect to a local `nodupcx` running at localhost or `127.0.0.1` and default port `8888`. Use the `nodupcx` command line arguments or [config.ini](../../01_nodupcx/02_usage/01_nodupcx-configuration/#configini-location) file to specify a different address.

## Before you Begin

Make sure you meet the following requirements:

- Install the currently supported version of `clupcx`.
  [[info | Note]]
  | `Clupcx` is bundled with the UPCX software. [Installing UPCX](../../00_install/index.md) will install the `clupcx` and `kupcxd` command line tools.
- You have access to an UPCX blockchain and the http afddress and port number of a `nodupcx` instance.

## Reference

See the following reference guides for command line usage and related options:

- [clupcx](../index.md) command

## Example

1. Add the `-url` option to specify the `nodupcx` instance

```shell
clupcx -url http://nodupcx-host:8888 COMMAND
```

**Where**

- `-url http://nodupcx-host:8888` = The http address and port number of the `nodupcx` instance to connect to
- COMMAND = The `clupcx`command.

## Summary

In conclusion, by following these instructions you are able to connect to a specified `nodupcx` instance.
