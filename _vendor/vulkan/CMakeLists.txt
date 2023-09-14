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

project( vulkan )


#####################################
#
# Setup platform-specific variables
#

if( WIN32 )
    set( SL_VULKAN_PLATFORM_KHR "VK_USE_PLATFORM_WIN32_KHR" )
elseif( APPLE )
    set( SL_VULKAN_PLATFORM_KHR "VK_USE_PLATFORM_MACOS_MVK" )
else()
    message( FATAL_ERROR "Platform currently not supported." )
endif()


#####################################
#
# Setup the library

add_library( ${PROJECT_NAME} STATIC
    src/vma/vma.cpp
    src/volk/volk.c
)

target_compile_definitions( ${PROJECT_NAME} PUBLIC
    ${SL_VULKAN_PLATFORM_KHR}
    VOLK_VULKAN_H_PATH="vk.h"
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

enable_warnings( ${PROJECT_NAME} )
