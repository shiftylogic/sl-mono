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

#ifndef __ERROR_H_A871243F31364C53B03C326D6E61912A__
#define __ERROR_H_A871243F31364C53B03C326D6E61912A__

#include <stdexcept>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace sl::app::win
{

    struct error : public std::runtime_error
    {
        error( const char* api, const char* message, int code )
            : std::runtime_error { message }
            , _api { api }
            , _code { code }
        {}

        template< typename Logger >
        void log( Logger& logger ) const
        {
            logger.error( "*** GLFW ERROR *** '%s' failed: %s (Error %d)", _api, what(), _code );
        }

        static void throw_if( bool condition, const char* apiName )
        {
            if ( !condition )
                return;

            const char* message;
            int code = glfwGetError( &message );

            throw win::error( apiName, message, code );
        }

    private:
        const char* _api;
        int _code;
    };

}   // namespace sl::app::win

#endif /* __ERROR_H_A871243F31364C53B03C326D6E61912A__ */
