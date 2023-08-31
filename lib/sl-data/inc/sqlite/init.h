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

#ifndef __INIT_H_5DA3C90CBACF4E4996BA0993F0E7973C__
#define __INIT_H_5DA3C90CBACF4E4996BA0993F0E7973C__

#include <sqlite3.h>

#include "./error.h"

namespace sl::data::sqlite
{

    struct lib_init
    {
        lib_init()
        {
            sqlite::error::throw_if( ::sqlite3_initialize(),
                                     "sqllite_initaialize",
                                     "failed to initialize sqlite library" );
        }

        ~lib_init() noexcept { ::sqlite3_shutdown(); }
    };

}   // namespace sl::data::sqlite

#endif /* __INIT_H_5DA3C90CBACF4E4996BA0993F0E7973C__ */
