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

#ifndef __LOAD_H_F9612BABF26B46F8869C646386B65F27__
#define __LOAD_H_F9612BABF26B46F8869C646386B65F27__

#include <filesystem>
#include <fstream>
#include <vector>

#if defined( _WIN32 )
#    include <windows.h>
#elif defined( __linux__ )
#    include <unistd.h>
#elif defined( __APPLE__ )
#    include <mach-o/dyld.h>
#    include <sys/syslimits.h>
#endif

namespace sl::io
{

    template< typename T >
    std::vector< T > load_file( const char* filename )
    {
        std::ifstream f( filename, std::ios::ate | std::ios::binary );
        if ( !f.is_open() )
            throw std::runtime_error( "failed to open file" );

        size_t count = static_cast< size_t >( f.tellg() );
        if ( count % sizeof( T ) != 0 )
            throw std::runtime_error( "file is not a multiple of requested type" );

        std::vector< T > buf( count / sizeof( T ) );

        f.seekg( 0 );
        f.read( reinterpret_cast< char* >( buf.data() ), count );
        f.close();

        return buf;
    }

    template< typename T >
    std::vector< T > load_file( std::filesystem::path filePath )
    {
        return load_file< T >( filePath.c_str() );
    }

}   // namespace sl::io

#endif /* __LOAD_H_F9612BABF26B46F8869C646386B65F27__ */
