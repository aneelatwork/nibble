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

#include <raks/nibble/iterator.hpp>
#include <vector>

template<>
class std::vector< raks::nibble::nibble > : public std::vector< std::byte >
{
public:
    using base_type = std::vector< std::byte >;
    using const_reference = raks::nibble::nibble;
    using reference = raks::nibble::iterator_impl::reference;
    using const_iterator = raks::nibble::const_iterator;
    using iterator = raks::nibble::iterator;
    using base_type::base_type;

    iterator begin() { return { base_type::data(), false }; }

    iterator end()
    {
        return {
          base_type::data() + base_type::size(), 0 != ( base_type::size() % 2 )
        };
    }

    [[nodiscard]] const_iterator cbegin() const
    {
        return { base_type::data(), false };
    }

    [[nodiscard]] const_iterator cend() const
    {
        return {
          base_type::data() + base_type::size(), 0 != ( base_type::size() % 2 )
        };
    }

    [[nodiscard]] const_iterator begin() const { return cbegin(); }

    [[nodiscard]] const_iterator end() const { return cend(); }

    reference at( std::ptrdiff_t const index )
    {
        return {
          base_type::data() + raks::nibble::nibble::byte_position( index ),
          0 != index % 2
        };
    }

    [[nodiscard]] const_reference at( std::ptrdiff_t const index ) const
    {
        return static_cast< const_reference >(
          raks::nibble::iterator_impl::const_reference{
            base_type::data() + raks::nibble::nibble::byte_position( index ),
            0 != index % 2,
          }
        );
    }

    reference operator[]( std::ptrdiff_t const index ) { return at( index ); }

    const_reference operator[]( std::ptrdiff_t const index ) const
    {
        return at( index );
    }

private:
};

#endif // !RAKS_NIBBLE_VECTOR_INCLUDED
