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

#ifndef __MAPPED_VIEW_H_2C54890D693F4B339888C90AC76F5834__
#define __MAPPED_VIEW_H_2C54890D693F4B339888C90AC76F5834__

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT

#include <windows.h>

#include <memory>

#include "../error.h"

namespace sl::io
{

    struct mapped_view : sl::utils::noncopyable
    {
    public:
        mapped_view( HANDLE hMapping, size_t offset, size_t size )
            : _size { size }
        {
            auto low  = static_cast< DWORD >( offset & 0xffffffff );
            auto high = static_cast< DWORD >( offset >> 32 );

            _view = ::MapViewOfFile( hMapping, FILE_MAP_READ, high, low, size );
            io::error::throw_if(
                _view == nullptr, "MapViewOfFile", ::GetLastError(), "failed to map view" );
        }

        ~mapped_view() noexcept
        {
            if ( _view != nullptr )
                ::UnMapViewOfFileE( _view );

            _view = nullptr;
            _size = 0;
        }

    private:
        size_t _size;
        void* _view;
    };


}   // namespace sl::io

#endif /* __MAPPED_VIEW_H_2C54890D693F4B339888C90AC76F5834__ */
