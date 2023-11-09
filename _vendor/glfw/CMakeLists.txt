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
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required( VERSION 3.13 )

project( glfw-root )

set( SL_GLFW_VERSION "3.3.8" )

file( ARCHIVE_EXTRACT
    INPUT "${CMAKE_CURRENT_SOURCE_DIR}/glfw-${SL_GLFW_VERSION}.zip"
    DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}"
)

set( GLFW_BUILD_EXAMPLES OFF )
set( GLFW_BUILD_TEST OFF )
set( GLFW_BUILD_DOCS OFF )
set( GLFW_INSTALL OFF )

# Only for Linux
set( GLFW_USE_WAYLAND ON )

add_subdirectory( "glfw-${SL_GLFW_VERSION}" )
