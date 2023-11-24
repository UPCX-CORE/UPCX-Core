#!/bin/bash
set -eo pipefail

NAME="$1"
EOS_PREFIX="${PREFIX}/${SUBPREFIX}"
mkdir -p "${PREFIX}/bin/"
mkdir -p "${EOS_PREFIX}/bin"
mkdir -p "${EOS_PREFIX}/licenses/upcxio"
mkdir -p "${EOS_PREFIX}/etc/upcxio/contracts"

# install binaries 
cp -R "${BUILD_DIR}"/bin/* ${EOS_PREFIX}/bin

# install licenses
cp -R "${BUILD_DIR}"/licenses/upcxio/* ${EOS_PREFIX}/licenses

# install bios and boot contracts
cp -R "${BUILD_DIR}"/contracts/contracts/upcxio.bios/upcxio.bios.* ${EOS_PREFIX}/etc/upcxio/contracts
cp -R "${BUILD_DIR}"/contracts/contracts/upcxio.boot/upcxio.boot.* ${EOS_PREFIX}/etc/upcxio/contracts

for f in $(ls "${BUILD_DIR}/bin/"); do
    bn=$(basename "$f")
    ln -sf ../"${SUBPREFIX}/bin/$bn" "${PREFIX}/bin/$bn"
done
echo "Compressing '$NAME.tar.gz'..."
tar -cvzf "$NAME.tar.gz" ./"${PREFIX}"/*
rm -r "${PREFIX}"
