#!/bin/bash

sudo rm -rf ./Build/ src.3gx src.elf
docker run --rm -v "$PWD":/src -w /src pablomk7/libctrpf:0.8.0 make
