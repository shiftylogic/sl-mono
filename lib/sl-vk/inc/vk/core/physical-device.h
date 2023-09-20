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

#ifndef __PHYSICAL_DEVICE_H_64D8DD3EE39D4B10ACAF141EA387AF27__
#define __PHYSICAL_DEVICE_H_64D8DD3EE39D4B10ACAF141EA387AF27__

#include <vk/core/device-feature.h>

namespace sl::vk::core
{

    struct physical_device
    {
        explicit physical_device( VkPhysicalDevice device,
                                  VkPhysicalDeviceProperties properties,
                                  VkPhysicalDeviceMemoryProperties memory_properties,
                                  VkPhysicalDeviceFeatures features )
            : _device { device }
            , _properties { properties }
            , _memory_properties { memory_properties }
            , _features { features }
        {}

        operator VkPhysicalDevice() const noexcept { return _device; }

        const char* name() const noexcept { return _properties.deviceName; }
        auto type() const noexcept { return _properties.deviceType; }
        auto api_version() const noexcept { return _properties.apiVersion; }
        auto driver_version() const noexcept { return _properties.driverVersion; }
        const auto& limits() const { return _properties.limits; }

        const auto& memory_properties() const { return _memory_properties; }

        bool has_feature( core::device_feature feature ) const
        {
            return core::has_device_feature( _features, feature );
        }

    private:
        const VkPhysicalDevice _device;
        const VkPhysicalDeviceProperties _properties;
        const VkPhysicalDeviceMemoryProperties _memory_properties;
        const VkPhysicalDeviceFeatures _features;
    };

}   // namespace sl::vk::core

#endif /* __PHYSICAL_DEVICE_H_64D8DD3EE39D4B10ACAF141EA387AF27__ */
