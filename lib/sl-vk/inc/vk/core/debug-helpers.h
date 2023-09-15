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

#include <charconv>

#include <vk/core/loader.h>

namespace sl::vk::core::debug
{

    namespace priv
    {
        /**
         * Turns a uint32_t API version number into a display string.
         *
         * Writes directly to a supplied character buffer and returns a pointer to it.
         *
         * Takes the form 'xxxx.yyyy.zzzz [v]'. The '[v] is the "variant" of the API.
         * The string length will never be more than 18 + 1 characters (NULL-terminated).
         */

        const char* api_to_string( uint32_t version, std::span< char, 20 > buf )
        {
            /**
             * Takes the form 'xxxx.yyyy.zzzz [v]'. The '[v] is the "variant" which appears
             * to almost always be 0 (zero).
             * This will always be 9-18 characters plus \0.
             */
            const uint32_t var   = version >> 29;                // 0-3
            const uint32_t major = ( version >> 22 ) & 0x3ffu;   // 0 - 1023
            const uint32_t minor = ( version >> 12 ) & 0x3ffu;   // 0 - 1023
            const uint32_t patch = version & 0xfffu;             // 0 - 4095
            const auto buf_end   = buf.data() + buf.size();

            auto r   = std::to_chars( buf.data(), buf_end, major );
            *r.ptr++ = '.';

            r        = std::to_chars( r.ptr, buf_end, minor );
            *r.ptr++ = '.';

            r        = std::to_chars( r.ptr, buf_end, patch );
            *r.ptr++ = ' ';
            *r.ptr++ = '[';

            r        = std::to_chars( r.ptr, buf_end, var );
            *r.ptr++ = ']';
            *r.ptr++ = '\0';

            return buf.data();
        }

        /**
         * Turns a uint32_t version number into a display string.
         *
         * Writes directly to a supplied character buffer and returns a pointer to it.
         *
         * Takes the form 'xxxx.yyyy.zzzz'.
         * The string length will never be more than 14 + 1 characters (NULL-terminated).
         */
        const char* version_to_string( uint32_t version, std::span< char, 16 > buf )
        {
            const uint32_t major = version >> 22;                // 0 - 1023
            const uint32_t minor = ( version >> 12 ) & 0x3ffu;   // 0 - 1023
            const uint32_t patch = version & 0xfffu;             // 0 - 4095
            const auto buf_end   = buf.data() + buf.size();

            auto r   = std::to_chars( buf.data(), buf_end, major );
            *r.ptr++ = '.';

            r        = std::to_chars( r.ptr, buf_end, minor );
            *r.ptr++ = '.';

            r        = std::to_chars( r.ptr, buf_end, patch );
            *r.ptr++ = '\0';

            return buf.data();
        }

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


        template< typename Logger, typename PTypeItems >
        void dump_properties( Logger& logger, PTypeItems props, const char* indent )
        {
            std::for_each(
                std::begin( props ), std::end( props ), [&logger, indent]( const auto& x ) {
                    logger.trace( "%s %s", indent, priv::ptype_to_name( x ) );
                } );
        }

    }   // namespace priv


    /**
     * Debug helpers for logging diagnostics information from various types.
     * This should be a collection of methods with type overrides.
     */

    template< typename Logger >
    void log_diagnostics( Logger& logger, const sl::vk::core::loader& loader )
    {
        char buf[20];

        logger.trace( "Built with Vulkan SDK v%s",
                      priv::api_to_string( VK_HEADER_VERSION_COMPLETE, buf ) );
        logger.trace( "Running with Vulkan v%s",
                      priv::api_to_string( loader.runtime_version(), buf ) );

        auto layers = loader.available_layers();
        logger.trace( "Available layers:" );
        priv::dump_properties( logger, layers, "  >>>" );

        auto exts = loader.available_extensions();
        logger.trace( "Available extensions:" );
        priv::dump_properties( logger, exts, "  >>>" );
    }

    template< typename Logger, typename AppConfigurator >
    void log_diagnostics( Logger& logger, const AppConfigurator& cfg )
    {
        char buf[16];

        logger.trace( "Vulkan instance configuration:" );
        logger.trace( "  -> App Name: %s", cfg.application_name() );
        logger.trace( "  -> App Version: %s",
                      priv::version_to_string( cfg.application_version(), buf ) );
        logger.trace( "  -> Engine Name: %s", cfg.engine_name() );
        logger.trace( "  -> Engine Version: %s",
                      priv::version_to_string( cfg.engine_version(), buf ) );
        logger.trace( "  -> API Version: %s", priv::version_to_string( cfg.api_version(), buf ) );

        auto layers = cfg.enabled_layers();
        logger.trace( "  -> Enabled Layers:" );
        priv::dump_properties( logger, layers, "    +++" );

        auto exts = cfg.enabled_extensions();
        logger.trace( "  -> Enabled Extensions:" );
        priv::dump_properties( logger, exts, "    +++" );
    }

}   // namespace sl::vk::core::debug

#endif /* __DEBUG_HELPERS_H_50FB080ED2FD40E28764DAFE7030B4BA__ */
