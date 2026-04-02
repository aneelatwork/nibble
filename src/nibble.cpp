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

#include "raks/dtype/nibble.hpp"
#include <cstddef>
namespace
{
std::byte encode( char hex )
{
    int const ten = 10;
    if( '0' <= hex && '9' >= hex )
    {
        return static_cast< std::byte >( hex - '0' );
    }
    if( 'a' <= hex && 'f' >= hex )
    {
        return static_cast< std::byte >( hex - 'a' + ten );
    }
    if( 'A' <= hex && 'F' >= hex )
    {
        return static_cast< std::byte >( hex - 'A' + ten );
    }
    return static_cast< std::byte >( hex ) & raks::dtype::nibble::bit_mask;
}

} // namespace

namespace raks::dtype
{

nibble::nibble( char hex )
: value( encode( hex ) )
{}

nibble::nibble( int number )
: value( static_cast< std::byte >( number % int_base ) )
{}

} // namespace raks::dtype
