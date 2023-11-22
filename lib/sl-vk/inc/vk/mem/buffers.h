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

#ifndef __BUFFERS_H_9C16611CE12846BD8AF654842D465DE6__
#define __BUFFERS_H_9C16611CE12846BD8AF654842D465DE6__

#include <vk/mem/allocator.h>

namespace sl::vk::mem
{

    auto make_index_buffer( const mem::allocator& alloc, VkDeviceSize size )
    {
        return alloc.create_buffer( size,
                                    mem::buffer_type::index,
                                    VK_BUFFER_USAGE_INDEX_BUFFER_BIT
                                        | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                    VMA_MEMORY_USAGE_AUTO );
    }

    auto make_readback_buffer( const mem::allocator& alloc, VkDeviceSize size )
    {
        return alloc.create_buffer( size,
                                    mem::buffer_type::storage,
                                    VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                    VMA_MEMORY_USAGE_AUTO,
                                    VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT );
    }

    auto make_staging_buffer( const mem::allocator& alloc, VkDeviceSize size )
    {
        return alloc.create_buffer( size,
                                    mem::buffer_type::storage,
                                    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                    VMA_MEMORY_USAGE_AUTO,
                                    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT );
    }

    auto make_storage_buffer( const mem::allocator& alloc, VkDeviceSize size )
    {
        return alloc.create_buffer( size,
                                    mem::buffer_type::storage,
                                    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
                                        | VK_BUFFER_USAGE_TRANSFER_DST_BIT
                                        | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                    VMA_MEMORY_USAGE_AUTO );
    }

    auto make_uniform_buffer( const mem::allocator& alloc, VkDeviceSize size )
    {
        return alloc.create_buffer(
            size,
            mem::buffer_type::uniform,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VMA_MEMORY_USAGE_AUTO,
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
                | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT );
    }

    auto make_vertex_buffer( const mem::allocator& alloc, VkDeviceSize size )
    {
        return alloc.create_buffer( size,
                                    mem::buffer_type::vertex,
                                    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
                                        | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                    VMA_MEMORY_USAGE_AUTO );
    }

}   // namespace sl::vk::mem

#endif /* __BUFFERS_H_9C16611CE12846BD8AF654842D465DE6__ */
