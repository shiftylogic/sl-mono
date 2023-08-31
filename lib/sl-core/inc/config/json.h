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

#ifndef __JSON_H_B03E82EB46C440738B36CEFC226BCCEA__
#define __JSON_H_B03E82EB46C440738B36CEFC226BCCEA__

#define JSON_HAS_CPP_20                      1
#define JSON_NO_IO                           0
#define JSON_SKIP_UNSUPPORTED_COMPILER_CHECK 1
#include <nlohmann/json.hpp>

#include <io/mapped-file.h>

namespace sl::config
{

    template< typename ConfigData >
    struct values
    {
        explicit values( ConfigData&& data )
            : _data { std::move( data ) }
        {}

        template< typename T >
        inline T get( const std::string& key ) const
        {
            return _data.template get< T >( key );
        }

    private:
        ConfigData _data;
    };

    struct json
    {
    public:
        explicit json( const std::string_view data )
        {
            auto j = nlohmann::json::parse( std::begin( data ), std::end( data ) );
            _data  = j.flatten();
        }

        template< typename T >
        T get( const std::string& key ) const
        {
            if ( !_data.contains( key ) )
                throw std::runtime_error( "config key not found" );

            return _data[key].get< T >();
        }

    private:
        nlohmann::json _data;
    };

    auto json_from_string( const std::string_view data )
    {
        return config::values { config::json { data } };
    }

    auto load_json( const char* const path )
    {
        auto mf   = io::mapped_file( path, io::cache_hint::sequential );
        auto mv   = mf.map_view( 0, mf.size() );
        auto data = mv.as_items< char >();

        return json_from_string( std::string_view { data.data(), data.size() } );
    }

}   // namespace sl::config


#endif /* __JSON_H_B03E82EB46C440738B36CEFC226BCCEA__ */
