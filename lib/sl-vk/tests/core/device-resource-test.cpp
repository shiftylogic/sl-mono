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

#include <vk/core/device-resource.h>

namespace
{

    struct mock_allocator
    {};

    using mock_device   = uint32_t;
    using mock_resource = uint32_t;

    typedef mock_device* VkDevice;
    typedef mock_resource* VkMock;

    using mock = sl::vk::core::device_resource< VkMock, VkDevice >;

    void incrementing_deleter( VkDevice device, VkMock m, void* allocator )
    {
        REQUIRE( device != nullptr );
        REQUIRE( m != nullptr );
        REQUIRE( allocator == nullptr );
        REQUIRE( *device == 42 );
        ++( *m );
    }

}   // namespace

TEST_CASE( "device_resource create empty + move assign", "[vk][core]" )
{
    mock_device device { 42 };
    mock_resource res { 0 };
    REQUIRE( res == 0 );

    {
        mock m;
        REQUIRE( !m );
        REQUIRE( m.get() == nullptr );
        REQUIRE( res == 0 );

        mock m2( &device, &res, incrementing_deleter );
        REQUIRE( m2 );
        REQUIRE( m2.get() == &res );
        REQUIRE( res == 0 );

        m = std::move( m2 );
        REQUIRE( !m2 );
        REQUIRE( m2.get() == nullptr );
        REQUIRE( m );
        REQUIRE( m.get() == &res );
        REQUIRE( res == 0 );
    }

    REQUIRE( res == 1 );
}

TEST_CASE( "device_resource basic mock", "[vk][core]" )
{
    mock_device device { 42 };
    mock_resource res { 0 };
    REQUIRE( res == 0 );

    {
        mock m( &device, &res, incrementing_deleter );
        REQUIRE( m );
        REQUIRE( m.get() == &res );
    }

    REQUIRE( res == 1 );
}

TEST_CASE( "device_resource move constructor", "[vk][core]" )
{
    mock_device device { 42 };
    mock_resource res { 0 };
    REQUIRE( res == 0 );

    {
        mock m1( &device, &res, incrementing_deleter );
        REQUIRE( m1 );
        REQUIRE( m1.get() == &res );

        mock m2( std::move( m1 ) );
        REQUIRE( !m1 );
        REQUIRE( m1.get() == nullptr );
        REQUIRE( m2 );
        REQUIRE( m2.get() == &res );
        REQUIRE( res == 0 );

        mock m3 = std::move( m2 );
        REQUIRE( !m2 );
        REQUIRE( m2.get() == nullptr );
        REQUIRE( m3 );
        REQUIRE( m3.get() == &res );
        REQUIRE( res == 0 );
    }

    REQUIRE( res == 1 );
}
