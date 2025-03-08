#!/bin/sh
set -e

export WORKDIR=$(mktemp -d)
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/local/lib"

echo "running tests"
cc -DTEST -lcriterion -lion -lssl -lcrypto -g -o "${WORKDIR}/neosh_test" ./src/*.h ./src/*.c
"${WORKDIR}/neosh_test" --verbose
