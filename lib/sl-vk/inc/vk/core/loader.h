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

#include <vulkan.h>

#include <vk/core/resources.h>
#include <vk/core/structs.h>
#include <vk/error.h>

namespace sl::vk::core
{

    struct loader
    {
        using device_table = ::VolkDeviceTable;

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

        auto create_instance( core::instance_create_info& ci ) const
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

        auto create_debug_messenger( core::instance& inst,
                                     core::debug_utils_messenger_create_info_ext& ci ) const
        {
            VkDebugUtilsMessengerEXT debug;
            vk::error::throw_if_error(
                "vkCreateDebugUtilsMessengerEXT",
                ::vkCreateDebugUtilsMessengerEXT( inst, &ci, nullptr, &debug ),
                "failed to enable debug utils" );

            return core::debug_utils_messenger { inst, debug, ::vkDestroyDebugUtilsMessengerEXT };
        }

        void load_device_table( device_table& table, VkDevice device )
        {
            ::volkLoadDeviceTable( &table, device );
        }
    };

}   // namespace sl::vk::core

#endif /* __LOADER_H_563420A83E5945238A07F6B49AEAC37C__ */
