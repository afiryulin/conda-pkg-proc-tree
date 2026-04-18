#!/bin/bash

mkdir -p build && cd $_

cmake $CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$PREFIX -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
make install