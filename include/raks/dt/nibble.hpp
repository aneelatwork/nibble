// mit license
//
// copyright (c) 2019 aneelatwork
//
// permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "software"), to deal
// in the software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the software, and to permit persons to whom the software is
// furnished to do so, subject to the following conditions:
//
// the above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the software.
//
// the software is provided "as is", without warranty of any kind, express or
// implied, including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement. in no event shall the
// authors or copyright holders be liable for any claim, damages or other
// liability, whether in an action of contract, tort or otherwise, arising from,
// out of or in connection with the software or the use or other dealings in the
// software.

#ifndef RAKS_DT_NIBBLE_INCLUDED
#define RAKS_DT_NIBBLE_INCLUDED

#include <compare>
#include <cstddef>
#include <type_traits>

namespace raks::dt
{

class nibble
{
public:
    static constexpr int bit_size = 4;
    static constexpr int int_base = 10;
    static constexpr std::byte const bit_mask =
      static_cast< std::byte >( 0x0f );

    static nibble from_impl( std::byte impl ) { return nibble( impl ); }
    static ptrdiff_t byte_position( ptrdiff_t nibble_position )
    {
        return ( ( nibble_position - 1 ) / 2 ) + 1;
    }
    explicit nibble( unsigned long number );
    explicit nibble( char hex );

    [[nodiscard]] std::byte get_impl() const { return value; }
    explicit operator char() const;
    explicit operator unsigned long() const
    {
        return static_cast< unsigned long >( value );
    }
    auto* get_impl_ptr( this auto&& self )
        requires( !std::is_rvalue_reference_v< decltype( self ) > )
    {
        return &self.value;
    }

    nibble() = default;
    nibble( nibble const& ) = default;
    nibble( nibble&& ) = default;
    nibble& operator=( nibble const& ) = default;
    nibble& operator=( nibble&& ) = default;
    ~nibble() = default;

    friend std::strong_ordering operator<=>(
      nibble const& lhs, nibble const& rhs
    )
    {
        return lhs.get_impl() <=> rhs.get_impl();
    }

    friend bool operator==( nibble const& lhs, nibble const& rhs )
    {
        return lhs.get_impl() == rhs.get_impl();
    }

    friend bool operator!=( nibble const& lhs, nibble const& rhs )
    {
        return lhs.get_impl() != rhs.get_impl();
    }

private:
    explicit nibble( std::byte impl )
    : value( impl )
    {}

    std::byte value;
};

} // namespace raks::dt

#endif // !RAKS_DT_NIBBLE_INCLUDED
