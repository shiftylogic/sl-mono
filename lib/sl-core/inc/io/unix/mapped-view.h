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

    struct mapped_view : sl::utils::noncopyable
    {
    public:
        mapped_view( int fd, size_t offset, size_t size, int hint )
            : _size { size }
        {
            _view = ::mmap( nullptr, size, PROT_READ, MAP_SHARED, fd, offset );
            io::error::throw_if( _view == MAP_FAILED, "c-lib::mmap", errno, "failed to map view" );

            // We are going to ignore the failure here. Worst case, we don't get to "tweak".
            ::madvise( _view, size, hint );
        }

        ~mapped_view() noexcept
        {
            if ( _view != nullptr )
                ::munmap( _view, _size );   // Any failure (unlikely) is ignored.

            _view = nullptr;
            _size = 0;
        }

        /**
         * Caller is responsible for not violating alignment rules for the CPU
         */
        template< typename T >
        T& as( size_t offset = 0 ) const
        {
            const auto sp = static_cast< unsigned char* >( _view );
            const auto ep = sp + _size;
            auto ptr      = sp + offset;

            io::error::throw_if(
                ptr >= ep, "offset-check", -1, "data offset is beyond mapped view" );
            io::error::throw_if(
                ptr + sizeof( T ) > ep, "T-size-check", -1, "object of type T exceeds data view" );

            return *static_cast< T* >( static_cast< void* >( ptr ) );
        }

        /**
         * Caller is responsible for not violating alignment rules for the CPU
         */
        template< typename T >
        std::span< T > as_items( size_t offset = 0, size_t count = 0 ) const
        {
            const auto sp = static_cast< unsigned char* >( _view );
            const auto ep = sp + _size;
            auto ptr      = sp + offset;

            if ( count == 0 )
                count = ( _size - offset ) / sizeof( T );

            io::error::throw_if(
                ptr >= ep, "offset-check", -1, "data offset is beyond mapped view" );
            io::error::throw_if(
                count == 0, "T-size-count-check", -1, "object of type T exceeds data view" );

            return std::span< T >( static_cast< T* >( static_cast< void* >( ptr ) ), count );
        }

        std::span< std::byte > as_bytes( size_t offset = 0, size_t count = 0 ) const
        {
            return as_items< std::byte >( offset, count );
        }

    private:
        size_t _size;
        void* _view;
    };

}   // namespace sl::io

#endif /* __MAPPED_VIEW_H_1C7D0FF2148D4F6E9534FDA575582FDE__ */
