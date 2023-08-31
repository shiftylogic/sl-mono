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

#include <config/json.h>

TEST_CASE( "Parse JSON config", "[config][json]" )
{
    auto text = R"(
    {
        "Image": {
            "Width":  800,
            "Height": 600,
            "Title":  "View from 15th Floor",
            "Thumbnail": {
                "Url":    "http://www.example.com/image/481989943",
                "Height": 125,
                "Width":  100
            },
            "Animated" : false,
            "IDs": [116, 943, 234, -38793],
            "DeletionDate": null,
            "Distance": 12.723374634
        }
    }
    )";

    auto cfg = sl::config::json_from_string( text );
    REQUIRE( 800 == cfg.get< uint64_t >( "/Image/Width" ) );
    REQUIRE( 600 == cfg.get< uint64_t >( "/Image/Height" ) );
    REQUIRE( cfg.get< std::string >( "/Image/Thumbnail/Url" ).starts_with( "http://" ) );
    REQUIRE( 943 == cfg.get< int64_t >( "/Image/IDs/1" ) );
    REQUIRE( -38793 == cfg.get< int64_t >( "/Image/IDs/3" ) );
    REQUIRE( false == cfg.get< bool >( "/Image/Animated" ) );
    REQUIRE( 12.723374634 == cfg.get< double >( "/Image/Distance" ) );
}
