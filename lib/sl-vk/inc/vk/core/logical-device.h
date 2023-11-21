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

#ifndef __LOGICAL_DEVICE_H_6FC08D1CA064493B88B86F73349F912C__
#define __LOGICAL_DEVICE_H_6FC08D1CA064493B88B86F73349F912C__

#include <vk/core/app-context.h>
#include <vk/core/physical-device.h>

namespace sl::vk::core
{
    template< typename device_function_t >
    struct logical_device;
}

namespace sl::vk::mem
{

    template< typename loader_t >
    auto make_allocator(
        const core::loader_base< loader_t >&,
        const core::logical_device< typename core::loader_base< loader_t >::device_functions_t >&,
        uint32_t );

}   // namespace sl::vk::mem

namespace sl::vk::core
{

    enum class device_queue
    {
        compute  = 0,
        graphics = 1,
        transfer = 2,
    };

    template< typename device_functions_t >
    struct logical_device
    {
        /**
         * Allocator construction needs access to a bunch of the private data of the logical device.
         * This is a workaround to not have to expose all that data to everyone.
         **/
        template< typename loader_t >
        friend auto sl::vk::mem::make_allocator(
            const core::loader_base< loader_t >&,
            const core::logical_device<
                typename core::loader_base< loader_t >::device_functions_t >&,
            uint32_t );


        /**
         * The nasty constructor.
         *
         * Prefer to use core::make_logical_device function below.
         *
         **/
        explicit logical_device( const core::instance& inst,
                                 const core::physical_device& gpu,
                                 core::device&& device,
                                 const device_functions_t& fns,
                                 uint32_t graphics_index,
                                 uint32_t compute_index,
                                 uint32_t transfer_index )
            : _instance { inst }
            , _gpu { gpu }
            , _device { std::move( device ) }
            , _fns { fns }
            , _queue_indices { compute_index, graphics_index, transfer_index }
            , _compute { VK_NULL_HANDLE }
            , _graphics { VK_NULL_HANDLE }
            , _transfer { VK_NULL_HANDLE }
        {
            if ( compute_index != static_cast< uint32_t >( -1 ) )
                _fns.vkGetDeviceQueue( _device, compute_index, 0, &_compute );
            if ( graphics_index != static_cast< uint32_t >( -1 ) )
                _fns.vkGetDeviceQueue( _device, graphics_index, 0, &_graphics );
            if ( transfer_index != static_cast< uint32_t >( -1 ) )
                _fns.vkGetDeviceQueue( _device, transfer_index, 0, &_transfer );
        }


        /**
         *
         * The collection of exported device functions for manipulaing the vulkan device.
         *
         **/

        //---------------------
        // COMMAND BUFFER

        inline auto allocate_command_buffer( const core::command_pool& pool ) const
        {
            auto info               = core::command_buffer_allocate_info {};
            info.commandPool        = pool;
            info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            info.commandBufferCount = 1;

            VkCommandBuffer buffer;
            vk::error::throw_if_error( "vkAllocateCommandBuffer",
                                       _fns.vkAllocateCommandBuffers( _device, &info, &buffer ),
                                       "failed to allocate command buffer from pool" );

            return buffer;
        }

        inline void begin_command_buffer( VkCommandBuffer buffer ) const
        {
            auto info  = core::command_buffer_begin_info {};
            info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vk::error::throw_if_error( "vkBeginCommandBuffer",
                                       _fns.vkBeginCommandBuffer( buffer, &info ),
                                       "failed to begin recording command buffer" );
        }

        inline void command_copy_buffer( VkCommandBuffer cmd_buf,
                                         uint32_t size_in_bytes,
                                         VkBuffer source,
                                         uint32_t source_offset,
                                         VkBuffer target,
                                         uint32_t target_offset ) const
        {
            VkBufferCopy region {
                .srcOffset = source_offset,
                .dstOffset = target_offset,
                .size      = size_in_bytes,
            };

            _fns.vkCmdCopyBuffer( cmd_buf, source, target, 1, &region );
        }

        inline void
        command_dispatch( VkCommandBuffer buffer, uint32_t x, uint32_t y, uint32_t z ) const
        {
            _fns.vkCmdDispatch( buffer, x, y, z );
        }

        //---------------------
        // COMMAND POOL

        inline auto create_command_pool( device_queue qt ) const
        {
            auto info             = core::command_pool_create_info {};
            info.flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            info.queueFamilyIndex = _queue_indices[static_cast< uint32_t >( qt )];

            VkCommandPool pool;
            vk::error::throw_if_error( "vkCreateCommandPool",
                                       _fns.vkCreateCommandPool( _device, &info, nullptr, &pool ),
                                       "failed to create command pool for specified queue type" );

            return core::command_pool { _device, pool, _fns.vkDestroyCommandPool };
        }

        inline void reset_command_pool( const core::command_pool& pool ) const
        {
            vk::error::throw_if_error( "vkResetCommandPool",
                                       _fns.vkResetCommandPool( _device, pool, 0 ),
                                       "failed to reset the command pool" );
        }

        //---------------------
        // DESCRIPTOR SETS

        inline auto create_descriptor_set_layout(
            const std::span< const VkDescriptorSetLayoutBinding > bindings ) const
        {
            auto info         = core::descriptor_set_layout_create_info {};
            info.bindingCount = bindings.size();
            info.pBindings    = bindings.data();

            VkDescriptorSetLayout layout;
            vk::error::throw_if_error(
                "vkCreateDescriptorSetLayout",
                _fns.vkCreateDescriptorSetLayout( _device, &info, nullptr, &layout ),
                "failed to create descriptor set layout" );

            return core::descriptor_set_layout {
                _device, layout, _fns.vkDestroyDescriptorSetLayout };
        }

        //---------------------
        // PIPELINES

        inline auto
        create_pipeline_layout( const std::span< const VkDescriptorSetLayout > set_layouts,
                                const std::span< const VkPushConstantRange > push_constants ) const
        {
            auto info                   = core::pipeline_layout_create_info {};
            info.pushConstantRangeCount = push_constants.size();
            info.pPushConstantRanges    = push_constants.data();
            info.setLayoutCount         = set_layouts.size();
            info.pSetLayouts            = set_layouts.data();

            VkPipelineLayout layout;
            vk::error::throw_if_error(
                "vkCreatePiplineLayout",
                _fns.vkCreatePipelineLayout( _device, &info, nullptr, &layout ),
                "failed to create pipeline layout" );

            return core::pipeline_layout { _device, layout, _fns.vkDestroyPipelineLayout };
        }

        inline auto create_compute_pipeline( const core::pipeline_layout& layout,
                                             const core::shader_module& shader,
                                             const char* entry_name ) const
        {
            auto stage_info   = core::pipeline_shader_stage_create_info {};
            stage_info.stage  = VK_SHADER_STAGE_COMPUTE_BIT;
            stage_info.module = shader;
            stage_info.pName  = entry_name;

            auto info   = core::compute_pipeline_create_info {};
            info.layout = layout;
            info.stage  = stage_info;

            VkPipeline pipeline;
            vk::error::throw_if_error( "vkCreateComputePipelines",
                                       _fns.vkCreateComputePipelines(
                                           _device, VK_NULL_HANDLE, 1, &info, nullptr, &pipeline ),
                                       "failed to create compute pipeline" );

            return core::pipeline { _device, pipeline, _fns.vkDestroyPipeline };
        }

        //---------------------
        // SHADERS

        inline auto create_shader( const std::span< const uint32_t > code ) const
        {
            auto info     = core::shader_module_create_info {};
            info.codeSize = code.size() * sizeof( uint32_t );
            info.pCode    = code.data();

            VkShaderModule shader;
            vk::error::throw_if_error(
                "vkCreateShaderModule",
                _fns.vkCreateShaderModule( _device, &info, nullptr, &shader ),
                "failed to create shader module from code segment" );

            return core::shader_module { _device, shader, _fns.vkDestroyShaderModule };
        }


    private:
        const core::instance& _instance;
        const core::physical_device& _gpu;
        const core::device _device;
        device_functions_t _fns;
        std::array< uint32_t, 3 > _queue_indices;
        VkQueue _compute;
        VkQueue _graphics;
        VkQueue _transfer;
    };

    template< typename loader_t, typename configurator_t >
    auto make_logical_device( const core::loader_base< loader_t >& loader,
                              const core::app_context& app,
                              const core::physical_device& gpu,
                              const core::device_configurator< configurator_t >& cfg )
    {
        using device_functions_t = typename core::loader_base< loader_t >::device_functions_t;

        auto exts               = cfg.enabled_extensions();
        auto enabled_features   = cfg.enabled_features();
        auto [gfx, compute, tx] = cfg.queues();

        VkPhysicalDeviceFeatures features {};
        std::for_each(
            std::begin( enabled_features ),
            std::end( enabled_features ),
            [&features]( auto feature ) { enable_device_feature( features, feature ); } );

        float priority = 1.0f;
        auto qcis      = std::array< core::device_queue_create_info, 3 > {};
        auto qcisCount = 0;
        auto add_queue = [&]( auto index ) {
            if ( index == static_cast< uint32_t >( -1 ) )
                return;

            qcis[qcisCount].queueFamilyIndex = index;
            qcis[qcisCount].queueCount       = 1;
            qcis[qcisCount].pQueuePriorities = &priority;
            ++qcisCount;
        };
        add_queue( gfx );
        add_queue( compute );
        add_queue( tx );

        auto ci                    = core::device_create_info {};
        ci.queueCreateInfoCount    = qcisCount;
        ci.pQueueCreateInfos       = &qcis[0];
        ci.pEnabledFeatures        = &features;
        ci.enabledExtensionCount   = static_cast< uint32_t >( exts.size() );
        ci.ppEnabledExtensionNames = exts.data();

        auto [dev, fns] = loader.create_device( app, gpu, ci );
        return core::logical_device< device_functions_t > {
            app, gpu, std::move( dev ), fns, gfx, compute, tx };
    }

}   // namespace sl::vk::core

#endif /* __LOGICAL_DEVICE_H_6FC08D1CA064493B88B86F73349F912C__ */
