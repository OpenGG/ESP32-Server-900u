#!/usr/bin/env bash

cp ESP32_Server_900u/build/**/*.merged.bin .

fn=$(basename *.merged.bin)

prefix="${fn/.bin/}"

zip $prefix.zip *.merged.bin
7z a $prefix.7z *.merged.bin
