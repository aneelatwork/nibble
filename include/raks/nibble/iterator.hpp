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

#ifndef __RAKS_NIBBLE_ITERATOR_DEFINED__
#define __RAKS_NIBBLE_ITERATOR_DEFINED__

#include <raks/nibble/nibble.hpp>
#include <cstddef>
#include <type_traits>
namespace raks::nibble
{
namespace iterator_impl
{

class ref
{
public:
    explicit( false ) ref( nibble& ref )
    : ptr( ref.get_impl_ptr() )
    , is_high( false )
    {}

    explicit( false ) ref( std::byte* const ptr, bool is_high )
    : ptr( ptr )
    , is_high( is_high )
    {}

    explicit( false ) operator nibble()
    {
        return nibble::from_impl(
          is_high ? *ptr >> nibble::bit_size : *ptr & nibble::bit_mask
        );
    }

    nibble operator=( nibble rval )
    {
        if( is_high )
        {
            *ptr = ( *ptr & nibble::bit_mask ) |
                   ( rval.get_impl() << nibble::bit_size );
        }
        else
        {
            *ptr = ( *ptr & ( ~nibble::bit_mask ) ) | ( rval.get_impl() );
        }
        return rval;
    }

    ref( ref& ) = default;
    ref( ref&& ) = default;
    ref& operator=( ref const& ) = delete;
    ref& operator=( ref&& ) = delete;
    ~ref() = default;

private:
    std::byte* const ptr;
    bool const is_high;
};

template< typename nibble_type >
    requires std::is_same_v< nibble_type, raks::nibble::nibble > ||
             std::is_same_v< nibble_type, raks::nibble::nibble const >
class iterator
{
public:
    iterator( std::byte* const ptr, bool is_high )
    : ptr( ptr )
    , is_high( is_high )
    {}

    nibble operator*() { return ( nibble )ref{ ptr, is_high }; }
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
    std::byte* get_ptr() { return ptr; }

private:
    std::byte* ptr;
    bool is_high;
};
} // namespace iterator_impl

using const_iterator = iterator_impl::iterator< nibble const >;

class iterator : public iterator_impl::iterator< nibble >
{
public:
    explicit( false ) operator const_iterator()
    {
        return const_iterator{ get_ptr(), get_is_high() };
    }

    iterator_impl::ref operator*()
    {
        return iterator_impl::ref{ get_ptr(), get_is_high() };
    }

    iterator_impl::ref operator[]( std::ptrdiff_t const index )
    {
        auto copy = *this;
        copy.change( index );
        return *copy;
    }
};

} // namespace raks::nibble

#endif //__RAKS_NIBBLE_ITERATOR_DEFINED__
