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

#ifndef __VERSION_H_23191919EDDF411784E4E9B0373DB169__
#define __VERSION_H_23191919EDDF411784E4E9B0373DB169__

#include <cstdint>
#include <string>

namespace sl::utils
{

    struct version
    {
        const uint32_t major : 10;
        const uint32_t minor : 10;
        const uint32_t patch : 12;

        constexpr version( uint32_t major, uint32_t minor, uint32_t patch )
            : major { major }
            , minor { minor }
            , patch { patch }
        {}

        constexpr version( uint32_t ver )
            : major { ver >> 22 }
            , minor { ( ver >> 12 ) & 0x3ffu }
            , patch { ver & 0xfffu }
        {}

        operator uint32_t() const { return ( major << 22 ) | ( minor << 12 ) | patch; }

        std::string as_string() const
        {
            // NOTE: Small string optimization should have this result in
            //       zero allocations on the heap.
            return "v" + std::to_string( major ) + "." + std::to_string( minor ) + "."
                   + std::to_string( patch );
        }
    };

}   // namespace sl::utils

#endif /* __VERSION_H_23191919EDDF411784E4E9B0373DB169__ */
