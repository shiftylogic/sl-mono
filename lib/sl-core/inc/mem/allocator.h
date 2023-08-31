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

#ifndef __MEMORY_ALLOCATOR_H_A9C88781D5F44B93BA58F78A062930D0__
#define __MEMORY_ALLOCATOR_H_A9C88781D5F44B93BA58F78A062930D0__

#include <functional>
#include <memory>

#include <utils/pointers.h>

namespace sl::mem
{

    struct allocator
    {
        struct raw_functions
        {
            std::function< void*( size_t ) > alloc;
            std::function< void*( void*, size_t ) > realloc;
            std::function< void( void* ) > free;
        };

        explicit allocator( raw_functions raw )
            : _mem( raw )
        {}

        /**
         * Raw memory allocations. Returns need to be cast, no management, must free using
         * this same allocator instance.
         **/
        void* alloc( size_t size ) const noexcept { return _mem.alloc( size ); }
        void* realloc( void* ptr, size_t size ) const noexcept { return _mem.realloc( ptr, size ); }
        void free( void* ptr ) const noexcept { _mem.free( ptr ); }

        /**
         * Templated allocators for allocating type instances, and returning raw pointers.
         **/
        template< typename T, typename... Args >
        T* alloc_t( Args... args ) const
        {
            auto mem_free = _mem.free;

            // Allocate and store the object into a custom std::unique_ptr
            auto sp = std::unique_ptr< void, decltype( mem_free ) >( _mem.alloc( sizeof( T ) ),
                                                                     mem_free );

            // To have the ctor run, we need to perform a placement new here.
            // If it throws, the unique_ptr should clean up the memory still.
            new ( sp.get() ) T( std::forward< Args >( args )... );

            // Don't forget to release pointer from unique_ptr so it stays alive
            return static_cast< T* >( sp.release() );
        }

        /**
         * Memory deletion for templated allocator above.
         **/
        template< typename T >
        void free_t( T* t ) const
        {
            // Call destructor directly on T first
            t->~T();

            // Now we can free the memory
            _mem.free( t );
        }

        /**
         * Templated allocation of typed values with "custom" unique_ptr returned for de-allocation.
         **/
        template< typename T, typename... Args >
        auto alloc_sp( Args... args ) const
        {
            // The custom deleter for this std::unique_ptr will need to properly
            // destruct object before freeing memory
            auto mem_free = _mem.free;
            auto deleter  = [mem_free]( T* t ) {
                // Call destructor directly on T first
                t->~T();

                // Now we can free the memory
                mem_free( t );
            };

            // Allocate and store the object into a custom std::unique_ptr
            auto sp = std::unique_ptr< void, decltype( mem_free ) >( _mem.alloc( sizeof( T ) ),
                                                                     mem_free );

            // To have the ctor run, we need to perform a placement new here.
            T* ptr = new ( sp.get() ) T( std::forward< Args >( args )... );

            // Steal the pointer into a new unique_ptr and release the temporary one.
            auto res = std::unique_ptr< T, decltype( deleter ) >( ptr, deleter );
            sp.release();

            return res;
        }

    private:
        raw_functions _mem;
    };

}   // namespace sl::mem

#endif /* __MEMORY_ALLOCATOR_H_A9C88781D5F44B93BA58F78A062930D0__ */
