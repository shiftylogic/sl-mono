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

#ifndef __DATABASE_H_9B8002C0A9BA49C1BD2828FD8B6404C1__
#define __DATABASE_H_9B8002C0A9BA49C1BD2828FD8B6404C1__

#include <string_view>

#include <sqlite3.h>
#include <utils/deferred.h>

#include "./command.h"
#include "./error.h"

namespace sl::data::sqlite
{

    struct database
    {
        explicit database( const char* uri )
            : _db { nullptr }
        {
            auto code = ::sqlite3_open_v2(
                uri, &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, nullptr );
            if ( code == SQLITE_OK )
                return;

            // Save DB handle for cleanup, but remove the one in the failing ctor.
            auto db = _db;
            _db     = nullptr;

            // Must close DB even on failure (nullptr is no-op).
            //
            // NOTE: It needs to be deferred until after the exception creation
            //       so that we don't lose the error message.
            auto _ = sl::utils::deferred( [=]() { ::sqlite3_close( db ); } );

            // Throw an error on failure
            sqlite::error::throw_if(
                code, "sqlite3_open_v2", "failed to initialize sqlite database instance", db );
        }

        ~database() noexcept { ::sqlite3_close_v2( _db ); }


        /**
         * Functions used for constructing evaluating / preparing statements.
         **/

        void execute( std::string_view sql )
        {
            char* err;
            auto code = ::sqlite3_exec( _db, sql.data(), nullptr, nullptr, &err );
            if ( code == SQLITE_OK )
                return;

            sqlite::error::throw_if( code, "sqlite3_exec", "failed to execute SQL statement", err );
            sqlite3_free( err );
        }

        sqlite::command prepare_command( std::string_view sql )
        {
            sqlite3_stmt* stmt { nullptr };
            sqlite::error::throw_if(
                ::sqlite3_prepare_v2( _db, sql.data(), sql.size(), &stmt, nullptr ),
                "sqlite3_prepare_v2",
                "failed to prepare SQL statement",
                _db );
            return sqlite::command { _db, stmt };
        }

    private:
        sqlite3* _db;
    };

}   // namespace sl::data::sqlite

#endif /* __DATABASE_H_9B8002C0A9BA49C1BD2828FD8B6404C1__ */
