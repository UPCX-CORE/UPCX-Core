#pragma once

#include "map_macro.h"
#include <type_traits>

namespace upcxio { namespace reflection {

   template <typename T>
   struct has_for_each_field {
    private:
      struct F {
         template <typename A, typename B>
         void operator()(const A&, const B&);
      };

      template <typename C>
      static char test(decltype(upcxio_for_each_field((C*)nullptr, std::declval<F>()))*);

      template <typename C>
      static long test(...);

    public:
      static constexpr bool value = sizeof(test<T>((void*)nullptr)) == sizeof(char);
   };

   template <typename T>
   inline constexpr bool has_for_each_field_v = has_for_each_field<T>::value;

#define UPCXIO_REFLECT_MEMBER(STRUCT, FIELD)                                                                            \
   f(#FIELD, [](auto p) -> decltype(&std::decay_t<decltype(*p)>::FIELD) { return &std::decay_t<decltype(*p)>::FIELD; });

#define UPCXIO_REFLECT_STRIP_BASEbase
#define UPCXIO_REFLECT_BASE(STRUCT, BASE)                                                                               \
   static_assert(std::is_base_of_v<UPCXIO_REFLECT_STRIP_BASE##BASE, STRUCT>, #BASE " is not a base class of " #STRUCT); \
   upcxio_for_each_field((UPCXIO_REFLECT_STRIP_BASE##BASE*)nullptr, f);

#define UPCXIO_REFLECT_SIGNATURE(STRUCT, ...)                                                                           \
   [[maybe_unused]] inline constexpr const char* get_type_name(STRUCT*) { return #STRUCT; }                                      \
   template <typename F>                                                                                               \
   constexpr void upcxio_for_each_field(STRUCT*, F f)

/**
 * UPCXIO_REFLECT(<struct>, <member or base spec>...)
 * Each parameter should be either the keyword 'base' followed by a base class of the struct or
 * an identifier which names a non-static data member of the struct.
 */
#define UPCXIO_REFLECT(...)                                                                                             \
   UPCXIO_REFLECT_SIGNATURE(__VA_ARGS__) { UPCXIO_MAP_REUSE_ARG0(UPCXIO_REFLECT_INTERNAL, __VA_ARGS__) }

// Identity the keyword 'base' followed by at least one token
#define UPCXIO_REFLECT_SELECT_I(a, b, c, d, ...) UPCXIO_REFLECT_##d
#define UPCXIO_REFLECT_IS_BASE() ~, ~
#define UPCXIO_REFLECT_IS_BASE_TESTbase ~, UPCXIO_REFLECT_IS_BASE

#define UPCXIO_APPLY(m, x) m x
#define UPCXIO_CAT(x, y) x##y
#define UPCXIO_REFLECT_INTERNAL(STRUCT, FIELD)                                                                          \
   UPCXIO_APPLY(UPCXIO_REFLECT_SELECT_I, (UPCXIO_CAT(UPCXIO_REFLECT_IS_BASE_TEST, FIELD()), MEMBER, BASE, MEMBER))         \
   (STRUCT, FIELD)

}} // namespace upcxio::reflection
