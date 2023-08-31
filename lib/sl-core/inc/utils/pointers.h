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

#ifndef __POINTERS_H_AFEDC1E41C814AE98DF188159B6A8D8E__
#define __POINTERS_H_AFEDC1E41C814AE98DF188159B6A8D8E__

namespace sl::utils
{

    template< auto fn >
    struct to_deleter
    {
        template< typename T >
        constexpr void operator()( T* p ) const
        {
            fn( p );
        }
    };

    template< typename T, auto fn >
    using custom_unique_ptr = std::unique_ptr< T, to_deleter< fn > >;

}   // namespace sl::utils

#endif /* __POINTERS_H_AFEDC1E41C814AE98DF188159B6A8D8E__ */
