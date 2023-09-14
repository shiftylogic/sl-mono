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

project( sl-vk )


#####################################
#
# Setup the main project library

add_library( ${PROJECT_NAME} INTERFACE )

target_include_directories( ${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries( ${PROJECT_NAME} INTERFACE
    sl-core
    vulkan
)

target_compile_definitions( ${PROJECT_NAME} INTERFACE
    SL_VKCORE_LOGGING_ENABLED
    # SL_VKCORE_TRACE_ENABLED
)


# Only necessary if this switches from INTERFACE to STATIC
# enable_warnings( ${PROJECT_NAME} )


###################
#
# Build examples

add_example(
    NAME vk-init
    SOURCES examples/vk-init.cpp
    LIBRARIES ${PROJECT_NAME}
)


###################
#
# Build tests

set( SL_VK_LIB_TEST_SRCS
    tests/core/device-resource-test.cpp
    tests/core/resource-test.cpp
)

build_tests(
    NAME vk-tests
    SOURCES ${SL_VK_LIB_TEST_SRCS}
    LIBRARIES ${PROJECT_NAME}
)
