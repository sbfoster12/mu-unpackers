#!/bin/bash

# Get absolute path to the root of the repo (assuming this script is in scripts/)
export UNPACKERS_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export CMAKE_PREFIX_PATH="$UNPACKERS_PATH/install:$CMAKE_PREFIX_PATH"

export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$UNPACKERS_PATH/install/include:$(pkg-config --cflags nlohmann_json 2>/dev/null | sed 's/-I//g')
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$UNPACKERS_PATH/install/lib
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$UNPACKERS_PATH/install/lib

echo "Environment configured from $UNPACKERS_PATH"
