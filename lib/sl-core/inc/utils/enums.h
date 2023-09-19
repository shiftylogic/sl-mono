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

#ifndef __ENUMS_H_DFD58CAFF9454796AC63A71CAA7520C8__
#define __ENUMS_H_DFD58CAFF9454796AC63A71CAA7520C8__

namespace sl::utils
{

    template< typename T, T start_val, T end_val >
    struct enum_iterator
    {
        typedef typename std::underlying_type< T >::type val_t;

        enum_iterator()
            : _val( static_cast< val_t >( start_val ) )
        {}

        enum_iterator( const T& val )
            : _val( static_cast< val_t >( val ) )
        {}

        enum_iterator operator++()
        {
            ++_val;
            return *this;
        }

        T operator*() { return static_cast< T >( _val ); }
        enum_iterator begin() { return *this; }
        enum_iterator end() { return ++enum_iterator { end_val }; }
        bool operator!=( const enum_iterator& i ) { return _val != i._val; }

    private:
        val_t _val;
    };

}   // namespace sl::utils

#endif /* __ENUMS_H_DFD58CAFF9454796AC63A71CAA7520C8__ */
