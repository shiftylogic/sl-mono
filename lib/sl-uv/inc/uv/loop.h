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

#ifndef __LOOP_H_FF7BCFBB6A214C2791A1B7DCC02C4273__
#define __LOOP_H_FF7BCFBB6A214C2791A1B7DCC02C4273__

#include <uv.h>

#include <utils/noncopyable.h>

#include "./error.h"
#include "./handle.h"

namespace sl::uv
{

    enum class run_mode
    {
        normal  = UV_RUN_DEFAULT,
        once    = UV_RUN_ONCE,
        no_wait = UV_RUN_NOWAIT,
    };


    template< typename Logger >
    class loop : sl::utils::noncopyable
    {
    public:
        explicit loop( Logger& logger )
            : _logger( logger )
        {
            uv::error::throw_if(
                ::uv_loop_init( &_loop ), "uv_loop_init", "error initializing uv loop" );
        }

        ~loop() noexcept
        {
            // Ensure all handles related to this loop are closed
            ::uv_walk( &_loop, &loop::on_walk, nullptr );

            // We need to purge the UV loop to catch any handle closes
            uv::error::log_if( _logger,
                               ::uv_run( &_loop, UV_RUN_DEFAULT ),
                               "uv_run",
                               "failed drain loop on teardown" );

            // Now shutdown the loop
            uv::error::log_if( _logger,
                               ::uv_loop_close( &_loop ),
                               "uv_loop_close",
                               "failed tearing down the uv loop" );
        }

        operator uv_loop_t*() noexcept { return &_loop; }

        void run( run_mode mode = run_mode::normal )
        {
            ::uv_run( &_loop, static_cast< uv_run_mode >( mode ) );
        }

        void stop() { ::uv_stop( &_loop ); }

    private:
        static void on_walk( uv_handle_t* h, void* )
        {
            if ( ::uv_is_closing( h ) )
                return;

            // The internal UV library does not use data and all of our wrapped handles do,
            // so this is not an object / handle that we manage.
            if ( !h->data )
                return;

#define SL_CLOSE_HANDLE( uc, lc )                                                                  \
case UV_##uc:                                                                                      \
    static_cast< handle< uv_##lc##_t >* >( h->data )->close();                                     \
    break;

            switch ( h->type )
            {
                UV_HANDLE_TYPE_MAP( SL_CLOSE_HANDLE )
            default:
                break;
            }

#undef SL_CLOSE_HANDLE
        }

    private:
        Logger& _logger;
        uv_loop_t _loop;
    };

}   // namespace sl::uv

#endif /* __LOOP_H_FF7BCFBB6A214C2791A1B7DCC02C4273__ */
