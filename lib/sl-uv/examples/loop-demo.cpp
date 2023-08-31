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

#include <cstdlib>
#include <stdexcept>

#include <logging/logger.h>
#include <uv/idler.h>
#include <uv/signaler.h>
#include <uv/timer.h>

namespace
{

    sl::logging::logger g_logger;

    void idle_me()
    {
        static uint64_t c = 0;
        if ( ( ++c % 1000000 ) == 0 )
            g_logger.info( "Idle..." );
    }

    void say_hello()
    {
        g_logger.info( "Hello" );
    }

    void say_sup()
    {
        g_logger.info( "What's up!" );
    }

}   // namespace

int main()
{
    try
    {
        g_logger.info( "Initializing..." );

        sl::uv::loop loop( g_logger );
        sl::uv::idler idler( loop, idle_me );
        sl::uv::timer timer( loop, 2000, say_hello );
        sl::uv::timer timer2( loop, 1000, 3000, say_sup );
        sl::uv::signaler signaler(
            loop,
            [&loop]() {
                g_logger.info( "Interrupted..." );
                loop.stop();
            },
            SIGINT );

        g_logger.info( "Starting main loop..." );
        loop.run();

        g_logger.info( "Shutting down..." );
    }
    catch ( const sl::uv::error& ex )
    {
        ex.log( g_logger );
    }
    catch ( const std::exception& ex )
    {
        g_logger.error( "[*** std::exception ***] %s", ex.what() );
    }
    catch ( ... )
    {
        g_logger.error( "[*** UNKNOWN EXCEPTION ***]" );
    }

    g_logger.info( "bye." );
    return 0;
}
