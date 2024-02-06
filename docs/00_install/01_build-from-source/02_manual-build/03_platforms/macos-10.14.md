---
content_title: MacOS 10.14
---

This section contains shell commands to manually download, build, install, test, and uninstall UPCX and dependencies on MacOS 10.14.

[[info | Building UPCX is for Advanced Developers]]
| If you are new to UPCX, it is recommended that you install the [UPCX Prebuilt Binaries](../../../00_install-prebuilt-binaries.md) instead of building from source.

Select a task below, then copy/paste the shell commands to a Unix terminal to execute:

- [Download UPCX Repository](#download-upcx-repository)
- [Install UPCX Dependencies](#install-upcx-dependencies)
- [Build UPCX](#build-upcx)
- [Install UPCX](#install-upcx)
- [Test UPCX](#test-upcx)
- [Uninstall UPCX](#uninstall-upcx)

[[info | Building UPCX on another OS?]]
| Visit the [Build UPCX from Source](../../index.md) section.

## Download UPCX Repository

These commands set the UPCX directories, install git, and clone the UPCX repository.

```sh
# set UPCX directories
export UPCX_LOCATION=~/upcx/upcx
export UPCX_INSTALL_LOCATION=$UPCX_LOCATION/../install
mkdir -p $UPCX_INSTALL_LOCATION
# install git
brew update && brew install git
# clone UPCX repository
git clone https://github.com/UPCX/upcx.git $UPCX_LOCATION
cd $UPCX_LOCATION && git submodule update --init --recursive
```

## Install UPCX Dependencies

These commands install the UPCX software dependencies. Make sure to [Download the UPCX Repository](#download-upcx-repository) first and set the UPCX directories.

```sh
# install dependencies
brew install cmake python libtool libusb graphviz automake wget gmp pkgconfig doxygen openssl@1.1 jq boost || :
export PATH=$UPCX_INSTALL_LOCATION/bin:$PATH
```

## Build UPCX

These commands build the UPCX software on the specified OS. Make sure to [Install UPCX Dependencies](#install-upcx-dependencies) first.

[[caution | `UPCX_BUILD_LOCATION` environment variable]]
| Do NOT change this variable. It is set for convenience only. It should always be set to the `build` folder within the cloned repository.

```sh
export UPCX_BUILD_LOCATION=$UPCX_LOCATION/build
mkdir -p $UPCX_BUILD_LOCATION
cd $UPCX_BUILD_LOCATION && cmake -DCMAKE_BUILD_TYPE='Release' -DCMAKE_INSTALL_PREFIX=$UPCX_INSTALL_LOCATION $UPCX_LOCATION
cd $UPCX_BUILD_LOCATION && make -j$(getconf _NPROCESSORS_ONLN)
```

## Install UPCX

This command installs the UPCX software on the specified OS. Make sure to [Build UPCX](#build-upcx) first.

```sh
cd $UPCX_BUILD_LOCATION && make install
```

## Test UPCX

These commands validate the UPCX software installation on the specified OS. This task is optional but recommended. Make sure to [Install UPCX](#install-upcx) first.

```sh
cd $UPCX_BUILD_LOCATION && make test
```

## Uninstall UPCX

These commands uninstall the UPCX software from the specified OS.

```sh
xargs rm < $UPCX_BUILD_LOCATION/install_manifest.txt
rm -rf $UPCX_BUILD_LOCATION
```
