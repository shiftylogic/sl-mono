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

#include <mem/allocator.h>

#define FAKE_PTR reinterpret_cast< void* >( 0xDeadBeef )

TEST_CASE( "Raw alloc / free", "[utils][memory]" )
{
    size_t allocs   = 0;
    size_t reallocs = 0;
    size_t frees    = 0;

    auto alloc = [&allocs]( size_t size ) {
        allocs += 1;
        return FAKE_PTR;
    };
    auto realloc = [&reallocs]( void*, size_t ) -> void* {
        reallocs += 1;
        REQUIRE( false );
        return nullptr;
    };
    auto free = [&frees]( void* ptr ) {
        frees += 1;
        REQUIRE( ptr == FAKE_PTR );
    };

    {
        auto allocator = sl::mem::allocator( { alloc, realloc, free } );

        auto ptr = allocator.alloc( 1 );
        REQUIRE( ptr == FAKE_PTR );
        REQUIRE( allocs == 1 );
        REQUIRE( frees == 0 );

        allocator.free( ptr );
        REQUIRE( allocs == 1 );
        REQUIRE( frees == 1 );
    }

    REQUIRE( allocs == 1 );
    REQUIRE( frees == 1 );
    REQUIRE( reallocs == 0 );
}

TEST_CASE( "Type alloc / free", "[utils][memory]" )
{
    auto alloc   = []( size_t size ) -> void* { return new unsigned char[size]; };
    auto realloc = []( void*, size_t ) {
        REQUIRE( false );
        return nullptr;
    };
    auto free = []( void* ptr ) { delete[] reinterpret_cast< unsigned char* >( ptr ); };

    static size_t ctor_called = 0;
    static size_t dtor_called = 0;

    struct foo
    {
        foo( int cookie )
            : _cookie( cookie )
        {
            ctor_called += 1;
        }

        ~foo() { dtor_called += 1; }

        int cookie() const { return _cookie; }

    private:
        int _cookie;
    };

    sl::mem::allocator allocator( { alloc, realloc, free } );

    auto f = allocator.alloc_t< foo >( 42 );
    REQUIRE( f != nullptr );
    REQUIRE( ctor_called == 1 );
    REQUIRE( dtor_called == 0 );
    REQUIRE( f->cookie() == 42 );

    allocator.free_t( f );
    REQUIRE( ctor_called == 1 );
    REQUIRE( dtor_called == 1 );
}

TEST_CASE( "Type alloc w/ std::unique_ptr", "[utils][memory]" )
{
    auto alloc   = []( size_t size ) -> void* { return new unsigned char[size]; };
    auto realloc = []( void*, size_t ) {
        REQUIRE( false );
        return nullptr;
    };
    auto free = []( void* ptr ) { delete[] reinterpret_cast< unsigned char* >( ptr ); };

    static size_t ctor_called = 0;
    static size_t dtor_called = 0;

    struct foo
    {
        foo( int cookie )
            : _cookie( cookie )
        {
            ctor_called += 1;
        }

        ~foo() { dtor_called += 1; }

        int cookie() const { return _cookie; }

    private:
        int _cookie;
    };

    sl::mem::allocator allocator( { alloc, realloc, free } );

    {
        auto f = allocator.alloc_sp< foo >( 73 );
        REQUIRE( f );
        REQUIRE( ctor_called == 1 );
        REQUIRE( dtor_called == 0 );
        REQUIRE( f->cookie() == 73 );
    }

    REQUIRE( ctor_called == 1 );
    REQUIRE( dtor_called == 1 );
}
