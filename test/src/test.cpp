#include <cstddef>
#include <raks/dtype/nibble.hpp>
#include <gtest/gtest.h>

namespace
{
TEST( NibbleTest, FromCharacters )
{
    EXPECT_EQ( ( std::byte )0x02, raks::dtype::nibble::lower( '2' ).get_impl() );
    EXPECT_EQ( ( std::byte )0xc0, raks::dtype::nibble::upper( 'C' ).get_impl() );
    EXPECT_EQ( ( std::byte )0x5a, raks::dtype::nibble::pair( raks::dtype::nibble::upper( '5' ), raks::dtype::nibble::lower( 'A' ) ).get_impl() );
}
} // namespace

int main( int argc, char** argv )
{
    // Initialize Google Test
    testing::InitGoogleTest( &argc, argv );

    // (Optional) Add global test environments here, before RUN_ALL_TESTS() is called
    // testing::AddGlobalTestEnvironment(new MyEnvironment);

    // Run all tests and return the result
    return RUN_ALL_TESTS(); // Must use the return value for proper test reporting
}
