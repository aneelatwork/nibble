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

#ifndef RAKS_NIBBLE_ITERATOR_INCLUDED
#define RAKS_NIBBLE_ITERATOR_INCLUDED

#include <raks/nibble/nibble.hpp>
#include <cstddef>
#include <type_traits>
namespace raks::nibble
{
namespace ref_impl
{
template< typename byte_type >
    requires std::is_same_v< byte_type, std::byte > ||
             std::is_same_v< byte_type, std::byte const >
class ref
{
public:
    using impl_ptr_type = byte_type*;

    explicit( false ) ref( nibble& nibble_ref )
    : ptr( nibble_ref.get_impl_ptr() )
    , is_high( false )
    {}

    explicit( false ) ref( byte_type* const ptr, bool is_high )
    : ptr( ptr )
    , is_high( is_high )
    {}

    explicit( false ) operator nibble()
    {
        return nibble::from_impl(
          is_high ? *ptr >> nibble::bit_size : *ptr & nibble::bit_mask
        );
    }

    ref( ref& ) = default;
    ref( ref&& ) = default;
    ref& operator=( ref const& ) = delete;
    ref& operator=( ref&& ) = delete;
    ~ref() = default;

protected:
    byte_type* const ptr;
    bool const is_high;
};

} // namespace ref_impl

namespace iterator_impl
{
using const_reference = raks::nibble::ref_impl::ref< std::byte const >;

class reference : public raks::nibble::ref_impl::ref< std::byte >
{
public:
    using ref_impl::ref< std::byte >::ref;

    explicit( false ) reference( nibble& nibble_ref )
    : raks::nibble::ref_impl::ref< std::byte >( nibble_ref )
    {}

    nibble operator=( nibble rval )
    {
        if( is_high )
        {
            *ptr = ( *ptr & nibble::bit_mask ) |
                   ( rval.get_impl() << nibble::bit_size );
        }
        else
        {
            *ptr = ( *ptr & ( ~nibble::bit_mask ) ) | rval.get_impl();
        }
        return rval;
    }
};

template< typename byte_type >
    requires std::is_same_v< byte_type, std::byte > ||
             std::is_same_v< byte_type, std::byte const >
class iterator
{
public:
    iterator( byte_type* const ptr, bool const is_high )
    : ptr( ptr )
    , is_high( is_high )
    {}

    nibble operator*()
    {
        return static_cast< nibble >(
          raks::nibble::iterator_impl::const_reference{ ptr, is_high }
        );
    }
    nibble operator[]( std::ptrdiff_t const index )
    {
        auto copy = *this;
        copy.change( index );
        return *copy;
    }

    iterator operator++()
    {
        auto result = iterator{ ptr, is_high };
        change( 1 );
        return result;
    }

    iterator operator++( int )
    {
        change( 1 );
        return *this;
    }

    iterator operator--()
    {
        auto result = iterator{ ptr, is_high };
        change( -1 );
        return result;
    }

    iterator operator--( int )
    {
        change( -1 );
        return *this;
    }

    iterator operator+=( std::ptrdiff_t value )
    {
        auto result = iterator{ ptr, is_high };
        change( value );
        return result;
    }

    iterator operator-=( std::ptrdiff_t value )
    {
        change( -value );
        return *this;
    }

    iterator( iterator& ) = default;
    iterator( iterator&& ) = default;
    iterator& operator=( iterator const& ) = default;
    iterator& operator=( iterator&& ) = default;
    ~iterator() = default;

protected:
    constexpr void change( std::ptrdiff_t value )
    {
        int direction = value >= 0 ? 1 : -1;
        if( is_high )
        {
            ++value;
        }
        ptr += ( ( value + direction ) / 2 ) + direction;
        is_high = 0 != ( value % 2 );
    }

    void reset( std::byte* ptr, bool is_high )
    {
        this->ptr = ptr;
        this->is_high = is_high;
    }

    bool get_is_high() { return is_high; }
    byte_type* get_ptr() { return ptr; }

private:
    byte_type* ptr;
    bool is_high;
};
} // namespace iterator_impl

using const_iterator = iterator_impl::iterator< std::byte const >;

class iterator : public iterator_impl::iterator< std::byte >
{
private:
    using base_type = iterator_impl::iterator< std::byte >;

public:
    using base_type::base_type;

    explicit( false ) operator const_iterator()
    {
        return const_iterator{ get_ptr(), get_is_high() };
    }

    iterator_impl::reference operator*()
    {
        return iterator_impl::reference{ get_ptr(), get_is_high() };
    }

    iterator_impl::reference operator[]( std::ptrdiff_t const index )
    {
        auto copy = *this;
        copy.change( index );
        return *copy;
    }
};

} // namespace raks::nibble

#endif // RAKS_NIBBLE_ITERATOR_INCLUDED
