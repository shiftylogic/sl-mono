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

#ifndef __UVERROR_H_9A908BFF019A43EFA755329EBB1A282F__
#define __UVERROR_H_9A908BFF019A43EFA755329EBB1A282F__

#include <stdexcept>
#include <uv.h>

#include <logging/logger.h>

namespace sl::uv
{

    class Error : public std::runtime_error
    {
    public:
        Error( const char* api, const char* message, int code )
            : std::runtime_error( message )
            , _api( api )
            , _code( code )
        {}

        void Log() const { LogIf( _code, _api, what() ); }

        static void LogIf( int code, const char* apiName, const char* message )
        {
            if ( code == 0 )
                return;

            SL_ERROR( "*** UV ERROR *** '%s' failed: %s (Error: %d / %s) - %s\n",
                      apiName,
                      message,
                      code,
                      ::uv_err_name( code ),
                      ::uv_strerror( code ) );
        }

        static void ThrowIf( int code, const char* apiName, const char* message )
        {
            if ( code == 0 )
                return;

            throw Error( apiName, message, code );
        }

    private:
        const char* _api;
        int _code;
    };

}   // namespace sl::uv

#endif /* __UVERROT_H_9A908BFF019A43EFA755329EBB1A282F__ */
