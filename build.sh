#!/usr/bin/env bash

CLR_RED="\033[0;31m"
CLR_YELLOW="\033[1;33m"
CLR_DEF="\033[0m"

command -v cmake >/dev/null 2>&1 || { echo -e >&2 "${CLR_RED}CMake is not found. Aborting.${CLR_DEF}"; return 1; }

CMAKE_VERSION=$(cmake --version | grep -Eo "cmake version ([[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+)" | cut -d\  -f3)

if [[ ${CMAKE_VERSION} < "3.2" ]]; then
    echo -e "${CLR_RED}Unmet requirement: CMake version${CLR_DEF}\n  required: >=3.2\n  found: ${CLR_YELLOW}${CMAKE_VERSION}${CLR_DEF}";
    return 1;
fi

if [ ! -d "build" ]; then
    mkdir build
fi
cd build
if [ ! -d "make" ]; then
    mkdir make
fi
cd make
rm -rf *
cmake -DCMAKE_BUILD_TYPE=MinSizeRel ../..
make
cd ../..
echo Done.
return 0
