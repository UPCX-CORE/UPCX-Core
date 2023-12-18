---
content_title: UPCXIO Overview
---

UPCXIO is the next-generation blockchain platform for creating and deploying smart contracts and distributed applications. UPCXIO comes with a number of programs. The primary ones included in UPCXIO are the following:

- [Nodeos](01_nodeos/index.md) (node + upcx = nodeos) - Core service daemon that runs a node for block production, API endpoints, or local development.
- [Cleos](02_cleos/index.md) (cli + upcx = cleos) - Command line interface to interact with the blockchain (via `nodeos`) and manage wallets (via `keosd`).
- [Keosd](03_keosd/index.md) (key + upcx = keosd) - Component that manages UPCXIO keys in wallets and provides a secure enclave for digital signing.

The basic relationship between these components is illustrated in the diagram below.

![UPCXIO components](upcxio_components.png)

Additional UPCXIO Resources:

- [UPCXIO Utilities](10_utilities/index.md) - Utilities that complement the UPCXIO software.
- [Upgrade Guides](20_upgrade-guides/index.md) - UPCXIO version/protocol upgrade guides.

[[info | What's Next?]]
| [Install the UPCXIO Software](00_install/index.md) before exploring the sections above.
