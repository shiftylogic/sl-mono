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

set( EXAMPLES_OUTPUT_DIR "examples" )

function(enable_warnings target)
    if(MSVC)
        # Disable default /W3
        target_compile_options(${target} PRIVATE /W4 /WX)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endfunction(enable_warnings)

function(add_example)
    set( prefix ARGS )
    set( options DISABLE_WARNINGS )
    set( singleValues NAME )
    set( multiValues LIBRARIES SOURCES )
    cmake_parse_arguments( PARSE_ARGV 0 "${prefix}" "${options}" "${singleValues}" "${multiValues}" )

    add_executable( ${ARGS_NAME}
        ${ARGS_SOURCES}
    )

    target_link_libraries( ${ARGS_NAME}
        ${ARGS_LIBRARIES}
    )

    set_property(
        TARGET ${ARGS_NAME}
        PROPERTY RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUTPUT_DIR}
    )

    if( NOT ${ARGS_DISABLE_WARNINGS} )
        message( "Enabled compiler warnings for ${ARGS_NAME}" )
        enable_warnings( ${ARGS_NAME} )
    else()
        message( "Disabled compiler warnings for ${ARGS_NAME}" )
    endif()
endfunction(add_example)
