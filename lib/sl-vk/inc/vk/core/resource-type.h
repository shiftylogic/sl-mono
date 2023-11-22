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

#ifndef __RESOURCE_TYPE_H_E00E6F2C9D0344D7855250622C0FAC47__
#define __RESOURCE_TYPE_H_E00E6F2C9D0344D7855250622C0FAC47__

#include <vulkan.h>

namespace sl::vk::core
{

    enum class resource_type
    {
        sampler                = VK_DESCRIPTOR_TYPE_SAMPLER,
        combined_image_sampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        sampled_image          = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        storage_image          = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        uniform_texel_buffer   = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
        storage_texel_buffer   = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
        uniform_buffer         = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        storage_buffer         = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        uniform_dynamic_buffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        storage_dynamic_buffer = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
        input_attachment       = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
    };

}

#endif /* __RESOURCE_TYPE_H_E00E6F2C9D0344D7855250622C0FAC47__ */
