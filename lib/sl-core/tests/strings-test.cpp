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

#include <utils/strings.h>

TEST_CASE( "Formatted lengths", "[utils][strings]" )
{
    auto count = sl::utils::calculate_string_length( "something: %s - with <%i>", "dude", 12387 );
    REQUIRE( count == strlen( "something: dude - with <12387>" ) );
}

TEST_CASE( "String formatting (non-allocating)", "[utils][strings]" )
{
    std::string expected( "Int: 42, String: test" );
    std::array< char, 256 > buffer;

    auto count = sl::utils::format_string( buffer, "Int: %d, String: %s", 42, "test" );
    REQUIRE( count == expected.size() + 1 );
    REQUIRE( buffer[count - 1] == 0 );
    REQUIRE( std::string( buffer.data(), count - 1 ) == expected );
}

TEST_CASE( "String formatting (allocating)", "[utils][strings]" )
{
    std::string expected( "This string contains a string (a string) and a float (1.123)." );

    auto actual = sl::utils::format_string(
        "This string contains a string (%s) and a float (%1.3f).", "a string", 1.123f );
    REQUIRE( actual.size() == expected.size() );
    REQUIRE( actual == expected );
}

TEST_CASE( "String replace all matching chars", "[utils][strings]" )
{
    std::string str = "aabbbbabaabaaacdd999sdf98";

    sl::utils::replace_all( str, 's', '+' );
    REQUIRE( str == "aabbbbabaabaaacdd999+df98" );

    sl::utils::replace_all( str, '8', '+' );
    REQUIRE( str == "aabbbbabaabaaacdd999+df9+" );

    sl::utils::replace_all( str, 'b', '+' );
    REQUIRE( str == "aa++++a+aa+aaacdd999+df9+" );

    sl::utils::replace_all( str, 'f', '+' );
    REQUIRE( str == "aa++++a+aa+aaacdd999+d+9+" );

    sl::utils::replace_all( str, '9', '+' );
    REQUIRE( str == "aa++++a+aa+aaacdd++++d+++" );

    sl::utils::replace_all( str, 'a', '+' );
    REQUIRE( str == "++++++++++++++cdd++++d+++" );

    sl::utils::replace_all( str, 'd', '+' );
    REQUIRE( str == "++++++++++++++c++++++++++" );

    sl::utils::replace_all( str, 'c', '+' );
    REQUIRE( str == "+++++++++++++++++++++++++" );

    sl::utils::replace_all( str, '+', '0' );
    REQUIRE( str == "0000000000000000000000000" );
}

TEST_CASE( "String snake to pascal", "[utils][strings]" )
{
    // Make sure it is not in-place
    std::string str = "test_the_thing";
    auto res        = sl::utils::snake_to_pascal( str );
    REQUIRE( res == "TestTheThing" );
    REQUIRE( res.size() == 12 );
    REQUIRE( str == "test_the_thing" );

    // Now, just a bunch of different variations.
    REQUIRE( sl::utils::snake_to_pascal( "" ) == "" );
    REQUIRE( sl::utils::snake_to_pascal( "_" ) == "" );
    REQUIRE( sl::utils::snake_to_pascal( "a" ) == "A" );
    REQUIRE( sl::utils::snake_to_pascal( "B" ) == "B" );
    REQUIRE( sl::utils::snake_to_pascal( "Ba" ) == "Ba" );
    REQUIRE( sl::utils::snake_to_pascal( "ba" ) == "Ba" );
    REQUIRE( sl::utils::snake_to_pascal( "bA" ) == "BA" );
    REQUIRE( sl::utils::snake_to_pascal( "aaa_bbbbb_c" ) == "AaaBbbbbC" );
    REQUIRE( sl::utils::snake_to_pascal( "_a_bbbbb_c" ) == "ABbbbbC" );
    REQUIRE( sl::utils::snake_to_pascal( "_a_bbbBb_c" ) == "ABbbBbC" );
    REQUIRE( sl::utils::snake_to_pascal( "_a_bbbbb_" ) == "ABbbbb" );
}
