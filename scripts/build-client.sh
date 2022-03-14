#!/usr/bin/env bash

set -e

# config
GZ_BAR="+5k"
GZ_EXCLUDE="usb.bin"


# exclude
EXCLUDE=""
for f in $GZ_EXCLUDE
do
  EXCLUDE="$EXCLUDE -not -name $f"
done

# build

cd client
pnpm i
pnpm run build

cd ..

# copy to data/
rm -f data/*

cp -r client/dist/* ./data/

# gz
cd data

find . -size "$GZ_BAR" $EXCLUDE -exec gzip -n {} \+

cd ..
