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

namespace sl::vk::mem
{

    constexpr auto k_default_buffer_flags = static_cast< VmaAllocationCreateFlagBits >( 0 );

    struct allocator
    {
        explicit allocator( VmaAllocator vma_alloc )
            : _allocator { vma_alloc }
        {}


        /**
         * Methods for allocating GPU memory buffers / images.
         **/

        auto create_buffer( VkDeviceSize size,
                            VkBufferUsageFlagBits usage_bits,
                            VmaMemoryUsage memory_usage,
                            VmaAllocationCreateFlagBits memory_flags
                            = k_default_buffer_flags ) const
        {
            return mem::device_buffer {
                _allocator.get(), size, usage_bits, memory_usage, memory_flags };
        }

    private:
        using allocator_ptr_t = utils::custom_unique_ptr< VmaAllocator_T, vmaDestroyAllocator >;

        allocator_ptr_t _allocator;
    };

    template< typename loader_t >
    auto make_allocator(
        const core::loader_base< loader_t >&,
        const core::logical_device< typename core::loader_base< loader_t >::device_functions_t >&
            device,
        uint32_t api_version )
    {
        const auto& device_fns = device.function_table();

        VmaVulkanFunctions fns = {};

        // Device-specific functions (note: from device table if change happens).
        fns.vkAllocateMemory                  = device_fns.vkAllocateMemory;
        fns.vkBindBufferMemory                = device_fns.vkBindBufferMemory;
        fns.vkBindBufferMemory2KHR            = device_fns.vkBindBufferMemory2KHR;
        fns.vkBindImageMemory                 = device_fns.vkBindImageMemory;
        fns.vkBindImageMemory2KHR             = device_fns.vkBindImageMemory2KHR;
        fns.vkCmdCopyBuffer                   = device_fns.vkCmdCopyBuffer;
        fns.vkCreateBuffer                    = device_fns.vkCreateBuffer;
        fns.vkCreateImage                     = device_fns.vkCreateImage;
        fns.vkDestroyBuffer                   = device_fns.vkDestroyBuffer;
        fns.vkDestroyImage                    = device_fns.vkDestroyImage;
        fns.vkFlushMappedMemoryRanges         = device_fns.vkFlushMappedMemoryRanges;
        fns.vkFreeMemory                      = device_fns.vkFreeMemory;
        fns.vkGetBufferMemoryRequirements     = device_fns.vkGetBufferMemoryRequirements;
        fns.vkGetBufferMemoryRequirements2KHR = device_fns.vkGetBufferMemoryRequirements2KHR;
        fns.vkGetImageMemoryRequirements      = device_fns.vkGetImageMemoryRequirements;
        fns.vkGetImageMemoryRequirements2KHR  = device_fns.vkGetImageMemoryRequirements2KHR;
        fns.vkInvalidateMappedMemoryRanges    = device_fns.vkInvalidateMappedMemoryRanges;
        fns.vkMapMemory                       = device_fns.vkMapMemory;
        fns.vkUnmapMemory                     = device_fns.vkUnmapMemory;

        // Global functions
        fns.vkGetPhysicalDeviceMemoryProperties     = ::vkGetPhysicalDeviceMemoryProperties;
        fns.vkGetPhysicalDeviceMemoryProperties2KHR = ::vkGetPhysicalDeviceMemoryProperties2;
        fns.vkGetPhysicalDeviceProperties           = ::vkGetPhysicalDeviceProperties;

        VmaAllocatorCreateInfo info = {};
        info.vulkanApiVersion       = api_version;
        info.instance               = device.instance();
        info.physicalDevice         = device.gpu();
        info.device                 = static_cast< const core::device& >( device );
        info.pVulkanFunctions       = &fns;

        VmaAllocator vma_alloc;
        vk::error::throw_if_error( "vmaCreateAllocator",
                                   vmaCreateAllocator( &info, &vma_alloc ),
                                   "failed to create vulkan memory allocator" );

        return allocator { vma_alloc };
    }

}   // namespace sl::vk::mem

#endif /* __ALLOCATOR_H_9FE6249EA090427AB634901DF8890966__ */
