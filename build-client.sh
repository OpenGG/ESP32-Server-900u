#!/usr/bin/env bash

cd client
pnpm i
pnpm run build

cd dist/*

find . -size +2k -not -name usb.bin -exec gzip -n {} \+

cd -

cd ..

rm -f data/*

cp -r client/dist/*/* ./data/
