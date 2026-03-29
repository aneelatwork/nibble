#include <raks/dtype/nibble.hpp>

namespace nibble = raks::dtype::nibble;
int main()
{
    auto lower = nibble::lower( 'C' );
    auto upper = nibble::lower( '3' );
    auto pair = nibble::pair( upper, lower );
}
