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

#include <iterator>
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

    template< typename nibble_type >
    explicit( false ) ref( nibble_type& nibble )
    : ptr( nibble.get_impl_ptr() )
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
    bool get_is_high() { return is_high; }

    byte_type* get_ptr() { return ptr; }

private:
    byte_type* const ptr; // NOLINT
    bool const is_high;   // NOLINT
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

    nibble set( nibble rval )
    {
        if( get_is_high() )
        {
            *get_ptr() = ( *get_ptr() & nibble::bit_mask ) |
                         ( rval.get_impl() << nibble::bit_size );
        }
        else
        {
            *get_ptr() =
              ( *get_ptr() & ( ~nibble::bit_mask ) ) | rval.get_impl();
        }
        return rval;
    }

    nibble operator=( nibble rval ) { return set( rval ); }
};

template< typename byte_type >
    requires std::is_same_v< byte_type, std::byte > ||
             std::is_same_v< byte_type, std::byte const >
class iterator
{
public:
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = nibble;
    using difference_type = std::ptrdiff_t;
    using pointer = byte_type*;
    using reference =
      std::conditional_t< std::is_const_v< byte_type >, nibble, reference >;

    iterator( byte_type* const ptr, bool const is_high )
    : ptr( ptr )
    , is_high( is_high )
    {}

    reference operator*() { return reference( ptr, is_high ); }

    reference operator[]( std::ptrdiff_t const index )
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
        change( value );
        return *this;
    }

    iterator operator-=( std::ptrdiff_t value )
    {
        change( -value );
        return *this;
    }

    iterator( iterator const& ) = default;
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
        ptr += ( ( value + direction ) / 2 ) + direction; // NOLINT
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

    iterator_impl::reference operator*() // NOLINT
    {
        return iterator_impl::reference{ get_ptr(), get_is_high() };
    }

    iterator_impl::reference operator[]( std::ptrdiff_t const index ) // NOLINT
    {
        auto copy = *this;
        copy.change( index );
        return *copy;
    }
};

} // namespace raks::nibble

#endif // RAKS_NIBBLE_ITERATOR_INCLUDED
