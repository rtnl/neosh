#!/bin/sh
set -e

export WORKDIR=$(mktemp -d)
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/local/lib"

cc -Wall -Wextra -lion -o "${WORKDIR}/neosh" ./src/*.h ./src/*.c
sudo install "${WORKDIR}/neosh" /usr/local/bin
