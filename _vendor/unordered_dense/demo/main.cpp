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

#include <any>
#include <iostream>
#include <string>

#include <ankerl/unordered_dense.h>

namespace
{

    template< typename KVStore >
    class Store
    {
    public:
        template< typename Value >
        Value Get( const std::string& key ) const
        {
            return std::any_cast< Value >(
                static_cast< const KVStore* >( this )->GetCore( std::string {}, key ) );
        }

        template< typename Value >
        void Put( const std::string& key, const Value value )
        {
            static_cast< KVStore* >( this )->PutCore( std::string {}, key, value );
        }
    };

    class InMemoryStore : public Store< InMemoryStore >
    {

        using ScopedMap = ankerl::unordered_dense::map< std::string, std::any >;
        using RootMap   = ankerl::unordered_dense::map< std::string, ScopedMap >;

    public:
        std::any GetCore( const std::string& scope, const std::string& key ) const
        {
            auto scoped = _data.find( scope );
            if ( scoped == std::end( _data ) )
                return std::any {};

            if ( scoped->second.find( key ) == std::end( scoped->second ) )
                return std::any {};

            return scoped->second.at( key );
        }

        void PutCore( const std::string& scope, const std::string& key, std::any&& value )
        {
            auto& scoped = _data[scope];
            scoped[key]  = std::move( value );
        }

    private:
        RootMap _data;
    };

}   // namespace

int main()
{
    auto store = InMemoryStore {};

    store.Put( "test", 8 );
    std::cout << "Getting \"test\" => [" << store.Get< int >( "test" ) << "]\n";

    store.Put( "dude", std::string( "test" ) );
    std::cout << "Getting \"dude\" => \"" << store.Get< std::string >( "dude" ) << "\"\n";

    return 0;
}
