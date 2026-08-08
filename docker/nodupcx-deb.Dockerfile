# syntax=docker/dockerfile:1.7

# Reproducible build of nodupcx.deb — the package the chain-node and sim images install.
#
# Ubuntu 20.04 is required rather than preferred: generate_deb.sh emits a control file
# depending on libssl1.1 and libtinfo5, neither of which exists on 22.04+.
#
# Two stages, so dependency resolution caches independently of the source:
#
#   deps  — runs the script against a stub project so it completes its dependency phase
#           and then configures a no-op build. Keyed on scripts/ alone, and about three
#           and a half minutes cold.
#   build — the real tree through the same entry point. ensure-boost/cmake/llvm find the
#           dependencies already present and go straight to the compile, which is the
#           long pole.
#
# Build locally:
#   docker buildx build -f docker/nodupcx-deb.Dockerfile --target export \
#     --output type=local,dest=./artifacts .

ARG UBUNTU_VERSION=20.04

FROM ubuntu:${UBUNTU_VERSION} AS deps

ENV DEBIAN_FRONTEND=noninteractive \
    TZ=Etc/UTC \
    UPCX_DEPS_DIR=/opt/upcx-deps

# upcx_build.sh installs the rest itself from scripts/upcx_build_ubuntu_deps; these are
# what it needs before it can run at all. `sudo` because ensure-sudo requires it on the
# PATH even when the build already runs as root.
RUN apt-get update -qq \
 && apt-get install -y -qq --no-install-recommends \
      git sudo curl ca-certificates lsb-release gnupg build-essential \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY scripts/ ./scripts/

# Everything after the dependency phase is cmake + make against whatever CMakeLists.txt
# is present, so a stub project is enough to reach the end of it. The git repo exists
# only to satisfy ensure-git-clone.
RUN printf 'cmake_minimum_required(VERSION 3.5)\nproject(upcx_deps_stub LANGUAGES CXX)\n' > CMakeLists.txt \
 && git init -q . \
 && git -c user.email=ci@upcx.local -c user.name=ci commit -q --allow-empty -m stub \
 && ./scripts/upcx_build.sh -y -i "${UPCX_DEPS_DIR}" \
 && rm -rf /src/build /src/CMakeLists.txt /src/.git /src/scripts


FROM deps AS build

# Baked into genesis_state::initial_key, so it fixes the genesis block, the chain id and
# the genesis authority of `upcx`. Empty keeps the CMakeLists default. LOCAL_CMAKE_FLAGS
# is only ever appended to by upcx_build.sh, so the environment reaches cmake intact.
ARG UPCX_ROOT_KEY=""

WORKDIR /src
COPY . .

RUN if [ -n "${UPCX_ROOT_KEY}" ]; then \
      export LOCAL_CMAKE_FLAGS="-DUPCX_ROOT_KEY='${UPCX_ROOT_KEY}'"; \
    fi; \
    ./scripts/upcx_build.sh -y -i "${UPCX_DEPS_DIR}"

# generate_package.sh resolves the binaries through a hardcoded relative BUILD_DIR of
# ../../build, so it only works from within <build dir>/packages.
RUN cd build/packages \
 && bash generate_package.sh deb \
 && mkdir -p /out \
 && cp ./*.deb /out/ \
 && cp "$(ls ./*.deb | head -1)" /out/nodupcx.deb


FROM scratch AS export
COPY --from=build /out/ /
