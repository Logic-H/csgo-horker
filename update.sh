#!/bin/bash

if git pull | grep -q 'Already up to date.'; then
  echo "Nothing to update, exiting..."
  exit
fi

git submodule update --init --recursive --remote
./build.sh
