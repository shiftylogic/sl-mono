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

    template< typename device_functions_t >
    struct logical_device
    {
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

    private:
        const core::instance& _instance;
        const core::physical_device& _gpu;
        const core::device _device;
        device_functions_t _fns;
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
        using device_functions_t = core::loader_base< loader_t >::device_functions_t;

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
