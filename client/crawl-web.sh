#!/usr/bin/env bash

set -e

download () {
    local u="$1"
    local f="$2"
    echo "Download $u => $f"
    curl --compressed "$u" -o "$f"
}

URL="$1"

base="${URL%/*}"
file="$(basename -- $URL)"

echo "manifest:"
echo "  url: $URL"
echo "  base: $base"
echo "  file: $file"

mkdir -p web
cd web

download $URL $file

files="$(cat $file | grep -v '#' | grep -i '\.')"

for item in $files
do
    echo ""
    download "$base/$item" $item
done

