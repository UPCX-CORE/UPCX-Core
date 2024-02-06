---
content_title: Build UPCX Binaries
---

[[info | Shell Scripts]]
| The build script is one of various automated shell scripts provided in the UPCX repository for building, installing, and optionally uninstalling the UPCX software and its dependencies. They are available in the `upcx/scripts` folder.

The build script first installs all dependencies and then builds UPCX. The script supports these [Operating Systems](../../index.md#supported-operating-systems). To run it, first change to the `~/upcx/upcx` folder, then launch the script:

```sh
cd ~/upcx/upcx
./scripts/upcx_build.sh
```

The build process writes temporary content to the `upcx/build` folder. After building, the program binaries can be found at `upcx/build/programs`.

[[info | What's Next?]]
| [Installing UPCX](03_install-upcx-binaries.md) is strongly recommended after building from source as it makes local development significantly more friendly.
