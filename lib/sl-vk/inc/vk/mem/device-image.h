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

#ifndef __DEVICE_IMAGE_H_929D27160A0B484A8757AEA7EFC68791__
#define __DEVICE_IMAGE_H_929D27160A0B484A8757AEA7EFC68791__

#include <vk/mem/resources.h>

namespace sl::vk::mem
{

    struct device_image
    {
        device_image( VmaAllocator alloc,
                      uint32_t width,
                      uint32_t height,
                      VkFormat format,
                      VkImageTiling tiling,
                      VkImageUsageFlags image_usage,
                      VmaMemoryUsage memory_usage,
                      VmaAllocationCreateFlags memory_flags )
            : _allocator { alloc }
            , _allocation { VK_NULL_HANDLE }
            , _height { height }
            , _width { width }
        {
            auto info          = core::image_create_info {};
            info.imageType     = VK_IMAGE_TYPE_2D;
            info.extent.width  = width;
            info.extent.height = height;
            info.extent.depth  = 1;
            info.mipLevels     = 1;
            info.arrayLayers   = 1;
            info.format        = format;
            info.tiling        = tiling;
            info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            info.usage         = image_usage;
            info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
            info.samples       = VK_SAMPLE_COUNT_1_BIT;

            auto vma_info     = VmaAllocationCreateInfo {};
            vma_info.usage    = memory_usage;
            vma_info.flags    = memory_flags;
            vma_info.priority = 1.0;

            VkImage img;
            vk::error::throw_if_error(
                "vmaCreateImage",
                ::vmaCreateImage( _allocator, &info, &vma_info, &img, &_allocation, nullptr ),
                "failed to allocate (vma) image" );

            // Wrap the allocation in a "smart" pointer for easy cleanup
            _image = mem::image { _allocator, img, ::vmaDestroyImage, _allocation };
        }

        operator VkImage() const noexcept { return _image; }
        auto height() const { return _height; }
        auto width() const { return _width; }

    private:
        VmaAllocator _allocator;
        VmaAllocation _allocation;
        mem::image _image;

        uint32_t _height;
        uint32_t _width;
    };

}   // namespace sl::vk::mem

#endif /* __DEVICE_IMAGE_H_929D27160A0B484A8757AEA7EFC68791__ */
