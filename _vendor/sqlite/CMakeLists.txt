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

project( sqlite )

set( SL_SQLITE_VERSION "3.42.0" )

file( ARCHIVE_EXTRACT
    INPUT "${CMAKE_CURRENT_SOURCE_DIR}/sqlite-v${SL_SQLITE_VERSION}.zip"
    DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}"
)


#
# Static library
#
add_library( ${PROJECT_NAME} STATIC
    sqlite-v${SL_SQLITE_VERSION}/src/sqlite3.c
)
target_include_directories( ${PROJECT_NAME} PUBLIC
    sqlite-v${SL_SQLITE_VERSION}/inc
)
target_compile_definitions( ${PROJECT_NAME} PUBLIC
    SQLITE_THREADSAFE=0

    SQLITE_DQS=0
    SQLITE_DEFAULT_MEMSTATUS=0
    SQLITE_DEFAULT_WAL_SYNCHRONOUS=1
    SQLITE_LIKE_DOESNT_MATCH_BLOBS=1
    SQLITE_MAX_EXPR_DEPTH=0

    SQLITE_OMIT_AUTOINIT=1
    SQLITE_OMIT_DECLTYPE=1
    SQLITE_OMIT_DEPRECATED=1
    SQLITE_OMIT_LOAD_EXTENSION=1
    SQLITE_OMIT_PROGRESS_CALLBACK=1
    SQLITE_OMIT_SHARED_CACHE=1

    SQLITE_USE_ALLOCA=1
)


#
# Also, build the CLI for debugging purposes.
#
add_executable( ${PROJECT_NAME}-cli
    sqlite-v${SL_SQLITE_VERSION}/src/shell.c
)
target_link_libraries( ${PROJECT_NAME}-cli PRIVATE
    sqlite
)


#
# Make sure the demo compiles
#
add_subdirectory( "demo" )

