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

#ifndef RAKS_NIBBLE_BCD_INCLUDED
#define RAKS_NIBBLE_BCD_INCLUDED

#include <raks/dt/nibble_proxy.hpp>

#include <vector>

namespace raks::nibble
{
namespace private_bcd_iterator
{
template< typename byte_type >
    requires std::is_same_v< byte_type, std::byte > ||
             std::is_same_v< byte_type, std::byte const >
class iterator : public raks::nibble::private_proxy::base< byte_type >
{
public:
    using base_type = raks::nibble::private_proxy::base< byte_type >;
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = nibble;
    using difference_type = std::ptrdiff_t;
    using pointer = std::
      conditional_t< std::is_const_v< byte_type >, const_pointer, pointer >;
    using reference = std::
      conditional_t< std::is_const_v< byte_type >, const_reference, reference >;

    using base_type::base;

    reference operator*()
    {
        return reference( base_type::get_ptr(), base_type::get_is_high() );
    }

    explicit( false ) operator iterator< std::byte const >() const
        requires( !std::is_const_v< byte_type > )
    {
        return iterator< std::byte const >{
          base_type::get_ptr(), base_type::get_is_high()
        };
    }

    reference operator[]( std::ptrdiff_t const index )
    {
        auto copy = *this;
        copy.change( index );
        return *copy;
    }

    iterator operator++()
    {
        auto result =
          iterator{ base_type::get_ptr(), base_type::get_is_high() };
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
        auto result =
          iterator{ base_type::get_ptr(), base_type::get_is_high() };
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

    template< typename rhs_byte_type >
        requires std::is_same_v< rhs_byte_type, std::byte > ||
                 std::is_same_v< rhs_byte_type, std::byte const >
    friend std::strong_ordering operator<=>(
      iterator const& lhs, iterator< rhs_byte_type > const& rhs
    )
    {
        auto compare = lhs.get_ptr() <=> rhs.get_ptr();
        if( 0 != compare )
        {
            return compare;
        }
        return lhs.get_is_high() <=> rhs.get_is_high();
    }

    template< typename rhs_byte_type >
        requires std::is_same_v< rhs_byte_type, std::byte > ||
                 std::is_same_v< rhs_byte_type, std::byte const >
    friend bool operator==(
      iterator const& lhs, iterator< rhs_byte_type > const& rhs
    )
    {
        return lhs.get_ptr() == rhs.get_ptr() &&
               lhs.get_is_high() == rhs.get_is_high();
    }

    template< typename rhs_byte_type >
        requires std::is_same_v< rhs_byte_type, std::byte > ||
                 std::is_same_v< rhs_byte_type, std::byte const >
    friend bool operator!=(
      iterator const& lhs, iterator< rhs_byte_type > const& rhs
    )
    {
        return lhs.get_ptr() != rhs.get_ptr() ||
               lhs.get_is_high() != rhs.get_is_high();
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
        if( base_type::get_is_high() )
        {
            ++value;
        }
        base_type::set_ptr(
          base_type::get_ptr() + ( ( value + direction ) / 2 ) +
          direction // NOLINT
        );
        base_type::set_is_high( 0 != ( value % 2 ) );
    }

    void reset( std::byte* ptr, bool is_high )
    {
        base_type::set_ptr( ptr );
        base_type::set_is_high( is_high );
    }
};

} // namespace private_bcd_iterator

struct bcd_size
{
public:
    static bcd_size make_from_byte_size(
      unsigned byte_size, bool const is_high
    )
    {
        return bcd_size{
          ( byte_size << 1 ) - static_cast< unsigned >( is_high ),
        };
    }
    unsigned size;
    [[nodiscard]] unsigned get_byte_size() const
    {
        return ( ( size - 1 ) / 2 ) + 1;
    }
};

class bcd
{
public:
    using const_iterator =
      raks::nibble::private_bcd_iterator::iterator< std::byte const >;
    using iterator = raks::nibble::private_bcd_iterator::iterator< std::byte >;
    using const_reference = const_iterator::reference;
    using reference = iterator::reference;

    bcd( bcd_size size )
    : impl( size.get_byte_size() )
    , is_high( false )
    {}

    iterator begin() { return { impl.data(), false }; }

    iterator end()
    {
        return { impl.data() + impl.size(), 0 != ( impl.size() % 2 ) };
    }

    // [[nodiscard]] auto rbegin() { return std::make_reverse_iterator( end() );
    // }
    //
    // [[nodiscard]] auto rend() { return std::make_reverse_iterator( begin() );
    // }
    //
    // [[nodiscard]] auto rbegin() const
    // {
    //     return std::make_reverse_iterator( cend() );
    // }
    //
    // [[nodiscard]] auto rend() const
    // {
    //     return std::make_reverse_iterator( cbegin() );
    // }
    //
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

    // [[nodiscard]] auto crbegin() const
    // {
    //     return std::make_reverse_iterator( cend() );
    // }
    //
    // [[nodiscard]] auto crend() const
    // {
    //     return std::make_reverse_iterator( cbegin() );
    // }

    reference at( std::ptrdiff_t const index )
    {
        return {
          impl.data() + raks::nibble::nibble::byte_position( index ),
          0 != index % 2
        };
    }

    [[nodiscard]] const_reference at( std::ptrdiff_t const index ) const
    {
        return {
          impl.data() + raks::nibble::nibble::byte_position( index ),
          0 != index % 2
        };
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

// bcd make( unsigned long value, bcd_size const size )
// {
//     bcd bcd{ size };
//     for( auto bi = bcd.begin(); bi != bcd.end(); bi++ )
//     {
//         *bi = nibble( value );
//         value /= nibble::int_base;
//     }
//
//     // for( auto ni: std::views::reverse( bcd ) )
//     // {
//     //     ni = raks::nibble::nibble( value );
//     // }
// }
//
} // namespace raks::nibble

#endif // !RAKS_NIBBLE_BCD_INCLUDED
