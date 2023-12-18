---
content_title: Nodupcx
---

## Introduction

`nodupcx` is the core service daemon that runs on every UPCXIO node. It can be configured to process smart contracts, validate transactions, produce blocks containing valid transactions, and confirm blocks to record them on the blockchain.

## Installation

`nodupcx` is distributed as part of the [UPCXIO software suite](https://github.com/UPCXIO/upcx/blob/master/README.md). To install `nodupcx`, visit the [UPCXIO Software Installation](../00_install/index.md) section.

## Explore

Navigate the sections below to configure and use `nodupcx`.

- [Usage](02_usage/index.md) - Configuring and using `nodupcx`, node setups/environments.
- [Plugins](03_plugins/index.md) - Using plugins, plugin options, mandatory vs. optional.
- [Replays](04_replays/index.md) - Replaying the chain from a snapshot or a blocks.log file.
- [RPC APIs](05_rpc_apis/index.md) - Remote Procedure Call API reference for plugin HTTP endpoints.
- [Logging](06_logging/index.md) - Logging config/usage, loggers, appenders, logging levels.
- [Concepts](07_concepts/index.md) - `nodupcx` concepts, explainers, implementation aspects.
- [Troubleshooting](08_troubleshooting/index.md) - Common `nodupcx` troubleshooting questions.
- [Deprecation Notices](https://github.com/UPCXIO/upcx/issues/7597) - Lists `nodupcx` deprecated functionality.

[[info | Access Node]]
| A local or remote UPCXIO access node running `nodupcx` is required for a client application or smart contract to interact with the blockchain.
