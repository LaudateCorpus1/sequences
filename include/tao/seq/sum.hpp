// Copyright (c) 2015-2018 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/sequences/

#ifndef TAOCPP_SEQUENCES_INCLUDE_SUM_HPP
#define TAOCPP_SEQUENCES_INCLUDE_SUM_HPP

#include <type_traits>
#include <utility>

#include "config.hpp"
#include "integer_sequence.hpp"

#ifndef TAOCPP_FOLD_EXPRESSIONS
#include "make_integer_sequence.hpp"
#include <cstddef>
#endif

namespace tao
{
   namespace seq
   {

#ifdef TAOCPP_FOLD_EXPRESSIONS

      template< typename T, T... Ns >
      struct sum
         : std::integral_constant< T, ( Ns + ... + T( 0 ) ) >
      {
      };

#else

      namespace impl
      {
         template< std::size_t, std::size_t N >
         struct chars
         {
            char dummy[ N + 1 ];
         };

         template< typename, std::size_t... >
         struct collector;

         template< std::size_t... Is, std::size_t... Ns >
         struct collector< index_sequence< Is... >, Ns... >
            : chars< Is, Ns >...
         {
         };

         template< bool, std::size_t N, typename T, T... Ns >
         struct sum
         {
            using Is = make_index_sequence< N >;
            using P = collector< Is, ( ( Ns > 0 ) ? Ns : 0 )... >;
            using M = collector< Is, ( ( Ns < 0 ) ? -Ns : 0 )... >;
            using type = std::integral_constant< T, T( sizeof( P ) ) - T( sizeof( M ) ) >;
         };

         template< std::size_t N, typename T, T... Ns >
         struct sum< true, N, T, Ns... >
         {
            using Is = make_index_sequence< N >;
            using C = collector< Is, Ns... >;
            using type = std::integral_constant< T, T( sizeof( C ) - N ) >;
         };
      }

      template< typename T, T... Ns >
      struct sum
         : impl::sum< std::is_unsigned< T >::value, sizeof...( Ns ) + 1, T, Ns..., 0 >::type
      {
      };

#endif

      template< typename T, T... Ns >
      struct sum< integer_sequence< T, Ns... > >
         : sum< T, Ns... >
      {
      };
   }
}

#endif  // TAOCPP_SEQUENCES_INCLUDE_SUM_HPP
