# Bios Boot Tutorial

The `bios-boot-tutorial.py` script simulates the UPCXIO bios boot sequence.

## Prerequisites

1. Python 3.x
2. CMake
3. git

## Steps

1. Install upcxio binaries by following the steps outlined in below tutorial
   [Install upcxio binaries](https://github.com/UPCXIO/upcx#mac-os-x-brew-install)

2. Install upcxio.cdt binaries by following the steps outlined in below tutorial
   [Install upcxio.cdt binaries](https://github.com/UPCXIO/upcxio.cdt#binary-releases)

3. Compile upcxio.contracts sources repository by following the [compile upcxio.contracts guidelines](https://github.com/UPCXIO/upcxio.contracts/blob/master/docs/02_compile-and-deploy.md) first part, the deploying steps from those guidelines should not be executed.

4. Make note of the full path of the directory where the contracts were compiled, if you followed the [compile upcxio.contracts guidelines](https://github.com/UPCXIO/upcxio.contracts/blob/master/docs/02_compile-and-deploy.md) it should be under the `build` folder, in `build/contracts/`, we'll reference it from now on as `EOSIO_CONTRACTS_DIRECTORY`

5. Launch the `bios-boot-tutorial.py` script
   Minimal command line to launch the script below, make sure you replace `EOSIO_CONTRACTS_DIRECTORY` with actual directory

```bash
$ cd ~
$ git clone https://github.com/UPCXIO/upcx.git
$ cd ./upcx/tutorials/bios-boot-tutorial/
$ python3 bios-boot-tutorial.py --cleos=cleos --nodeos=nodeos --keosd=keosd --contracts-dir="EOSIO_CONTRACTS_DIRECTORY" -w -a

```

See [UPCXIO Documentation Wiki: Tutorial - Bios Boot](https://github.com/UPCXIO/upcx/wiki/Tutorial-Bios-Boot-Sequence) for additional information.
