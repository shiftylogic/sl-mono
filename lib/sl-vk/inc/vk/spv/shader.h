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
#include <span>
#include <vector>

#include <vulkan.h>

#include <spirv/spirv_reflect.h>

#include "error.h"
#include "helpers.h"

namespace sl::vk::spv
{

    enum class shader_stage : VkShaderStageFlags
    {
        compute  = VK_SHADER_STAGE_COMPUTE_BIT,
        fragment = VK_SHADER_STAGE_FRAGMENT_BIT,
        geometry = VK_SHADER_STAGE_GEOMETRY_BIT,
        vertex   = VK_SHADER_STAGE_VERTEX_BIT,
    };

    struct set_layout_data
    {
        uint32_t set;
        std::vector< VkDescriptorSetLayoutBinding > bindings;
    };

    struct shader
    {
        // No copy / move
        shader( const shader& )            = delete;
        shader& operator=( const shader& ) = delete;
        shader( shader&& )                 = default;
        shader& operator=( shader&& )      = default;


        /**
         * Can be constructed with a vector (transfers ownership / moves)
         *   or
         * Initialized with any 'span' type, but creates a new vector (allocation).
         **/

        explicit shader( std::vector< uint32_t >&& code )
            : _code( std::move( code ) )
        {
            spv::error::throw_if_error(
                "spvReflectCreateShaderModule2",
                ::spvReflectCreateShaderModule2( SPV_REFLECT_MODULE_FLAG_NO_COPY,
                                                 _code.size() * sizeof( uint32_t ),
                                                 _code.data(),
                                                 &_spv ),
                "failed to create SPV shader module" );
        }

        explicit shader( const std::span< uint32_t > code )
            : shader { std::vector( std::begin( code ), std::end( code ) ) }
        {}

        ~shader() { ::spvReflectDestroyShaderModule( &_spv ); }


        /**
         * Reflected shader module accessors
         **/

        const char* entry() const { return _spv.entry_point_name; }
        shader_stage stage() const { return static_cast< shader_stage >( _spv.shader_stage ); }

        auto vertex_inputs( VkVertexInputBindingDescription& binding ) const
        {
            if ( _spv.shader_stage != SPV_REFLECT_SHADER_STAGE_VERTEX_BIT )
                throw std::runtime_error { "vertex inputs only valid for vertex shaders" };

            ///////////////////////////////////////////////////////
            // Fetch input variables from shader code
            uint32_t count;
            spv::error::throw_if_error(
                "spvReflectEnumerateInputVariables",
                ::spvReflectEnumerateInputVariables( &_spv, &count, nullptr ),
                "failed to enumerate input variables (count)" );

            std::vector< SpvReflectInterfaceVariable* > inputs( count );
            spv::error::throw_if_error(
                "spvReflectEnumerateInputVariables",
                ::spvReflectEnumerateInputVariables( &_spv, &count, inputs.data() ),
                "failed to enumerate input variables" );

            ///////////////////////////////////////////////////////
            // Build initial vertex input attribute descriptions
            std::vector< VkVertexInputAttributeDescription > attrs( inputs.size() );
            std::for_each( std::begin( inputs ), std::end( inputs ), [&]( const auto* input ) {
                if ( input->decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN )
                    return;

                attrs.push_back( {
                    .location = input->location,
                    .binding  = binding.binding,
                    .format   = static_cast< VkFormat >( input->format ),
                    .offset   = 0,   // To be fixed up below
                } );
            } );

            ///////////////////////////////////////////////////////
            // Sort attributes by location
            std::sort( std::begin( attrs ), std::end( attrs ), []( const auto& x, const auto& y ) {
                return x.location < y.location;
            } );

            ///////////////////////////////////////////////////////
            // Fix up attribute offset and compute binding stride
            uint32_t stride = 0;
            std::for_each( std::begin( attrs ), std::end( attrs ), [&stride]( auto& attr ) {
                attr.offset = stride;
                stride += spv::format_size( attr.format );
            } );

            // Patch the binding stride with the correct computed value
            binding.stride = stride;

            return attrs;
        }

        auto set_layouts() const
        {
            ///////////////////////////////////////////////////////
            // Fetch descriptor sets from shader code
            uint32_t count;
            spv::error::throw_if_error(
                "spvReflectEnumerateDescriptorSets",
                ::spvReflectEnumerateDescriptorSets( &_spv, &count, nullptr ),
                "failed to enumerate descriptor sets (count)" );

            std::vector< SpvReflectDescriptorSet* > sets( count );
            spv::error::throw_if_error(
                "spvReflectEnumerateDescriptorSets",
                ::spvReflectEnumerateDescriptorSets( &_spv, &count, sets.data() ),
                "failed to enumerate descriptor sets" );

            ///////////////////////////////////////////////////////
            // Build descriptor set layout structures
            std::vector< set_layout_data > layouts( sets.size() );
            auto stage            = static_cast< VkShaderStageFlagBits >( _spv.shader_stage );
            auto process_bindings = [stage]( const auto* binding ) {
                auto slb = VkDescriptorSetLayoutBinding {
                    .binding         = binding->binding,
                    .descriptorType  = static_cast< VkDescriptorType >( binding->descriptor_type ),
                    .descriptorCount = 1,
                    .stageFlags      = stage,
                };

                auto dims = std::span( binding->array.dims, binding->array.dims_count );
                std::for_each( std::begin( dims ), std::end( dims ), [&slb]( const auto& dim ) {
                    slb.descriptorCount *= dim;
                } );

                return slb;
            };
            auto process_set = [&, n = 0]( const auto* set ) mutable {
                auto bindings = std::span( set->bindings, set->binding_count );
                auto& layout  = layouts.at( n++ );
                layout.set    = set->set;

                layout.bindings.reserve( set->binding_count );
                std::transform( std::begin( bindings ),
                                std::end( bindings ),
                                std::back_inserter( layout.bindings ),
                                process_bindings );
            };
            std::for_each( std::begin( sets ), std::end( sets ), process_set );

            return layouts;
        }

        auto push_constants() const
        {
            uint32_t count;
            spv::error::throw_if_error(
                "spvReflectEnumeratePushConstantBlocks",
                ::spvReflectEnumeratePushConstantBlocks( &_spv, &count, nullptr ),
                "failed to enumerate push constant blocks (count)" );

            std::vector< SpvReflectBlockVariable* > blocks( count );
            spv::error::throw_if_error(
                "spvReflectEnumeratePushConstantBlocks",
                ::spvReflectEnumeratePushConstantBlocks( &_spv, &count, blocks.data() ),
                "failed to enumerate push constant blocks" );

            ///////////////////////////////////////////////////////
            // Build push constant range structures
            std::vector< VkPushConstantRange > pcs( blocks.size() );
            std::for_each(
                std::begin( blocks ), std::end( blocks ), [&, n = 0]( const auto* block ) mutable {
                    auto& pc      = pcs.at( n++ );
                    pc.stageFlags = _spv.shader_stage;
                    pc.offset     = block->offset;
                    pc.size       = block->size;
                } );

            return pcs;
        }

    private:
        std::vector< uint32_t > _code;
        SpvReflectShaderModule _spv {};
    };

}   // namespace sl::vk::spv

#endif /* __SHADER_H_5D51EAEC6017404DB29EC6C44009CA1B__ */
