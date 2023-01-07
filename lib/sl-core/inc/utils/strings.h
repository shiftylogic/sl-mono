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

#ifndef __STRINGS_H_7F4C61F262C340A09B9D54E42E195235__
#define __STRINGS_H_7F4C61F262C340A09B9D54E42E195235__

#include <memory>
#include <span>
#include <stdexcept>
#include <string>

namespace sl::utils
{

    // This entire computation allows compile-time string constant to
    // to be concatenates into a single string_view instances.
    template< std::string_view const&... Strs >
    struct StringMerger
    {
        static constexpr auto Merge() noexcept
        {
            constexpr std::size_t len = ( Strs.size() + ... + 0 );

            std::array< char, len + 1 > arr {};
            auto append = [i = 0, &arr]( auto const& s ) mutable {
                for ( auto c : s )
                    arr[i++] = c;
            };

            ( append( Strs ), ... );
            arr[len] = 0;

            return arr;
        }

        static constexpr auto _buffer = Merge();
        static constexpr const char* value { _buffer.data() };
    };

    // Helper to get the value out
    template< std::string_view const&... Strs >
    static constexpr auto ConstJoin = StringMerger< Strs... >::value;


    template< typename... Args >
    size_t CalculateFormattedLength( const char* format, Args... args )
    {
        auto length = std::snprintf( nullptr, 0, format, args... );
        if ( length <= 0 )
            throw std::runtime_error( "error computing format string length" );

        return length;
    }

    template< typename... Args >
    size_t FormatString( std::span< char > buffer, const char* format, Args... args )
    {
        auto count = std::snprintf( buffer.data(), buffer.size(), format, args... );
        if ( count <= 0 )
            throw std::runtime_error( "error formatting string" );
        if ( static_cast< size_t >( count ) >= buffer.size() )
            throw std::runtime_error( "buffer too small" );

        return count + 1;   // The NULL-termination was also written
    }

    template< typename... Args >
    std::string FormatString( const char* format, Args... args )
    {
        auto needed = CalculateFormattedLength( format, args... );
        auto str    = std::string( needed, 0 );
        auto buf    = std::span( &str[0], str.capacity() );

        FormatString( buf, format, args... );

        return str;
    }

    template< typename StringType >
    std::string Join( const std::span< StringType > ss )
    {
        if ( ss.size() == 0 )
            return std::string();

        std::string res( ss[0] );
        for ( size_t i = 1; i < ss.size(); i++ )
            res = res + ", " + ss[i];

        return res;
    }

}   // namespace sl::utils

#endif /* __STRINGS_H_7F4C61F262C340A09B9D54E42E195235__ */
