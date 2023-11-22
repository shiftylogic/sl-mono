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

#ifndef __FIXED_DESCRIPTOR_CACHE_H_D373172D500A49BC9F400CFC4776C671__
#define __FIXED_DESCRIPTOR_CACHE_H_D373172D500A49BC9F400CFC4776C671__

#include <span>
#include <unordered_map>
#include <vector>

#include <vk/core/logical-device.h>
#include <vk/core/resource-type.h>

namespace sl::vk::core
{

    struct fixed_descriptor_cache
    {

        struct config
        {
            uint32_t pool_count { 1 };
            uint32_t max_sets { 0 };
            std::unordered_map< resource_type, uint32_t > pool_sizes;
        };


        fixed_descriptor_cache( std::vector< core::descriptor_pool >&& pools )
            : _pools { std::move( pools ) }
            , _index { 0 }
        {}

        template< typename device_functions_t >
        auto allocate( const core::logical_device< device_functions_t >& device,
                       const core::descriptor_set_layout& set_layout )
        {
            while ( _index < _pools.size() )
            {
                auto set = device.allocate_descriptor_set( _pools[_index], set_layout );
                if ( set )
                    return set;

                ++_index;
            }

            throw vk::error( "fixed-descriptor-cache::allocate",
                             VK_ERROR_UNKNOWN,
                             "descriptor cache out of pools" );
        }

        template< typename device_functions_t >
        auto reset( const core::logical_device< device_functions_t >& device )
        {
            // Reset any used descriptor pool
            std::for_each_n( std::begin( _pools ), _index, [&device]( auto const& pool ) {
                device.reset_descriptor_pool( pool );
            } );

            _index = 0;
        }

    private:
        std::vector< core::descriptor_pool > _pools;
        size_t _index;
    };


    template< typename device_functions_t >
    auto make_fixed_descriptor_cache( const core::logical_device< device_functions_t >& device,
                                      const fixed_descriptor_cache::config& config )
    {
        // Prep the pool size structures
        auto count      = uint32_t { 0 };
        auto pool_sizes = std::vector< VkDescriptorPoolSize >( config.pool_sizes.size() );
        std::for_each( std::begin( config.pool_sizes ),
                       std::end( config.pool_sizes ),
                       [&pool_sizes, &count, n = 0]( const auto& kv ) mutable {
                           pool_sizes[n].type = static_cast< VkDescriptorType >( kv.first );
                           pool_sizes[n].descriptorCount = kv.second;

                           n++;
                           count += kv.second;
                       } );

        auto max_sets = config.max_sets > 0 ? config.max_sets : count;

        // Create the fixed set of descriptor pools
        auto pools = std::vector< core::descriptor_pool >( config.pool_count );
        for ( uint32_t i = 0; i < config.pool_count; ++i )
            pools[i] = device.create_descriptor_pool( pool_sizes, max_sets );

        return fixed_descriptor_cache { std::move( pools ) };
    }

}   // namespace sl::vk::core

#endif /* __FIXED_DESCRIPTOR_CACHE_H_D373172D500A49BC9F400CFC4776C671__ */
