#!/usr/bin/env bash

CMAKE_ROOT=$PWD/.tools/cmake
TOOLCHAIN_ROOT=$PWD/.tools/llvm

case "$OSTYPE" in
  darwin*)
    CMAKE_TOOL=${CMAKE_ROOT}/CMake.app/Contents/bin/cmake
    ;;
  *)
    CMAKE_TOOL=${CMAKE_ROOT}/bin/cmake
    ;;
esac



#
# Make sure we have a local 'recent' CMake
#
if [[ ! -d "$PWD/.tools/cmake" ]];
then
  sh "$PWD/scripts/install-local-cmake.sh"
fi

#
# Make sure we have a local 'recent' LLVM / CLang
#
if [[ ! -d "$PWD/.tools/llvm" ]];
then
  sh "$PWD/scripts/install-local-llvm.sh"
fi


#
# If 'build' exists, delete it.
#
if [[ -d build ]];
then
  rm -rf build
fi

# Then re-create it for builds
mkdir -p build
cd build


#
# Use the custom CMake and make sure it uses our local CLang
#
${CMAKE_TOOL} \
  -G Ninja \
  -DCMAKE_C_COMPILER=${TOOLCHAIN_ROOT}/bin/clang \
  -DCMAKE_CXX_COMPILER=${TOOLCHAIN_ROOT}/bin/clang++ \
  ..

