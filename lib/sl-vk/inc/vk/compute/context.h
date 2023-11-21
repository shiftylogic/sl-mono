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

#ifndef __CONTEXT_H_EC2721296A87430CAAE6401FAB73FAC3__
#define __CONTEXT_H_EC2721296A87430CAAE6401FAB73FAC3__

#include <vk/core/logical-device.h>
#include <vk/mem/allocator.h>

namespace sl::vk::compute
{

    template< typename device_functions_t >
    struct context
    {
        explicit context( const core::logical_device< device_functions_t >& device,
                          const mem::allocator& alloc )
            : _device { device }
            , _allocator { alloc }
            , _command_pool { _device.create_command_pool( core::device_queue::compute ) }
            , _buffer { _device.allocate_command_buffer( _command_pool ) }
        {
            _device.begin_command_buffer( _buffer );
        }


        /**
         *
         * Buffer management / binding methods
         *
         **/

        inline void copy( const mem::device_buffer& source,
                          uint32_t source_offset,
                          const mem::device_buffer& target,
                          uint32_t target_offset ) const
        {
            _device.command_copy_buffer(
                _buffer, source.size(), source, source_offset, target, target_offset );
        }

        inline void copy( const mem::device_buffer& source,
                          const mem::device_buffer& target,
                          uint32_t target_offset = 0 ) const
        {
            copy( source, 0, target, target_offset );
        }

        template< typename data_t >
        void copy( const std::span< data_t > data,
                   const mem::device_buffer& target,
                   uint32_t target_offset = 0 ) const
        {
            auto map_and_copy = [&data]( const mem::device_buffer& buf, uint32_t offset ) {
                auto view = buf.map_view< data_t >();
                view.write( data, offset );
            };

            if ( target.is_host_visible() )
            {
                map_and_copy( target, target_offset );
            }
            else
            {
                auto staging
                    = mem::make_staging_buffer( _allocator, data.size() * sizeof( data_t ) );
                map_and_copy( staging, 0 );
                copy( staging, target );
            }
        }


        /**
         *
         **/

    private:
        const core::logical_device< device_functions_t >& _device;
        const mem::allocator& _allocator;
        core::command_pool _command_pool;
        VkCommandBuffer _buffer;
    };

}   // namespace sl::vk::compute

#endif /* __CONTEXT_H_EC2721296A87430CAAE6401FAB73FAC3__ */
