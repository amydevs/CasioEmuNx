#!/bin/bash

cmake -S . -B build -G "Ninja"

cmake --build build

cmake --install build \
    --prefix "$HOME/.local"