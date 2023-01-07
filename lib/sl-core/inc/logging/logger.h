/**
 * MIT License
 *
 * Copyright (c) 2023 Robert Anderson
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

#ifndef __LOGGER__72f05cc0_82e2_40ad_8073_d8771df22ef8__
#define __LOGGER__72f05cc0_82e2_40ad_8073_d8771df22ef8__

#include <utils/lazy.h>

#include <array>
#include <fstream>
#include <iostream>

namespace sl::logging
{

    enum class Level
    {
        Fatal,
        Error,
        Warning,
        Info,
        Trace,
    };

    template< unsigned int kMaxLogLine = 256U >
    class Logger
    {
    public:
        inline static sl::utils::Lazy< Logger > Default {};

        Logger( const Logger& ) = delete;
        Logger( Logger&& )      = delete;
        Logger& operator=( const Logger& ) = delete;
        Logger&& operator=( Logger&& ) = delete;

        Logger()
            : _log( std::cout )
        {}

        Logger( const char* logFile )
            : _fLog( logFile )
            , _log( _fLog )
        {}

        void Log( Level level, const char* msg )
        {
            const std::array< const char*, 5 > levels {
                "FATAL", "ERROR", "WARNING", "INFO", "TRACE" };

            _log << "[" << levels[static_cast< int >( level )] << "] " << msg << "\n";
        }

        template< typename... Args >
        void Log( Level level, const char* format, Args... args )
        {
            std::array< char, kMaxLogLine > buf;

            auto count = std::snprintf( buf.data(), buf.size(), format, args... );
            if ( count <= 0 )
                throw std::runtime_error( "error formatting log string" );

            Log( level, buf.data() );
        }

    private:
        std::ofstream _fLog;
        std::ostream& _log;
    };

}   // namespace sl::logging

#define SL_LOG( level, ... ) sl::logging::Logger< 256 >::Default.Get().Log( level, __VA_ARGS__ );
#define SL_FATAL( ... )      SL_LOG( sl::logging::Level::Fatal, __VA_ARGS__ );
#define SL_ERROR( ... )      SL_LOG( sl::logging::Level::Error, __VA_ARGS__ );
#define SL_WARN( ... )       SL_LOG( sl::logging::Level::Warning, __VA_ARGS__ );
#define SL_INFO( ... )       SL_LOG( sl::logging::Level::Info, __VA_ARGS__ );
#define SL_TRACE( ... )      SL_LOG( sl::logging::Level::Trace, __VA_ARGS__ );

#endif   //__LOGGER__72f05cc0_82e2_40ad_8073_d8771df22ef8__
