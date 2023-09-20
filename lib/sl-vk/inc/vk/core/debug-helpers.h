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

#ifndef __DEBUG_HELPERS_H_50FB080ED2FD40E28764DAFE7030B4BA__
#define __DEBUG_HELPERS_H_50FB080ED2FD40E28764DAFE7030B4BA__

#include <charconv>
#include <functional>
#include <span>

#include <vk/core/loader.h>
#include <vk/core/physical-device.h>

namespace sl::vk::core::debug
{

    namespace priv
    {

        const char* device_type_string( VkPhysicalDeviceType type )
        {
            switch ( type )
            {
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                return "Other";
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                return "Integrated";
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                return "Discrete";
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                return "Virtual";
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                return "CPU";
            default:
                return "Unknown";
            }
        }

        const char* device_feature_string( device_feature feature )
        {
#define DEVICE_FEATURE_STRING_CASE( e, s )                                                         \
case core::device_feature::e:                                                                      \
    return s

            switch ( feature )
            {
                DEVICE_FEATURE_STRING_CASE( anisotropic_filtering, "Anisotropic Filtering" );
                DEVICE_FEATURE_STRING_CASE( depth_bounds, "Depth Bounds Tests" );
                DEVICE_FEATURE_STRING_CASE( dual_src_blend, "Dual Source Blend Operations" );
                DEVICE_FEATURE_STRING_CASE( depth_clamp, "Depth Clamping" );
                DEVICE_FEATURE_STRING_CASE( depth_bias_clamp, "Depth Bias Clamping" );
                DEVICE_FEATURE_STRING_CASE( fill_mode_non_solid, "Point and Wireframe Fill Mode" );
                DEVICE_FEATURE_STRING_CASE( geometry_shader, "Geometry Shader" );
                DEVICE_FEATURE_STRING_CASE( large_points, "Large Points" );
                DEVICE_FEATURE_STRING_CASE( logic_op, "Logic Operations" );
                DEVICE_FEATURE_STRING_CASE( multi_draw_indirect, "Multiple Draw Indirect" );
                DEVICE_FEATURE_STRING_CASE( multi_viewport, "Multi Viewport" );
                DEVICE_FEATURE_STRING_CASE( robust_buffer_access,
                                            "Bounds-checked Buffer Descriptors" );
                DEVICE_FEATURE_STRING_CASE( sample_rate_shading,
                                            "Sample Shading + Multisample Interpolation" );
                DEVICE_FEATURE_STRING_CASE( tessellation_shader, "Tessellation Shader" );
                DEVICE_FEATURE_STRING_CASE( wide_lines, "Wide Lines" );
            }

#undef DEVICE_FEATURE_STRING_CASE
        }

        /**
         * Turns a uint32_t API version number into a display string.
         *
         * Writes directly to a supplied character buffer and returns a pointer to it.
         *
         * Takes the form 'xxxx.yyyy.zzzz [v]'. The '[v] is the "variant" of the API.
         * The string length will never be more than 18 + 1 characters (NULL-terminated).
         */
        template< size_t N >
        const char* api_to_string( uint32_t version, std::span< char, N > buf )
        {
            static_assert( N >= 19 );

            const uint32_t var   = version >> 29;                // 0-3
            const uint32_t major = ( version >> 22 ) & 0x3ffu;   // 0 - 1023
            const uint32_t minor = ( version >> 12 ) & 0x3ffu;   // 0 - 1023
            const uint32_t patch = version & 0xfffu;             // 0 - 4095
            const auto buf_end   = buf.data() + buf.size();

            auto r   = std::to_chars( buf.data(), buf_end, major );
            *r.ptr++ = '.';

            r        = std::to_chars( r.ptr, buf_end, minor );
            *r.ptr++ = '.';

            r        = std::to_chars( r.ptr, buf_end, patch );
            *r.ptr++ = ' ';
            *r.ptr++ = '[';

            r        = std::to_chars( r.ptr, buf_end, var );
            *r.ptr++ = ']';
            *r.ptr++ = '\0';

            return buf.data();
        }

        /**
         * Turns a uint32_t version number into a display string.
         *
         * Writes directly to a supplied character buffer and returns a pointer to it.
         *
         * Takes the form 'xxxx.yyyy.zzzz'.
         * The string length will never be more than 14 + 1 characters (NULL-terminated).
         */
        template< size_t N >
        const char* version_to_string( uint32_t version, std::span< char, N > buf )
        {
            static_assert( N >= 15 );

            const uint32_t major = version >> 22;                // 0 - 1023
            const uint32_t minor = ( version >> 12 ) & 0x3ffu;   // 0 - 1023
            const uint32_t patch = version & 0xfffu;             // 0 - 4095
            const auto buf_end   = buf.data() + buf.size();

            auto r   = std::to_chars( buf.data(), buf_end, major );
            *r.ptr++ = '.';

            r        = std::to_chars( r.ptr, buf_end, minor );
            *r.ptr++ = '.';

            r        = std::to_chars( r.ptr, buf_end, patch );
            *r.ptr++ = '\0';

            return buf.data();
        }

        const char* ptype_to_name( const char* name )
        {
            return name;
        }

        const char* ptype_to_name( const VkLayerProperties& layer )
        {
            return layer.layerName;
        }

        const char* ptype_to_name( const VkExtensionProperties& ext )
        {
            return ext.extensionName;
        }

        template< typename logger_t >
        void
        dump_features( logger_t& logger, const core::physical_device& device, const char* indent )
        {
            auto log_f = [&]( const core::device_feature feature ) {
                logger.trace( "%s %s: %s",
                              indent,
                              device_feature_string( feature ),
                              device.has_feature( feature ) ? "Yes" : "No" );
            };

            core::for_each_device_feature( log_f );
        }

        template< typename logger_t, typename props_t >
        void dump_properties( logger_t& logger, props_t props, const char* indent )
        {
            std::for_each(
                std::begin( props ), std::end( props ), [&logger, indent]( const auto& x ) {
                    logger.trace( "%s %s", indent, priv::ptype_to_name( x ) );
                } );
        }

    }   // namespace priv


    /**
     * Debug helpers for logging diagnostics information from various types.
     * This should be a collection of methods with type overrides.
     */

    template< typename logger_t, typename loader_t >
    void log_diagnostics( logger_t& logger, const core::loader_base< loader_t >& loader )
    {
        char scratch[20];
        auto buf = std::span( scratch );

        logger.trace( "Built with Vulkan SDK v%s",
                      priv::api_to_string( VK_HEADER_VERSION_COMPLETE, buf ) );
        logger.trace( "Running with Vulkan v%s",
                      priv::api_to_string( loader.runtime_version(), buf ) );

        auto layers = loader.available_layers();
        logger.trace( "Available layers:" );
        priv::dump_properties( logger, layers, "  >>>" );

        auto exts = loader.available_extensions();
        logger.trace( "Available extensions:" );
        priv::dump_properties( logger, exts, "  >>>" );
    }

    template< typename logger_t, typename configurator_t >
    void log_diagnostics( logger_t& logger, const app_configurator< configurator_t >& cfg )
    {
        char scratch[16];
        auto buf = std::span( scratch );

        logger.trace( "Vulkan instance configuration:" );
        logger.trace( "  -> App Name: %s", cfg.application_name() );
        logger.trace( "  -> App Version: %s",
                      priv::version_to_string( cfg.application_version(), buf ) );
        logger.trace( "  -> Engine Name: %s", cfg.engine_name() );
        logger.trace( "  -> Engine Version: %s",
                      priv::version_to_string( cfg.engine_version(), buf ) );
        logger.trace( "  -> API Version: %s", priv::version_to_string( cfg.api_version(), buf ) );

        auto layers = cfg.enabled_layers();
        logger.trace( "  -> Enabled Layers:" );
        priv::dump_properties( logger, layers, "    +++" );

        auto exts = cfg.enabled_extensions();
        logger.trace( "  -> Enabled Extensions:" );
        priv::dump_properties( logger, exts, "    +++" );
    }

    template< typename logger_t, typename configurator_t >
    void log_diagnostics( logger_t& logger, const device_configurator< configurator_t >& cfg )
    {
        logger.trace( "Vulkan device configuration:" );

        auto exts = cfg.enabled_extensions();
        logger.trace( "  -> Enabled Extensions:" );
        priv::dump_properties( logger, exts, "    +++" );

        auto features = cfg.enabled_features();
        logger.trace( "  -> Enabled Features:" );
        std::for_each( std::begin( features ),
                       std::end( features ),
                       [&logger]( const core::device_feature feature ) {
                           logger.trace( "    +++ %s", priv::device_feature_string( feature ) );
                       } );
    }

    template< typename logger_t >
    void log_diagnostics( logger_t& logger, const sl::vk::core::physical_device& device )
    {
        char scratch[20];
        auto buf = std::span( scratch );

        logger.trace( "Available Physical Device:" );
        logger.trace( "  => %s", device.name() );
        logger.trace( "    > API Version:    %s",
                      priv::api_to_string( device.api_version(), buf ) );
        logger.trace( "    > Driver Version: 0x%08x", device.driver_version() );
        logger.trace( "    > Device Type:    %s", priv::device_type_string( device.type() ) );

        const auto& limits = device.limits();
        logger.trace( "    > Device Limits:" );
        logger.trace( "      -> Discrete Queue Priorities:                 %lu",
                      limits.discreteQueuePriorities );
        logger.trace( "      -> Maximum Bound Descriptor Sets:             %lu",
                      limits.maxBoundDescriptorSets );
        logger.trace( "      -> Maximum Descriptor Set Input Attachments:  %lu",
                      limits.maxDescriptorSetInputAttachments );
        logger.trace( "      -> Maximum Descriptor Set Sampled Images:     %lu",
                      limits.maxDescriptorSetSampledImages );
        logger.trace( "      -> Maximum Descriptor Set Samplers:           %lu",
                      limits.maxDescriptorSetSamplers );
        logger.trace( "      -> Maximum Descriptor Set Storage Buffers:    %lu",
                      limits.maxDescriptorSetStorageBuffers );
        logger.trace( "      -> Maximum Descriptor Set Storage Images:     %lu",
                      limits.maxDescriptorSetStorageImages );
        logger.trace( "      -> Maximum Descriptor Set Uniform Buffers:    %lu",
                      limits.maxDescriptorSetUniformBuffers );
        logger.trace( "      -> Maximum Per Stage Resources:               %lu",
                      limits.maxPerStageResources );
        logger.trace( "      -> Maximum Framebuffer Height:                %lu",
                      limits.maxFramebufferHeight );
        logger.trace( "      -> Maximum Framebuffer Width:                 %lu",
                      limits.maxFramebufferWidth );
        logger.trace( "      -> Maximum Memory Allocations:                %lu",
                      limits.maxMemoryAllocationCount );
        logger.trace( "      -> Maximum Push Constant Bytes:               %lu",
                      limits.maxPushConstantsSize );
        logger.trace( "      -> Maximum Vertex Input Attributes:           %lu",
                      limits.maxVertexInputAttributes );
        logger.trace( "      -> Maximum Vertex Input Bindings:             %lu",
                      limits.maxVertexInputBindings );
        logger.trace( "      -> Texel Buffer Alignment:                    %lu",
                      limits.minTexelBufferOffsetAlignment );
        logger.trace( "      -> Storage Buffer Alignment:                  %lu",
                      limits.minStorageBufferOffsetAlignment );
        logger.trace( "      -> Uniform Buffer Alignment:                  %lu",
                      limits.minUniformBufferOffsetAlignment );

        logger.trace( "   > Device Features:" );
        priv::dump_features( logger, device, "     -> " );
    }

    template< typename logger_t >
    void log_diagnostics( logger_t& logger,
                          const std::span< const sl::vk::core::physical_device > devices )
    {
        std::for_each( std::begin( devices ), std::end( devices ), [&logger]( const auto& device ) {
            log_diagnostics( logger, device );
        } );
    }

}   // namespace sl::vk::core::debug

#endif /* __DEBUG_HELPERS_H_50FB080ED2FD40E28764DAFE7030B4BA__ */
