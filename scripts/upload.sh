#!/usr/bin/env bash

# upload files

set -xe

cd ./data

for filename in ./*; do
    curl -f -F "file=@${filename}" http://10.1.1.1/admin/files/upload
done

# curl "file=@"