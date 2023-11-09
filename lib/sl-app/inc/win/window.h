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

#ifndef __WINDOW_H_50C2D9F14E07461B86A4D800AC5C97FA__
#define __WINDOW_H_50C2D9F14E07461B86A4D800AC5C97FA__

#include "./error.h"
#include "./glfw-init.h"

namespace sl::app
{

    template< typename InputHandler >
    struct window
    {
        using self = window< InputHandler >;

        struct config
        {
            /* Required fields */
            const char* title;
            int width;
            int height;

            /*
             * Optional (with defaults)
             */
            bool decorated { true };
            bool fullscreen { false };
            bool resizable { true };
            bool windowed { true };
        };

        window( InputHandler& input, config cfg )
            : _glfw { priv::glfw_init::shared() }
            , _input { input }
            , _height { cfg.height }
            , _width { cfg.width }
            , _window { nullptr }
        {
            /*
             * Create the window instance first
             */
            glfwWindowHint( GLFW_DECORATED, cfg.decorated ? GLFW_TRUE : GLFW_FALSE );
            glfwWindowHint( GLFW_RESIZABLE, cfg.resizable ? GLFW_TRUE : GLFW_FALSE );
            glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

            // The "No API" hint makes Vulkan possible (we don't want OpenGL).
            glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            if ( cfg.windowed && cfg.fullscreen )
            {
                const GLFWvidmode* vmode = glfwGetVideoMode( monitor );
                _width                   = vmode->width;
                _height                  = vmode->height;
            }

            _window = glfwCreateWindow( _width,
                                        _height,
                                        cfg.title,
                                        cfg.fullscreen && !cfg.windowed ? monitor : nullptr,
                                        nullptr );
            win::error::throw_if( _window == nullptr, "glfwCreateWindow" );


            /*
             * Set the window up for handling callbacks.
             */
            glfwSetWindowUserPointer( _window, this );
            glfwSetFramebufferSizeCallback( _window, &self::on_resize );
            glfwSetKeyCallback( _window, &self::on_key );
            glfwSetMouseButtonCallback( _window, &self::on_mouse_click );
            glfwSetCursorPosCallback( _window, &self::on_mouse_move );
            glfwSetScrollCallback( _window, &self::on_mouse_scroll );
        }

        ~window() noexcept
        {
            if ( _window != nullptr )
                glfwDestroyWindow( _window );
        }

        bool poll() const
        {
            if ( glfwWindowShouldClose( _window ) )
                return false;

            glfwPollEvents();
            return true;
        }

    private:
        static void on_key( GLFWwindow* window, int key, int code, int action, int mods )
        {
            auto gw = reinterpret_cast< self* >( glfwGetWindowUserPointer( window ) );
            gw->_input.on_key( key, code, action, mods );
        }

        static void on_mouse_click( GLFWwindow* window, int button, int action, int mods )
        {
            auto gw = reinterpret_cast< self* >( glfwGetWindowUserPointer( window ) );
            gw->_input.on_mouse_click( button, action, mods );
        }

        static void on_mouse_move( GLFWwindow* window, double px, double py )
        {
            auto gw = reinterpret_cast< self* >( glfwGetWindowUserPointer( window ) );
            gw->_input.on_mouse_move( px, py );
        }

        static void on_mouse_scroll( GLFWwindow* window, double px, double py )
        {
            auto gw = reinterpret_cast< self* >( glfwGetWindowUserPointer( window ) );
            gw->_input.on_mouse_scroll( px, py );
        }

        static void on_resize( GLFWwindow* window, int width, int height )
        {
            auto gw     = reinterpret_cast< self* >( glfwGetWindowUserPointer( window ) );
            gw->_width  = width;
            gw->_height = height;
        }

    private:
        std::shared_ptr< priv::glfw_init > _glfw;
        InputHandler& _input;
        int _height;
        int _width;
        GLFWwindow* _window;
    };

}   // namespace sl::app

#endif /* __WINDOW_H_50C2D9F14E07461B86A4D800AC5C97FA__ */
