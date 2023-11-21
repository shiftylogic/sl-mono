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

#ifndef __PROGRAM_H_C74161AFB993442ABDA3F4025844D8CD__
#define __PROGRAM_H_C74161AFB993442ABDA3F4025844D8CD__

#include <vk/compute/context.h>
#include <vk/spv/shader.h>

namespace sl::vk::compute
{

    template< size_t k_max_descriptor_sets = 4 >
    struct program
    {
        explicit program(
            core::shader_module&& shader,
            core::pipeline_layout&& layout,
            core::pipeline&& pipeline,
            const std::span< core::descriptor_set_layout, k_max_descriptor_sets > set_layouts )
            : _shader { std::move( shader ) }
            , _layout { std::move( layout ) }
            , _pipeline { std::move( pipeline ) }
        {
            std::for_each(
                std::begin( set_layouts ),
                std::end( set_layouts ),
                [&, n = 0]( auto& layout ) mutable { _set_layouts[n++] = std::move( layout ); } );
        }

        void execute() const {}

    private:
        core::shader_module _shader;
        core::pipeline_layout _layout;
        core::pipeline _pipeline;
        std::array< core::descriptor_set_layout, k_max_descriptor_sets > _set_layouts;
    };


    template< typename device_functions_t,
              size_t k_max_descriptor_sets = 4,
              size_t k_max_push_blocks     = 4 >
    auto make_program( const core::logical_device< device_functions_t >& device,
                       const std::vector< uint32_t >& code )
    {
        auto spv_module = sl::vk::spv::make_shader( code );
        vk::error::throw_if( spv_module.stage() != spv::shader_stage::compute,
                             "load-compute-shader",
                             VK_ERROR_UNKNOWN,
                             "shader code is not a compute shader" );

        // Build the push constant ranges
        auto push_blocks = spv_module.push_constant_blocks();
        auto pcs         = std::array< VkPushConstantRange, k_max_push_blocks > {};
        std::for_each( std::begin( push_blocks ),
                       std::end( push_blocks ),
                       [&pcs, n = 0]( const auto& pcb ) mutable {
                           pcs[n].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
                           pcs[n].offset     = pcb.offset;
                           pcs[n].size       = pcb.size;
                           ++n;
                       } );

        // Build the set layouts
        auto desc_sets   = spv_module.descriptor_sets();
        auto set_layouts = std::array< core::descriptor_set_layout, k_max_descriptor_sets > {};
        auto set_layout_handles = std::array< VkDescriptorSetLayout, k_max_descriptor_sets > {};
        std::for_each( std::begin( desc_sets ), std::end( desc_sets ), [&]( const auto& set ) {
            std::vector< VkDescriptorSetLayoutBinding > bindings;
            bindings.reserve( set.bindings.size() );

            std::transform( std::begin( set.bindings ),
                            std::end( set.bindings ),
                            std::back_inserter( bindings ),
                            []( const auto& b ) {
                                return VkDescriptorSetLayoutBinding {
                                    .binding         = b.index,
                                    .descriptorType  = b.type,
                                    .descriptorCount = b.count,
                                    .stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT,
                                };
                            } );

            set_layouts[set.index]        = device.create_descriptor_set_layout( bindings );
            set_layout_handles[set.index] = set_layouts[set.index];
        } );

        // Attempt to create a VK shader module
        auto shader = device.create_shader( std::span( code ) );

        // Build the compute pipeline
        auto layout
            = device.create_pipeline_layout( std::span( &set_layout_handles[0], desc_sets.size() ),
                                             std::span( &pcs[0], push_blocks.size() ) );
        auto pipeline = device.create_compute_pipeline( layout, shader, spv_module.entry() );

        // We should have a valid "compute progream" now.
        return program< k_max_descriptor_sets > {
            std::move( shader ),
            std::move( layout ),
            std::move( pipeline ),
            set_layouts,
        };
    }

}   // namespace sl::vk::compute

#endif /* __PROGRAM_H_C74161AFB993442ABDA3F4025844D8CD__ */
