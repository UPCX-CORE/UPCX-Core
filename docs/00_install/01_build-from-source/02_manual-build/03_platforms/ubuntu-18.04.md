---
content_title: Ubuntu 18.04
---

This section contains shell commands to manually download, build, install, test, and uninstall UPCXIO and dependencies on Ubuntu 18.04.

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
export UPCXIO_LOCATION=~/upcxio/upcx
export UPCXIO_INSTALL_LOCATION=$UPCXIO_LOCATION/../install
mkdir -p $UPCXIO_INSTALL_LOCATION
# install git
apt-get update && apt-get upgrade -y && DEBIAN_FRONTEND=noninteractive apt-get install -y git
# clone UPCXIO repository
git clone https://github.com/UPCXIO/upcx.git $UPCXIO_LOCATION
cd $UPCXIO_LOCATION && git submodule update --init --recursive
```

## Install UPCXIO Dependencies

These commands install the UPCXIO software dependencies. Make sure to [Download the UPCXIO Repository](#download-upcxio-repository) first and set the UPCXIO directories.

```sh
# install dependencies
apt-get install -y make bzip2 automake libbz2-dev libssl-dev doxygen graphviz libgmp3-dev \
    autotools-dev python2.7 python2.7-dev python3 python3-dev \
    autoconf libtool curl zlib1g-dev sudo ruby libusb-1.0-0-dev \
    libcurl4-gnutls-dev pkg-config patch llvm-7-dev clang-7 vim-common jq
# build cmake
export PATH=$UPCXIO_INSTALL_LOCATION/bin:$PATH
cd $UPCXIO_INSTALL_LOCATION && curl -LO https://cmake.org/files/v3.13/cmake-3.13.2.tar.gz && \
    tar -xzf cmake-3.13.2.tar.gz && \
    cd cmake-3.13.2 && \
    ./bootstrap --prefix=$UPCXIO_INSTALL_LOCATION && \
    make -j$(nproc) && \
    make install && \
    rm -rf $UPCXIO_INSTALL_LOCATION/cmake-3.13.2.tar.gz $UPCXIO_INSTALL_LOCATION/cmake-3.13.2
# build boost
cd $UPCXIO_INSTALL_LOCATION && curl -LO https://boostorg.jfrog.io/artifactory/main/release/1.71.0/source/boost_1_71_0.tar.bz2 && \
    tar -xjf boost_1_71_0.tar.bz2 && \
    cd boost_1_71_0 && \
    ./bootstrap.sh --prefix=$UPCXIO_INSTALL_LOCATION && \
    ./b2 --with-iostreams --with-date_time --with-filesystem --with-system --with-program_options --with-chrono --with-test -q -j$(nproc) install && \
    rm -rf $UPCXIO_INSTALL_LOCATION/boost_1_71_0.tar.bz2 $UPCXIO_INSTALL_LOCATION/boost_1_71_0
```

## Build UPCXIO

These commands build the UPCXIO software on the specified OS. Make sure to [Install UPCXIO Dependencies](#install-upcxio-dependencies) first.

[[caution | `UPCXIO_BUILD_LOCATION` environment variable]]
| Do NOT change this variable. It is set for convenience only. It should always be set to the `build` folder within the cloned repository.

```sh
export UPCXIO_BUILD_LOCATION=$UPCXIO_LOCATION/build
mkdir -p $UPCXIO_BUILD_LOCATION
cd $UPCXIO_BUILD_LOCATION && cmake -DCMAKE_BUILD_TYPE='Release' -DCMAKE_CXX_COMPILER='clang++-7' -DCMAKE_C_COMPILER='clang-7' -DLLVM_DIR='/usr/lib/llvm-7/lib/cmake/llvm' -DCMAKE_INSTALL_PREFIX=$UPCXIO_INSTALL_LOCATION $UPCXIO_LOCATION
cd $UPCXIO_BUILD_LOCATION && make -j$(nproc)
```

## Install UPCXIO

This command installs the UPCXIO software on the specified OS. Make sure to [Build UPCXIO](#build-upcxio) first.

```sh
cd $UPCXIO_BUILD_LOCATION && make install
```

## Test UPCXIO

These commands validate the UPCXIO software installation on the specified OS. Make sure to [Install UPCXIO](#install-upcxio) first. (**Note**: This task is optional but recommended.)

```sh
cd $UPCXIO_BUILD_LOCATION && make test
```

## Uninstall UPCXIO

These commands uninstall the UPCXIO software from the specified OS.

```sh
xargs rm < $UPCXIO_BUILD_LOCATION/install_manifest.txt
rm -rf $UPCXIO_BUILD_LOCATION
```
