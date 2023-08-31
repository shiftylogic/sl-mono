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

#ifndef __LOGGER__72f05cc0_82e2_40ad_8073_d8771df22ef8__
#define __LOGGER__72f05cc0_82e2_40ad_8073_d8771df22ef8__

#include <array>
#include <fstream>
#include <iostream>

#include <utils/lazy.h>
#include <utils/noncopyable.h>

#ifndef SL_MAX_LOG_LINE
#    define SL_MAX_LOG_LINE 512U
#endif

namespace sl::logging
{

    enum class log_level
    {
        fatal,
        error,
        warning,
        info,
        trace,
    };

    struct logger : public sl::utils::noncopyable
    {
    public:
        explicit logger()
            : _log( std::cout )
        {}

        explicit logger( const char* log_file )
            : _f_log( log_file, std::ios::app )
            , _log( _f_log )
        {}

        void log( log_level level, const char* msg )
        {
            const std::array< const char*, 5 > levels {
                "FATAL", "ERROR", "WARNING", "INFO", "TRACE" };

            _log << "[" << levels[static_cast< int >( level )] << "] " << msg << "\n";
            _log.flush();
        }

        template< typename... Args >
        void log( log_level level, const char* format, Args... args )
        {
            std::array< char, SL_MAX_LOG_LINE > buf;

            auto count = std::snprintf( buf.data(), buf.size(), format, args... );
            if ( count <= 0 )
                throw std::runtime_error( "error formatting log string" );

            log( level, buf.data() );
        }

        template< typename... Args >
        void fatal( const char* format, Args... args )
        {
            log( log_level::fatal, format, std::forward< Args >( args )... );
        }

        template< typename... Args >
        void error( const char* format, Args... args )
        {
            log( log_level::error, format, std::forward< Args >( args )... );
        }

        template< typename... Args >
        void warn( const char* format, Args... args )
        {
            log( log_level::warning, format, std::forward< Args >( args )... );
        }

        template< typename... Args >
        void info( const char* format, Args... args )
        {
            log( log_level::info, format, std::forward< Args >( args )... );
        }

        template< typename... Args >
        void trace( const char* format, Args... args )
        {
            log( log_level::trace, format, std::forward< Args >( args )... );
        }

    private:
        std::ofstream _f_log;
        std::ostream& _log;
    };

    inline sl::utils::lazy< logger > s_default {};

}   // namespace sl::logging

#define SL_LOG( level, ... ) sl::logging::s_default.get().log( level, __VA_ARGS__ );
#define SL_FATAL( ... )      SL_LOG( sl::logging::log_level::fatal, __VA_ARGS__ );
#define SL_ERROR( ... )      SL_LOG( sl::logging::log_level::error, __VA_ARGS__ );
#define SL_WARN( ... )       SL_LOG( sl::logging::log_level::warning, __VA_ARGS__ );
#define SL_INFO( ... )       SL_LOG( sl::logging::log_level::info, __VA_ARGS__ );
#define SL_TRACE( ... )      SL_LOG( sl::logging::log_level::trace, __VA_ARGS__ );

#endif   //__LOGGER__72f05cc0_82e2_40ad_8073_d8771df22ef8__
