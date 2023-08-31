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

#include <uv.h>

#include <logging/logger.h>
#include <uv/idler.h>

using namespace std::chrono_literals;

TEST_CASE( "UV idler executes", "[uv]" )
{
    std::atomic< int > count { 0 };
    sl::logging::logger logger;
    sl::uv::loop loop( logger );
    sl::uv::idler idler( loop, [&]() { count++; } );

    loop.run( sl::uv::run_mode::no_wait );
    REQUIRE( count == 1 );
}

TEST_CASE( "UV idler executes each loop iteration", "[uv]" )
{
    auto [completed, count] = sl::test::run_async< int >( 2000ms, []() -> int {
        std::atomic< int > count { 0 };
        sl::logging::logger logger;
        sl::uv::loop loop( logger );
        sl::uv::idler idler( loop, [&]() { count++; } );

        for ( int i = 1; i <= 10; i++ )
            loop.run( sl::uv::run_mode::once );

        return count;
    } );

    REQUIRE( completed );
    REQUIRE( count == 10 );
}

TEST_CASE( "UV loop outlives idler", "[uv]" )
{
    std::atomic< int > count { 0 };
    std::atomic< int > hcount { 0 };

    {
        sl::logging::logger logger;
        sl::uv::loop loop( logger );
        {
            sl::uv::idler idler( loop, [&count]() { ++count; } );
            loop.run( sl::uv::run_mode::once );
        }

        ::uv_walk(
            loop,
            []( uv_handle_s*, void* p ) { ++*static_cast< std::atomic< int >* >( p ); },
            &hcount );
        REQUIRE( hcount == 1 );
    }

    REQUIRE( hcount == 1 );
    REQUIRE( count == 1 );
}

TEST_CASE( "UV idler outlives Loop", "[uv]" )
{
    std::atomic< int > count { 0 };

    sl::logging::logger logger;
    auto loop  = std::make_unique< sl::uv::loop< sl::logging::logger > >( logger );
    auto idler = sl::uv::idler { *loop, [&]() { count++; } };

    loop->run( sl::uv::run_mode::once );
    REQUIRE( count == 1 );

    loop.reset();
    REQUIRE( count == 1 );
}

TEST_CASE( "UV idler movable", "[uv]" )
{
    std::atomic< int > count { 0 };

    {
        sl::logging::logger logger;
        sl::uv::loop loop( logger );
        sl::uv::idler idler { loop, [&]() { count++; } };
        sl::uv::idler idler2 { std::move( idler ) };

        loop.run( sl::uv::run_mode::once );
        REQUIRE( count == 1 );
    }

    REQUIRE( count == 1 );
}
