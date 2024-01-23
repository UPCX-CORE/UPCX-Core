# UPCX - The Most Powerful Infrastructure for Decentralized Applications

[![Build status](https://badge.buildkite.com/370fe5c79410f7d695e4e34c500b4e86e3ac021c6b1f739e20.svg?branch=master)](https://buildkite.com/UPCX/upcx)

Welcome to the UPCX source code repository! This software enables businesses to rapidly build and deploy high-performance and high-security blockchain-based applications.

Some of the groundbreaking features of UPCX include:

1. Free Rate Limited Transactions
1. Low Latency Block confirmation (0.5 seconds)
1. Low-overhead Byzantine Fault Tolerant Finality
1. Designed for optional high-overhead, low-latency BFT finality
1. Smart contract platform powered by WebAssembly
1. Designed for Sparse Header Light Client Validation
1. Scheduled Recurring Transactions
1. Time Delay Security
1. Hierarchical Role Based Permissions
1. Support for Biometric Hardware Secured Keys (e.g. Apple Secure Enclave)
1. Designed for Parallel Execution of Context Free Validation Logic
1. Designed for Inter Blockchain Communication

## Disclaimer

Block.one is neither launching nor operating any initial public blockchains based upon the UPCX software. This release refers only to version 1.0 of our open source software. We caution those who wish to use blockchains built on UPCX to carefully vet the companies and organizations launching blockchains based on UPCX before disclosing any private keys to their derivative software.

## Official Testnet

[testnet.upcx.io](https://testnet.upcx.io/)

## Supported Operating Systems

UPCX currently supports the following operating systems:

1. Amazon Linux 2
2. CentOS 7
3. CentOS 7.x
4. CentOS 8
5. Ubuntu 16.04
6. Ubuntu 18.04
7. Ubuntu 20.04
8. MacOS 10.14 (Mojave)
9. MacOS 10.15 (Catalina)

---

**Note: It may be possible to install UPCX on other Unix-based operating systems. This is not officially supported, though.**

---

## Software Installation

If you are new to UPCX, it is recommended that you install the [UPCX Prebuilt Binaries](#prebuilt-binaries), then proceed to the [Getting Started Guide](https://developers.upcx.io/welcome/latest/getting-started-guide). If you are an advanced developer, a block producer, or no binaries are available for your platform, you may need to [Build UPCX from source](https://developers.upcx.io/manuals/upcx/latest/install/build-from-source).

---

**Note: If you used our scripts to build/install UPCX, please run the [Uninstall Script](#uninstall-script) before using our prebuilt binary packages.**

---

## Prebuilt Binaries

Prebuilt UPCX software packages are available for the operating systems below. Find and follow the instructions for your OS:

### Mac OS X:

#### Mac OS X Brew Install

```sh
brew tap upcx/upcx
brew install upcx
```

Note: On MacOS 10.15 (Catalina), there is a chance to face the linking error below which prevents successful installation of UPCX:

```
Reinstalling 1 broken dependent from source:
upcx/upcx/upcx
```

The following Homebrew commands will resolve this issue:

```sh
brew link upcx
```

#### Mac OS X Brew Uninstall

```sh
brew remove upcx
```

### Ubuntu Linux:

#### Ubuntu 20.04 Package Install

```sh
wget https://github.com/upcx/upcx/releases/download/v2.1.0/upcx_2.1.0-1-ubuntu-20.04_amd64.deb
sudo apt install ./upcx_2.1.0-1-ubuntu-20.04_amd64.deb
```

#### Ubuntu 18.04 Package Install

```sh
wget https://github.com/upcx/upcx/releases/download/v2.1.0/upcx_2.1.0-1-ubuntu-18.04_amd64.deb
sudo apt install ./upcx_2.1.0-1-ubuntu-18.04_amd64.deb
```

#### Ubuntu 16.04 Package Install

```sh
wget https://github.com/upcx/upcx/releases/download/v2.1.0/upcx_2.1.0-1-ubuntu-16.04_amd64.deb
sudo apt install ./upcx_2.1.0-1-ubuntu-16.04_amd64.deb
```

#### Ubuntu Package Uninstall

```sh
sudo apt remove upcx
```

### RPM-based (CentOS, Amazon Linux, etc.):

#### RPM Package Install CentOS 7

```sh
wget https://github.com/upcx/upcx/releases/download/v2.1.0/upcx-2.1.0-1.el7.x86_64.rpm
sudo yum install ./upcx-2.1.0-1.el7.x86_64.rpm
```

#### RPM Package Install CentOS 8

```sh
wget https://github.com/upcx/upcx/releases/download/v2.1.0/upcx-2.1.0-1.el8.x86_64.rpm
sudo yum install ./upcx-2.1.0-1.el8.x86_64.rpm
```

#### RPM Package Uninstall

```sh
sudo yum remove upcx
```

## Uninstall Script

To uninstall the UPCX built/installed binaries and dependencies, run:

```sh
./scripts/upcx_uninstall.sh
```

## Documentation

1. [Nodupcx](https://developers.upcx.io/manuals/upcx/latest/nodupcx)
   - [Usage](https://developers.upcx.io/manuals/upcx/latest/nodupcx/usage)
   - [Replays](https://developers.upcx.io/manuals/upcx/latest/nodupcx/replays)
   - [Chain API Reference](https://developers.upcx.io/manuals/upcx/latest/nodupcx/plugins/chain_api_plugin/api-reference)
   - [Troubleshooting](https://developers.upcx.io/manuals/upcx/latest/nodupcx/troubleshooting)
1. [Clupcx](https://developers.upcx.io/manuals/upcx/latest/clupcx)
1. [Kupcxd](https://developers.upcx.io/manuals/upcx/latest/kupcxd)

## Resources

1. [Website](https://upcx.io)
1. [Blog](https://medium.com/upcx)
1. [Developer Portal](https://developers.upcx.io)
1. [StackExchange for Q&A](https://upcx.stackexchange.com/)
1. [Community Telegram Group](https://t.me/UPCXProject)
1. [Developer Telegram Group](https://t.me/joinchat/EaEnSUPktgfoI-XPfMYtcQ)
1. [White Paper](https://github.com/UPCX/Documentation/blob/master/TechnicalWhitePaper.md)
1. [Roadmap](https://github.com/UPCX/Documentation/blob/master/Roadmap.md)

## Getting Started

Instructions detailing the process of getting the software, building it, running a simple test network that produces blocks, account creation and uploading a sample contract to the blockchain can be found in the [Getting Started Guide](https://developers.upcx.io/welcome/latest/getting-started-guide).

## Contributing

[Contributing Guide](./CONTRIBUTING.md)

[Code of Conduct](./CONTRIBUTING.md#conduct)

## License

UPCX is released under the open source [MIT](./LICENSE) license and is offered “AS IS” without warranty of any kind, express or implied. Any security provided by the UPCX software depends in part on how it is used, configured, and deployed. UPCX is built upon many third-party libraries such as WABT (Apache License) and WAVM (BSD 3-clause) which are also provided “AS IS” without warranty of any kind. Without limiting the generality of the foregoing, Block.one makes no representation or guarantee that UPCX or any third-party libraries will perform as intended or will be free of errors, bugs or faulty code. Both may fail in large or small ways that could completely or partially limit functionality or compromise computer systems. If you use or implement UPCX, you do so at your own risk. In no event will Block.one be liable to any party for any damages whatsoever, even if it had been advised of the possibility of damage.

## Important

See [LICENSE](./LICENSE) for copyright and license terms.

All repositories and other materials are provided subject to the terms of this [IMPORTANT](./IMPORTANT.md) notice and you must familiarize yourself with its terms. The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements. By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.
