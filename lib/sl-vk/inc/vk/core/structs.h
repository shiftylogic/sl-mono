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

#ifndef __STRUCTS_H_3492D4BB7E1347C9BE535A83FDA06211__
#define __STRUCTS_H_3492D4BB7E1347C9BE535A83FDA06211__

namespace sl::vk::core
{

    namespace details
    {

        template< typename Base, VkStructureType SType >
        struct struct_base : public Base
        {
            struct_base()
                : Base {}
            {
                this->sType = SType;
            }
        };

    }   // namespace details


    // Application / instance structures
    //
    using application_info
        = vk::core::details::struct_base< VkApplicationInfo, VK_STRUCTURE_TYPE_APPLICATION_INFO >;
    using instance_create_info
        = vk::core::details::struct_base< VkInstanceCreateInfo,
                                          VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO >;

    // Create info structures
    //
    using buffer_create_info
        = vk::core::details::struct_base< VkBufferCreateInfo,
                                          VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO >;
    using command_pool_create_info
        = vk::core::details::struct_base< VkCommandPoolCreateInfo,
                                          VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO >;
    using debug_utils_messenger_create_info_ext
        = vk::core::details::struct_base< VkDebugUtilsMessengerCreateInfoEXT,
                                          VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT >;
    using descriptor_pool_create_info
        = vk::core::details::struct_base< VkDescriptorPoolCreateInfo,
                                          VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO >;
    using descriptor_set_layout_create_info
        = vk::core::details::struct_base< VkDescriptorSetLayoutCreateInfo,
                                          VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO >;
    using descriptor_update_template_create_info = vk::core::details::struct_base<
        VkDescriptorUpdateTemplateCreateInfo,
        VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO >;
    using device_create_info
        = vk::core::details::struct_base< VkDeviceCreateInfo,
                                          VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO >;
    using device_queue_create_info
        = vk::core::details::struct_base< VkDeviceQueueCreateInfo,
                                          VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO >;
    using fence_create_info
        = vk::core::details::struct_base< VkFenceCreateInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO >;
    using framebuffer_create_info
        = vk::core::details::struct_base< VkFramebufferCreateInfo,
                                          VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO >;
    using compute_pipeline_create_info
        = vk::core::details::struct_base< VkComputePipelineCreateInfo,
                                          VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO >;
    using graphics_pipeline_create_info
        = vk::core::details::struct_base< VkGraphicsPipelineCreateInfo,
                                          VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO >;
    using image_create_info
        = vk::core::details::struct_base< VkImageCreateInfo, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO >;
    using image_view_create_info
        = vk::core::details::struct_base< VkImageViewCreateInfo,
                                          VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO >;
    using pipeline_color_blend_state_create_info = vk::core::details::struct_base<
        VkPipelineColorBlendStateCreateInfo,
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO >;
    using pipeline_depth_stencil_state_create_info = vk::core::details::struct_base<
        VkPipelineDepthStencilStateCreateInfo,
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO >;
    using pipeline_dynamic_state_create_info
        = vk::core::details::struct_base< VkPipelineDynamicStateCreateInfo,
                                          VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO >;
    using pipeline_input_assembly_state_create_info = vk::core::details::struct_base<
        VkPipelineInputAssemblyStateCreateInfo,
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO >;
    using pipeline_layout_create_info
        = vk::core::details::struct_base< VkPipelineLayoutCreateInfo,
                                          VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO >;
    using pipeline_multisample_state_create_info = vk::core::details::struct_base<
        VkPipelineMultisampleStateCreateInfo,
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO >;
    using pipeline_shader_stage_create_info
        = vk::core::details::struct_base< VkPipelineShaderStageCreateInfo,
                                          VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO >;
    using pipeline_rasterization_state_create_info = vk::core::details::struct_base<
        VkPipelineRasterizationStateCreateInfo,
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO >;
    using pipeline_vertex_input_state_create_info = vk::core::details::struct_base<
        VkPipelineVertexInputStateCreateInfo,
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO >;
    using pipeline_viewport_state_create_info
        = vk::core::details::struct_base< VkPipelineViewportStateCreateInfo,
                                          VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO >;
    using render_pass_create_info
        = vk::core::details::struct_base< VkRenderPassCreateInfo,
                                          VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO >;
    using sampler_create_info
        = vk::core::details::struct_base< VkSamplerCreateInfo,
                                          VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO >;
    using semaphore_create_info
        = vk::core::details::struct_base< VkSemaphoreCreateInfo,
                                          VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO >;
    using shader_module_create_info
        = vk::core::details::struct_base< VkShaderModuleCreateInfo,
                                          VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO >;
    using swapchain_create_info_khr
        = vk::core::details::struct_base< VkSwapchainCreateInfoKHR,
                                          VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR >;

    // Allocation structures
    //
    using command_buffer_allocate_info
        = vk::core::details::struct_base< VkCommandBufferAllocateInfo,
                                          VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO >;
    using descriptor_set_allocate_info
        = vk::core::details::struct_base< VkDescriptorSetAllocateInfo,
                                          VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO >;
    using memory_allocate_info
        = vk::core::details::struct_base< VkMemoryAllocateInfo,
                                          VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO >;

    // Writer structures
    //
    using write_descriptor_set
        = vk::core::details::struct_base< VkWriteDescriptorSet,
                                          VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET >;

    // Command buffer recording structures
    //
    using command_buffer_begin_info
        = vk::core::details::struct_base< VkCommandBufferBeginInfo,
                                          VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO >;
    using image_memory_barrier
        = vk::core::details::struct_base< VkImageMemoryBarrier,
                                          VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER >;
    using submit_info
        = vk::core::details::struct_base< VkSubmitInfo, VK_STRUCTURE_TYPE_SUBMIT_INFO >;

    // Render / presentation structures
    //
    using present_info_khr
        = vk::core::details::struct_base< VkPresentInfoKHR, VK_STRUCTURE_TYPE_PRESENT_INFO_KHR >;
    using render_pass_begin_info
        = vk::core::details::struct_base< VkRenderPassBeginInfo,
                                          VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO >;

}   // namespace sl::vk::core

#endif /* __STRUCTS_H_3492D4BB7E1347C9BE535A83FDA06211__ */
