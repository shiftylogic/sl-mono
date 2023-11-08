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

#ifndef __ERROR_H_628B2BE3C81B426388ABB336865CDAC0__
#define __ERROR_H_628B2BE3C81B426388ABB336865CDAC0__

#include <stdexcept>

#include <spirv/spirv_reflect.h>

namespace sl::vk::spv
{

    namespace priv
    {

        const char* lookup_error( SpvReflectResult code )
        {
            switch ( code )
            {
            case SPV_REFLECT_RESULT_SUCCESS:
                return "success";
            case SPV_REFLECT_RESULT_NOT_READY:
                return "not ready";
            case SPV_REFLECT_RESULT_ERROR_PARSE_FAILED:
                return "parse failed";
            case SPV_REFLECT_RESULT_ERROR_ALLOC_FAILED:
                return "memory allocation failed";
            case SPV_REFLECT_RESULT_ERROR_RANGE_EXCEEDED:
                return "out of range";
            case SPV_REFLECT_RESULT_ERROR_NULL_POINTER:
                return "null pointer";
            case SPV_REFLECT_RESULT_ERROR_INTERNAL_ERROR:
                return "internal error";
            case SPV_REFLECT_RESULT_ERROR_COUNT_MISMATCH:
                return "count mismatch";
            case SPV_REFLECT_RESULT_ERROR_ELEMENT_NOT_FOUND:
                return "element not found";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_CODE_SIZE:
                return "invalid code size";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_MAGIC_NUMBER:
                return "invalid magic number";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_EOF:
                return "unexpected EOF";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ID_REFERENCE:
                return "invalid id reference";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_SET_NUMBER_OVERFLOW:
                return "set number overflow";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_STORAGE_CLASS:
                return "invalid storage class";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_RECURSION:
                return "recursion error";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_INSTRUCTION:
                return "invalid instruction";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_UNEXPECTED_BLOCK_DATA:
                return "unexpected block data";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_BLOCK_MEMBER_REFERENCE:
                return "invalid block member reference";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_ENTRY_POINT:
                return "invalid entry point";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_INVALID_EXECUTION_MODE:
                return "invalid execution mode";
            case SPV_REFLECT_RESULT_ERROR_SPIRV_MAX_RECURSIVE_EXCEEDED:
                return "maximum recursive exceeded";
            }
        }

    }   // namespace priv

    struct error : public std::runtime_error
    {
        error( const char* api, SpvReflectResult code, const char* message )
            : std::runtime_error { message }
            , _api { api }
            , _code { code }
        {}

        template< typename logger_t >
        void log( logger_t& logger ) const
        {
            logger.error( "*** SPV Reflect Error *** '%s' failed: %s (Code: %d) - %s",
                          _api,
                          spv::priv::lookup_error( _code ),
                          _code,
                          what() );
        }

        static void throw_if_error( const char* api, SpvReflectResult result, const char* message )
        {
            if ( result != SPV_REFLECT_RESULT_SUCCESS )
                throw spv::error { api, result, message };
        }

    private:
        const char* _api;
        SpvReflectResult _code;
    };

}   // namespace sl::vk::spv

#endif /* __ERROR_H_628B2BE3C81B426388ABB336865CDAC0__ */
