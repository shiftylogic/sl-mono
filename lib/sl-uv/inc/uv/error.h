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

#ifndef __ERROR_H_7B71C74D57224A61B242BD41E35C53FE__
#define __ERROR_H_7B71C74D57224A61B242BD41E35C53FE__

#include <stdexcept>
#include <uv.h>

namespace sl::uv
{

    class error : public std::runtime_error
    {
    public:
        error( const char* api, const char* message, int code )
            : std::runtime_error { message }
            , _api { api }
            , _code { code }
        {}

        template< typename Logger >
        void log( Logger& logger ) const
        {
            log_if( logger, _code, _api, what() );
        }

        template< typename Logger >
        static void log_if( Logger& logger, int code, const char* apiName, const char* message )
        {
            if ( code == 0 )
                return;

            logger.error( "*** UV ERROR *** '%s' failed: %s (Error: %d / %s) - %s",
                          apiName,
                          message,
                          code,
                          ::uv_err_name( code ),
                          ::uv_strerror( code ) );
        }

        static void throw_if( int code, const char* apiName, const char* message )
        {
            if ( code == 0 )
                return;

            throw error( apiName, message, code );
        }

    private:
        const char* _api;
        int _code;
    };

}   // namespace sl::uv

#endif /* __ERROR_H_7B71C74D57224A61B242BD41E35C53FE__ */
