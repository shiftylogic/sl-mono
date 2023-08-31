#!/usr/bin/env bash

if [[ -z ${DL_LLVM_VERSION} ]]; then
  DL_LLVM_VERSION="$( cat LLVM-VERSION )"
fi

echo "Installing LLVM + CLang v${DL_LLVM_VERSION}..."


################################################
##
## Figure out what platform we are running in
##

case "$OSTYPE" in
  darwin*)
    if [[ "$(uname -m)" == "arm64" ]]; then
      DL_LLVM_ARCH_TAG="arm64-apple-darwin22.0"
    else
      DL_LLVM_ARCH_TAG="x64_64-apple-darwin"
    fi
    ;;
  linux*)
    DL_LLVM_ARCH_TAG="$(uname -m)-linux-gnu"
    ;;
  *)
    echo "ERROR: Unsupported platform."
    ;;
esac


################################################
##
## Attempt to acquire an LLVM toolchain
##

DL_LLVM_BASE="https://github.com/llvm/llvm-project/releases/download"
DL_LLVM_RELEASE_DIR="llvmorg-${DL_LLVM_VERSION}"
DL_LLVM_RELEASE_FILE="clang+llvm-${DL_LLVM_VERSION}-${DL_LLVM_ARCH_TAG}.tar.xz"
DL_LLVM_URI="${DL_LLVM_BASE}/${DL_LLVM_RELEASE_DIR}/${DL_LLVM_RELEASE_FILE}"

echo "  > Base:    ${DL_LLVM_BASE}"
echo "  > Release: v${DL_LLVM_VERSION}"
echo "  > File:    ${DL_LLVM_RELEASE_FILE}"

mkdir -p .tools/llvm
curl -sL "${DL_LLVM_URI}" | tar -xJ --strip-components=1 -C .tools/llvm

echo "complete"

