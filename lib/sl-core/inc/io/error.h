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

#ifndef __ERROR_H_3B53E4DB5D6142259E8C52A7BFD4F9B9__
#define __ERROR_H_3B53E4DB5D6142259E8C52A7BFD4F9B9__

#include <stdexcept>

namespace sl::io
{

    struct error : public std::runtime_error
    {
        explicit error( const char* api, int code, const char* message )
            : std::runtime_error { message }
            , _api { api }
            , _code { code }
        {}

        static void
        throw_if( bool condition, const char* api, int code, const char* message = nullptr )
        {
            if ( condition )
                throw io::error { api, code, message };
        }

        template< typename Logger >
        void log( Logger& logger ) const
        {
            logger.error( "*** IO ERROR *** '%s' failed: %s (Code: %d)", _api, what(), _code );
        }

    private:
        const char* _api;
        const int _code;
    };

}   // namespace sl::io

#endif /* __ERROR_H_3B53E4DB5D6142259E8C52A7BFD4F9B9__ */
