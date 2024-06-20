#!/bin/bash
set -eo pipefail

NAME="$1"
UPCX_PREFIX="${PREFIX}/${SUBPREFIX}"
mkdir -p "${PREFIX}/bin/"
mkdir -p "${UPCX_PREFIX}/bin"
mkdir -p "${UPCX_PREFIX}/licenses/upcx"
mkdir -p "${UPCX_PREFIX}/etc/upcx/contracts"

# install binaries 
cp -R "${BUILD_DIR}"/bin/* ${UPCX_PREFIX}/bin

# install licenses
cp -R "${BUILD_DIR}"/licenses/upcx/* ${UPCX_PREFIX}/licenses

# install bios and boot contracts
cp -R "${BUILD_DIR}"/contracts/contracts/upcx.bios/upcx.bios.* ${UPCX_PREFIX}/etc/upcx/contracts
cp -R "${BUILD_DIR}"/contracts/contracts/upcx.boot/upcx.boot.* ${UPCX_PREFIX}/etc/upcx/contracts

for f in $(ls "${BUILD_DIR}/bin/"); do
    bn=$(basename "$f")
    ln -sf ../"${SUBPREFIX}/bin/$bn" "${PREFIX}/bin/$bn"
done
echo "Compressing '$NAME.tar.gz'..."
tar -cvzf "$NAME.tar.gz" ./"${PREFIX}"/*
rm -r "${PREFIX}"
