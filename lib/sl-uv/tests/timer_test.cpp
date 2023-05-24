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
#include <test/async.h>

#include <uv/timer.h>

using namespace std::chrono_literals;

TEST_CASE( "UV Timer executes", "[uv]" )
{
    auto [completed, count] = sl::test::RunAsync< int >( 2000ms, []() -> int {
        std::atomic< int > count { 0 };
        sl::uv::Loop loop;
        sl::uv::Timer timer( loop, 20, 0, [&]() { count++; } );

        loop.Run( sl::uv::Loop::RunMode::Once );
        return count;
    } );

    REQUIRE( completed );
    REQUIRE( count == 1 );
}

TEST_CASE( "UV Timer repeats", "[uv]" )
{
    auto [completed, count] = sl::test::RunAsync< int >( 2000ms, []() -> int {
        std::atomic< int > count { 0 };
        sl::uv::Loop loop;
        sl::uv::Timer timer( loop, 20, 20, [&]() { count++; } );

        loop.Run( sl::uv::Loop::RunMode::Once );
        loop.Run( sl::uv::Loop::RunMode::Once );
        loop.Run( sl::uv::Loop::RunMode::Once );
        loop.Run( sl::uv::Loop::RunMode::Once );

        return count;
    } );

    REQUIRE( completed );
    REQUIRE( count == 4 );
}
