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

#include <sqlite/database.h>
#include <sqlite/init.h>

namespace
{

    constexpr const char* k_create_db = R"QRY(
        CREATE TABLE IF NOT EXISTS players (
            id INT PRIMARY KEY,
            name VARCHAR(512) NOT NULL,
            position TINYINT NOT NULL
        );
    )QRY";

    constexpr const char* k_insert_player = R"QRY(
        INSERT INTO players (id, name, position)
            VALUES (NULL, :name, :pos);
    )QRY";

}   // namespace

int main()
{
    auto sqlite_init = sl::data::sqlite::lib_init {};
    auto db          = sl::data::sqlite::database { "file:data.db?mode=rwc" };

    // Create the player table (as needed)
    db.execute( k_create_db );

    // Attempt an INSERT
    auto cmd = db.prepare_command( k_insert_player );
    cmd.bind( 1, "Nathaniel Anderson" );
    cmd.bind( ":pos", 0b0000110 );
    cmd.execute();
    cmd.bind( ":name", "Alex Lau" );
    cmd.bind( 2, 0b0111000 );
    cmd.execute();

    return 0;
}
