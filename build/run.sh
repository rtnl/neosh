#!/bin/sh
set -e

export WORKDIR=$(mktemp -d)
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/local/lib"

cc -Wall -Wextra -fsanitize=address -g -o "${WORKDIR}/neosh" -lion ./src/*.c ./src/*.h
"${WORKDIR}/neosh"
