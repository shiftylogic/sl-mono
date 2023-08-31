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

#ifndef __MAPPED_FILE_H_D86DDCE938D0464783393790BE8181E3__
#define __MAPPED_FILE_H_D86DDCE938D0464783393790BE8181E3__

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT

#include <windows.h>

#include <utils/noncopyable.h>

namespace sl::io
{

    /*
     * This implementation is not tested (nor even compiled) yet.
     *
     * This is meant as a placeholder for now to not lose information.
     */

    struct mapped_file : sl::utils::noncopyable
    {
    public:
        mapped_file( const char* name )
            : _file { INVALID_HANDLE_VALUE }
            , _mapping { nullptr }
        {
            _file = ::CreateFileA(
                name, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, hint, nullptr );
            io::error::throw_if( hFile == INVALID_HANDLE_VALUE,
                                 "CreateFile",
                                 ::GetLastError(),
                                 "failed to open file" );

            _mapping = ::CreateFileMapping( hFile, nullptr, PAGE_READONLY, 0, 0, nullptr );
            if ( _mapping == nullptr )
            {
                auto err = ::GetLastError();
                ::CloseHandle( _file );
                io::error::throw_if(
                    true, "CreateFileMapping", err, "failed to create file mapping" );
            }

            LARGE_INTEGER size;
            if ( !::GetFileSizeEx( hFile, &size ) )
            {
                auto err = ::GetLastError();
                ::CloseHandle( _mapping );
                ::CloseHandle( _file );
                io::error::throw_if( true, "GetFileSizeEx", err, "failed to get file size" );
            }

            _size = static_cast< size_t >( size.QuadPart );
        }

        ~mapped_file() noexcept
        {
            if ( _mapping != nullptr )
                ::CloseHandle( _mapping );

            if ( _file != INVALID_HANDLE_VALUE )
                ::CloseHandle( _file );

            _file    = INVALID_HANDLE_VALUE;
            _mapping = nullptr;
            _size    = 0;
        }

        size_t size() const noexcept { return _size; }

        mapped_view map_view( size_t offset, size_t size ) const
        {
            return mapped_view( _mapping, offset, size );
        }

    private:
        HANDLE _file;
        HANDLE _mapping;
        size_t _size;
    };

}   // namespace sl::io

#endif /* __MAPPED_FILE_H_D86DDCE938D0464783393790BE8181E3__ */
