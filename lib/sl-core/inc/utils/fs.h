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

#ifndef __FS_H_F9612BABF26B46F8869C646386B65F27__
#define __FS_H_F9612BABF26B46F8869C646386B65F27__

#include <array>
#include <filesystem>

#if defined( _WIN32 )
#    include <windows.h>
#elif defined( __linux__ )
#    include <unistd.h>
#elif defined( __APPLE__ )
#    include <mach-o/dyld.h>
#    include <sys/syslimits.h>
#endif

namespace sl::utils
{

    std::filesystem::path executable_path()
    {
#if defined( _WIN32 )
        std::array< char, MAX_PATH > buf;
        DWORD len = ::GetModuleFileNameA( nullptr, &buf[0], MAX_PATH );
        if ( len == 0 )
            throw std::runtime_error( "failed 'GetModuleFileName' call" );
        return std::filesystem::path( &buf[0] );

#elif defined( __linux__ )
        std::array< char, PATH_MAX > buf;
        int len = ::readlink( "/proc/self/exe", &buf[0], sizeof( buf ) - 1 );
        if ( len == -1 )
            throw std::runtime_error( "could not read '/proc/self/exe'" );
        buf[len] = '\0';
        return std::filesystem::path( &buf[0] );

#elif defined( __APPLE__ )
        std::array< char, PATH_MAX > buf;
        unsigned int bufSize = PATH_MAX;
        if ( ::_NSGetExecutablePath( &buf[0], &bufSize ) )
            throw std::runtime_error( "failed to get executable path" );
        return std::filesystem::path( &buf[0] );

#else
#    error Unsupported platform
#endif
    }

    std::filesystem::path base_directory()
    {
        return executable_path().parent_path();
    }

}   // namespace sl::utils

#endif /* __FS_H_F9612BABF26B46F8869C646386B65F27__ */
