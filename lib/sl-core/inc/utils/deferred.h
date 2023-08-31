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

#ifndef __DEFERRED_H_4E075915165242F882C9A69B8CAC2660__
#define __DEFERRED_H_4E075915165242F882C9A69B8CAC2660__

namespace sl::utils
{

    /**
     *
     * This struct acts like the go-lang "defer" construct. It allows a lambda
     * to execute as the enclosing scope is destructing the object.
     *
     * Ex.
     *  {
     *      auto p = new char[10];
     *      sl::utils::deferred _( [&p]() { delete[] p; } );
     *
     *      // Do things with p
     *      // pointer 'p' is released as this scope exits.
     *  }
     *
     **/
    template< typename Fn >
    struct deferred
    {
        deferred( Fn fn )
            : _fn( fn )
        {}

        ~deferred() { _fn(); }

    private:
        Fn _fn;
    };

}   // namespace sl::utils

#endif /* __DEFERRED_H_4E075915165242F882C9A69B8CAC2660__ */
