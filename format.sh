#!/usr/bin/env bash

cd ESP32_Server_900u

clang-format -i -style=WebKit *.cpp *.h *.ino
