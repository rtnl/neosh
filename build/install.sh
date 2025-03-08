#!/bin/sh
set -e

export WORKDIR=$(mktemp -d)
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/local/lib"

cc -Wall -Wextra -lion -lssl -lcrypto -o "${WORKDIR}/neosh" ./src/*.c

sudo install "${WORKDIR}/neosh" /usr/local/bin
