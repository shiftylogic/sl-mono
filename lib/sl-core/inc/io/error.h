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

#include <utils/strings.h>

namespace sl::io
{

    class Error : public std::runtime_error
    {
    public:
        Error( const char* api, int code, const char* message )
            : std::runtime_error { io::Error::MakeMessage( api, code, message ) }
        {}

        static void Throw( const char* api, int code, const char* message = nullptr )
        {
            throw io::Error { api, code, message };
        }

        static void
        ThrowIf( bool condition, const char* api, int code, const char* message = nullptr )
        {
            if ( condition )
                Throw( api, code, message );
        }

    private:
        static std::string MakeMessage( const char* api, int code, const char* message )
        {
            const char* msg = ( nullptr == message ) ? "<unspecified>" : message;
            return sl::utils::FormatString( "IO Error (%s): [%d] %s", api, code, msg );
        }
    };

}   // namespace sl::io

#endif /* __ERROR_H_3B53E4DB5D6142259E8C52A7BFD4F9B9__ */
