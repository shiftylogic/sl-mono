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

    template< size_t max_desc_set_layouts = 4 >
    struct program
    {
        explicit program() {}

        void execute() const {}

    private:
        // core::shader_module _shader;
        // core::pipeline _pipeline;
        // core::pipeline_layout _pipeline_layout;
        // std::array< VkDescriptorSetLayout > _layouts;
    };

    auto load_program( std::vector< uint32_t >&& code )
    {
        auto spv_module = sl::vk::spv::shader { code };
        return program {};
    }

}   // namespace sl::vk::compute

#endif /* __PROGRAM_H_C74161AFB993442ABDA3F4025844D8CD__ */
