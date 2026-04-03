#include <cstddef>
#include <raks/dtype/nibble_ref.hpp>
#include <gtest/gtest.h>

namespace
{
TEST( NibbleTest, FromCharacters )
{
    EXPECT_EQ( ( std::byte )0x02, raks::dtype::nibble( '2' ).get_impl() );
    EXPECT_EQ( ( std::byte )0x0c, raks::dtype::nibble( 'C' ).get_impl() );
    EXPECT_EQ( ( std::byte )0x0d, raks::dtype::nibble( '=' ).get_impl() );
}

TEST( NibbleTest, FromInteger )
{
    EXPECT_EQ( ( std::byte )0x05, raks::dtype::nibble( 135 ).get_impl() );
    EXPECT_EQ( ( std::byte )0x09, raks::dtype::nibble( 2009 ).get_impl() );
}

TEST( NibbleRefTest, RValue )
{
    auto testval = std::byte( 0 );
    auto ref1 = raks::dtype::nibble_ref( &testval, false );
    ref1 = raks::dtype::nibble( 'A' );
    EXPECT_EQ( ( std::byte )0x0a, ( ( raks::dtype::nibble )ref1 ).get_impl() );

    auto ref2 = raks::dtype::nibble_ref( &testval, true );
    ref2 = raks::dtype::nibble( '5' );
    EXPECT_EQ( ( std::byte )0x05, ( ( raks::dtype::nibble )ref2 ).get_impl() );
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
