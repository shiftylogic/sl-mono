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

#ifndef __APP_CONTEXT_H_695CFE28DC364AC0BDBB28804FB77F87__
#define __APP_CONTEXT_H_695CFE28DC364AC0BDBB28804FB77F87__

#include <charconv>
#include <functional>
#include <optional>

#include <vk/core/resources.h>
#include <vk/core/structs.h>
#include <vk/error.h>

namespace sl::vk::core
{

    namespace priv
    {

        const char* version_to_string( uint32_t version, std::span< char, 16 > buf )
        {
            const uint32_t major = version >> 22;                // 0 - 1023
            const uint32_t minor = ( version >> 12 ) & 0x3ffu;   // 0 - 1023
            const uint32_t patch = version & 0xfffu;             // 0 - 4095
            const auto buf_end   = buf.data() + buf.size();

            auto r     = std::to_chars( buf.data(), buf_end, major );
            *( r.ptr ) = '.';

            r          = std::to_chars( r.ptr + 1, buf_end, minor );
            *( r.ptr ) = '.';

            r          = std::to_chars( r.ptr + 1, buf_end, patch );
            *( r.ptr ) = '\0';

            return buf.data();
        }

        template< typename Logger, typename Loader, typename AppConfigurator >
        auto create_instance( Logger& logger, Loader& loader, AppConfigurator& cfg )
        {
            char buf[16];

            /**
             * Gather up all the application configuration settings. We use these a
             * couple times here, so just holding them for duration of this funciton.
             */
            auto app_name       = cfg.application_name();
            auto app_version    = cfg.application_version();
            auto engine_name    = cfg.engine_name();
            auto engine_version = cfg.engine_version();
            auto api_version    = cfg.api_version();

            auto flags          = cfg.instance_flags();
            auto enabled_exts   = cfg.enabled_extensions();
            auto enabled_layers = cfg.enabled_layers();

            /**
             * Document in the logger all the things we are initializing.
             */
            logger.info( "Creating Vulkan instance..." );
            logger.info( "  -> App Name: %s", app_name );
            logger.info( "  -> App Version: %s", version_to_string( app_version, buf ) );
            logger.info( "  -> Engine Name: %s", engine_name );
            logger.info( "  -> Engine Version: %s", version_to_string( engine_version, buf ) );
            logger.info( "  -> API Version: %s", version_to_string( api_version, buf ) );

            /**
             * Now construct the instance
             */
            auto app_info               = core::application_info {};
            app_info.pApplicationName   = app_name;
            app_info.applicationVersion = app_version;
            app_info.pEngineName        = engine_name;
            app_info.engineVersion      = engine_version;
            app_info.apiVersion         = api_version;

            auto create_info                    = core::instance_create_info {};
            create_info.flags                   = flags;
            create_info.enabledExtensionCount   = static_cast< uint32_t >( enabled_exts.size() );
            create_info.ppEnabledExtensionNames = enabled_exts.data();
            create_info.enabledLayerCount       = static_cast< uint32_t >( enabled_layers.size() );
            create_info.ppEnabledLayerNames     = enabled_layers.data();

            return loader.create_instance( create_info );
        }

        template< typename Logger, typename Loader, typename DebugHandler >
        auto create_debug( Logger& logger,
                           Loader& loader,
                           core::instance& inst,
                           DebugHandler debug_handler )
        {
            try
            {
                auto create = loader.template lookup_function< PFN_vkCreateDebugUtilsMessengerEXT >(
                    inst, "vkCreateDebugUtilsMessengerEXT" );
                auto destroy
                    = loader.template lookup_function< PFN_vkDestroyDebugUtilsMessengerEXT >(
                        inst, "vkDestroyDebugUtilsMessengerEXT" );

                core::debug_utils_messenger_create_info_ext ci;
                ci.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                ci.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                ci.pfnUserCallback = *debug_handler;
                ci.pUserData       = nullptr;

                VkDebugUtilsMessengerEXT debug;
                vk::error::throw_if_error( "vkCreateDebugUtilsMessengerEXT",
                                           create( inst, &ci, nullptr, &debug ),
                                           "failed to enable debug utils" );
                logger.info( "Debug extension enabled" );

                return core::debug_utils_messenger { inst, debug, destroy };
            }
            catch ( const vk::error& ex )
            {
                // This failure is fine. We are just trying to enable debug.
                ex.log( logger );
                logger.warn( "Cannot enable DEBUG extension. Debug extension not available." );
            }

            return core::debug_utils_messenger {};
        }

    }   // namespace priv

    struct app_context
    {
        explicit app_context( core::instance&& inst, core::debug_utils_messenger&& debug )
            : _instance { std::move( inst ) }
            , _debug { std::move( debug ) }
        {}

    private:
        core::instance _instance;
        core::debug_utils_messenger _debug;
    };

    template< typename Logger, typename Loader, typename Initializer, typename DebugHandler >
    auto
    make_app_context( Logger& logger, Loader& loader, Initializer init, DebugHandler debug_handler )
    {
        auto inst  = priv::create_instance( logger, loader, init );
        auto debug = priv::create_debug( logger, loader, inst, debug_handler );

        return core::app_context { std::move( inst ), std::move( debug ) };
    }

    template< typename Logger, typename Loader, typename Initializer >
    auto make_app_context( Logger& logger, Loader& loader, Initializer init )
    {
        return core::app_context { priv::create_instance( logger, loader, init ),
                                   core::debug_utils_messenger {} };
    }

}   // namespace sl::vk::core

#endif /* __APP_CONTEXT_H_695CFE28DC364AC0BDBB28804FB77F87__ */
