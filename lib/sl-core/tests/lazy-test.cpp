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

#include <utils/lazy.h>

TEST_CASE( "Lazy no args", "[utils]" )
{
    static size_t ctor_called = 0;
    static size_t dtor_called = 0;

    struct thing
    {
        thing() { ctor_called += 1; }
        ~thing() { dtor_called += 1; }
    };

    {
        sl::utils::lazy< thing > thng;
        REQUIRE( ctor_called == 0 );
        REQUIRE( dtor_called == 0 );

        thng.get();
        REQUIRE( ctor_called == 1 );
        REQUIRE( dtor_called == 0 );
    }

    REQUIRE( ctor_called == 1 );
    REQUIRE( dtor_called == 1 );
}

TEST_CASE( "Lazy w/ args", "[utils]" )
{
    static size_t ctor_called = 0;
    static size_t dtor_called = 0;

    struct thing
    {
        thing( int cookie1, std::string cookie2 )
            : _cookie1( cookie1 )
            , _cookie2( cookie2 )
        {
            ctor_called += 1;
        }

        ~thing() { dtor_called += 1; }

        int cookie1() const { return _cookie1; }
        std::string cookie2() const { return _cookie2; }

    private:
        int _cookie1;
        std::string _cookie2;
    };

    {
        sl::utils::lazy< thing, int, std::string > thng( 42, "dude" );
        REQUIRE( ctor_called == 0 );
        REQUIRE( dtor_called == 0 );

        auto& t = thng.get();
        REQUIRE( ctor_called == 1 );
        REQUIRE( dtor_called == 0 );

        auto& t2 = thng.get();
        REQUIRE( &t == &t2 );
        REQUIRE( t.cookie1() == 42 );
        REQUIRE( t.cookie2() == "dude" );
    }

    REQUIRE( ctor_called == 1 );
    REQUIRE( dtor_called == 1 );
}
