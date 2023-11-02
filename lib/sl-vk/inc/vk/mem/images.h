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

#ifndef __IMAGES_H_1FCEB4A8597D4ED99AD33AC2DFAA2E43__
#define __IMAGES_H_1FCEB4A8597D4ED99AD33AC2DFAA2E43__

namespace sl::vk::mem
{

    auto make_color_image( const mem::allocator& alloc,
                           uint32_t width,
                           uint32_t height,
                           VkFormat format )
    {
        return alloc.create_image( width,
                                   height,
                                   format,
                                   VK_IMAGE_TILING_OPTIMAL,
                                   VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                   VMA_MEMORY_USAGE_AUTO,
                                   VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT );
    }

    auto make_depth_image( const mem::allocator& alloc,
                           uint32_t width,
                           uint32_t height,
                           VkFormat format )
    {
        return alloc.create_image( width,
                                   height,
                                   format,
                                   VK_IMAGE_TILING_OPTIMAL,
                                   VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
                                       | VK_IMAGE_USAGE_SAMPLED_BIT,
                                   VMA_MEMORY_USAGE_AUTO,
                                   VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT );
    }

}   // namespace sl::vk::mem

#endif /* __IMAGES_H_1FCEB4A8597D4ED99AD33AC2DFAA2E43__ */
