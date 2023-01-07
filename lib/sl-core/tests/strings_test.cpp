/**
 * MIT License
 *
 * Copyright (c) 2023 Robert Anderson
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

TEST_CASE("Formatted lengths", "[utils][strings]")
{
    auto count = sl::utils::CalculateFormattedLength("something: %s - with <%i>", "dude", 12387);
    REQUIRE(count == strlen("something: dude - with <12387>"));
}

TEST_CASE("String formatting (non-allocating)", "[utils][strings]")
{
    std::string expected("Int: 42, String: test");
    std::array<char, 256> buffer;

    auto count = sl::utils::FormatString(buffer, "Int: %d, String: %s", 42, "test");
    REQUIRE(count == expected.size() + 1);
    REQUIRE(buffer[count - 1] == 0);
    REQUIRE(std::string(buffer.data(), count - 1) == expected);
}

TEST_CASE("String formatting (allocating)", "[utils][strings]")
{
    std::string expected("This string contains a string (a string) and a float (1.123).");

    auto actual = sl::utils::FormatString(
        "This string contains a string (%s) and a float (%1.3f).", "a string", 1.123f);
    REQUIRE(actual.size() == expected.size());
    REQUIRE(actual == expected);
}
