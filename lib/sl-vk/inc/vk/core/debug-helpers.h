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

#ifndef __DEBUG_HELPERS_H_50FB080ED2FD40E28764DAFE7030B4BA__
#define __DEBUG_HELPERS_H_50FB080ED2FD40E28764DAFE7030B4BA__

namespace sl::vk::core::debug
{

    namespace priv
    {

        const char* ptype_to_name( const char* name )
        {
            return name;
        }

        const char* ptype_to_name( const VkLayerProperties& layer )
        {
            return layer.layerName;
        }

        const char* ptype_to_name( const VkExtensionProperties& ext )
        {
            return ext.extensionName;
        }

    }   // namespace priv

    template< typename Logger, typename PType >
    void dump_properties( Logger& logger, const std::span< PType > props, const char* label )
    {
        logger.trace( label );
        std::for_each( std::begin( props ), std::end( props ), [&logger]( const auto& x ) {
            logger.trace( "  >>> %s", priv::ptype_to_name( x ) );
        } );
    }

}   // namespace sl::vk::core::debug

#endif /* __DEBUG_HELPERS_H_50FB080ED2FD40E28764DAFE7030B4BA__ */
