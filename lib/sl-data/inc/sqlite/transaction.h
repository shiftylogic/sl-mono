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

#ifndef __TRANSACTION_H_ECAA44F1D842445FB1D81A8FC88CABBC__
#define __TRANSACTION_H_ECAA44F1D842445FB1D81A8FC88CABBC__

#include <utils/noncopyable.h>

#include "./database.h"

namespace sl::data::sqlite
{

    struct transaction : sl::utils::noncopyable
    {
        enum class mode
        {
            deferred,
            immediate,
            exclusive,
        };

        explicit transaction( sqlite::database& db,
                              transaction::mode m = transaction::mode::deferred )
            : _db { db }
            , _active { true }
        {
            switch ( m )
            {
            case mode::deferred:
                _db.execute( "BEGIN DEFERRED" );
                break;

            case mode::immediate:
                _db.execute( "BEGIN IMMEDIATE" );
                break;

            case mode::exclusive:
                _db.execute( "BEGIN EXCLUSIVE" );
                break;
            }
        }

        ~transaction() noexcept
        {
            // Default to rollback if not explicitly committed.
            if ( _active )
                rollback();
        }

        inline void commit() noexcept
        {
            if ( _active )
                _db.execute( "COMMIT" );
            _active = false;
        }

        inline void rollback() noexcept
        {
            if ( _active )
                _db.execute( "ROLLBACK" );
            _active = false;
        }

    private:
        sqlite::db& _db;
        bool _active;
    };

}   // namespace sl::data::sqlite

#endif /* __TRANSACTION_H_ECAA44F1D842445FB1D81A8FC88CABBC__ */
