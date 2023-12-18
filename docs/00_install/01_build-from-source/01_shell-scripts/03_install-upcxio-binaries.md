---
content_title: Install UPCXIO Binaries
---

## UPCXIO install script

For ease of contract development, content can be installed at the `/usr/local` folder using the `upcxio_install.sh` script within the `upcx/scripts` folder. Adequate permission is required to install on system folders:

```sh
cd ~/upcxio/upcx
./scripts/upcxio_install.sh
```

## UPCXIO manual install

In lieu of the `upcxio_install.sh` script, you can install the UPCXIO binaries directly by invoking `make install` within the `upcx/build` folder. Again, adequate permission is required to install on system folders:

```sh
cd ~/upcxio/upcx/build
make install
```

[[info | What's Next?]]
| Configure and use [Nodeos](../../../01_nodeos/index.md), or optionally [Test the UPCXIO binaries](04_test-upcxio-binaries.md).
