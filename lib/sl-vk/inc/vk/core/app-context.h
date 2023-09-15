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

#include <vk/core/resources.h>
#include <vk/core/structs.h>
#include <vk/error.h>

namespace sl::vk::core
{

    namespace priv
    {

        template< typename Loader, typename AppConfigurator >
        auto create_instance( Loader& loader, const AppConfigurator& cfg )
        {
            auto enabled_exts   = cfg.enabled_extensions();
            auto enabled_layers = cfg.enabled_layers();

            auto app_info               = core::application_info {};
            app_info.pApplicationName   = cfg.application_name();
            app_info.applicationVersion = cfg.application_version();
            app_info.pEngineName        = cfg.engine_name();
            app_info.engineVersion      = cfg.engine_version();
            app_info.apiVersion         = cfg.api_version();

            auto create_info                    = core::instance_create_info {};
            create_info.flags                   = cfg.instance_flags();
            create_info.enabledExtensionCount   = static_cast< uint32_t >( enabled_exts.size() );
            create_info.ppEnabledExtensionNames = enabled_exts.data();
            create_info.enabledLayerCount       = static_cast< uint32_t >( enabled_layers.size() );
            create_info.ppEnabledLayerNames     = enabled_layers.data();

            return loader.create_instance( create_info );
        }

        template< typename Loader, typename DebugHandler >
        auto create_debug( Loader& loader, core::instance& inst, DebugHandler debug_handler )
        {
            auto create = loader.template lookup_function< PFN_vkCreateDebugUtilsMessengerEXT >(
                inst, "vkCreateDebugUtilsMessengerEXT" );
            auto destroy = loader.template lookup_function< PFN_vkDestroyDebugUtilsMessengerEXT >(
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

            return core::debug_utils_messenger { inst, debug, destroy };
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

    template< typename Loader, typename AppConfigurator, typename DebugHandler >
    auto make_app_context( Loader& loader, const AppConfigurator& cfg, DebugHandler debug_handler )
    {
        auto inst  = priv::create_instance( loader, cfg );
        auto debug = priv::create_debug( loader, inst, debug_handler );

        return core::app_context { std::move( inst ), std::move( debug ) };
    }

    template< typename Loader, typename AppConfigurator >
    auto make_app_context( Loader& loader, const AppConfigurator& cfg )
    {
        return core::app_context { priv::create_instance( loader, cfg ),
                                   core::debug_utils_messenger {} };
    }

}   // namespace sl::vk::core

#endif /* __APP_CONTEXT_H_695CFE28DC364AC0BDBB28804FB77F87__ */
