#!/bin/bash

docker run --rm -v "$PWD":/src -w /src pablomk7/libctrpf:0.8.0 make clean

docker run --rm -v "$PWD":/src -w /src pablomk7/libctrpf:0.8.0 make
