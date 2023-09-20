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

#ifndef __LOADER_H_563420A83E5945238A07F6B49AEAC37C__
#define __LOADER_H_563420A83E5945238A07F6B49AEAC37C__

#include <functional>
#include <tuple>
#include <type_traits>
#include <vector>

#include <vk/core/base.h>
#include <vk/error.h>

namespace sl::vk::core
{

    struct loader;

    /**
     * Specialization of device_functions_traits for the device_functions_t type
     * used in the sl::vk::core::loader default loader.
     */
    template<>
    struct device_functions_traits< core::loader >
    {
        using type = ::VolkDeviceTable;
    };

    struct loader : loader_base< loader >
    {
        static_assert( std::is_base_of< device_functions_t, VolkDeviceTable >::value );

        explicit loader()
        {
            vk::error::throw_if_error( "volkInitialize",
                                       ::volkInitialize(),
                                       "failed to initialize the Vulkan loader (Volk)" );
        }


        /**
         * Methods available before an instance is even created. Essentially available
         * in the Vulkan runtime before anything else is done.
         **/

        auto available_layers() const
        {
            uint32_t count;
            vk::error::throw_if_error( "vkEnumerateInstanceLayerProperties",
                                       ::vkEnumerateInstanceLayerProperties( &count, nullptr ),
                                       "failed to enumerate instance layer properties (count)" );

            std::vector< VkLayerProperties > layers( count );
            vk::error::throw_if_error(
                "vkEnumerateInstanceLayerProperties",
                ::vkEnumerateInstanceLayerProperties( &count, layers.data() ),
                "failed to enumerate instance layer properties" );

            return layers;
        }

        auto available_extensions() const
        {
            uint32_t count;
            vk::error::throw_if_error(
                "vkEnumerateInstanceExtensionProperties",
                ::vkEnumerateInstanceExtensionProperties( nullptr, &count, nullptr ),
                "failed to enumerate instance extension properties (count)" );

            std::vector< VkExtensionProperties > exts( count );
            vk::error::throw_if_error(
                "vkEnumerateInstanceExtensionProperties",
                ::vkEnumerateInstanceExtensionProperties( nullptr, &count, exts.data() ),
                "failed to enumerate instance extension properties" );

            return exts;
        }

        auto create_instance( const core::instance_create_info& ci ) const
        {
            VkInstance inst;
            vk::error::throw_if_error( "vkCreateInstance",
                                       ::vkCreateInstance( &ci, nullptr, &inst ),
                                       "failed to create vulkan instance" );

            // We will still need to initialize the VkInstance with Volk
            ::volkLoadInstanceOnly( inst );

            return core::instance( inst, ::vkDestroyInstance );
        }

        auto runtime_version() const { return ::volkGetInstanceVersion(); }


        /**
         * Methods available based on a specific Vulkan instance.
         **/

        auto create_debug_messenger( const core::instance& inst,
                                     const core::debug_utils_messenger_create_info_ext& ci ) const
        {
            VkDebugUtilsMessengerEXT debug;
            vk::error::throw_if_error(
                "vkCreateDebugUtilsMessengerEXT",
                ::vkCreateDebugUtilsMessengerEXT( inst, &ci, nullptr, &debug ),
                "failed to enable debug utils" );

            return core::debug_utils_messenger { inst, debug, ::vkDestroyDebugUtilsMessengerEXT };
        }

        auto create_device( const core::instance&,
                            const core::physical_device& gpu,
                            const core::device_create_info& ci ) const
        {
            VkDevice device;
            vk::error::throw_if_error( "vkCreateDevice",
                                       ::vkCreateDevice( gpu, &ci, nullptr, &device ),
                                       "failed to create logical device" );

            VolkDeviceTable vkFuncs;
            ::volkLoadDeviceTable( &vkFuncs, device );

            return std::make_tuple( core::device { device, vkFuncs.vkDestroyDevice }, vkFuncs );
        }

        auto get_physical_devices( const core::instance& inst ) const
        {
            uint32_t count;
            vk::error::throw_if_error( "vkEnumeratePhysicalDevices",
                                       ::vkEnumeratePhysicalDevices( inst, &count, nullptr ),
                                       "failed to acquire physical devices (count)" );
            if ( count == 0 )
                throw std::runtime_error( "no Vulkan compatible device found" );

            std::vector< VkPhysicalDevice > device_handles( count );
            vk::error::throw_if_error(
                "vkEnumeratePhysicalDevices",
                ::vkEnumeratePhysicalDevices( inst, &count, device_handles.data() ),
                "failed to acquire physical devices (count)" );

            std::vector< core::physical_device > devices;
            devices.reserve( device_handles.size() );

            std::transform(
                std::begin( device_handles ),
                std::end( device_handles ),
                std::back_inserter( devices ),
                []( auto device_handle ) {
                    VkPhysicalDeviceProperties props;
                    ::vkGetPhysicalDeviceProperties( device_handle, &props );

                    VkPhysicalDeviceMemoryProperties memory_props;
                    ::vkGetPhysicalDeviceMemoryProperties( device_handle, &memory_props );

                    VkPhysicalDeviceFeatures features;
                    ::vkGetPhysicalDeviceFeatures( device_handle, &features );

                    return physical_device { device_handle, props, memory_props, features };
                } );

            return devices;
        }
    };

}   // namespace sl::vk::core

#endif /* __LOADER_H_563420A83E5945238A07F6B49AEAC37C__ */
