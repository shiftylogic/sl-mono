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

#ifndef __RESOURCES_H_CC5ED38782CC4B9EA4687D297C621B67__
#define __RESOURCES_H_CC5ED38782CC4B9EA4687D297C621B67__

#include <vulkan.h>

#include "./device-resource.h"

namespace sl::vk::core
{

    /*
     * Core Vulkan Resources
     */

    using device   = vk::core::resource< VkDevice, const VkAllocationCallbacks >;
    using instance = vk::core::resource< VkInstance, const VkAllocationCallbacks >;


    /*
     * Instance-specific Vulkan Resources
     */

    using debug_utils_messenger = vk::core::
        device_resource< VkDebugUtilsMessengerEXT, VkInstance, const VkAllocationCallbacks >;

    /*
     * Device-specific Vulkan Resources
     */

    using command_pool
        = vk::core::device_resource< VkCommandPool, VkDevice, const VkAllocationCallbacks >;
    using descriptor_set_layout
        = vk::core::device_resource< VkDescriptorSetLayout, VkDevice, const VkAllocationCallbacks >;
    using pipeline = vk::core::device_resource< VkPipeline, VkDevice, const VkAllocationCallbacks >;
    using pipeline_layout
        = vk::core::device_resource< VkPipelineLayout, VkDevice, const VkAllocationCallbacks >;
    using shader_module
        = vk::core::device_resource< VkShaderModule, VkDevice, const VkAllocationCallbacks >;

}   // namespace sl::vk::core

#endif /* __RESOURCES_H_CC5ED38782CC4B9EA4687D297C621B67__ */
