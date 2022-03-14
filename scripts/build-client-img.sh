#!/usr/bin/env bash

set -e

./scripts/build-client.sh

# fs size of 0x170000
python3 scripts/mklfs.py -t 1507328
