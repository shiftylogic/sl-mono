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

#ifndef __VK_H_C0F3ABF8451E4052B1C9A6830AE9AD52__
#define __VK_H_C0F3ABF8451E4052B1C9A6830AE9AD52__

/*
 * Pre-load all the necessary Vulkan libraries for Volk. The
 * headers are local to this project, so we can't rely on the
 * Volk.h header to correctly find them.
 */
#if defined( VK_USE_PLATFORM_WIN32_KHR )
#    include <vk/vk_platform.h>
#    include <vk/vulkan_core.h>

/* When VK_USE_PLATFORM_WIN32_KHR is defined, instead of including vulkan.h directly, we include
 * individual parts of the SDK This is necessary to avoid including <windows.h> which is very heavy
 * - it takes 200ms to parse without WIN32_LEAN_AND_MEAN and 100ms to parse with it. vulkan_win32.h
 * only needs a few symbols that are easy to redefine ourselves.
 */
typedef unsigned long DWORD;
typedef const wchar_t* LPCWSTR;
typedef void* HANDLE;
typedef struct HINSTANCE__* HINSTANCE;
typedef struct HWND__* HWND;
typedef struct HMONITOR__* HMONITOR;
typedef struct _SECURITY_ATTRIBUTES SECURITY_ATTRIBUTES;

#    include <vk/vulkan_win32.h>

#    ifdef VK_ENABLE_BETA_EXTENSIONS
#        include <vk/vulkan_beta.h>
#    endif
#else
#    include <vk/vulkan.h>
#endif

#endif /* __VK_H_C0F3ABF8451E4052B1C9A6830AE9AD52__ */
