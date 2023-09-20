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

#ifndef __ERROR_H_EC11284DE9F244FA8489B9F2A3BEEA88__
#define __ERROR_H_EC11284DE9F244FA8489B9F2A3BEEA88__

#include <stdexcept>

#include <vulkan.h>

namespace sl::vk
{

    class error : public std::runtime_error
    {
    public:
        error( const char* api, VkResult code, const char* message )
            : std::runtime_error { message }
            , _api { api }
            , _code { code }
        {}

        template< typename logger_t >
        void log( logger_t& logger ) const
        {
            logger.error( "*** Vulkan Error *** '%s' failed: %s (Code: %d)", _api, what(), _code );
        }

        static void throw_if( bool condition, const char* api, VkResult code, const char* message )
        {
            if ( condition )
                throw vk::error( api, code, message );
        }

        static void throw_if_error( const char* api, VkResult result, const char* message )
        {
            if ( result != VK_SUCCESS )
                throw vk::error( api, result, message );
        }

    private:
        const char* _api;
        VkResult _code;
    };

}   // namespace sl::vk

#endif /* __ERROR_H_EC11284DE9F244FA8489B9F2A3BEEA88__ */
