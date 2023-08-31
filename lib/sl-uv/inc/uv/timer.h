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

#ifndef __TIMER_H_24A31FF5F5134B87934C41EDECFD4106__
#define __TIMER_H_24A31FF5F5134B87934C41EDECFD4106__

#include <uv.h>

#include "./error.h"
#include "./handle.h"
#include "./loop.h"

namespace sl::uv
{

    template< typename Logger, typename Callable >
    class timer : handle< uv_timer_t >
    {
    public:
        explicit timer( uv::loop< Logger >& loop, uint64_t timeout, Callable fn )
            : timer( loop, timeout, 0, fn )
        {}

        explicit timer( uv::loop< Logger >& loop, uint64_t timeout, uint64_t repeat, Callable fn )
            : _fn( fn )
        {
            uv::error::throw_if( ::uv_timer_init( loop, *this ),
                                 "uv_timer_init",
                                 "error initializing timer handle" );

            uv::error::throw_if( ::uv_timer_start( *this, &timer::on_timer, timeout, repeat ),
                                 "uv_timer_start",
                                 "failed to start timer" );
        }

    private:
        static void on_timer( uv_timer_t* h ) { handle::self< timer >( h )->_fn(); }

    private:
        Callable _fn;
    };

}   // namespace sl::uv

#endif /* __TIMER_H_24A31FF5F5134B87934C41EDECFD4106__ */
