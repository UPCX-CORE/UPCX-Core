---
content_title: Centos 7.7
---

This section contains shell commands to manually download, build, install, test, and uninstall UPCX and dependencies on Centos 7.7.

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
yum update -y && yum install -y git
# clone UPCX repository
git clone https://github.com/UPCX/upcx.git $UPCX_LOCATION
cd $UPCX_LOCATION && git submodule update --init --recursive
```

## Install UPCX Dependencies

These commands install the UPCX software dependencies. Make sure to [Download the UPCX Repository](#download-upcx-repository) first and set the UPCX directories.

```sh
# install dependencies
yum update -y && \
    yum install -y epel-release && \
    yum --enablerepo=extras install -y centos-release-scl && \
    yum --enablerepo=extras install -y devtoolset-8 && \
    yum --enablerepo=extras install -y which git autoconf automake libtool make bzip2 doxygen \
    graphviz bzip2-devel openssl-devel gmp-devel ocaml \
    python python-devel rh-python36 file libusbx-devel \
    libcurl-devel patch vim-common jq llvm-toolset-7.0-llvm-devel llvm-toolset-7.0-llvm-static
# build cmake
export PATH=$UPCX_INSTALL_LOCATION/bin:$PATH
cd $UPCX_INSTALL_LOCATION && curl -LO https://cmake.org/files/v3.13/cmake-3.13.2.tar.gz && \
    source /opt/rh/devtoolset-8/enable && \
    tar -xzf cmake-3.13.2.tar.gz && \
    cd cmake-3.13.2 && \
    ./bootstrap --prefix=$UPCX_INSTALL_LOCATION && \
    make -j$(nproc) && \
    make install && \
    rm -rf $UPCX_INSTALL_LOCATION/cmake-3.13.2.tar.gz $UPCX_INSTALL_LOCATION/cmake-3.13.2
# apply clang patch
cp -f $UPCX_LOCATION/scripts/clang-devtoolset8-support.patch /tmp/clang-devtoolset8-support.patch
# build boost
cd $UPCX_INSTALL_LOCATION && curl -LO https://boostorg.jfrog.io/artifactory/main/release/1.71.0/source/boost_1_71_0.tar.bz2 && \
    source /opt/rh/devtoolset-8/enable && \
    tar -xjf boost_1_71_0.tar.bz2 && \
    cd boost_1_71_0 && \
    ./bootstrap.sh --prefix=$UPCX_INSTALL_LOCATION && \
    ./b2 --with-iostreams --with-date_time --with-filesystem --with-system --with-program_options --with-chrono --with-test -q -j$(nproc) install && \
    rm -rf $UPCX_INSTALL_LOCATION/boost_1_71_0.tar.bz2 $UPCX_INSTALL_LOCATION/boost_1_71_0
```

## Build UPCX

These commands build the UPCX software on the specified OS. Make sure to [Install UPCX Dependencies](#install-upcx-dependencies) first.

[[caution | `UPCX_BUILD_LOCATION` environment variable]]
| Do NOT change this variable. It is set for convenience only. It should always be set to the `build` folder within the cloned repository.

```sh
export UPCX_BUILD_LOCATION=$UPCX_LOCATION/build
mkdir -p $UPCX_BUILD_LOCATION
cd $UPCX_BUILD_LOCATION && source /opt/rh/devtoolset-8/enable && cmake -DCMAKE_BUILD_TYPE='Release' -DLLVM_DIR='/opt/rh/llvm-toolset-7.0/root/usr/lib64/cmake/llvm' -DCMAKE_INSTALL_PREFIX=$UPCX_INSTALL_LOCATION $UPCX_LOCATION
cd $UPCX_BUILD_LOCATION && make -j$(nproc)
```

## Install UPCX

This command installs the UPCX software on the specified OS. Make sure to [Build UPCX](#build-upcx) first.

```sh
cd $UPCX_BUILD_LOCATION && make install
```

## Test UPCX

These commands validate the UPCX software installation on the specified OS. This task is optional but recommended. Make sure to [Install UPCX](#install-upcx) first.

```sh
cd $UPCX_BUILD_LOCATION && source /opt/rh/rh-python36/enable && make test
```

## Uninstall UPCX

These commands uninstall the UPCX software from the specified OS.

```sh
xargs rm < $UPCX_BUILD_LOCATION/install_manifest.txt
rm -rf $UPCX_BUILD_LOCATION
```
