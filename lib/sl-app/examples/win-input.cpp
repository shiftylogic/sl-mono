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
#include <win/input-manager.h>
#include <win/window.h>

#include <atomic>

int main()
{
    auto logger = sl::logging::logger {};

    try
    {
        auto quit   = std::atomic< bool > { false };
        auto inputs = sl::app::input_manager { logger };
        auto wnd
            = sl::app::window { inputs, { .title = "Window Demo", .width = 800, .height = 600 } };

        // Add some kindings
        inputs.bind( GLFW_KEY_Q,
                     sl::app::input_action::down,
                     sl::app::input_modifier::none,
                     [&quit]() { quit = true; } );


        while ( !quit.load() && wnd.poll() )
            ;
    }
    catch ( const sl::app::win::error& ex )
    {
        ex.log( logger );
    }
    catch ( const std::exception& ex )
    {
        logger.error( "*** Exception ***\n", ex.what() );
    }
    catch ( ... )
    {
        logger.error( "[*** UNKNOWN EXCEPTION ***]" );
    }

    logger.info( "bye." );
    return 0;
}
