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


#ifndef __ALLOCATOR_H_9FE6249EA090427AB634901DF8890966__
#define __ALLOCATOR_H_9FE6249EA090427AB634901DF8890966__

#include <utils/pointers.h>

#include <vk/core/logical-device.h>
#include <vk/mem/device-buffer.h>
#include <vk/mem/device-image.h>

namespace sl::vk::mem
{

    struct allocator
    {
        explicit allocator( VmaAllocator vma_alloc )
            : _allocator { vma_alloc }
        {}


        /**
         * Methods for allocating GPU memory buffers / images.
         **/

        auto create_buffer( VkDeviceSize size,
                            mem::buffer_type type,
                            VkBufferUsageFlags usage_bits,
                            VmaMemoryUsage memory_usage,
                            VmaAllocationCreateFlags memory_flags = 0 ) const
        {
            return mem::device_buffer {
                _allocator.get(), size, type, usage_bits, memory_usage, memory_flags };
        }

        auto create_image( uint32_t width,
                           uint32_t height,
                           VkFormat format,
                           VkImageTiling tiling,
                           VkImageUsageFlags image_usage,
                           VmaMemoryUsage memory_usage,
                           VmaAllocationCreateFlags memory_flags = 0 ) const
        {
            return mem::device_image { _allocator.get(),
                                       width,
                                       height,
                                       format,
                                       tiling,
                                       image_usage,
                                       memory_usage,
                                       memory_flags };
        }

    private:
        using allocator_ptr_t = utils::custom_unique_ptr< VmaAllocator_T, ::vmaDestroyAllocator >;

        allocator_ptr_t _allocator;
    };


    /**
     * core::logical_device allows this function to peek inside the logical_device to grab
     * the appropriate set of functions for the vulkan memory allocations. This just wraps
     * the details of grabbing that list of functions.
     **/
    template< typename loader_t >
    auto make_allocator(
        const core::loader_base< loader_t >&,
        const core::logical_device< typename core::loader_base< loader_t >::device_functions_t >&
            device,
        uint32_t api_version )
    {
        // auto fns = mem::allocator::get_allocator_function_table< loader_t >( device );

        VmaVulkanFunctions fns = {};

        // Device-specific functions (note: from device table if change happens).
        fns.vkAllocateMemory                  = device._fns.vkAllocateMemory;
        fns.vkBindBufferMemory                = device._fns.vkBindBufferMemory;
        fns.vkBindBufferMemory2KHR            = device._fns.vkBindBufferMemory2KHR;
        fns.vkBindImageMemory                 = device._fns.vkBindImageMemory;
        fns.vkBindImageMemory2KHR             = device._fns.vkBindImageMemory2KHR;
        fns.vkCmdCopyBuffer                   = device._fns.vkCmdCopyBuffer;
        fns.vkCreateBuffer                    = device._fns.vkCreateBuffer;
        fns.vkCreateImage                     = device._fns.vkCreateImage;
        fns.vkDestroyBuffer                   = device._fns.vkDestroyBuffer;
        fns.vkDestroyImage                    = device._fns.vkDestroyImage;
        fns.vkFlushMappedMemoryRanges         = device._fns.vkFlushMappedMemoryRanges;
        fns.vkFreeMemory                      = device._fns.vkFreeMemory;
        fns.vkGetBufferMemoryRequirements     = device._fns.vkGetBufferMemoryRequirements;
        fns.vkGetBufferMemoryRequirements2KHR = device._fns.vkGetBufferMemoryRequirements2KHR;
        fns.vkGetImageMemoryRequirements      = device._fns.vkGetImageMemoryRequirements;
        fns.vkGetImageMemoryRequirements2KHR  = device._fns.vkGetImageMemoryRequirements2KHR;
        fns.vkInvalidateMappedMemoryRanges    = device._fns.vkInvalidateMappedMemoryRanges;
        fns.vkMapMemory                       = device._fns.vkMapMemory;
        fns.vkUnmapMemory                     = device._fns.vkUnmapMemory;

        // Global functions
        fns.vkGetPhysicalDeviceMemoryProperties     = ::vkGetPhysicalDeviceMemoryProperties;
        fns.vkGetPhysicalDeviceMemoryProperties2KHR = ::vkGetPhysicalDeviceMemoryProperties2;
        fns.vkGetPhysicalDeviceProperties           = ::vkGetPhysicalDeviceProperties;

        VmaAllocatorCreateInfo info = {};
        info.vulkanApiVersion       = api_version;
        info.instance               = device._instance;
        info.physicalDevice         = device._gpu;
        info.device                 = device._device;
        info.pVulkanFunctions       = &fns;

        VmaAllocator vma_alloc;
        vk::error::throw_if_error( "vmaCreateAllocator",
                                   vmaCreateAllocator( &info, &vma_alloc ),
                                   "failed to create vulkan memory allocator" );

        return allocator { vma_alloc };
    }

}   // namespace sl::vk::mem

#endif /* __ALLOCATOR_H_9FE6249EA090427AB634901DF8890966__ */
