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

enable_testing()

set( SL_TEST_OUTPUT_DIR "tests" )

function(build_tests)
    set( prefix ARGS )
    set( options )
    set( oneValueArgs NAME )
    set( multiValueArgs SOURCES LIBRARIES )
    cmake_parse_arguments( PARSE_ARGV 0 "${prefix}" "${options}" "${oneValueArgs}" "${multiValueArgs}" )

    add_executable( ${ARGS_NAME}
        ${CMAKE_SOURCE_DIR}/test/runner.cpp
        ${ARGS_SOURCES}
    )

    target_compile_definitions( ${ARGS_NAME} PRIVATE
        CATCH_CONFIG_CPP11_TO_STRING
        CATCH_CONFIG_CPP17_UNCAUGHT_EXCEPTIONS
        CATCH_CONFIG_CPP17_STRING_VIEW
        CATCH_CONFIG_CPP17_BYTE
        CATCH_CONFIG_FAST_COMPILE
    )

    target_include_directories( ${ARGS_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/test/inc
    )

    target_link_libraries( ${ARGS_NAME}
        catch2
        ${ARGS_LIBRARIES}
    )

    set_property(
        TARGET ${ARGS_NAME}
        PROPERTY RUNTIME_OUTPUT_DIRECTORY ${SL_TEST_OUTPUT_DIR}
    )

    add_test( NAME ${ARGS_NAME} COMMAND ${ARGS_NAME} )
endfunction(build_tests)

