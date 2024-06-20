---
content_title: Download UPCX Source
---

To download the UPCX source code, clone the `upcx` repo and its submodules. It is adviced to create a home `upcx` folder first and download all the UPCX related software there:

```sh
mkdir -p ~/upcx && cd ~/upcx
git clone --recursive https://github.com/UPCX/upcx
```

## Update Submodules

If a repository is cloned without the `--recursive` flag, the submodules _must_ be updated before starting the build process:

```sh
cd ~/upcx/upcx
git submodule update --init --recursive
```

## Pull Changes

When pulling changes, especially after switching branches, the submodules _must_ also be updated. This can be achieved with the `git submodule` command as above, or using `git pull` directly:

```sh
[git checkout <branch>]  (optional)
git pull --recurse-submodules
```

[[info | What's Next?]]
| [Build UPCX binaries](02_build-upcx-binaries.md)
