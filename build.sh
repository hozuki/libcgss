#!/usr/bin/env bash

CLR_RED="\033[0;31m"
CLR_YELLOW="\033[1;33m"
CLR_DEF="\033[0m"

command -v cmake >/dev/null 2>&1 || {
  echo -e "${CLR_RED}CMake is not found. Aborting.${CLR_DEF}" >&2
  return 1
}

CMAKE_VERSION_FOUND=$(cmake --version | grep -Eo "cmake version ([[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+)" | cut -d\  -f3)
CMAKE_VERSION_MINIMUM=3.2

# https://stackoverflow.com/a/4024263
verlte() {
  [ "$1" = "$(echo -e "$1\n$2" | sort -V | head -n1)" ]
}

if [[ $(verlte "$CMAKE_VERSION_MINIMUM" "$CMAKE_VERSION_FOUND") ]]; then
  echo -e "${CLR_RED}Unmet requirement: CMake version${CLR_DEF}\n  required: >=${CMAKE_VERSION_MINIMUM}\n  found: ${CLR_YELLOW}${CMAKE_VERSION_FOUND}${CLR_DEF}"
  exit 1
fi

if [ ! -d "build" ]; then
  mkdir build
fi
cd build || exit 2
if [ ! -d "make" ]; then
  mkdir make
fi
cd make || exit 2
rm -rf ./*
cmake -DCMAKE_BUILD_TYPE=MinSizeRel ../..
make
cd ../..
echo Done.
exit 0
