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

#ifndef __LOOP_H_9EF25771C17049F494239E8FFFE1516C__
#define __LOOP_H_9EF25771C17049F494239E8FFFE1516C__

#include <uv.h>

#include <utils/noncopyable.h>

#include "error.h"

namespace sl::uv
{

    class Loop : sl::utils::NonCopyable
    {
    public:
        enum class RunMode
        {
            Default = UV_RUN_DEFAULT,
            Once    = UV_RUN_ONCE,
            NoWait  = UV_RUN_NOWAIT,
        };

    public:
        Loop()
        {
            uv::Error::ThrowIf(
                uv_loop_init( &_loop ), "uv_loop_init", "error initializing uv loop" );
        }

        ~Loop() noexcept
        {
            // We need to purge the UV loop to catch any handle closes
            while ( ::uv_loop_alive( &_loop ) && ::uv_run( &_loop, UV_RUN_ONCE ) )
                ;

            // Now shutdown the loop
            uv::Error::LogIf(
                ::uv_loop_close( &_loop ), "uv_loop_close", "failed tearing down the uv loop" );
        }

        operator uv_loop_t*() noexcept { return &_loop; }

        void Run( RunMode mode = RunMode::Default )
        {
            ::uv_run( &_loop, static_cast< uv_run_mode >( mode ) );
        }

        void Stop() { ::uv_stop( &_loop ); }

    private:
        uv_loop_t _loop;
    };

}   // namespace sl::uv

#endif /* __LOOP_H_9EF25771C17049F494239E8FFFE1516C__ */
