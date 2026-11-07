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

#ifndef RAKS_DT_NIBBLE_PROXY_INCLUDED
#define RAKS_DT_NIBBLE_PROXY_INCLUDED

#include <raks/dt/nibble.hpp>
#include <type_traits>

namespace raks::dt
{
namespace private_proxy
{

template< typename byte_type >
    requires std::is_same_v< byte_type, std::byte > ||
             std::is_same_v< byte_type, std::byte const >
class base
{
public:
    explicit( false ) base( byte_type* const ptr, bool is_high )
    : ptr( ptr )
    , is_high( is_high )
    {}

    [[nodiscard]] bool get_is_high() const { return is_high; }
    [[nodiscard]] byte_type* get_ptr() const { return ptr; }

protected:
    void set_ptr( byte_type* const ptr ) { this->ptr = ptr; }
    void set_is_high( bool const is_high ) { this->is_high = is_high; }

private:
    byte_type* ptr;
    bool is_high;
};

template< typename byte_type >
    requires std::is_same_v< byte_type, std::byte > ||
             std::is_same_v< byte_type, std::byte const >
class base_ptr;

template< typename byte_type >
    requires std::is_same_v< byte_type, std::byte > ||
             std::is_same_v< byte_type, std::byte const >
class base_ref : public base< byte_type >
{
public:
    using base_type = base< byte_type >;
    using value_type = std::conditional_t<
      std::is_const_v< byte_type >,
      raks::dt::nibble const,
      raks::dt::nibble >;

    using pointer = base_ptr< byte_type >;
    using base< byte_type >::base;

    explicit( false ) base_ref( value_type& nibble )
    : base_type( nibble.get_impl_ptr(), false )
    {}

    explicit( false ) operator nibble() const
    {
        return nibble::from_impl(
          base_type::get_is_high() ? *base_type::get_ptr() >> nibble::bit_size
                                   : *base_type::get_ptr() & nibble::bit_mask
        );
    }

    base_ref operator=( raks::dt::nibble rval ) // NOLINT
        requires( !std::is_const_v< byte_type > )
    {
        return set( rval );
    }

    base_ref operator=( base_ref const& rval ) // NOLINT
        requires( !std::is_const_v< byte_type > )
    {
        return set( rval );
    }

    base_ref operator=( base_ref&& rval ) // NOLINT
        requires( !std::is_const_v< byte_type > )
    {
        return set( rval );
    }

    template< typename rhs_byte_type >
        requires std::is_same_v< rhs_byte_type, std::byte > ||
                 std::is_same_v< rhs_byte_type, std::byte const >
    std::strong_ordering operator<=>(
      base_ref< rhs_byte_type > const& rhs
    ) const
    {
        return this->operator raks::dt::nibble() <=>
               static_cast< raks::dt::nibble >( rhs );
    }

    template< typename rhs_byte_type >
        requires std::is_same_v< rhs_byte_type, std::byte > ||
                 std::is_same_v< rhs_byte_type, std::byte const >
    bool operator==( base_ref< rhs_byte_type > const& rhs ) const
    {
        return this->operator raks::dt::nibble() ==
               static_cast< raks::dt::nibble >( rhs );
    }

    template< typename rhs_byte_type >
        requires std::is_same_v< rhs_byte_type, std::byte > ||
                 std::is_same_v< rhs_byte_type, std::byte const >
    bool operator!=( base_ref< rhs_byte_type > const& rhs ) const
    {
        return this->operator raks::dt::nibble() !=
               static_cast< raks::dt::nibble >( rhs );
    }

    pointer operator&() const;

    base_ref( base_ref& ) = default;
    base_ref( base_ref&& ) = default;
    ~base_ref() = default;

private:
    base_ref set( raks::dt::nibble rval )
        requires( !std::is_const_v< byte_type > )
    {
        if( base_type::get_is_high() )
        {
            *base_type::get_ptr() =
              ( *base_type::get_ptr() & nibble::bit_mask ) |
              ( rval.get_impl() << nibble::bit_size );
        }
        else
        {
            *base_type::get_ptr() =
              ( *base_type::get_ptr() & ( ~nibble::bit_mask ) ) |
              rval.get_impl();
        }
        return *this;
    }
};

template< typename byte_type >
    requires std::is_same_v< byte_type, std::byte > ||
             std::is_same_v< byte_type, std::byte const >
class base_ptr : public base< byte_type >
{
public:
    using base_type = base< byte_type >;
    using pointer = base_ptr;

    using element_type = std::conditional_t<
      std::is_const_v< byte_type >,
      raks::dt::nibble const,
      raks::dt::nibble >;

    using difference_type = std::ptrdiff_t;

    using base< byte_type >::base;

    template< typename other_type >
    using rebind = base_ptr;

    static base_ptr pointer_to( base_ref< byte_type > const& ref )
    {
        return base_ptr( ref.get_ptr(), ref.get_is_high() );
    }

    base_ptr const* operator->() const { return this; }
    base_ptr* operator->() { return this; }

    base_ref< byte_type > operator*() const
    {
        return base_ref< byte_type >(
          base_type::get_ptr(), base_type::get_is_high()
        );
    }
    base_ref< byte_type > operator*()
    {
        return base_ref< byte_type >(
          base_type::get_ptr(), base_type::get_is_high()
        );
    }

    template< typename rhs_byte_type >
        requires std::is_same_v< rhs_byte_type, std::byte > ||
                 std::is_same_v< rhs_byte_type, std::byte const >
    friend std::strong_ordering operator<=>(
      base_ptr const lhs, base_ptr< rhs_byte_type > const rhs
    )
    {
        auto compare = lhs.get_ptr() <=> rhs.get_ptr();
        if( 0 != compare )
        {
            return compare;
        }
        return lhs.is_high() <=> rhs.is_high();
    }

    template< typename rhs_byte_type >
        requires std::is_same_v< rhs_byte_type, std::byte > ||
                 std::is_same_v< rhs_byte_type, std::byte const >
    friend bool operator==(
      base_ptr const lhs, base_ptr< rhs_byte_type > const rhs
    )
    {
        return lhs.get_ptr() == rhs.get_ptr() &&
               lhs.get_is_high() == rhs.get_is_high();
    }

    template< typename rhs_byte_type >
        requires std::is_same_v< rhs_byte_type, std::byte > ||
                 std::is_same_v< rhs_byte_type, std::byte const >
    friend bool operator!=(
      base_ptr const lhs, base_ptr< rhs_byte_type > const rhs
    )
    {
        return lhs.get_ptr() != rhs.get_ptr() ||
               lhs.get_is_high() != rhs.get_is_high();
    }
};

template< typename byte_type >
    requires std::is_same_v< byte_type, std::byte > ||
             std::is_same_v< byte_type, std::byte const >
base_ref< byte_type >::pointer base_ref< byte_type >::operator&() const
{
    return base_ref< byte_type >::pointer(
      base_type::get_ptr(), base_type::get_is_high()
    );
}
} // namespace private_proxy
using const_reference = raks::dt::private_proxy::base_ref< std::byte const >;
using const_pointer = raks::dt::private_proxy::base_ptr< std::byte const >;

using reference = raks::dt::private_proxy::base_ref< std::byte >;
using pointer = raks::dt::private_proxy::base_ptr< std::byte >;

} // namespace raks::dt

#endif // RAKS_DT_NIBBLE_PROXY_INCLUDED
