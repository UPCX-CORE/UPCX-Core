#pragma once

#include "map_macro.h"
#include <type_traits>

namespace upcx { namespace reflection {

   template <typename T>
   struct has_for_each_field {
    private:
      struct F {
         template <typename A, typename B>
         void operator()(const A&, const B&);
      };

      template <typename C>
      static char test(decltype(upcx_for_each_field((C*)nullptr, std::declval<F>()))*);

      template <typename C>
      static long test(...);

    public:
      static constexpr bool value = sizeof(test<T>((void*)nullptr)) == sizeof(char);
   };

   template <typename T>
   inline constexpr bool has_for_each_field_v = has_for_each_field<T>::value;

#define UPCX_REFLECT_MEMBER(STRUCT, FIELD)                                                                            \
   f(#FIELD, [](auto p) -> decltype(&std::decay_t<decltype(*p)>::FIELD) { return &std::decay_t<decltype(*p)>::FIELD; });

#define UPCX_REFLECT_STRIP_BASEbase
#define UPCX_REFLECT_BASE(STRUCT, BASE)                                                                               \
   static_assert(std::is_base_of_v<UPCX_REFLECT_STRIP_BASE##BASE, STRUCT>, #BASE " is not a base class of " #STRUCT); \
   upcx_for_each_field((UPCX_REFLECT_STRIP_BASE##BASE*)nullptr, f);

#define UPCX_REFLECT_SIGNATURE(STRUCT, ...)                                                                           \
   [[maybe_unused]] inline constexpr const char* get_type_name(STRUCT*) { return #STRUCT; }                                      \
   template <typename F>                                                                                               \
   constexpr void upcx_for_each_field(STRUCT*, F f)

/**
 * UPCX_REFLECT(<struct>, <member or base spec>...)
 * Each parameter should be either the keyword 'base' followed by a base class of the struct or
 * an identifier which names a non-static data member of the struct.
 */
#define UPCX_REFLECT(...)                                                                                             \
   UPCX_REFLECT_SIGNATURE(__VA_ARGS__) { UPCX_MAP_REUSE_ARG0(UPCX_REFLECT_INTERNAL, __VA_ARGS__) }

// Identity the keyword 'base' followed by at least one token
#define UPCX_REFLECT_SELECT_I(a, b, c, d, ...) UPCX_REFLECT_##d
#define UPCX_REFLECT_IS_BASE() ~, ~
#define UPCX_REFLECT_IS_BASE_TESTbase ~, UPCX_REFLECT_IS_BASE

#define UPCX_APPLY(m, x) m x
#define UPCX_CAT(x, y) x##y
#define UPCX_REFLECT_INTERNAL(STRUCT, FIELD)                                                                          \
   UPCX_APPLY(UPCX_REFLECT_SELECT_I, (UPCX_CAT(UPCX_REFLECT_IS_BASE_TEST, FIELD()), MEMBER, BASE, MEMBER))         \
   (STRUCT, FIELD)

}} // namespace upcx::reflection
