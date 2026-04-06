#include <cstddef>
#include <raks/nibble/iterator.hpp>
#include <gtest/gtest.h>

namespace
{
TEST( NibbleTest, FromCharacters )
{
    EXPECT_EQ( ( std::byte )0x02, raks::nibble::nibble( '2' ).get_impl() );
    EXPECT_EQ( ( std::byte )0x0c, raks::nibble::nibble( 'C' ).get_impl() );
    EXPECT_EQ( ( std::byte )0x0d, raks::nibble::nibble( '=' ).get_impl() );
}

TEST( NibbleTest, FromInteger )
{
    EXPECT_EQ( ( std::byte )0x05, raks::nibble::nibble( 135 ).get_impl() );
    EXPECT_EQ( ( std::byte )0x09, raks::nibble::nibble( 2009 ).get_impl() );
}

TEST( NibbleRefTest, RValue )
{
    auto testval = std::byte( 0 );
    auto ref1 = raks::nibble::iterator_impl::ref( &testval, false );
    ref1 = raks::nibble::nibble( 'A' );
    EXPECT_EQ( ( std::byte )0x0a, ( ( raks::nibble::nibble )ref1 ).get_impl() );

    auto ref2 = raks::nibble::iterator_impl::ref( &testval, true );
    ref2 = raks::nibble::nibble( '5' );
    EXPECT_EQ( ( std::byte )0x05, ( ( raks::nibble::nibble )ref2 ).get_impl() );
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
