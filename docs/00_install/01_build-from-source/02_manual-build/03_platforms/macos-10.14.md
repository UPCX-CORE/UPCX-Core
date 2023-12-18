---
content_title: MacOS 10.14
---

This section contains shell commands to manually download, build, install, test, and uninstall UPCXIO and dependencies on MacOS 10.14.

[[info | Building UPCXIO is for Advanced Developers]]
| If you are new to UPCXIO, it is recommended that you install the [UPCXIO Prebuilt Binaries](../../../00_install-prebuilt-binaries.md) instead of building from source.

Select a task below, then copy/paste the shell commands to a Unix terminal to execute:

- [Download UPCXIO Repository](#download-upcxio-repository)
- [Install UPCXIO Dependencies](#install-upcxio-dependencies)
- [Build UPCXIO](#build-upcxio)
- [Install UPCXIO](#install-upcxio)
- [Test UPCXIO](#test-upcxio)
- [Uninstall UPCXIO](#uninstall-upcxio)

[[info | Building UPCXIO on another OS?]]
| Visit the [Build UPCXIO from Source](../../index.md) section.

## Download UPCXIO Repository

These commands set the UPCXIO directories, install git, and clone the UPCXIO repository.

```sh
# set UPCXIO directories
export EOSIO_LOCATION=~/upcxio/upcx
export EOSIO_INSTALL_LOCATION=$EOSIO_LOCATION/../install
mkdir -p $EOSIO_INSTALL_LOCATION
# install git
brew update && brew install git
# clone UPCXIO repository
git clone https://github.com/UPCXIO/upcx.git $EOSIO_LOCATION
cd $EOSIO_LOCATION && git submodule update --init --recursive
```

## Install UPCXIO Dependencies

These commands install the UPCXIO software dependencies. Make sure to [Download the UPCXIO Repository](#download-upcxio-repository) first and set the UPCXIO directories.

```sh
# install dependencies
brew install cmake python libtool libusb graphviz automake wget gmp pkgconfig doxygen openssl@1.1 jq boost || :
export PATH=$EOSIO_INSTALL_LOCATION/bin:$PATH
```

## Build UPCXIO

These commands build the UPCXIO software on the specified OS. Make sure to [Install UPCXIO Dependencies](#install-upcxio-dependencies) first.

[[caution | `EOSIO_BUILD_LOCATION` environment variable]]
| Do NOT change this variable. It is set for convenience only. It should always be set to the `build` folder within the cloned repository.

```sh
export EOSIO_BUILD_LOCATION=$EOSIO_LOCATION/build
mkdir -p $EOSIO_BUILD_LOCATION
cd $EOSIO_BUILD_LOCATION && cmake -DCMAKE_BUILD_TYPE='Release' -DCMAKE_INSTALL_PREFIX=$EOSIO_INSTALL_LOCATION $EOSIO_LOCATION
cd $EOSIO_BUILD_LOCATION && make -j$(getconf _NPROCESSORS_ONLN)
```

## Install UPCXIO

This command installs the UPCXIO software on the specified OS. Make sure to [Build UPCXIO](#build-upcxio) first.

```sh
cd $EOSIO_BUILD_LOCATION && make install
```

## Test UPCXIO

These commands validate the UPCXIO software installation on the specified OS. This task is optional but recommended. Make sure to [Install UPCXIO](#install-upcxio) first.

```sh
cd $EOSIO_BUILD_LOCATION && make test
```

## Uninstall UPCXIO

These commands uninstall the UPCXIO software from the specified OS.

```sh
xargs rm < $EOSIO_BUILD_LOCATION/install_manifest.txt
rm -rf $EOSIO_BUILD_LOCATION
```
