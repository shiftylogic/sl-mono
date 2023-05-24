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

#include <logging/logger.h>
#include <uv/idler.h>
#include <uv/signaler.h>
#include <uv/timer.h>

namespace
{

    void IdleMe()
    {
        static uint64_t c = 0;
        if ( ( ++c % 1000000 ) == 0 )
            SL_INFO( "Idle..." );
    }

    void SayHello() { SL_INFO( "Hello" ); }
    void SaySup() { SL_INFO( "What's up!" ); }

}   // namespace

int main()
{
    try
    {
        SL_INFO( "Initializing..." );

        sl::uv::Loop loop;
        sl::uv::Idler idler( loop, IdleMe );
        sl::uv::Timer timer( loop, 2000, SayHello );
        sl::uv::Timer timer2( loop, 1000, 3000, SaySup );
        sl::uv::Signaler signaler(
            loop,
            [&loop]() {
                SL_INFO( "Interrupted..." );
                loop.Stop();
            },
            SIGINT );

        SL_INFO( "Starting main loop..." );
        loop.Run();

        SL_INFO( "Shutting down..." );
    }
    catch ( const sl::uv::Error& ex )
    {
        ex.Log();
    }
    catch ( const std::exception& ex )
    {
        SL_ERROR( "[*** std::exception ***] %s", ex.what() );
    }
    catch ( ... )
    {
        SL_ERROR( "[*** UNKNOWN EXCEPTION ***]" );
    }

    SL_INFO( "bye." );
    return 0;
}
