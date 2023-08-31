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

#ifndef __HANDLE_H_E5F9CEC25E0B4425A67C092E22D94942__
#define __HANDLE_H_E5F9CEC25E0B4425A67C092E22D94942__

#include <uv.h>

#include <utils/pointers.h>

namespace sl::uv
{

    template< typename HandleType >
    class handle
    {
    public:
        explicit handle()
            : _handle { new HandleType }
        {
            _handle->data = this;
        }

        operator HandleType*() const noexcept { return _handle.get(); }

        void close() { _handle.reset(); }

        template< typename T >
        static T* self( HandleType* handle )
        {
            return static_cast< T* >( handle->data );
        }


    private:
        static void close_internal( HandleType* h )
        {
            ::uv_close( reinterpret_cast< uv_handle_t* >( h ), &handle::on_closed );
        }

        static void on_closed( uv_handle_t* h )
        {
            // Handle closing is asynchronous. When it is complete, then we can delete
            // the underlying type
            delete reinterpret_cast< HandleType* >( h );
        }

    protected:
        sl::utils::custom_unique_ptr< HandleType, handle::close_internal > _handle;
    };

}   // namespace sl::uv

#endif /* __HANDLE_H_E5F9CEC25E0B4425A67C092E22D94942__ */
