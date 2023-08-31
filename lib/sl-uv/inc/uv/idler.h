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

#ifndef __IDLER_H_86F33456780F42A9BCC0889C3E285725__
#define __IDLER_H_86F33456780F42A9BCC0889C3E285725__

#include <uv.h>

#include "./error.h"
#include "./handle.h"
#include "./loop.h"

namespace sl::uv
{

    template< typename Logger, typename Callable >
    class idler : handle< uv_idle_t >
    {
    public:
        explicit idler( uv::loop< Logger >& loop, Callable fn )
            : _fn( fn )
        {
            uv::error::throw_if(
                ::uv_idle_init( loop, *this ), "uv_idle_init", "failed to initialize idle handle" );

            uv::error::throw_if( ::uv_idle_start( *this, &idler::on_execute ),
                                 "uv_idle_start",
                                 "failed to start idle polling" );
        }

    private:
        static void on_execute( uv_idle_t* h ) { handle::self< idler >( h )->_fn(); }

    private:
        Callable _fn;
    };

}   // namespace sl::uv

#endif /* __IDLER_H_86F33456780F42A9BCC0889C3E285725__ */
