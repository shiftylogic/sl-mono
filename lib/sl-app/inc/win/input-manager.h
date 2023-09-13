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

#ifndef __INPUT_MANAGER_H_994C5C95A406409E96735A2D4A40F0B8__
#define __INPUT_MANAGER_H_994C5C95A406409E96735A2D4A40F0B8__

#include <array>
#include <functional>
#include <span>

#include <ankerl/unordered_dense.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace sl::app
{

    using key_binding_fn  = std::function< void() >;
    using mouse_button_fn = std::function< void() >;
    using mouse_move_fn   = std::function< void( double, double ) >;
    using mouse_scroll_fn = std::function< void( double, double ) >;

    enum class input_action : int
    {
        down = GLFW_PRESS,
        up   = GLFW_RELEASE,
        hold = GLFW_REPEAT,
    };

    enum class mouse_button : int
    {
        left   = GLFW_MOUSE_BUTTON_LEFT,
        middle = GLFW_MOUSE_BUTTON_MIDDLE,
        right  = GLFW_MOUSE_BUTTON_RIGHT,
    };

    enum class input_modifier : int
    {
        none    = 0,
        shift   = GLFW_MOD_SHIFT,
        control = GLFW_MOD_CONTROL,
        alt     = GLFW_MOD_ALT,
        meta    = GLFW_MOD_SUPER,
    };


    namespace priv
    {
        struct noop
        {
            template< typename... A >
            void operator()( A... )
            {}
        };

        static const char* action_string( int action )
        {
            switch ( action )
            {
            case static_cast< int >( input_action::down ):
                return "PRESS";
            case static_cast< int >( input_action::up ):
                return "RELEASE";
            case static_cast< int >( input_action::hold ):
                return "REPEAT";
            default:
                return "UNKNOWN";
            }
        }

        static const char* mods_string( std::span< char, 8 > buf, int mods )
        {
            size_t i = 0;

            buf[i++] = ( mods & GLFW_MOD_SHIFT ) ? 'S' : '-';
            buf[i++] = ( mods & GLFW_MOD_CONTROL ) ? 'c' : '-';
            buf[i++] = ( mods & GLFW_MOD_ALT ) ? 'A' : '-';
            buf[i++] = ( mods & GLFW_MOD_SUPER ) ? 'M' : '-';
            buf[i++] = ( mods & GLFW_MOD_CAPS_LOCK ) ? 'L' : '-';
            buf[i++] = ( mods & GLFW_MOD_NUM_LOCK ) ? 'N' : '-';
            buf[i++] = '\0';

            return buf.data();
        }

    }   // namespace priv


    template< typename Logger >
    struct input_manager
    {
    public:
        explicit input_manager( Logger& logger )
            : _logger { logger }
        {}

        void bind( int key, input_action action, input_modifier mods, key_binding_fn fn )
        {
            [[maybe_unused]] std::array< char, 8 > buf;

            uint64_t binding_key = static_cast< uint64_t >( action )
                                   | ( static_cast< uint64_t >( mods ) << 16 )
                                   | ( static_cast< uint64_t >( key ) << 32 );

            if ( _key_bindings.contains( binding_key ) )
            {
                _logger.warn( "Binding replaced [key: %i, mods: %s, action: %s]",
                              key,
                              priv::mods_string( buf, static_cast< int >( mods ) ),
                              priv::action_string( static_cast< int >( action ) ) );
            }

            _logger.info( "Binding key [key: %i, mods: %s, action: %s]",
                          key,
                          priv::mods_string( buf, static_cast< int >( mods ) ),
                          priv::action_string( static_cast< int >( action ) ) );
            _key_bindings[binding_key] = fn;
        }

        void bind( mouse_button btn, input_action action, input_modifier mods, mouse_button_fn fn )
        {
            [[maybe_unused]] std::array< char, 8 > buf;

            uint64_t binding_key = static_cast< uint64_t >( action )
                                   | ( static_cast< uint64_t >( mods ) << 16 )
                                   | ( static_cast< uint64_t >( btn ) );

            if ( _mouse_bindings.contains( binding_key ) )
            {
                _logger.warn( "Binding replaced [button: %i, mods: %s, action: %s]",
                              btn,
                              priv::mods_string( buf, static_cast< int >( mods ) ),
                              priv::action_string( static_cast< int >( action ) ) );
            }

            _logger.info( "Binding mouse click [button: %i, mods: %s, action: %s]",
                          btn,
                          priv::mods_string( buf, static_cast< int >( mods ) ),
                          priv::action_string( static_cast< int >( action ) ) );
            _mouse_bindings[binding_key] = fn;
        }

        void bind_mouse_move( mouse_move_fn fn ) { _mouse_move = fn; }

        void bind_mouse_scroll( mouse_scroll_fn fn ) { _scroll = fn; }


        /////////////////////////////////////
        //
        // Callbacks for the input system

        void on_key( int key, [[maybe_unused]] int code, int action, int mods )
        {
            [[maybe_unused]] std::array< char, 8 > buf;
            _logger.info( "Key pressed: key<%i>, code<%i>, action<%s>, mods<%s>",
                          key,
                          code,
                          priv::action_string( action ),
                          priv::mods_string( buf, mods ) );

            uint64_t binding_key = static_cast< uint64_t >( action )
                                   | ( static_cast< uint64_t >( mods ) << 16 )
                                   | ( static_cast< uint64_t >( key ) << 32 );
            auto entry = _key_bindings.find( binding_key );
            if ( entry == std::end( _key_bindings ) )
                return;

            _logger.info( "  >>>> binding match" );
            entry->second();
        }

        void on_mouse_click( int button, int action, int mods )
        {
            [[maybe_unused]] std::array< char, 8 > buf;
            _logger.info( "Mouse pressed: button<%i>, action<%s>, mods<%s>",
                          button,
                          priv::action_string( action ),
                          priv::mods_string( buf, mods ) );

            uint64_t binding_key = static_cast< uint64_t >( action )
                                   | ( static_cast< uint64_t >( mods ) << 16 )
                                   | ( static_cast< uint64_t >( button ) );
            auto entry = _mouse_bindings.find( binding_key );
            if ( entry == std::end( _mouse_bindings ) )
                return;

            _logger.info( "  >>>> binding match" );
            entry->second();
        }

        void on_mouse_move( double cx, double cy )
        {
            // _logger.trace( "Mouse moved (x: %lf, y: %lf)", cx, cy );
            _mouse_move( cx - _mouse_x, cy - _mouse_y );
            _mouse_x = cx;
            _mouse_y = cy;
        }

        void on_mouse_scroll( double xoff, double yoff )
        {
            // _logger.trace( "Mouse scrolled (x: %lf, y: %lf)", xoff, yoff );
            _scroll( xoff, yoff );
        }

    private:
        Logger& _logger;

        ankerl::unordered_dense::map< uint64_t, key_binding_fn > _key_bindings;
        ankerl::unordered_dense::map< uint64_t, mouse_button_fn > _mouse_bindings;
        mouse_move_fn _mouse_move { priv::noop {} };
        mouse_scroll_fn _scroll { priv::noop {} };

        double _mouse_x;
        double _mouse_y;
    };

    constexpr input_modifier operator|( const input_modifier v1, const input_modifier v2 )
    {
        return static_cast< input_modifier >( static_cast< int >( v1 ) | static_cast< int >( v2 ) );
    }

}   // namespace sl::app

#endif /* __INPUT_MANAGER_H_994C5C95A406409E96735A2D4A40F0B8__ */
