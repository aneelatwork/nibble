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

#ifndef RAKS_NIBBLE_VECTOR_INCLUDED
#define RAKS_NIBBLE_VECTOR_INCLUDED

#include <cstddef>
#include <iterator>
#include <raks/nibble/iterator.hpp>
#include <vector>

namespace raks::nibble
{

struct bcd_size
{
    unsigned size;
    [[nodiscard]] unsigned get_byte_size() const
    {
        return ( ( size - 1 ) / 2 ) + 1;
    }
};

class bcd
{
public:
    using const_reference = raks::nibble::nibble;
    using reference = raks::nibble::iterator_impl::reference;
    using const_iterator = raks::nibble::const_iterator;
    using iterator = raks::nibble::iterator;

    bcd() = default;

    iterator begin() { return { impl.data(), false }; }

    iterator end()
    {
        return { impl.data() + impl.size(), 0 != ( impl.size() % 2 ) };
    }

    [[nodiscard]] auto rbegin() { return std::make_reverse_iterator( end() ); }

    [[nodiscard]] auto rend() { return std::make_reverse_iterator( begin() ); }

    [[nodiscard]] auto rbegin() const
    {
        return std::make_reverse_iterator( cend() );
    }

    [[nodiscard]] auto rend() const
    {
        return std::make_reverse_iterator( cbegin() );
    }

    [[nodiscard]] const_iterator cbegin() const
    {
        return { impl.data(), false };
    }

    [[nodiscard]] const_iterator cend() const
    {
        return { impl.data() + impl.size(), 0 != ( impl.size() % 2 ) };
    }

    [[nodiscard]] const_iterator begin() const { return cbegin(); }

    [[nodiscard]] const_iterator end() const { return cend(); }

    [[nodiscard]] auto crbegin() const
    {
        return std::make_reverse_iterator( cend() );
    }

    [[nodiscard]] auto crend() const
    {
        return std::make_reverse_iterator( cbegin() );
    }

    bcd( unsigned long value, bcd_size size )
    : impl( size.get_byte_size() )
    , is_high( true )
    {
        for( auto ni = rbegin(); ni != rend(); ni++ )
        {
            *ni = raks::nibble::nibble( value );
        }
    }

    reference at( std::ptrdiff_t const index )
    {
        return {
          impl.data() + raks::nibble::nibble::byte_position( index ),
          0 != index % 2
        };
    }

    [[nodiscard]] const_reference at( std::ptrdiff_t const index ) const
    {
        return static_cast< const_reference >(
          raks::nibble::iterator_impl::const_reference{
            impl.data() + raks::nibble::nibble::byte_position( index ),
            0 != index % 2,
          }
        );
    }

    reference operator[]( std::ptrdiff_t const index ) { return at( index ); }

    const_reference operator[]( std::ptrdiff_t const index ) const
    {
        return at( index );
    }

    void push_back( nibble nibble )
    {
        if( is_high )
        {
            impl.push_back( nibble.get_impl() );
        }
        else
        {
            std::byte back =
              impl.back() | ( nibble.get_impl() << nibble::bit_size );
        }
        is_high = !is_high;
    }

    bcd_size size()
    {
        return bcd_size{
          ( static_cast< unsigned >( impl.size() ) * 2 ) - ( is_high ? 0 : 1 ),
        };
    }

private:
    std::vector< std::byte > impl;
    bool is_high;
};
} // namespace raks::nibble

#endif // !RAKS_NIBBLE_VECTOR_INCLUDED
