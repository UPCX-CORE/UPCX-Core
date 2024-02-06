# Bios Boot Tutorial

The `bios-boot-tutorial.py` script simulates the UPCX bios boot sequence.

## Prerequisites

1. Python 3.x
2. CMake
3. git

## Steps

1. Install upcx binaries by following the steps outlined in below tutorial
   [Install upcx binaries](https://github.com/UPCX/upcx#mac-os-x-brew-install)

2. Install upcx.cdt binaries by following the steps outlined in below tutorial
   [Install upcx.cdt binaries](https://github.com/UPCX/upcx.cdt#binary-releases)

3. Compile upcx.contracts sources repository by following the [compile upcx.contracts guidelines](https://github.com/UPCX/upcx.contracts/blob/master/docs/02_compile-and-deploy.md) first part, the deploying steps from those guidelines should not be executed.

4. Make note of the full path of the directory where the contracts were compiled, if you followed the [compile upcx.contracts guidelines](https://github.com/UPCX/upcx.contracts/blob/master/docs/02_compile-and-deploy.md) it should be under the `build` folder, in `build/contracts/`, we'll reference it from now on as `UPCX_CONTRACTS_DIRECTORY`

5. Launch the `bios-boot-tutorial.py` script
   Minimal command line to launch the script below, make sure you replace `UPCX_CONTRACTS_DIRECTORY` with actual directory

```bash
$ cd ~
$ git clone https://github.com/UPCX/upcx.git
$ cd ./upcx/tutorials/bios-boot-tutorial/
$ python3 bios-boot-tutorial.py --clupcx=clupcx --nodupcx=nodupcx --kupcxd=kupcxd --contracts-dir="UPCX_CONTRACTS_DIRECTORY" -w -a

```

See [UPCX Documentation Wiki: Tutorial - Bios Boot](https://github.com/UPCX/upcx/wiki/Tutorial-Bios-Boot-Sequence) for additional information.
