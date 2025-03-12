#!/bin/sh
set -e

export WORKDIR=$(mktemp -d)

cargo build --release --target-dir "${WORKDIR}"

sudo install "${WORKDIR}/release/neosh" /usr/local/bin
