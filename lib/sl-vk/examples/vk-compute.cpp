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

#include <algorithm>
#include <array>
#include <chrono>
#include <random>
#include <span>
#include <vector>

#include <io/load.h>
#include <logging/logger.h>
#include <utils/fs.h>
#include <utils/version.h>

#include <vk/core/app-context.h>
#include <vk/core/debug-helpers.h>
#include <vk/core/fixed-descriptor-cache.h>
#include <vk/core/loader.h>
#include <vk/core/logical-device.h>
#include <vk/mem/allocator.h>
#include <vk/mem/buffers.h>

#include <vk/compute/context.h>
#include <vk/compute/program.h>

namespace
{

    constexpr auto k_number_count = 128 * 1024 * 1024;
    const auto k_square_spv_file  = sl::utils::base_directory() / "assets/square.spv";
    auto debug_logger             = sl::logging::logger {};

    const auto dcache_config = sl::vk::core::fixed_descriptor_cache::config {
        .pool_count = 1,
        .max_sets = 2,
        .pool_sizes = {
            { sl::vk::core::resource_type::storage_buffer, 2 },
        },
    };

    static VKAPI_ATTR VkBool32 VKAPI_CALL
    handle_vulkan_debug( VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                         VkDebugUtilsMessageTypeFlagsEXT type,
                         [[maybe_unused]] const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                         void* /*pUserData*/ )
    {
        if ( type == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT )
        {
            debug_logger.warn( "*** Performance *** ==> %s", pCallbackData->pMessage );
            return VK_FALSE;
        }

        switch ( severity )
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            debug_logger.trace( pCallbackData->pMessage );
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            debug_logger.info( pCallbackData->pMessage );
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            debug_logger.warn( pCallbackData->pMessage );
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            debug_logger.error( pCallbackData->pMessage );
            break;

        default:
            debug_logger.info( "Unknown severity - %s", pCallbackData->pMessage );
        }

        return VK_FALSE;
    }

    struct app_configurator : sl::vk::core::app_configurator< app_configurator >
    {
        static constexpr std::array k_layers = {
            "VK_LAYER_KHRONOS_validation",
        };

        static constexpr std::array k_extensions = {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
        };

        static constexpr const char* k_app_name = "vk-compute";
        static constexpr sl::utils::version k_app_version { 0, 1, 0 };

        static constexpr const char* k_engine_name = "sl-engine";
        static constexpr sl::utils::version k_engine_version { 0, 2, 1 };


        /**
         * Required interface for passing this struct to app_context ctor
         * as an application configurator.
         */

        const char* application_name() const { return k_app_name; }
        uint32_t application_version() const { return k_app_version; }

        const char* engine_name() const { return k_engine_name; }
        uint32_t engine_version() const { return k_engine_version; }

        uint32_t api_version() const { return VK_API_VERSION_1_1; }

        auto instance_flags() const { return VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; }

        auto enabled_layers() const { return k_layers; }
        auto enabled_extensions() const { return k_extensions; }
    };

    struct device_configurator : sl::vk::core::device_configurator< device_configurator >
    {
        static constexpr std::array k_extensions = {
            "VK_KHR_bind_memory2",
            "VK_KHR_dedicated_allocation",
            "VK_KHR_get_memory_requirements2",
            "VK_KHR_portability_subset",
        };

        /**
         * Required interface for passing this struct to logical_device ctor
         * to be used for device configuration.
         */

        auto enabled_extensions() const { return k_extensions; }
        auto enabled_features() const { return sl::vk::core::k_no_device_features; }
        auto queues() const { return std::make_tuple( -1, 0, 1 ); }
    };


    auto make_input_data( uint32_t count )
    {
        auto rd   = std::random_device {};
        auto rng  = std::mt19937 { rd() };
        auto dist = std::uniform_int_distribution< uint32_t > {
            0, std::numeric_limits< uint16_t >::max() };
        auto gen = [&dist, &rng]() { return dist( rng ); };

        auto data = std::vector< uint32_t >( count );
        std::generate( std::begin( data ), std::end( data ), gen );

        return data;
    }

    void validate_squares( const std::span< const uint32_t > in_data,
                           const std::span< const uint32_t > res_data )
    {
        if ( in_data.size() != res_data.size() )
            throw std::runtime_error( "result data and input data differ in length" );

        uint32_t res = 0;
        for ( size_t i = 0; i < in_data.size(); ++i )
        {
            const auto x = in_data[i];
            const auto y = res_data[i];

            res |= x ^ y;
        }

        if ( res != 0 )
            std::runtime_error( "data results are inconsistent" );
    }

}   // namespace

int main()
{
    auto logger    = sl::logging::logger {};
    auto app_start = std::chrono::high_resolution_clock::now();

    try
    {
        logger.info( "Initializing loader..." );

        auto loader     = sl::vk::core::loader {};
        auto app_config = app_configurator {};

        sl::vk::core::debug::log_diagnostics( logger, loader );
        sl::vk::core::debug::log_diagnostics( logger, app_config );

        logger.info( "Creating an application context..." );
        auto app_context
            = sl::vk::core::make_app_context( loader, app_config, handle_vulkan_debug );
        auto time_to_app_context = std::chrono::high_resolution_clock::now();

        logger.info( "Enumerating GPU devices..." );
        auto gpus = app_context.gpus();
        sl::vk::core::debug::log_diagnostics( logger, gpus );

        // TODO: Select a device based on some scoring algorithm...
        //       For now, just grab the first one.
        const auto& gpu = gpus[0];

        logger.info( "Creating a logical device..." );
        auto device_config = device_configurator {};
        sl::vk::core::debug::log_diagnostics( logger, device_config );

        logger.info( "Setting up compute environment..." );
        auto device  = sl::vk::core::make_logical_device( loader, app_context, gpu, device_config );
        auto mem     = sl::vk::mem::make_allocator( loader, device, app_config.api_version() );
        auto dcache  = sl::vk::core::make_fixed_descriptor_cache( device, dcache_config );
        auto compute = sl::vk::compute::context { device, mem };
        auto time_to_compute_context = std::chrono::high_resolution_clock::now();

        logger.info( "Loading compute program..." );
        auto program = sl::vk::compute::make_program(
            device, sl::io::load_file< uint32_t >( k_square_spv_file ) );
        auto time_to_load_program = std::chrono::high_resolution_clock::now();

        logger.info( "Allocating GPU memory..." );
        auto staging_buf
            = sl::vk::mem::make_staging_buffer( mem, k_number_count * sizeof( uint32_t ) );
        auto in_buf  = sl::vk::mem::make_storage_buffer( mem, k_number_count * sizeof( uint32_t ) );
        auto out_buf = sl::vk::mem::make_storage_buffer( mem, k_number_count * sizeof( uint32_t ) );
        auto result_buf
            = sl::vk::mem::make_readback_buffer( mem, k_number_count * sizeof( uint32_t ) );
        auto time_to_create_buffers = std::chrono::high_resolution_clock::now();

        logger.info( "Preparing random data..." );
        auto in_data                      = make_input_data( k_number_count );
        auto time_to_initialize_test_data = std::chrono::high_resolution_clock::now();

        logger.info( "Copying data into GPU buffers..." );
        compute.copy( std::span { in_data }, staging_buf );
        compute.copy( staging_buf, in_buf );

        logger.info( "Allocate descriptor sets for GPU program..." );
        program.allocate_sets( compute, dcache );

        logger.info( "Update descriptor sets to point at our input and output buffers..." );
        program.bind( compute, 0, 0, in_buf );
        program.bind( compute, 0, 1, out_buf );

        // TODO: Add barrier for compute phase?

        logger.info( "Dispatching program to GPU..." );
        program.execute( compute, k_number_count, 1, 1 );

        logger.info( "Copy data back from GPU buffers..." );
        compute.copy( out_buf, result_buf );

        // Flush commands to GPU and wait for completion
        auto time_to_submit = std::chrono::high_resolution_clock::now();
        compute.flush_and_wait();
        auto time_to_gpu_complete = std::chrono::high_resolution_clock::now();

        // Validate results
        {
            auto view = result_buf.map_view< uint32_t >();
            validate_squares( in_data, view.data() );
        }
        auto time_to_validate = std::chrono::high_resolution_clock::now();

        std::cout << "*** *** App context: "
                  << std::chrono::duration_cast< std::chrono::milliseconds >( time_to_app_context
                                                                              - app_start )
                         .count()
                  << "\n";
        std::cout << "*** *** Compute context: "
                  << std::chrono::duration_cast< std::chrono::milliseconds >(
                         time_to_compute_context - time_to_app_context )
                         .count()
                  << "\n";
        std::cout << "*** *** Load program: "
                  << std::chrono::duration_cast< std::chrono::milliseconds >(
                         time_to_load_program - time_to_compute_context )
                         .count()
                  << "\n";
        std::cout << "*** *** Create buffers: "
                  << std::chrono::duration_cast< std::chrono::milliseconds >(
                         time_to_create_buffers - time_to_load_program )
                         .count()
                  << "\n";
        std::cout << "*** *** Initialize test data: "
                  << std::chrono::duration_cast< std::chrono::milliseconds >(
                         time_to_initialize_test_data - time_to_create_buffers )
                         .count()
                  << "\n";
        std::cout << "*** *** Submit: "
                  << std::chrono::duration_cast< std::chrono::milliseconds >(
                         time_to_submit - time_to_initialize_test_data )
                         .count()
                  << "\n";
        std::cout << "*** *** GPU complete: "
                  << std::chrono::duration_cast< std::chrono::milliseconds >( time_to_gpu_complete
                                                                              - time_to_submit )
                         .count()
                  << "\n";
        std::cout << "*** *** Validated: "
                  << std::chrono::duration_cast< std::chrono::milliseconds >(
                         time_to_validate - time_to_gpu_complete )
                         .count()
                  << "\n";

        logger.info( "Shutting down..." );
    }
    catch ( const sl::vk::spv::error& ex )
    {
        ex.log( logger );
    }
    catch ( const sl::vk::error& ex )
    {
        ex.log( logger );
    }
    catch ( const std::exception& ex )
    {
        logger.error( "*** std::exception *** %s", ex.what() );
    }
    catch ( ... )
    {
        logger.error( "*** UNKNOWN EXCEPTION ***" );
    }

    logger.info( "bye." );
    return 0;
}
