/**
 * MIT License
 *
 * Copyright (c) 2023 Robert Anderson
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

#ifndef __TYPES_H_0C6DA7EDA6AE4B39A3C4DD8E309063EE__
#define __TYPES_H_0C6DA7EDA6AE4B39A3C4DD8E309063EE__

#include <string_view>

namespace sl::utils
{

    template< class T >
    constexpr std::string_view TypeName()
    {
#ifdef __clang__
        std::string_view p = __PRETTY_FUNCTION__;
        return std::string_view( p.data() + 34, p.size() - 34 - 1 );
#elif defined( __GNUC__ )
        std::string_view p = __PRETTY_FUNCTION__;
#    if __cplusplus < 201402
        return std::string_view( p.data() + 36, p.size() - 36 - 1 );
#    else
        return std::string_view( p.data() + 49, p.find( ';', 49 ) - 49 );
#    endif
#elif defined( _MSC_VER )
        std::string_view p = __FUNCSIG__;
        return std::string_view( p.data() + 84, p.size() - 84 - 7 );
#endif
    }

}   // namespace sl::utils

#endif /* __TYPES_H_0C6DA7EDA6AE4B39A3C4DD8E309063EE__ */
