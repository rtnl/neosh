#!/bin/sh
set -e

export WORKDIR=$(mktemp -d)
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/local/lib"

cc -Wall -Wextra -fsanitize=address -lion -lssl -lcrypto -g -o "${WORKDIR}/neosh" ./src/*.c
"${WORKDIR}/neosh"
