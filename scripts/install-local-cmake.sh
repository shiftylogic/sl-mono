#!/usr/bin/env bash

if [[ -z ${DL_CMAKE_VERSION} ]]; then
  DL_CMAKE_VERSION="$( cat CMAKE-VERSION )"
fi

echo "Installing CMake v${DL_CMAKE_VERSION}..."


################################################
##
## Figure out what platform we are running in
##

case "$OSTYPE" in
  darwin*)
    DL_CMAKE_ARCH_TAG="macos-universal"
    ;;
  linux*)
    DL_CMAKE_ARCH_TAG="linux-$(uname -m)"
    ;;
  *)
    echo "ERROR: Unsupported platform."
    ;;
esac


################################################
##
## Attempt to acquire CMake
##

DL_CMAKE_BASE="https://github.com/Kitware/CMake/releases/download"
DL_CMAKE_RELEASE_DIR="v${DL_CMAKE_VERSION}"
DL_CMAKE_RELEASE_FILE="cmake-${DL_CMAKE_VERSION}-${DL_CMAKE_ARCH_TAG}.tar.gz"
DL_CMAKE_URI="${DL_CMAKE_BASE}/${DL_CMAKE_RELEASE_DIR}/${DL_CMAKE_RELEASE_FILE}"

echo "  > Base:    ${DL_CMAKE_BASE}"
echo "  > Release: v${DL_CMAKE_VERSION}"
echo "  > File:    ${DL_CMAKE_RELEASE_FILE}"

mkdir -p .tools/cmake
curl -sL "${DL_CMAKE_URI}" | tar -xz --strip-components=1 -C .tools/cmake

echo "complete"

