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

#ifndef __GLFW_INIT_H_F95B0630206443609F99163A5E7DD4D0__
#define __GLFW_INIT_H_F95B0630206443609F99163A5E7DD4D0__

#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "./error.h"

namespace sl::app::priv
{

    struct glfw_init
    {
        glfw_init()
        {
            glfwSetErrorCallback( glfw_init::error_callback );
            win::error::throw_if( GLFW_FALSE == glfwInit(), "glfwInit" );
        }

        ~glfw_init() { glfwTerminate(); }

        static auto shared()
        {
            static std::weak_ptr< glfw_init > s_glfw;

            if ( auto sg = s_glfw.lock() )
                return sg;

            // GLFW is not currently initialized. Do it now.
            auto sg = std::make_shared< glfw_init >();
            s_glfw  = sg;

            return sg;
        }

    private:
        static void error_callback( int error, const char* description )
        {
            throw win::error( "<error callback>", description, error );
        }
    };

}   // namespace sl::app::priv

#endif /* __GLFW_INIT_H_F95B0630206443609F99163A5E7DD4D0__ */
