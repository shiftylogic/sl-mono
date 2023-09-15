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
#include <win/error.h>
#include <win/window.h>

struct noop_handler
{
    void on_key( int, int, int, int ) {}
    void on_mouse_click( int, int, int ) {}
    void on_mouse_move( double, double ) {}
    void on_mouse_scroll( double, double ) {}
};

int main()
{
    auto logger = sl::logging::logger {};

    try
    {
        auto noop = noop_handler {};
        auto wnd  = sl::app::window {
            noop,
             {
                 .height = 800,
                 .width  = 1200,
                 .title  = "Window Demo",
            },
        };

        while ( wnd.poll() )
            ;
    }
    catch ( const sl::app::win::error& ex )
    {
        ex.log( logger );
    }
    catch ( const std::exception& ex )
    {
        logger.error( "[*** std::exception ***] %s", ex.what() );
    }
    catch ( ... )
    {
        logger.error( "[*** UNKNOWN EXCEPTION ***]" );
    }

    logger.info( "bye." );
    return 0;
}