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

#ifndef __MAPPED_FILE_H_6133C9D048BC47ABA2C77BB97832F4AD__
#define __MAPPED_FILE_H_6133C9D048BC47ABA2C77BB97832F4AD__

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <utils/noncopyable.h>

#include "mapped-view.h"

namespace sl::io
{

    struct MappedFile : sl::utils::NonCopyable
    {
    public:
        MappedFile( const char* name, CacheHint hint = CacheHint::Default )
            : _hint { MADV_NORMAL }
            , _fd { -1 }
            , _size { 0 }
        {
            _fd = ::open( name, O_RDONLY );
            io::Error::ThrowIf( _fd == -1, "c-lib::open", errno, "failed to open file" );

            struct stat si;
            if ( ::fstat( _fd, &si ) < 0 )
            {
                auto err = errno;
                ::close( _fd );
                io::Error::Throw( "c-lib::fstat", err, "failed to get file size" );
            }

            _size = si.st_size;

            switch ( hint )
            {
            case CacheHint::Default:
                _hint = MADV_NORMAL;
                break;
            case CacheHint::Random:
                _hint = MADV_RANDOM;
                break;
            case CacheHint::Sequential:
                _hint = MADV_SEQUENTIAL;
                break;
            }
        }

        ~MappedFile() noexcept
        {
            if ( _fd >= 0 )
                ::close( _fd );

            _fd   = -1;
            _size = 0;
        }

        size_t Size() const { return _size; }

        MappedView MapView( size_t offset, size_t size ) const
        {
            return MappedView( _fd, offset, size, _hint );
        }

    private:
        int _hint;
        int _fd;
        size_t _size;
    };

}   // namespace sl::io

#endif /* __MAPPED_FILE_H_6133C9D048BC47ABA2C77BB97832F4AD__ */
