#ifndef RAKS_NIBBLE_ITERATOR_INCLUDED
#define RAKS_NIBBLE_ITERATOR_INCLUDED

#include <concepts>
#include <iterator>
#include <cstddef>

namespace raks::nibble
{
template< typename T >
concept iterator_base =
  std::bidirectional_iterator< T > &&
  ( std::same_as< std::iter_value_t< T >, std::byte > ||
    std::same_as< std::iter_value_t< T >, const std::byte > );
}
#endif // !RAKS_NIBBLE_ITERATOR_INCLUDED
