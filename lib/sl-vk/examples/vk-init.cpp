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

    struct app_configurator
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
         * Required interface for passing this struct to make_app_context
         * as an application configurator.
         */

        const char* application_name() const { return k_app_name; }
        uint32_t application_version() const { return k_app_version; }

        const char* engine_name() const { return k_engine_name; }
        uint32_t engine_version() const { return k_engine_version; }

        uint32_t api_version() const { return VK_API_VERSION_1_1; }

        auto instance_flags() const { return VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; }

        auto enabled_layers() const { return k_layers; }   // return std::span< const char* > {}; }
        auto enabled_extensions() const { return k_extensions; }
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

        logger.info( "Initializing application context..." );
        auto app_context = sl::vk::core::app_context { loader, app_config };
        app_context.enable_debug( handle_vulkan_debug );

        logger.info( "Enumerating GPU devices..." );
        auto physical_devices = app_context.get_physical_devices();
        sl::vk::core::debug::log_diagnostics( logger, std::span( physical_devices ) );

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
