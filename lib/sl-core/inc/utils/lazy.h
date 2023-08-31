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

#ifndef __LAZY_H_C394AA8C06F347DCB57004A53C3B4286__
#define __LAZY_H_C394AA8C06F347DCB57004A53C3B4286__

#include <optional>
#include <tuple>

namespace sl::utils
{

    template< typename T, typename... Args >
    struct lazy
    {
        explicit lazy( Args... args )
            : _args( args... )
        {}

        T& get()
        {
            if ( !_value )
                std::apply( [&]( auto&&... args ) { _value.emplace( args... ); }, _args );

            return *_value;
        }

    private:
        std::optional< T > _value;
        std::tuple< Args... > _args;
    };

}   // namespace sl::utils

#endif /* __LAZY_H_C394AA8C06F347DCB57004A53C3B4286__ */
