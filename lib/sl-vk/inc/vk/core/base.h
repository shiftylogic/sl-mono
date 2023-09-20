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

#ifndef __BASE_H_5C1B94B6DA9F42E0B35894ED8C9CAAA9__
#define __BASE_H_5C1B94B6DA9F42E0B35894ED8C9CAAA9__

#include <span>
#include <vector>

#include <vk/core/resources.h>
#include <vk/core/structs.h>

#include <vk/core/physical-device.h>

namespace sl::vk::core
{

    /**
     * Workaround to the below 'Impl' template not being complete at the time
     * we need the internal device function table typedef.
     */
    template< typename impl_t >
    struct device_functions_traits;

    /**
     * Defines the template constraints for passing a defined struct
     * to the app_context as a Vulkan 'loader' instance. The Loader
     * is responsible for ensuring the Vulkan libraries are loaded,
     * initialized, and fetches function addresses into the system.
     *
     * An example of a loader would be 'Volk' which we provide a
     * simple wrapper around below [struct loadtemplate er].
     *
     * This is standard CRTP pattern for compile-time polymorphism.
     */
    template< typename impl_t >
    struct loader_base
    {
        typedef typename device_functions_traits< impl_t >::type device_functions_t;

        auto available_layers() const { return self().available_layers(); }

        auto available_extensions() const { return self().available_extensions(); }

        core::instance create_instance( const core::instance_create_info& ci ) const
        {
            return self().create_instance( ci );
        }

        uint32_t runtime_version() const { return self().runtime_version(); }

        auto create_device( const core::instance& inst,
                            const core::physical_device& gpu,
                            const core::device_create_info& ci ) const
        {
            return self().create_device( inst, gpu, ci );
        }

        core::debug_utils_messenger
        create_debug_messenger( const core::instance& inst,
                                const core::debug_utils_messenger_create_info_ext& ci ) const
        {
            return self().create_debug_messenger( inst, ci );
        }

        auto get_physical_devices( const core::instance& inst ) const
        {
            return self().get_physical_devices( inst );
        }

    private:
        inline impl_t const& self() const { return static_cast< impl_t const& >( *this ); }
    };


    /**
     * Defines the template constraints for passing a defined struct
     * to the app_context constructor for performing application
     * configuration.
     *
     * This is standard CRTP pattern for compile-time polymorphism.
     */
    template< typename Impl >
    struct app_configurator
    {
        const char* application_name() const { return self().application_name(); }
        uint32_t application_version() const { return self().application_version(); }
        const char* engine_name() const { return self().engine_name(); }
        uint32_t engine_version() const { return self().engine_version(); }
        uint32_t api_version() const { return self().api_version(); }

        uint32_t instance_flags() const { return self().instance_flags(); }
        auto enabled_layers() const { return self().enabled_layers(); }
        auto enabled_extensions() const { return self().enabled_extensions(); }

    private:
        inline Impl const& self() const { return static_cast< Impl const& >( *this ); }
    };


    /**
     * Defines the template constraints for passing a defined struct to the
     * logical_device constructor for performing device configuration.
     *
     * This is standard CRTP pattern for compile-time polymorphism.
     */
    template< typename Impl >
    struct device_configurator
    {
        auto enabled_extensions() const { return self().enabled_extensions(); }
        auto enabled_features() const { return self().enabled_features(); }

    private:
        inline Impl const& self() const { return static_cast< Impl const& >( *this ); }
    };

}   // namespace sl::vk::core

#endif /* __BASE_H_5C1B94B6DA9F42E0B35894ED8C9CAAA9__ */
