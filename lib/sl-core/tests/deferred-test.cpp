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

#include <catch2/catch.hpp>

#include <utils/deferred.h>

TEST_CASE( "Deferred executes leaving scopes", "[utils][deferred]" )
{
    size_t count = 0;
    auto bump    = [&count]() { ++count; };

    {
        {
            REQUIRE( count == 0 );
            sl::utils::deferred _( bump );
            REQUIRE( count == 0 );
        }
        REQUIRE( count == 1 );

        auto fn = [&]() {
            REQUIRE( count == 1 );
            sl::utils::deferred _( bump );
            REQUIRE( count == 1 );
        };
        REQUIRE( count == 1 );
        fn();
        REQUIRE( count == 2 );
    }

    REQUIRE( count == 2 );
}
