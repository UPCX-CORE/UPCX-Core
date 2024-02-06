---
content_title: Install UPCX Binaries
---

## UPCX install script

For ease of contract development, content can be installed at the `/usr/local` folder using the `upcx_install.sh` script within the `upcx/scripts` folder. Adequate permission is required to install on system folders:

```sh
cd ~/upcx/upcx
./scripts/upcx_install.sh
```

## UPCX manual install

In lieu of the `upcx_install.sh` script, you can install the UPCX binaries directly by invoking `make install` within the `upcx/build` folder. Again, adequate permission is required to install on system folders:

```sh
cd ~/upcx/upcx/build
make install
```

[[info | What's Next?]]
| Configure and use [Nodupcx](../../../01_nodupcx/index.md), or optionally [Test the UPCX binaries](04_test-upcx-binaries.md).
