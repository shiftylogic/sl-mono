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

#ifndef __MAPPED_VIEW_H_1C7D0FF2148D4F6E9534FDA575582FDE__
#define __MAPPED_VIEW_H_1C7D0FF2148D4F6E9534FDA575582FDE__

#include <errno.h>
#include <sys/mman.h>

#include <span>

#include <utils/noncopyable.h>

namespace sl::io
{

    struct MappedView : sl::utils::NonCopyable
    {
    public:
        MappedView( int fd, size_t offset, size_t size, int hint )
            : _size { size }
        {
            _view = ::mmap( nullptr, size, PROT_READ, MAP_SHARED, fd, offset );
            io::Error::ThrowIf( _view == MAP_FAILED, "c-lib::mmap", errno, "failed to map view" );

            // We are going to ignore the failure here. Worst case, we don't get to "tweak".
            ::madvise( _view, size, hint );
        }

        ~MappedView() noexcept
        {
            if ( _view != nullptr )
                ::munmap( _view, _size );   // Any failure (unlikely) is ignored.

            _view = nullptr;
            _size = 0;
        }

        template< typename T >
        T& As( size_t offset = 0 ) const
        {
            const auto sp = static_cast< unsigned char* >( _view );
            const auto ep = sp + _size;
            auto ptr      = sp + offset;

            io::Error::ThrowIf(
                ptr >= ep, "offset-check", -1, "data offset is beyond mapped view" );
            io::Error::ThrowIf(
                ptr + sizeof( T ) > ep, "T-size-check", -1, "object of type T exceeds data view" );

            return *reinterpret_cast< T* >( ptr );
        }

        template< typename T >
        std::span< T > AsItems( size_t offset = 0, size_t count = 0 ) const
        {
            const auto sp = static_cast< unsigned char* >( _view );
            const auto ep = sp + _size;
            auto ptr      = sp + offset;

            if ( count == 0 )
                count = ( _size - offset ) / sizeof( T );

            io::Error::ThrowIf(
                ptr >= ep, "offset-check", -1, "data offset is beyond mapped view" );
            io::Error::ThrowIf(
                count == 0, "T-size-count-check", -1, "object of type T exceeds data view" );

            return std::span< T >( reinterpret_cast< T* >( ptr ), count );
        }

    private:
        size_t _size;
        void* _view;
    };

}   // namespace sl::io

#endif /* __MAPPED_VIEW_H_1C7D0FF2148D4F6E9534FDA575582FDE__ */
