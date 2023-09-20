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

#include <span>
#include <vector>

#include <vk/core/base.h>
#include <vk/error.h>

namespace sl::vk::core
{

    namespace priv
    {

        template< typename Loader, typename Configurator >
        auto create_instance( const core::loader_base< Loader >& loader,
                              const core::app_configurator< Configurator >& cfg )
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
        auto create_debug_messenger( const core::loader_base< Loader >& loader,
                                     const core::instance& inst,
                                     DebugHandler debug_handler )
        {
            auto ci            = core::debug_utils_messenger_create_info_ext {};
            ci.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            ci.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            ci.pfnUserCallback = *debug_handler;
            ci.pUserData       = nullptr;

            return loader.create_debug_messenger( inst, ci );
        }

    }   // namespace priv

    struct app_context
    {
        explicit app_context( core::instance&& inst, std::vector< core::physical_device >&& gpus )
            : _instance { std::move( inst ) }
            , _gpus { std::move( gpus ) }
        {}

        explicit app_context( core::instance&& inst,
                              std::vector< core::physical_device >&& gpus,
                              core::debug_utils_messenger&& debug )
            : _instance { std::move( inst ) }
            , _gpus { std::move( gpus ) }
            , _debug { std::move( debug ) }
        {}

        operator const core::instance&() const noexcept { return _instance; }

        auto gpus() const { return std::span { _gpus }; }

    private:
        const core::instance _instance;
        const std::vector< core::physical_device > _gpus;
        core::debug_utils_messenger _debug;
    };

    template< typename Loader, typename Configurator >
    auto make_app_context( const core::loader_base< Loader >& loader,
                           const core::app_configurator< Configurator >& cfg )
    {
        auto inst = priv::create_instance( loader, cfg );
        auto gpus = loader.get_physical_devices( inst );

        return core::app_context { std::move( inst ), std::move( gpus ) };
    }

    template< typename Loader, typename Configurator, typename DebugHandler >
    auto make_app_context( const core::loader_base< Loader >& loader,
                           const core::app_configurator< Configurator >& cfg,
                           DebugHandler debug_handler )
    {
        auto inst = priv::create_instance( loader, cfg );
        auto gpus = loader.get_physical_devices( inst );

        return core::app_context { std::move( inst ),
                                   std::move( gpus ),
                                   priv::create_debug_messenger( loader, inst, debug_handler ) };
    }

}   // namespace sl::vk::core

#endif /* __APP_CONTEXT_H_695CFE28DC364AC0BDBB28804FB77F87__ */
