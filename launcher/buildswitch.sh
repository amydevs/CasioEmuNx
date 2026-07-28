#!/bin/bash

cmake -S . -B "cmake-build-switch-arm64" -G "Ninja" \
   -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/Switch.cmake" \
   -DCMAKE_BUILD_TYPE="Release"

cmake --build cmake-build-switch-arm64 --target casiolauncher-nro