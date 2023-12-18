---
content_title: Download UPCXIO Source
---

To download the UPCXIO source code, clone the `upcx` repo and its submodules. It is adviced to create a home `upcxio` folder first and download all the UPCXIO related software there:

```sh
mkdir -p ~/upcxio && cd ~/upcxio
git clone --recursive https://github.com/UPCXIO/upcx
```

## Update Submodules

If a repository is cloned without the `--recursive` flag, the submodules _must_ be updated before starting the build process:

```sh
cd ~/upcxio/upcx
git submodule update --init --recursive
```

## Pull Changes

When pulling changes, especially after switching branches, the submodules _must_ also be updated. This can be achieved with the `git submodule` command as above, or using `git pull` directly:

```sh
[git checkout <branch>]  (optional)
git pull --recurse-submodules
```

[[info | What's Next?]]
| [Build UPCXIO binaries](02_build-upcxio-binaries.md)
