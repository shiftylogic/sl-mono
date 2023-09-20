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

#ifndef __RESOURCE_H_DAC04CE03FD445BDA3C7EACB1B3B6651__
#define __RESOURCE_H_DAC04CE03FD445BDA3C7EACB1B3B6651__

#include <functional>

namespace sl::vk::core
{

    template< typename handle_t, typename context_t = void >
    struct resource
    {
        using deleter_fn = std::function< void( handle_t, context_t* ) >;

        resource( const resource& )            = delete;
        resource& operator=( const resource& ) = delete;

        resource& operator=( resource&& r ) noexcept
        {
            if ( this != &r )
            {
                if ( _handle != nullptr )
                    _deleter( _handle, _context );

                _handle  = r._handle;
                _deleter = r._deleter;
                _context = r._context;

                r._handle  = nullptr;
                r._context = nullptr;
            }

            return *this;
        }

        resource() noexcept
            : _handle { nullptr }
            , _context { nullptr }
        {}

        resource( resource&& r ) noexcept
            : _handle { r._handle }
            , _deleter { r._deleter }
            , _context { r._context }
        {
            r._handle  = nullptr;
            r._context = nullptr;
        }

        explicit resource( handle_t handle, deleter_fn deleter, context_t* context = nullptr )
            : _handle { handle }
            , _deleter { deleter }
            , _context { context }
        {}

        virtual ~resource() noexcept
        {
            if ( nullptr != _handle )
                _deleter( _handle, _context );
        }

        operator handle_t() const noexcept { return _handle; }

        explicit operator bool() const noexcept { return _handle; }

        handle_t get() const { return _handle; }

    private:
        handle_t _handle;
        deleter_fn _deleter;
        context_t* _context;
    };

}   // namespace sl::vk::core

#endif /* __RESOURCE_H_DAC04CE03FD445BDA3C7EACB1B3B6651__ */
