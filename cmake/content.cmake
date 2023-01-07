# MIT License
#
# Copyright (c) 2023 Robert Anderson
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

function(cache_content)
    set( prefix ARGS )
    set( options FORCE )
    set( singleValues LOCAL URL HASH )
    set( multiValues )
    cmake_parse_arguments( PARSE_ARGV 0 "${prefix}" "${options}" "${singleValues}" "${multiValues}" )

    if( ARGS_FORCE AND EXISTS "${ARGS_LOCAL}" )
        message( "-- [cache_content] caching content - forced" )
        file( REMOVE "${ARGS_LOCAL}" )
    endif()

    # Check if the file already exists in the cache location.
    # If it does, delete on force, or return and do nothing.
    if( EXISTS "${ARGS_LOCAL}" )
        file( SHA256 ${ARGS_LOCAL} SL_CC_LOCAL_HASH )
        if( "${ARGS_HASH}" EQUAL "${SL_CC_LOCAL_HASH}" )
            message( STATUS "[cache_content] caching content - exists" )
            return()
        endif()
    endif()

    # Make sure the cached content file directory exists
    get_filename_component( SL_CC_TARGET_DIR "${ARGS_LOCAL}" DIRECTORY )
    file( MAKE_DIRECTORY "${SL_CC_TARGET_DIR}" )

    # Fetch the URL
    file( DOWNLOAD "${ARGS_URL}" "${ARGS_LOCAL}"
        INACTIVITY_TIMEOUT 30
        STATUS "SL_CC_STATUS;SL_CC_ERRMSG"
        TLS_VERIFY ON
        EXPECTED_HASH "SHA256=${ARGS_HASH}"
    )

    if( ${SL_CC_STATUS} )
        message( FATAL_ERROR "[cache_content] cache_content failed (code: ${SL_CC_STATUS}) - ${SL_CC_ERRMSG}" )
    else()
        message( STATUS "[cache_content] caching content - done" )
    endif()
endfunction(cache_content)
