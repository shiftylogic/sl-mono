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

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>

#include <sqlite3.h>

namespace h
{

    void check_db_error( int code, const char* api, const char* message, const char* err = nullptr )
    {
        if ( code == SQLITE_OK )
            return;

        std::array< char, 256 > buf;
        std::snprintf( buf.data(),
                       buf.size(),
                       "*** DB ERROR *** '%s' (Code: [%d] %s) %s - %s",
                       api,
                       code,
                       ::sqlite3_errstr( code ),
                       message,
                       err ? err : "<unknown error>" );
        throw std::runtime_error { buf.data() };
    }

    void check_db_error( int code, const char* api, const char* message, sqlite3* db )
    {
        check_db_error( code, api, message, ::sqlite3_errmsg( db ) );
    }

    struct sqlite_init
    {
        sqlite_init()
        {
            check_db_error(
                ::sqlite3_initialize(), "sqlite3_initialize", "failed to initialize sqlite" );
        }

        ~sqlite_init() { ::sqlite3_shutdown(); }
    };

    struct statement
    {
        struct row
        {
            row( sqlite3_stmt* stmt )
                : _stmt { stmt }
            {}

            template< typename T >
            T column_as( int ) const;

            template<>
            std::string_view column_as( int column ) const
            {
                return std::string_view {
                    static_cast< const char* >(
                        static_cast< const void* >( ::sqlite3_column_text( _stmt, column ) ) ),
                    static_cast< size_t >( ::sqlite3_column_bytes( _stmt, column ) ) };
            }

            template<>
            int column_as( int column ) const
            {
                return ::sqlite3_column_int( _stmt, column );
            }

            template<>
            double column_as( int column ) const
            {
                return ::sqlite3_column_double( _stmt, column );
            }

        private:
            sqlite3_stmt* _stmt;
        };

        statement( sqlite3* db, std::string_view sql )
            : _db { db }
            , _stmt { nullptr }
        {
            check_db_error( ::sqlite3_prepare_v2( db, sql.data(), sql.size(), &_stmt, nullptr ),
                            "sqlite3_prepare_v2",
                            "failed to prepare SQL statement",
                            db );
        }

        ~statement() noexcept { ::sqlite3_finalize( _stmt ); }

        template< typename T >
        void bind( int, T );

        template<>
        void bind( int index, std::string_view value )
        {
            check_db_error(
                ::sqlite3_bind_text( _stmt, index, value.data(), value.size(), SQLITE_STATIC ),
                "sqlite_bind_text",
                "failed to bind text value to prepared statement",
                _db );
        }

        template<>
        void bind( int index, int value )
        {
            check_db_error( ::sqlite3_bind_int( _stmt, index, value ),
                            "sqlite_bind_int",
                            "failed to bind integer value to prepared statement",
                            _db );
        }

        template<>
        void bind( int index, double value )
        {
            check_db_error( ::sqlite3_bind_double( _stmt, index, value ),
                            "sqlite_bind_double",
                            "failed to bind double value to prepared statement",
                            _db );
        }

        void reset()
        {
            check_db_error( ::sqlite3_reset( _stmt ),
                            "sqlite3_reset",
                            "failed to reset prepared SQL statement",
                            _db );
            check_db_error( ::sqlite3_clear_bindings( _stmt ),
                            "sqlite3_clear_bindings",
                            "failed to clear prepared SQL statement bindings",
                            _db );
        }

        void execute()
        {
            auto code = ::sqlite3_step( _stmt );
            if ( code == SQLITE_DONE )
                return;

            check_db_error( code, "sqlite3_step", "failed to run SQL statement", _db );
        }

        void query( std::function< bool( const row& ) > cb )
        {
            for ( ;; )
            {
                auto code = ::sqlite3_step( _stmt );

                if ( code == SQLITE_DONE )
                    break;

                if ( code != SQLITE_ROW )
                {
                    check_db_error(
                        code, "sqlite3_step", "failed to step to next SQL result", _db );
                    break;
                }

                auto r = row { _stmt };
                if ( !cb( r ) )
                    break;
            }
        }

    private:
        sqlite3* _db;
        sqlite3_stmt* _stmt;
    };

    struct db
    {
        db()
            : _db { nullptr }
        {}

        ~db() noexcept { ::sqlite3_close( _db ); }

        void create()
        {
            auto code = ::sqlite3_open( ":memory:", &_db );
            check_db_error( code, "sqlite3_open", "failed to create db", _db );
        }

        void execute( std::string_view sql )
        {
            char* err;
            auto code = ::sqlite3_exec( _db, sql.data(), nullptr, nullptr, &err );
            if ( code == SQLITE_OK )
                return;

            check_db_error( code, "sqlite3_exec", "failed to execute SQL statement", err );
            sqlite3_free( err );
        }

        auto prepare_statement( std::string_view sql ) { return h::statement { _db, sql }; }

    private:
        sqlite3* _db;
    };

    constexpr const char* k_create_employees_table = R"QRY(
        CREATE TABLE employees (
            emp_id INTEGER PRIMARY KEY,
            name VARCHAR(512),
            level TINYINT,
            months_in_level SMALLINT,
            salary MONEY
        );
    )QRY";

    constexpr const char* k_insert_employee = R"QRY(
        INSERT INTO employees (emp_id, name, level, months_in_level, salary)
            VALUES (NULL, ?1, ?2, ?3, ?4);
    )QRY";

    constexpr const char* k_select_employees = R"QRY(
        SELECT * FROM employees;
    )QRY";

    constexpr const char* k_record_format
        = "Employee #%d\n"
          "    ID          => %d\n"
          "    Name        => %s\n"
          "    Level       => %d\n"
          "    In Level    => %d\n"
          "    Salary      => %.2f\n\n";


}   // namespace h

void demo()
{
    h::sqlite_init init;
    h::db db {};

    // Initialize the DB and table
    {
        db.create();
        db.execute( h::k_create_employees_table );
    }

    // Insert some records
    {
        auto insert_statement = db.prepare_statement( h::k_insert_employee );
        auto ins_fn           = [&]( std::string_view nim, int level, int months, double salary ) {
            insert_statement.bind( 1, nim );
            insert_statement.bind( 2, level );
            insert_statement.bind( 3, months );
            insert_statement.bind( 4, salary );
            insert_statement.execute();
            insert_statement.reset();
        };

        ins_fn( "Jim Beam", 4, 53, 23'650.00 );
        ins_fn( "Sandy Burger", 8, 14, 473'250.00 );
        ins_fn( "Kate Caller", 3, 172, 293'750.00 );
        ins_fn( "Doug Holl", 7, 34, 1'323'000.00 );
        ins_fn( "Jack Quick", 1, 3, 13'497.00 );
    }

    // Query all the records and dump them
    {
        auto select_statement = db.prepare_statement( h::k_select_employees );
        select_statement.query( [&, index = 1]( const auto& row ) mutable -> bool {
            std::array< char, 1024 > buf;
            std::snprintf( buf.data(),
                           buf.size(),
                           h::k_record_format,
                           index,
                           row.template column_as< int >( 0 ),
                           row.template column_as< std::string_view >( 1 ).data(),
                           row.template column_as< int >( 2 ),
                           row.template column_as< int >( 3 ),
                           row.template column_as< double >( 4 ) );
            std::cout << buf.data();

            ++index;
            return true;
        } );
    }
}

int main()
{
    try
    {
        demo();
    }
    catch ( const std::exception& ex )
    {
        std::cout << "*** Run-time exception *** - " << ex.what() << "\n";
    }
    catch ( ... )
    {
        std::cout << "*** UNKNOWN EXCEPTION ***\n";
    }

    return 0;
}
