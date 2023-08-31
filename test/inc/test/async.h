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

#ifndef __ASYNC_H_F46C90D0CFFE4DBFA17CC30527617F62__
#define __ASYNC_H_F46C90D0CFFE4DBFA17CC30527617F62__

#include <future>
#include <tuple>

namespace sl::test
{

    static bool run_async( std::chrono::milliseconds wait, std::function< void() > fn )
    {
        auto task   = std::async( std::launch::async, fn );
        auto status = task.wait_for( wait );

        return status == std::future_status::ready;
    }

    template< typename T >
    static std::tuple< bool, T > run_async( std::chrono::milliseconds wait,
                                            std::function< T() > fn )
    {
        auto task   = std::async( std::launch::async, fn );
        auto status = task.wait_for( wait );

        if ( status == std::future_status::ready )
            return std::make_tuple( true, task.get() );
        else
            return std::make_tuple( false, T() );
    }

}   // namespace sl::test

#endif /* __ASYNC_H_F46C90D0CFFE4DBFA17CC30527617F62__ */
