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

#ifndef __COMMAND_H_B1FB2EA91B604B64A083743B46F3EB24__
#define __COMMAND_H_B1FB2EA91B604B64A083743B46F3EB24__

#include <string_view>
#include <type_traits>

#include <sqlite3.h>
#include <utils/noncopyable.h>

#include "./error.h"

namespace sl::data::sqlite
{

    struct command : sl::utils::noncopyable
    {
        explicit command( sqlite3* db, sqlite3_stmt* stmt )
            : _db { db }
            , _stmt { stmt }
        {}

        ~command() noexcept { ::sqlite3_finalize( _stmt ); }


        /**
         * A collection of methods for binding parameters / values.
         **/

        template< typename T >
        void bind( int, T );

        template<>
        inline void bind( int index, std::string_view value )
        {
            sqlite::error::throw_if(
                ::sqlite3_bind_text( _stmt, index, value.data(), value.size(), SQLITE_STATIC ),
                "sqlite3_bind_text",
                "failed to bind text value to prepared statement",
                _db );
        }

        template<>
        inline void bind( int index, const char* value )
        {
            bind( index, std::string_view { value } );
        }

        template<>
        inline void bind( int index, int value )
        {
            sqlite::error::throw_if( ::sqlite3_bind_int( _stmt, index, value ),
                                     "sqlite3_bind_int",
                                     "failed to bind integer value to prepared statement",
                                     _db );
        }

        template<>
        inline void bind( int index, double value )
        {
            sqlite::error::throw_if( ::sqlite3_bind_double( _stmt, index, value ),
                                     "sqlite3_bind_double",
                                     "failed to bind double value to prepared statement",
                                     _db );
        }

        template< typename T >
        inline void bind( const char* name, T value )
        {
            auto index = ::sqlite3_bind_parameter_index( _stmt, name );
            if ( index <= 0 )
                sqlite::error::throw_if( SQLITE_ERROR,
                                         "sqlite3_bind_parameter_index",
                                         "failed to acquire index of named paramter",
                                         "invalid named parameter" );

            bind( index, value );
        }


        /**
         * Command execution (and reset) methods
         **/

        inline void execute( bool auto_reset = true )
        {
            auto code = ::sqlite3_step( _stmt );
            if ( code != SQLITE_DONE )
                sqlite::error::throw_if( code, "sqlite3_step", "failed to run SQL statement", _db );

            if ( auto_reset )
                reset();
        }

        inline void reset()
        {
            sqlite::error::throw_if( ::sqlite3_reset( _stmt ),
                                     "sqlite3_reset",
                                     "failed to reset prepared SQL statement",
                                     _db );
            sqlite::error::throw_if( ::sqlite3_clear_bindings( _stmt ),
                                     "sqlite3_clear_bindings",
                                     "failed to clear prepared SQL statement bindings",
                                     _db );
        }

    private:
        sqlite3* _db;
        sqlite3_stmt* _stmt;
    };

}   // namespace sl::data::sqlite

#endif /* __COMMAND_H_B1FB2EA91B604B64A083743B46F3EB24__ */
