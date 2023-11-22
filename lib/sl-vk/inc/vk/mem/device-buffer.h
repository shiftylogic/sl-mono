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

#ifndef __DEVICE_BUFFER_H_2753A9D2B23649E28E91C68049337E5F__
#define __DEVICE_BUFFER_H_2753A9D2B23649E28E91C68049337E5F__

#include <algorithm>
#include <span>

#include <vk/mem/resources.h>

namespace sl::vk::mem
{

    enum class buffer_type
    {
        index,
        storage,
        vertex,
        uniform,
    };

    struct device_buffer
    {
        device_buffer( VmaAllocator allocator,
                       VkDeviceSize size,
                       buffer_type type,
                       VkBufferUsageFlags usage_bits,
                       VmaMemoryUsage memory_usage,
                       VmaAllocationCreateFlags memory_flags )
            : _allocator { allocator }
            , _allocation { VK_NULL_HANDLE }
            , _size { size }
            , _type { type }
        {
            auto info        = core::buffer_create_info {};
            info.size        = size;
            info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            info.usage       = usage_bits;

            auto vma_info  = VmaAllocationCreateInfo {};
            vma_info.usage = memory_usage;
            vma_info.flags = memory_flags;

            VkBuffer buf;
            vk::error::throw_if_error(
                "vmaCreateBuffer",
                ::vmaCreateBuffer( _allocator, &info, &vma_info, &buf, &_allocation, nullptr ),
                "failed to allocate (vma) buffer" );

            // Wrap the allocation in a "smart" pointer for easy cleanup
            _buffer = mem::buffer { _allocator, buf, ::vmaDestroyBuffer, _allocation };

            // Grab the flags for answering questions later.
            ::vmaGetAllocationMemoryProperties( _allocator, _allocation, &_flags );
        }

        operator VkBuffer() const noexcept { return _buffer; }
        auto size() const noexcept { return _size; }
        auto type() const noexcept { return _type; }

        bool is_host_visible() const { return _flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT; }

        template< typename data_t >
        auto map_view() const
        {
            return device_buffer::view< data_t > { _allocator, _allocation, _size };
        }

        /**
         * Wrapper 'view' around buffer data (read / write) access.
         **/
        template< typename data_t = uint8_t >
        struct view
        {
            // No copy / move
            view()                         = delete;
            view( const view& )            = delete;
            view& operator=( const view& ) = delete;
            view( view&& )                 = delete;
            view& operator=( view&& )      = delete;

            view( VmaAllocator alloc, VmaAllocation allocation, VkDeviceSize size )
                : _allocator( alloc )
                , _allocation( allocation )
            {
                void* data;
                vk::error::throw_if_error( "vmaMapMemory",
                                           ::vmaMapMemory( alloc, allocation, &data ),
                                           "failed to map device memory to host" );
                _data = std::span( static_cast< data_t* >( data ), size / sizeof( data_t ) );
            }

            ~view() noexcept { ::vmaUnmapMemory( _allocator, _allocation ); }

            auto data() const { return _data; }

            size_t read( std::span< data_t > data, size_t offset ) noexcept
            {
                auto src   = _data.subspan( offset );
                auto count = std::min( src.size(), data.size() );
                std::copy_n( std::begin( src ), count, std::begin( data ) );
                return count;
            }

            size_t write( const std::span< const data_t > data, size_t offset ) noexcept
            {
                auto dst   = _data.subspan( offset );
                auto count = std::min( dst.size(), data.size() );
                std::copy_n( std::begin( data ), count, std::begin( dst ) );
                return count;
            }

        private:
            VmaAllocator _allocator;
            VmaAllocation _allocation;
            std::span< data_t > _data;
        };

    private:
        VmaAllocator _allocator;
        VmaAllocation _allocation;
        VkMemoryPropertyFlags _flags;
        VkDeviceSize _size;
        buffer_type _type;
        mem::buffer _buffer;
    };

}   // namespace sl::vk::mem

#endif /* __DEVICE_BUFFER_H_2753A9D2B23649E28E91C68049337E5F__ */
