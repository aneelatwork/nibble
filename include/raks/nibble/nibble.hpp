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

#ifndef __RAKS_NIBBLE_NIBBLE_DEFINED__
#define __RAKS_NIBBLE_NIBBLE_DEFINED__

#include <cstddef>
#include <utility>

namespace raks::nibble
{

class nibble
{
public:
    static constexpr int bit_size = 4;
    static constexpr int int_base = 10;
    static constexpr std::byte const bit_mask = ( std::byte )0x0f;

    static nibble from_impl( std::byte impl ) { return nibble( impl ); }
    explicit nibble( char hex );
    explicit nibble( int number );
    [[nodiscard]] std::byte get_impl() const { return value; }
    explicit operator char() const;
    explicit operator int() const;

    template< typename self_type >
    auto get_impl_ptr( this self_type&& self ) -> decltype( auto )
    {
        return &std::forward_like< decltype( self ) >( self.value );
    }

private:
    explicit nibble( std::byte impl )
    : value( impl )
    {}

    std::byte value;
};

} // namespace raks::nibble

#endif
