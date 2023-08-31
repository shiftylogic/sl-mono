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

#ifndef __SIGNALER_H_3FDE59DDA15C44A8926FE46B3BE4F3C2__
#define __SIGNALER_H_3FDE59DDA15C44A8926FE46B3BE4F3C2__

#include <uv.h>

#include "./error.h"
#include "./handle.h"
#include "./loop.h"

namespace sl::uv
{

    template< typename Logger, typename Callable >
    class signaler : handle< uv_signal_t >
    {
    public:
        explicit signaler( uv::loop< Logger >& loop, Callable fn, int signum )
            : _fn( fn )
        {
            uv::error::throw_if( ::uv_signal_init( loop, *this ),
                                 "uv_signal_init",
                                 "error initializing signal handle" );

            uv::error::throw_if( ::uv_signal_start( *this, &signaler::on_signal, signum ),
                                 "uv_signal_start",
                                 "failed to start watching signal" );
        }

    private:
        static void on_signal( uv_signal_t* h, int /* signum */ )
        {
            handle::self< signaler >( h )->_fn();
        }

    private:
        Callable _fn;
    };

}   // namespace sl::uv

#endif /* __SIGNALER_H_3FDE59DDA15C44A8926FE46B3BE4F3C2__ */
