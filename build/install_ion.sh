#!/bin/sh
set -e

export WORKDIR=$(mktemp -d)

git clone https://github.com/rtnl/ion "${WORKDIR}/libion"
cd "${WORKDIR}/libion"
sudo ./build/install.sh
