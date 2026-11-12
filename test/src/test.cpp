#include <raks/nibble/proxy.hpp>

#include <array>
#include <cstddef>
#include <gtest/gtest.h>

namespace
{
TEST( NibbleTest, FromCharacters )
{
    auto nb2 = raks::dt::nibble( '2' );
    auto nbc = raks::dt::nibble( 'C' );
    auto nbd = raks::dt::nibble( 'd' );
    auto nbq = raks::dt::nibble( '=' );

    EXPECT_EQ( ( std::byte )0x02, nb2.get_impl() );
    EXPECT_EQ( ( std::byte )0x0c, nbc.get_impl() );
    EXPECT_EQ( ( std::byte )0x0d, nbd.get_impl() );
    EXPECT_EQ( ( std::byte )0x0d, nbq.get_impl() );
}

TEST( NibbleTest, ToCharacters )
{
    auto nb3 = raks::dt::nibble( '3' );
    auto nbf = raks::dt::nibble( 'f' );
    auto nba = raks::dt::nibble( 'a' );
    auto nbq = raks::dt::nibble( '=' );

    EXPECT_EQ( '3', static_cast< char >( nb3 ) );
    EXPECT_EQ( 'F', static_cast< char >( nbf ) );
    EXPECT_EQ( 'A', static_cast< char >( nba ) );
    EXPECT_EQ( 'D', static_cast< char >( nbq ) );
}

TEST( NibbleTest, FromInteger )
{
    auto nb5 = raks::dt::nibble{ 135UL };  // NOLINT
    auto nb9 = raks::dt::nibble{ 2009UL }; // NOLINT
    EXPECT_EQ( ( std::byte )0x05, nb5.get_impl() );
    EXPECT_EQ( ( std::byte )0x09, nb9.get_impl() );
}

TEST( NibbleTest, ToInteger )
{
    auto nb7 = raks::dt::nibble{ 43527UL }; // NOLINT
    auto nb4 = raks::dt::nibble{ 294UL };   // NOLINT
    EXPECT_EQ( 7UL, static_cast< unsigned long >( nb7 ) );
    EXPECT_EQ( 4UL, static_cast< unsigned long >( nb4 ) );
}

TEST( NibbleTest, Equality )
{
    auto cnb6 = raks::dt::nibble( '6' );
    auto cnba = raks::dt::nibble( 'A' );
    auto cnbd = raks::dt::nibble( 'D' );
    auto cnbq = raks::dt::nibble( '=' );

    auto inb5 = raks::dt::nibble{ 135UL }; // NOLINT
    auto inb6 = raks::dt::nibble{ 6UL };   // NOLINT

    EXPECT_TRUE( cnba == cnba );
    EXPECT_FALSE( cnba != cnba );

    EXPECT_TRUE( inb5 == inb5 );
    EXPECT_FALSE( inb5 != inb5 );

    EXPECT_TRUE( cnb6 == inb6 );
    EXPECT_FALSE( cnb6 != inb6 );

    EXPECT_TRUE( cnbd != cnba );

    EXPECT_TRUE( cnbd == cnbq );
    EXPECT_FALSE( cnbd != cnbq );

    EXPECT_TRUE( cnba != inb6 );
    EXPECT_FALSE( cnbd == inb6 );
}

TEST( NibbleRefTest, RValue )
{
    auto testval = std::array< std::byte, 2 >();
    auto ref1 = raks::nibble::reference( &testval[ 0 ], false ); // NOLINT
    auto ref2 = raks::nibble::reference( &testval[ 0 ], true );  // NOLINT
    auto ref3 = raks::nibble::reference( &testval[ 1 ], false ); // NOLINT
    auto ref4 = raks::nibble::reference( &testval[ 1 ], true );  // NOLINT
    ref1 = raks::dt::nibble( 'A' );
    ref2 = raks::dt::nibble( '5' );
    ref3 = raks::dt::nibble( 'E' );
    ref4 = raks::dt::nibble( '9' );

    EXPECT_EQ( ( std::byte )0x5a, testval[ 0 ] ); // NOLINT
    EXPECT_EQ( ( std::byte )0x9e, testval[ 1 ] ); // NOLINT
    EXPECT_EQ( ( std::byte )0x0a, ( ( raks::dt::nibble )ref1 ).get_impl() );
    EXPECT_EQ( ( std::byte )0x05, ( ( raks::dt::nibble )ref2 ).get_impl() );
    EXPECT_EQ( ( std::byte )0x0e, ( ( raks::dt::nibble )ref3 ).get_impl() );
    EXPECT_EQ( ( std::byte )0x09, ( ( raks::dt::nibble )ref4 ).get_impl() );
}

// TODO
TEST( NibbleRefTest, NibbleEquality )
{
    auto testval = std::byte( 0xc6 );                            // NOLINT
    auto ref1 = raks::nibble::reference( &testval, false );      // NOLINT
    auto ref2 = raks::nibble::const_reference( &testval, true ); // NOLINT

    auto nib1 = raks::dt::nibble( 'C' );
    auto nib2 = raks::dt::nibble( 6UL ); // NOLINT

    EXPECT_TRUE( nib1 == ref2 );
    EXPECT_TRUE( ref2 == nib1 );
    EXPECT_FALSE( nib1 != ref2 );
    EXPECT_FALSE( ref2 != nib1 );

    EXPECT_TRUE( nib2 == ref1 );
    EXPECT_TRUE( ref1 == nib2 );
    EXPECT_FALSE( nib2 != ref1 );
    EXPECT_FALSE( ref1 != nib2 );

    EXPECT_TRUE( nib1 != ref1 );
    EXPECT_TRUE( ref1 != nib1 );
    EXPECT_FALSE( nib1 == ref1 );
    EXPECT_FALSE( ref1 == nib1 );

    EXPECT_TRUE( nib2 != ref2 );
    EXPECT_TRUE( ref2 != nib2 );
    EXPECT_FALSE( nib2 == ref2 );
    EXPECT_FALSE( ref2 == nib2 );
}

TEST( NibbleRefTest, NibbleRefEquality )
{
    auto testval = std::array< std::byte, 2 >{
      {
        std::byte( 0x2e ), // NOLINT
        std::byte( 0xe2 ), // NOLINT
      },
    };

    auto ref1 = raks::nibble::reference( &testval[ 0 ], false );      // NOLINT
    auto ref2 = raks::nibble::const_reference( &testval[ 0 ], true ); // NOLINT
    auto ref3 = raks::nibble::reference( &testval[ 1 ], false );      // NOLINT
    auto ref4 = raks::nibble::const_reference( &testval[ 1 ], true ); // NOLINT

    EXPECT_TRUE( ref1 == ref1 );
    EXPECT_FALSE( ref4 != ref4 );

    EXPECT_TRUE( ref1 == ref4 );
    EXPECT_TRUE( ref4 == ref1 );
    EXPECT_FALSE( ref1 != ref4 );
    EXPECT_FALSE( ref4 != ref1 );

    EXPECT_TRUE( ref3 == ref3 );
    EXPECT_FALSE( ref2 != ref2 );

    EXPECT_TRUE( ref2 == ref3 );
    EXPECT_TRUE( ref3 == ref2 );
    EXPECT_FALSE( ref2 != ref3 );
    EXPECT_FALSE( ref3 != ref2 );
}

TEST( NibblePtrTest, Rvalue )
{
    auto testval = std::array< std::byte, 2 >();
    auto ptr1 = raks::nibble::pointer( &testval[ 0 ], false ); // NOLINT
    auto ptr2 = raks::nibble::pointer( &testval[ 0 ], true );  // NOLINT
    auto ptr3 = raks::nibble::pointer( &testval[ 1 ], false ); // NOLINT
    auto ptr4 = raks::nibble::pointer( &testval[ 1 ], true );  // NOLINT
    *ptr1 = raks::dt::nibble( '4' );
    *ptr2 = raks::dt::nibble( 'b' );
    *ptr3 = raks::dt::nibble( 'F' );
    *ptr4 = raks::dt::nibble( '1' );

    EXPECT_EQ( ( std::byte )0xb4, testval[ 0 ] ); // NOLINT
    EXPECT_EQ( ( std::byte )0x1f, testval[ 1 ] ); // NOLINT
    EXPECT_EQ(
      ( std::byte )0x04, ( ( raks::dt::nibble )( *ptr1 ) ).get_impl()
    );
    EXPECT_EQ(
      ( std::byte )0x0b, ( ( raks::dt::nibble )( *ptr2 ) ).get_impl()
    );
    EXPECT_EQ(
      ( std::byte )0x0f, ( ( raks::dt::nibble )( *ptr3 ) ).get_impl()
    );
    EXPECT_EQ(
      ( std::byte )0x01, ( ( raks::dt::nibble )( *ptr4 ) ).get_impl()
    );
}

TEST( NibblePtrTest, NibbleEquality )
{
    auto testval = std::byte( 0xc6 );                          // NOLINT
    auto ptr1 = raks::nibble::pointer( &testval, false );      // NOLINT
    auto ptr2 = raks::nibble::const_pointer( &testval, true ); // NOLINT

    auto nib1 = raks::dt::nibble( 'C' );
    auto nib2 = raks::dt::nibble( 6UL ); // NOLINT

    EXPECT_TRUE( nib1 == *ptr2 );
    EXPECT_TRUE( *ptr2 == nib1 );
    EXPECT_FALSE( nib1 != *ptr2 );
    EXPECT_FALSE( *ptr2 != nib1 );

    EXPECT_TRUE( nib2 == *ptr1 );
    EXPECT_TRUE( *ptr1 == nib2 );
    EXPECT_FALSE( nib2 != *ptr1 );
    EXPECT_FALSE( *ptr1 != nib2 );

    EXPECT_TRUE( nib1 != *ptr1 );
    EXPECT_TRUE( *ptr1 != nib1 );
    EXPECT_FALSE( nib1 == *ptr1 );
    EXPECT_FALSE( *ptr1 == nib1 );

    EXPECT_TRUE( nib2 != *ptr2 );
    EXPECT_TRUE( *ptr2 != nib2 );
    EXPECT_FALSE( nib2 == *ptr2 );
    EXPECT_FALSE( *ptr2 == nib2 );
}

TEST( NibblePtrTest, NibbleRefEquality )
{
    auto testval = std::array< std::byte, 2 >{
      {
        std::byte( 0x2e ), // NOLINT
        std::byte( 0xe2 ), // NOLINT
      },
    };

    auto ptr1 = raks::nibble::pointer( &testval[ 0 ], false );      // NOLINT
    auto ptr2 = raks::nibble::const_pointer( &testval[ 0 ], true ); // NOLINT
    auto ptr3 = raks::nibble::pointer( &testval[ 1 ], false );      // NOLINT
    auto ptr4 = raks::nibble::const_pointer( &testval[ 1 ], true ); // NOLINT

    EXPECT_TRUE( *ptr1 == *ptr1 );
    EXPECT_FALSE( *ptr4 != *ptr4 );

    EXPECT_TRUE( *ptr1 == *ptr4 );
    EXPECT_TRUE( *ptr4 == *ptr1 );
    EXPECT_FALSE( *ptr1 != *ptr4 );
    EXPECT_FALSE( *ptr4 != *ptr1 );

    EXPECT_TRUE( *ptr3 == *ptr3 );
    EXPECT_FALSE( *ptr2 != *ptr2 );

    EXPECT_TRUE( *ptr2 == *ptr3 );
    EXPECT_TRUE( *ptr3 == *ptr2 );
    EXPECT_FALSE( *ptr2 != *ptr3 );
    EXPECT_FALSE( *ptr3 != *ptr2 );
}

TEST( NibblePtrTest, NibblePtrEquality )
{
    auto testval = std::byte( 0xc6 ); // NOLINT

    auto nptr1 = raks::nibble::pointer( &testval, false );       // NOLINT
    auto nptr2 = raks::nibble::pointer( &testval, false );       // NOLINT
    auto cptr1 = raks::nibble::const_pointer( &testval, false ); // NOLINT
    auto cptr2 = raks::nibble::const_pointer( &testval, false ); // NOLINT
    auto nptr3 = raks::nibble::pointer( &testval, true );        // NOLINT
    auto cptr3 = raks::nibble::const_pointer( &testval, true );  // NOLINT

    EXPECT_TRUE( nptr1 == nptr1 );
    EXPECT_TRUE( cptr2 == cptr2 );

    EXPECT_FALSE( nptr3 != nptr3 );
    EXPECT_FALSE( cptr3 != cptr3 );

    EXPECT_TRUE( nptr1 == nptr2 );
    EXPECT_TRUE( nptr1 == cptr1 );
    EXPECT_TRUE( cptr2 == nptr2 );
    EXPECT_TRUE( cptr2 == cptr1 );

    EXPECT_FALSE( nptr2 != nptr1 );
    EXPECT_FALSE( nptr2 != cptr1 );
    EXPECT_FALSE( cptr2 != nptr1 );
    EXPECT_FALSE( cptr1 != nptr2 );

    EXPECT_TRUE( nptr3 != nptr2 );
    EXPECT_TRUE( nptr3 != cptr1 );
    EXPECT_TRUE( cptr2 != nptr3 );
    EXPECT_TRUE( cptr3 != cptr2 );

    EXPECT_FALSE( nptr2 == nptr3 );
    EXPECT_FALSE( nptr3 == cptr1 );
    EXPECT_FALSE( cptr2 == nptr3 );
    EXPECT_FALSE( cptr3 == cptr1 );
}

} // namespace

int main( int argc, char** argv )
{
    // Initialize Google Test
    testing::InitGoogleTest( &argc, argv );

    // (Optional) Add global test environments here, before RUN_ALL_TESTS() is
    // called testing::AddGlobalTestEnvironment(new MyEnvironment);

    // Run all tests and return the result
    return RUN_ALL_TESTS(); // Must use the return value for proper test
                            // reporting
}
