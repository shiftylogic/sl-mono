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

#ifndef __DEVICE_RESOURCE_H_4ADDAF4CA65840FBB539ED86E9D3B9C4__
#define __DEVICE_RESOURCE_H_4ADDAF4CA65840FBB539ED86E9D3B9C4__

#include "./resource.h"

namespace sl::vk::core
{

    template< typename handle_t, typename device_t, typename context_t = void >
    struct device_resource : sl::vk::core::resource< handle_t, context_t >
    {
        using deleter_fn = std::function< void( device_t, handle_t, context_t* ) >;

        device_resource& operator=( device_resource&& r ) noexcept
        {
            if ( this != &r )
            {
                vk::core::resource< handle_t, context_t >::operator=(
                    std::forward< vk::core::resource< handle_t, context_t > >( r ) );

                _device   = r._device;
                r._device = nullptr;
            }

            return *this;
        }

        device_resource() noexcept
            : vk::core::resource< handle_t, context_t > {}
            , _device { nullptr }
        {}

        device_resource( device_resource&& r ) noexcept
            : vk::core::resource< handle_t, context_t >(
                std::forward< vk::core::resource< handle_t, context_t > >( r ) )
            , _device( r._device )
        {
            r._device = nullptr;
        }

        explicit device_resource( device_t device,
                                  handle_t handle,
                                  deleter_fn deleter,
                                  context_t* context = nullptr )
            : vk::core::resource< handle_t, context_t >(
                handle,
                std::bind( deleter, device, std::placeholders::_1, std::placeholders::_2 ),
                context )
            , _device( device )
        {}

    private:
        device_t _device;
    };

}   // namespace sl::vk::core

#endif /* __DEVICE_RESOURCE_H_4ADDAF4CA65840FBB539ED86E9D3B9C4__ */
