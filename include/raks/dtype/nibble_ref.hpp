// MIT License
//
// Copyright (c) 2019 aneelatwork
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <raks/dtype/nibble.hpp>
namespace raks::dtype
{
class nibble_ref
{
public:
    explicit( false ) nibble_ref( nibble& ref )
    : ptr( ref.get_impl_ptr() )
    , isHigh( false )
    {}

    explicit( false ) nibble_ref( std::byte* const ptr, bool isHigh )
    : ptr( ptr )
    , isHigh( isHigh )
    {}

    explicit( false ) operator nibble()
    {
        return nibble::from_impl(
          isHigh ? *ptr >> nibble::bit_size : *ptr & nibble::bit_mask
        );
    }

    nibble_ref& operator=( nibble const& rval )
    {
        if( isHigh )
        {
            *ptr = ( *ptr & nibble::bit_mask ) |
                   ( rval.get_impl() << nibble::bit_size );
        }
        else
        {
            *ptr = ( *ptr & ( ~nibble::bit_mask ) ) | ( rval.get_impl() );
        }
        return *this;
    }

    nibble_ref( nibble_ref& ) = delete;
    nibble_ref( nibble_ref&& ) = delete;
    nibble_ref& operator=( nibble_ref& ) = delete;
    nibble_ref& operator=( nibble_ref&& ) = delete;
    ~nibble_ref() = default;

private:
    std::byte* const ptr;
    bool const isHigh;
};

} // namespace raks::dtype
