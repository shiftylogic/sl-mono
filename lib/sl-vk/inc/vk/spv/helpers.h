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

#ifndef __HELPERS_H_2A577946409C4CC693DA84AF8FAF1D6C__
#define __HELPERS_H_2A577946409C4CC693DA84AF8FAF1D6C__

namespace sl::vk::spv
{

    uint32_t format_size( VkFormat format )
    {
        switch ( format )
        {
        case VK_FORMAT_UNDEFINED:
            return 0;
        case VK_FORMAT_R4G4_UNORM_PACK8:
            return 1;
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            return 2;
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
            return 2;
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            return 2;
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
            return 2;
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
            return 2;
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
            return 2;
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            return 2;
        case VK_FORMAT_R8_UNORM:
            return 1;
        case VK_FORMAT_R8_SNORM:
            return 1;
        case VK_FORMAT_R8_USCALED:
            return 1;
        case VK_FORMAT_R8_SSCALED:
            return 1;
        case VK_FORMAT_R8_UINT:
            return 1;
        case VK_FORMAT_R8_SINT:
            return 1;
        case VK_FORMAT_R8_SRGB:
            return 1;
        case VK_FORMAT_R8G8_UNORM:
            return 2;
        case VK_FORMAT_R8G8_SNORM:
            return 2;
        case VK_FORMAT_R8G8_USCALED:
            return 2;
        case VK_FORMAT_R8G8_SSCALED:
            return 2;
        case VK_FORMAT_R8G8_UINT:
            return 2;
        case VK_FORMAT_R8G8_SINT:
            return 2;
        case VK_FORMAT_R8G8_SRGB:
            return 2;
        case VK_FORMAT_R8G8B8_UNORM:
            return 3;
        case VK_FORMAT_R8G8B8_SNORM:
            return 3;
        case VK_FORMAT_R8G8B8_USCALED:
            return 3;
        case VK_FORMAT_R8G8B8_SSCALED:
            return 3;
        case VK_FORMAT_R8G8B8_UINT:
            return 3;
        case VK_FORMAT_R8G8B8_SINT:
            return 3;
        case VK_FORMAT_R8G8B8_SRGB:
            return 3;
        case VK_FORMAT_B8G8R8_UNORM:
            return 3;
        case VK_FORMAT_B8G8R8_SNORM:
            return 3;
        case VK_FORMAT_B8G8R8_USCALED:
            return 3;
        case VK_FORMAT_B8G8R8_SSCALED:
            return 3;
        case VK_FORMAT_B8G8R8_UINT:
            return 3;
        case VK_FORMAT_B8G8R8_SINT:
            return 3;
        case VK_FORMAT_B8G8R8_SRGB:
            return 3;
        case VK_FORMAT_R8G8B8A8_UNORM:
            return 4;
        case VK_FORMAT_R8G8B8A8_SNORM:
            return 4;
        case VK_FORMAT_R8G8B8A8_USCALED:
            return 4;
        case VK_FORMAT_R8G8B8A8_SSCALED:
            return 4;
        case VK_FORMAT_R8G8B8A8_UINT:
            return 4;
        case VK_FORMAT_R8G8B8A8_SINT:
            return 4;
        case VK_FORMAT_R8G8B8A8_SRGB:
            return 4;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return 4;
        case VK_FORMAT_B8G8R8A8_SNORM:
            return 4;
        case VK_FORMAT_B8G8R8A8_USCALED:
            return 4;
        case VK_FORMAT_B8G8R8A8_SSCALED:
            return 4;
        case VK_FORMAT_B8G8R8A8_UINT:
            return 4;
        case VK_FORMAT_B8G8R8A8_SINT:
            return 4;
        case VK_FORMAT_B8G8R8A8_SRGB:
            return 4;
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            return 4;
        case VK_FORMAT_R16_UNORM:
            return 2;
        case VK_FORMAT_R16_SNORM:
            return 2;
        case VK_FORMAT_R16_USCALED:
            return 2;
        case VK_FORMAT_R16_SSCALED:
            return 2;
        case VK_FORMAT_R16_UINT:
            return 2;
        case VK_FORMAT_R16_SINT:
            return 2;
        case VK_FORMAT_R16_SFLOAT:
            return 2;
        case VK_FORMAT_R16G16_UNORM:
            return 4;
        case VK_FORMAT_R16G16_SNORM:
            return 4;
        case VK_FORMAT_R16G16_USCALED:
            return 4;
        case VK_FORMAT_R16G16_SSCALED:
            return 4;
        case VK_FORMAT_R16G16_UINT:
            return 4;
        case VK_FORMAT_R16G16_SINT:
            return 4;
        case VK_FORMAT_R16G16_SFLOAT:
            return 4;
        case VK_FORMAT_R16G16B16_UNORM:
            return 6;
        case VK_FORMAT_R16G16B16_SNORM:
            return 6;
        case VK_FORMAT_R16G16B16_USCALED:
            return 6;
        case VK_FORMAT_R16G16B16_SSCALED:
            return 6;
        case VK_FORMAT_R16G16B16_UINT:
            return 6;
        case VK_FORMAT_R16G16B16_SINT:
            return 6;
        case VK_FORMAT_R16G16B16_SFLOAT:
            return 6;
        case VK_FORMAT_R16G16B16A16_UNORM:
            return 8;
        case VK_FORMAT_R16G16B16A16_SNORM:
            return 8;
        case VK_FORMAT_R16G16B16A16_USCALED:
            return 8;
        case VK_FORMAT_R16G16B16A16_SSCALED:
            return 8;
        case VK_FORMAT_R16G16B16A16_UINT:
            return 8;
        case VK_FORMAT_R16G16B16A16_SINT:
            return 8;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return 8;
        case VK_FORMAT_R32_UINT:
            return 4;
        case VK_FORMAT_R32_SINT:
            return 4;
        case VK_FORMAT_R32_SFLOAT:
            return 4;
        case VK_FORMAT_R32G32_UINT:
            return 8;
        case VK_FORMAT_R32G32_SINT:
            return 8;
        case VK_FORMAT_R32G32_SFLOAT:
            return 8;
        case VK_FORMAT_R32G32B32_UINT:
            return 12;
        case VK_FORMAT_R32G32B32_SINT:
            return 12;
        case VK_FORMAT_R32G32B32_SFLOAT:
            return 12;
        case VK_FORMAT_R32G32B32A32_UINT:
            return 16;
        case VK_FORMAT_R32G32B32A32_SINT:
            return 16;
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return 16;
        case VK_FORMAT_R64_UINT:
            return 8;
        case VK_FORMAT_R64_SINT:
            return 8;
        case VK_FORMAT_R64_SFLOAT:
            return 8;
        case VK_FORMAT_R64G64_UINT:
            return 16;
        case VK_FORMAT_R64G64_SINT:
            return 16;
        case VK_FORMAT_R64G64_SFLOAT:
            return 16;
        case VK_FORMAT_R64G64B64_UINT:
            return 24;
        case VK_FORMAT_R64G64B64_SINT:
            return 24;
        case VK_FORMAT_R64G64B64_SFLOAT:
            return 24;
        case VK_FORMAT_R64G64B64A64_UINT:
            return 32;
        case VK_FORMAT_R64G64B64A64_SINT:
            return 32;
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            return 32;
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            return 4;
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
            return 4;
        default:
            throw std::runtime_error( "unsupported format" );
        }
    }

}   // namespace sl::vk::spv

#endif /* __HELPERS_H_2A577946409C4CC693DA84AF8FAF1D6C__ */
