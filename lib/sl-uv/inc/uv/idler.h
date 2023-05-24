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

#ifndef __IDLER_H_45380EE7F20043AC8444CDC1891D2397__
#define __IDLER_H_45380EE7F20043AC8444CDC1891D2397__

#include "handle.h"
#include "loop.h"

namespace sl::uv
{

    template< typename Callable >
    class Idler
    {
    public:
        Idler( Loop& loop, Callable fn )
            : _fn( fn )
        {
            ::uv_idle_init( loop, _idle );
            _idle.Data( this );

            ::uv_idle_start( _idle, &Idler::OnExecute );
        }

    private:
        static void OnExecute( uv_idle_t* h ) { Handle< uv_idle_t >::Self< Idler >( h )->_fn(); }

    private:
        Handle< uv_idle_t > _idle;
        Callable _fn;
    };

}   // namespace sl::uv

#endif /* __IDLER_H_45380EE7F20043AC8444CDC1891D2397__ */
