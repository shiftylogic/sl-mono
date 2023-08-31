/**
 * MIT License
 *
 * Copyright (c) 2023-present Robert Anderson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ERROR_H_1F3B204FE97747E59EEAC34F1FA81AD0__
#define __ERROR_H_1F3B204FE97747E59EEAC34F1FA81AD0__

#include <array>
#include <cstdio>
#include <stdexcept>

#include <sqlite3.h>

namespace sl::data::sqlite
{

    struct error : public std::runtime_error
    {
        error( const char* api, int code, const char* message )
            : std::runtime_error { message }
            , _api { api }
            , _code { code }
        {}

        const char* api() const { return _api; }
        int code() const { return _code; }

        static void
        throw_if( int code, const char* apiName, const char* message, const char* err = nullptr )
        {
            if ( code == SQLITE_OK )
                return;

            std::array< char, 256 > buf;
            std::snprintf( buf.data(),
                           buf.size(),
                           "*** SQLITE ERROR *** '%s' (Code [%d] %s) %s - %s",
                           apiName,
                           code,
                           ::sqlite3_errstr( code ),
                           message,
                           err ? err : "<unknown error>" );

            throw sqlite::error( apiName, code, buf.data() );
        }

        static void throw_if( int code, const char* apiName, const char* message, sqlite3* db )
        {
            if ( code == SQLITE_OK )
                return;

            throw_if( code, apiName, message, ::sqlite3_errmsg( db ) );
        }

    private:
        const char* _api;
        int _code;
    };

}   // namespace sl::data::sqlite

#endif /* __ERROR_H_1F3B204FE97747E59EEAC34F1FA81AD0__ */
