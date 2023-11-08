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

#include <io/load.h>
#include <logging/logger.h>
#include <utils/fs.h>
#include <utils/version.h>

#include <vk/core/app-context.h>
#include <vk/core/debug-helpers.h>
#include <vk/core/loader.h>
#include <vk/core/logical-device.h>
#include <vk/mem/allocator.h>
#include <vk/mem/buffers.h>
#include <vk/spv/shader.h>

#include <glm/vec4.hpp>

namespace
{

    using pixel = glm::vec4;

    constexpr auto k_image_width  = 3200;
    constexpr auto k_image_height = 2400;

    constexpr auto k_image_buffer_size = sizeof( pixel ) * k_image_width * k_image_height;

    const auto k_mandelbrot_spv_file = sl::utils::base_directory() / "assets/mandelbrot.spv";


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
            debug_logger.trace( pCallbackData->pMessage );
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

        static constexpr const char* k_app_name = "vk-compute";
        static constexpr sl::utils::version k_app_version { 0, 1, 0 };

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

        /**
         * Required interface for passing this struct to logical_device ctor
         * to be used for device configuration.
         */

        auto enabled_extensions() const { return k_extensions; }
        auto enabled_features() const { return sl::vk::core::k_no_device_features; }
        auto queues() const { return std::make_tuple( -1, 0, 1 ); }
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

        logger.info( "Setting up compute environment..." );
        auto device = sl::vk::core::make_logical_device( loader, app_context, gpu, device_config );
        auto mem    = sl::vk::mem::make_allocator( loader, device, VK_API_VERSION_1_1 );
        auto stbuf  = sl::vk::mem::make_storage_buffer( mem, k_image_buffer_size );

        auto spv_module
            = sl::vk::spv::shader { sl::io::load_file< uint32_t >( k_mandelbrot_spv_file ) };
        // TODO: Load a shader program
        // TODO: Setup descriptor set pointing to storage buffer
        // TODO: Dispatch GPU compute
        // TODO: Add barrier for compute phase
        // TODO: Add transfer from storage buffer to readback buffer
        // TODO: Flush & wait
        // TODO: Write resulting image to disk

        logger.info( "Shutting down..." );
    }
    catch ( const sl::vk::spv::error& ex )
    {
        ex.log( logger );
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
