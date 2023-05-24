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

#ifndef __TIMER_H_55C29E1533904525B82D2EF18FADFD65__
#define __TIMER_H_55C29E1533904525B82D2EF18FADFD65__

#include "error.h"
#include "handle.h"
#include "loop.h"

namespace sl::uv
{

    template< typename Callable >
    class Timer
    {
    public:
        Timer( Loop& loop, uint64_t timeout, Callable fn )
            : Timer( loop, timeout, 0, fn )
        {}

        Timer( Loop& loop, uint64_t timeout, uint64_t repeat, Callable fn )
            : _fn( fn )
        {
            int res = ::uv_timer_init( loop, _timer );
            uv::Error::ThrowIf( res, "uv_timer_init", "error initializing timer handle" );

            _timer.Data( this );

            ::uv_timer_start( _timer, &Timer::OnTimer, timeout, repeat );
        }

    private:
        static void OnTimer( uv_timer_t* h ) { Handle< uv_timer_t >::Self< Timer >( h )->_fn(); }

    private:
        Handle< uv_timer_t > _timer;
        Callable _fn;
    };

}   // namespace sl::uv

#endif /* __TIMER_H_55C29E1533904525B82D2EF18FADFD65__ */
