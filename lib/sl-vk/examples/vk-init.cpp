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

#include <array>
#include <span>

#include <logging/logger.h>
#include <utils/version.h>

#include <vk/core/app-context.h>
#include <vk/core/debug-helpers.h>
#include <vk/core/loader.h>
#include <vk/core/logical-device.h>
#include <vk/mem/allocator.h>
#include <vk/mem/buffers.h>
#include <vk/mem/images.h>

namespace
{

    auto debug_logger = sl::logging::logger {};

    static VKAPI_ATTR VkBool32 VKAPI_CALL
    handle_vulkan_debug( VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                         VkDebugUtilsMessageTypeFlagsEXT type,
                         [[maybe_unused]] const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                         void* /*pUserData*/ )
    {
        if ( type == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT )
        {
            debug_logger.warn( "*** Performance *** ==> %s", pCallbackData->pMessage );
            return VK_FALSE;
        }

        switch ( severity )
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            // debug_logger.trace( pCallbackData->pMessage );
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            debug_logger.info( pCallbackData->pMessage );
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            debug_logger.warn( pCallbackData->pMessage );
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            debug_logger.error( pCallbackData->pMessage );
            break;

        default:
            debug_logger.info( "Unknown severity - %s", pCallbackData->pMessage );
        }

        return VK_FALSE;
    }

    struct app_configurator : sl::vk::core::app_configurator< app_configurator >
    {
        static constexpr std::array k_layers = {
            "VK_LAYER_KHRONOS_validation",
        };

        static constexpr std::array k_extensions = {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
        };

        static constexpr const char* k_app_name = "vk-init";
        static constexpr sl::utils::version k_app_version { 0, 3, 0 };

        static constexpr const char* k_engine_name = "sl-engine";
        static constexpr sl::utils::version k_engine_version { 0, 2, 1 };


        /**
         * Required interface for passing this struct to app_context ctor
         * as an application configurator.
         */

        const char* application_name() const { return k_app_name; }
        uint32_t application_version() const { return k_app_version; }

        const char* engine_name() const { return k_engine_name; }
        uint32_t engine_version() const { return k_engine_version; }

        uint32_t api_version() const { return VK_API_VERSION_1_1; }

        auto instance_flags() const { return VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; }

        auto enabled_layers() const { return k_layers; }
        auto enabled_extensions() const { return k_extensions; }
    };

    struct device_configurator : sl::vk::core::device_configurator< device_configurator >
    {
        static constexpr std::array k_extensions = {
            "VK_KHR_bind_memory2",
            "VK_KHR_dedicated_allocation",
            "VK_KHR_get_memory_requirements2",
            "VK_KHR_portability_subset",
        };

        static constexpr std::array k_features = {
            sl::vk::core::device_feature::anisotropic_filtering,
            sl::vk::core::device_feature::depth_clamp,
            sl::vk::core::device_feature::fill_mode_non_solid,
            sl::vk::core::device_feature::multi_draw_indirect,
        };

        /**
         * Required interface for passing this struct to logical_device ctor
         * to be used for device configuration.
         */

        auto enabled_extensions() const { return k_extensions; }
        auto enabled_features() const { return k_features; }
        auto queues() const { return std::make_tuple( 0, 1, 2 ); }
    };

}   // namespace

int main()
{
    auto logger = sl::logging::logger {};

    try
    {
        logger.info( "Initializing loader..." );

        auto loader     = sl::vk::core::loader {};
        auto app_config = app_configurator {};

        sl::vk::core::debug::log_diagnostics( logger, loader );
        sl::vk::core::debug::log_diagnostics( logger, app_config );

        logger.info( "Creating an application context..." );
        auto app_context
            = sl::vk::core::make_app_context( loader, app_config, handle_vulkan_debug );

        logger.info( "Enumerating GPU devices..." );
        auto gpus = app_context.gpus();
        sl::vk::core::debug::log_diagnostics( logger, gpus );

        // TODO: Select a device based on some scoring algorithm...
        //       For now, just grab the first one.
        const auto& gpu = gpus[0];

        logger.info( "Creating a logical device..." );
        auto device_config = device_configurator {};
        sl::vk::core::debug::log_diagnostics( logger, device_config );

        auto device = sl::vk::core::make_logical_device( loader, app_context, gpu, device_config );

        {
            logger.info( "Creating allocator..." );
            auto mem = sl::vk::mem::make_allocator( loader, device, VK_API_VERSION_1_1 );

            {
                logger.info( "Creating a staging buffer..." );
                auto stbo = sl::vk::mem::make_staging_buffer( mem, 4096 );
                logger.info( "   -> Host Visible? %s", stbo.is_host_visible() ? "YES" : "NO" );

                logger.info( "Creating a readback buffer..." );
                auto rbo = sl::vk::mem::make_readback_buffer( mem, 4096 );
                logger.info( "   -> Host Visible? %s", rbo.is_host_visible() ? "YES" : "NO" );

                logger.info( "Creating an index buffer..." );
                auto ibo = sl::vk::mem::make_index_buffer( mem, 65536 );
                logger.info( "   -> Host Visible? %s", ibo.is_host_visible() ? "YES" : "NO" );

                logger.info( "Creating a vertex buffer..." );
                auto vbo = sl::vk::mem::make_uniform_buffer( mem, 65536 );
                logger.info( "   -> Host Visible? %s", vbo.is_host_visible() ? "YES" : "NO" );

                logger.info( "Creating a storage buffer..." );
                auto ssbo = sl::vk::mem::make_storage_buffer( mem, 65536 );
                logger.info( "   -> Host Visible? %s", ssbo.is_host_visible() ? "YES" : "NO" );

                logger.info( "Creating a uniform buffer..." );
                auto ubo = sl::vk::mem::make_uniform_buffer( mem, 65536 );
                logger.info( "   -> Host Visible? %s", ubo.is_host_visible() ? "YES" : "NO" );
            }

            {
                logger.info( "Creating color attachment image..." );
                auto color_image
                    = sl::vk::mem::make_color_image( mem, 3840, 2160, VK_FORMAT_R8G8B8A8_UNORM );

                logger.info( "Creating depth stencil attachment image..." );
                auto depth_image
                    = sl::vk::mem::make_depth_image( mem, 3840, 2160, VK_FORMAT_R8G8B8A8_UNORM );
            }
        }

        logger.info( "Shutting down..." );
    }
    catch ( const sl::vk::error& ex )
    {
        ex.log( logger );
    }
    catch ( const std::exception& ex )
    {
        logger.error( "*** std::exception *** %s", ex.what() );
    }
    catch ( ... )
    {
        logger.error( "*** UNKNOWN EXCEPTION ***" );
    }

    logger.info( "bye." );
    return 0;
}
