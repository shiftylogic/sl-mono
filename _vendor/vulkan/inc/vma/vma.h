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

#ifndef __VMA_H_4839E09DBB9845BA9F4C3740F0F16942__
#define __VMA_H_4839E09DBB9845BA9F4C3740F0F16942__

#include "../vulkan.h"

#ifdef _MSVC_LANG

#    pragma warning( push, 4 )
// #    pragma warning( disable : 4127 )   // conditional expression is constant
// #    pragma warning( disable : 4100 )   // unreferenced formal parameter
// #    pragma warning( disable : 4189 )   // local variable is initialized but not referenced
// #    pragma warning( disable : 4324 )   // structure was padded due to alignment specifier

#endif   // #ifdef _MSVC_LANG


#ifdef __clang__

#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wunused-parameter"
#    pragma clang diagnostic ignored "-Wunused-variable"
#    pragma clang diagnostic ignored "-Wmissing-field-initializers"
#    pragma clang diagnostic ignored "-Wnullability-completeness"
#    pragma clang diagnostic ignored "-Wnullability-extension"

#endif

#define VMA_STATIC_VULKAN_FUNCTIONS  0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#include "../../src/vma/vk_mem_alloc.h"

#ifdef __clang__
#    pragma clang diagnostic pop
#endif

#ifdef _MSVC_LANG
#    pragma warning( pop )
#endif

#endif /* __VMA_H_4839E09DBB9845BA9F4C3740F0F16942__ */
