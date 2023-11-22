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

#ifndef __SHADER_H_5D51EAEC6017404DB29EC6C44009CA1B__
#define __SHADER_H_5D51EAEC6017404DB29EC6C44009CA1B__

#include <algorithm>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include <vulkan.h>

#include <spirv/spirv_reflect.h>
#include <utils/deferred.h>

#include "error.h"
#include "helpers.h"

namespace sl::vk::spv
{

    struct descriptor_binding_info
    {
        uint32_t index;
        VkDescriptorType type;
        uint32_t count;
    };

    struct descriptor_set_info
    {
        uint32_t index;
        std::vector< descriptor_binding_info > bindings;
    };

    struct push_constant_block_info
    {
        uint32_t offset;
        uint32_t size;
    };

    struct vertex_input_info
    {
        uint32_t location;
        VkFormat format;
    };


    namespace priv
    {

        auto collect_vertex_inputs( const SpvReflectShaderModule& spv_mod )
        {
            // If this is NOT a vertex shader, vertex inputs don't matter.
            if ( spv_mod.shader_stage != SPV_REFLECT_SHADER_STAGE_VERTEX_BIT )
                return std::vector< vertex_input_info > {};

            const auto v_count  = spv_mod.input_variable_count;
            const auto* v_items = spv_mod.input_variables;

            std::vector< vertex_input_info > vars;
            vars.reserve( v_count );

            for ( uint32_t i = 0; i < v_count; ++i )
            {
                const auto* item = v_items[i];

                if ( item->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN )
                    continue;

                vars.push_back( vertex_input_info {
                    .location = item->location,
                    .format   = static_cast< VkFormat >( item->format ),
                } );
            }

            vars.shrink_to_fit();
            return vars;
        }

        auto collect_descriptor_sets( const SpvReflectShaderModule& spv_mod )
        {
            const auto s_count  = spv_mod.descriptor_set_count;
            const auto* s_items = spv_mod.descriptor_sets;

            std::vector< descriptor_set_info > sets( s_count );
            for ( uint32_t i = 0; i < s_count; ++i )
            {
                const auto& item = s_items[i];
                auto& set        = sets.at( i );

                set.index = item.set;
                set.bindings.resize( item.binding_count );

                for ( uint32_t j = 0; j < item.binding_count; ++j )
                {
                    const auto* binding = item.bindings[j];
                    auto& b             = set.bindings.at( j );

                    b.index = binding->binding;
                    b.type  = static_cast< VkDescriptorType >( binding->descriptor_type );
                    b.count = binding->count;
                }
            }

            return sets;
        }

        auto collect_local_sizes( const SpvReflectShaderModule& spv_mod )
        {
            auto ep = ::spvReflectGetEntryPoint( &spv_mod, spv_mod.entry_point_name );
            return std::make_tuple( ep->local_size.x, ep->local_size.y, ep->local_size.z );
        }

        auto collect_push_constant_blocks( const SpvReflectShaderModule& spv_mod )
        {
            const auto pc_count   = spv_mod.push_constant_block_count;
            const auto* pc_blocks = spv_mod.push_constant_blocks;

            std::vector< push_constant_block_info > pcbs( pc_count );
            for ( uint32_t i = 0; i < pc_count; ++i )
            {
                const auto* block = &pc_blocks[i];
                auto& pcb         = pcbs.at( i );

                pcb.offset = block->offset;
                pcb.size   = block->size;
            }

            return pcbs;
        }

    }   // namespace priv


    enum class shader_stage : VkShaderStageFlags
    {
        compute  = VK_SHADER_STAGE_COMPUTE_BIT,
        fragment = VK_SHADER_STAGE_FRAGMENT_BIT,
        geometry = VK_SHADER_STAGE_GEOMETRY_BIT,
        vertex   = VK_SHADER_STAGE_VERTEX_BIT,
    };

    struct shader
    {
        explicit shader( const char* entry,
                         shader_stage stage,
                         std::tuple< uint32_t, uint32_t, uint32_t > local_sizes,
                         std::vector< descriptor_set_info >&& descriptor_sets,
                         std::vector< push_constant_block_info >&& push_constant_blocks,
                         std::vector< vertex_input_info >&& vertex_inputs )
            : _entry { entry }
            , _stage { stage }
            , _local_sizes { local_sizes }
            , _descriptor_sets { std::move( descriptor_sets ) }
            , _push_constant_blocks { std::move( push_constant_blocks ) }
            , _vertex_inputs { std::move( vertex_inputs ) }
        {}

        const char* entry() const { return _entry.c_str(); }
        shader_stage stage() const { return _stage; }
        const auto& local_sizes() const { return _local_sizes; }

        auto descriptor_sets() const { return std::span( _descriptor_sets ); }
        auto push_constant_blocks() const { return std::span( _push_constant_blocks ); }
        auto vertex_inputs() const { return std::span( _vertex_inputs ); }

    private:
        std::string _entry;
        shader_stage _stage;
        std::tuple< uint32_t, uint32_t, uint32_t > _local_sizes;

        std::vector< descriptor_set_info > _descriptor_sets;
        std::vector< push_constant_block_info > _push_constant_blocks;
        std::vector< vertex_input_info > _vertex_inputs;
    };


    auto make_shader( const std::span< const uint32_t > code )
    {
        SpvReflectShaderModule spv_raw {};
        spv::error::throw_if_error(
            "spvReflectCreateShaderModule2",
            ::spvReflectCreateShaderModule2( SPV_REFLECT_MODULE_FLAG_NO_COPY,
                                             code.size() * sizeof( uint32_t ),
                                             code.data(),
                                             &spv_raw ),
            "failed to create SPV shader module" );

        // Clean everything up when exiting this block
        sl::utils::deferred _ { [&spv_raw]() { ::spvReflectDestroyShaderModule( &spv_raw ); } };

        return spv::shader {
            spv_raw.entry_point_name,
            static_cast< shader_stage >( spv_raw.shader_stage ),
            priv::collect_local_sizes( spv_raw ),
            priv::collect_descriptor_sets( spv_raw ),
            priv::collect_push_constant_blocks( spv_raw ),
            priv::collect_vertex_inputs( spv_raw ),
        };
    }

}   // namespace sl::vk::spv

#endif /* __SHADER_H_5D51EAEC6017404DB29EC6C44009CA1B__ */
