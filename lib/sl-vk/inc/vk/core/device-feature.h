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

#ifndef __DEVICE_FEATURE_H_BA9463F026D74873AE46671D293DA1E2__
#define __DEVICE_FEATURE_H_BA9463F026D74873AE46671D293DA1E2__

#include <utils/enums.h>

namespace sl::vk::core
{

    enum class device_feature
    {
        anisotropic_filtering,
        depth_bounds,
        depth_clamp,
        depth_bias_clamp,
        dual_src_blend,
        fill_mode_non_solid,
        geometry_shader,
        large_points,
        logic_op,
        multi_draw_indirect,
        multi_viewport,
        robust_buffer_access,
        sample_rate_shading,
        tessellation_shader,
        wide_lines,
    };

    constexpr auto k_no_device_features = std::span< device_feature > {};

    typedef sl::utils::enum_iterator< device_feature,
                                      device_feature::anisotropic_filtering,
                                      device_feature::wide_lines >
        device_features_iterator;

    template< typename fn_t >
    inline void for_each_device_feature( fn_t fn )
    {
        for ( auto f : device_features_iterator {} )
            fn( f );
    }

    inline void enable_device_feature( VkPhysicalDeviceFeatures& features, device_feature feature )
    {
#define ENABLE_DEVICE_FEATURE_CASE( e, p )                                                         \
case device_feature::e:                                                                            \
    features.p = VK_TRUE;                                                                          \
    break

        switch ( feature )
        {
            ENABLE_DEVICE_FEATURE_CASE( anisotropic_filtering, samplerAnisotropy );
            ENABLE_DEVICE_FEATURE_CASE( depth_bounds, depthBounds );
            ENABLE_DEVICE_FEATURE_CASE( dual_src_blend, dualSrcBlend );
            ENABLE_DEVICE_FEATURE_CASE( depth_clamp, depthClamp );
            ENABLE_DEVICE_FEATURE_CASE( depth_bias_clamp, depthBiasClamp );
            ENABLE_DEVICE_FEATURE_CASE( fill_mode_non_solid, fillModeNonSolid );
            ENABLE_DEVICE_FEATURE_CASE( geometry_shader, geometryShader );
            ENABLE_DEVICE_FEATURE_CASE( large_points, largePoints );
            ENABLE_DEVICE_FEATURE_CASE( logic_op, logicOp );
            ENABLE_DEVICE_FEATURE_CASE( multi_draw_indirect, multiDrawIndirect );
            ENABLE_DEVICE_FEATURE_CASE( multi_viewport, multiViewport );
            ENABLE_DEVICE_FEATURE_CASE( robust_buffer_access, robustBufferAccess );
            ENABLE_DEVICE_FEATURE_CASE( sample_rate_shading, sampleRateShading );
            ENABLE_DEVICE_FEATURE_CASE( tessellation_shader, tessellationShader );
            ENABLE_DEVICE_FEATURE_CASE( wide_lines, wideLines );
        }

#undef ENABLE_DEVICE_FEATURE_CASE
    }

    inline bool has_device_feature( const VkPhysicalDeviceFeatures& features,
                                    device_feature feature )
    {

#define HAS_DEVICE_FEATURE_CASE( e, p )                                                            \
case device_feature::e:                                                                            \
    return features.p == VK_TRUE

        switch ( feature )
        {
            HAS_DEVICE_FEATURE_CASE( anisotropic_filtering, samplerAnisotropy );
            HAS_DEVICE_FEATURE_CASE( depth_bounds, depthBounds );
            HAS_DEVICE_FEATURE_CASE( dual_src_blend, dualSrcBlend );
            HAS_DEVICE_FEATURE_CASE( depth_clamp, depthClamp );
            HAS_DEVICE_FEATURE_CASE( depth_bias_clamp, depthBiasClamp );
            HAS_DEVICE_FEATURE_CASE( fill_mode_non_solid, fillModeNonSolid );
            HAS_DEVICE_FEATURE_CASE( geometry_shader, geometryShader );
            HAS_DEVICE_FEATURE_CASE( large_points, largePoints );
            HAS_DEVICE_FEATURE_CASE( logic_op, logicOp );
            HAS_DEVICE_FEATURE_CASE( multi_draw_indirect, multiDrawIndirect );
            HAS_DEVICE_FEATURE_CASE( multi_viewport, multiViewport );
            HAS_DEVICE_FEATURE_CASE( robust_buffer_access, robustBufferAccess );
            HAS_DEVICE_FEATURE_CASE( sample_rate_shading, sampleRateShading );
            HAS_DEVICE_FEATURE_CASE( tessellation_shader, tessellationShader );
            HAS_DEVICE_FEATURE_CASE( wide_lines, wideLines );
        }

#undef HAS_DEVICE_FEATURE_CASE
    }

}   // namespace sl::vk::core

#endif /* __DEVICE_FEATURE_H_BA9463F026D74873AE46671D293DA1E2__ */
