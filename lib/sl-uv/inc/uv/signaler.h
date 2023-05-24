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

#ifndef __SIGNALER_H_E7618F793B464BCBA9EBB6D70A11C562__
#define __SIGNALER_H_E7618F793B464BCBA9EBB6D70A11C562__

#include "error.h"
#include "handle.h"
#include "loop.h"

namespace sl::uv
{

    template< typename Callable >
    class Signaler
    {
    public:
        Signaler( Loop& loop, Callable fn, int signum )
            : _fn( fn )
        {
            int res = ::uv_signal_init( loop, _signal );
            uv::Error::ThrowIf( res, "uv_signal_init", "error initializing signal handle" );

            _signal.Data( this );

            ::uv_signal_start( _signal, &Signaler::OnSignal, signum );
        }

    private:
        static void OnSignal( uv_signal_t* h, int /* signum */ )
        {
            Handle< uv_signal_t >::Self< Signaler >( h )->_fn();
        }

    private:
        Handle< uv_signal_t > _signal;
        Callable _fn;
    };

}   // namespace sl::uv

#endif /* __SIGNALER_H_E7618F793B464BCBA9EBB6D70A11C562__ */
