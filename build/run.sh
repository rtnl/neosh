#!/bin/sh
set -e

export WORKDIR=$(mktemp -d)

cargo run
