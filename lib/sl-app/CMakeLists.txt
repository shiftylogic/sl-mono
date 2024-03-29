# MIT License
#
# Copyright (c) 2023-present Robert Anderson
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FOM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required( VERSION 3.13 )

project( sl-app )

add_library( ${PROJECT_NAME} INTERFACE )

target_include_directories( ${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries( ${PROJECT_NAME} INTERFACE
    sl-core
    glfw
    unordered_dense
)

# Only necessary if this switches from INTERFACE to STATIC
# enable_warnings( ${PROJECT_NAME} )

###################
#
# Build examples

add_example(
    NAME win-bare
    SOURCES examples/win-bare.cpp
    LIBRARIES sl-core ${PROJECT_NAME}
)

add_example(
    NAME win-input
    SOURCES examples/win-input.cpp
    LIBRARIES sl-core ${PROJECT_NAME}
)


###################
#
# Build tests

# set( SL_APP_LIB_TEST_SRCS
# )
#
# build_tests(
#     NAME app-tests
#     SOURCES ${SL_APP_LIB_TEST_SRCS}
#     LIBRARIES ${PROJECT_NAME}
# )
