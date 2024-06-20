#pragma once

#include "for_each_field.hpp"

namespace upcx { namespace operators {

// Defines comparison operators for a reflected struct
#define UPCX_COMPARE(...)                                                                                             \
   auto                      upcx_enable_comparison(const __VA_ARGS__&)->bool;                                        \
   using ::upcx::operators::operator==;                                                                               \
   using ::upcx::operators::operator!=;                                                                               \
   using ::upcx::operators::operator<;                                                                                \
   using ::upcx::operators::operator>;                                                                                \
   using ::upcx::operators::operator<=;                                                                               \
   using ::upcx::operators::operator>=;                                                                               \
   using ::upcx::operators::upcx_compare

   template <typename T>
   constexpr auto operator==(const T& lhs, const T& rhs) -> decltype(upcx_enable_comparison(lhs)) {
      bool result = true;
      for_each_field<T>([&](const char*, auto&& member) { result = result && (member(&lhs) == member(&rhs)); });
      return result;
   }
   template <typename T>
   constexpr auto operator!=(const T& lhs, const T& rhs) -> decltype(upcx_enable_comparison(lhs)) {
      return !(lhs == rhs);
   }

   namespace internal_use_do_not_use {
      // This is a worse match than the user-visible overload
      template <typename T, typename U>
      constexpr int upcx_compare(const T& lhs, const U& rhs) {
         if (lhs < rhs)
            return -1;
         else if (rhs < lhs)
            return 1;
         else
            return 0;
      }
   } // namespace internal_use_do_not_use

   template <typename T>
   constexpr auto upcx_compare(const T& lhs, const T& rhs) -> decltype((upcx_enable_comparison(lhs), 0)) {
      int result = 0;
      for_each_field<T>([&](const char*, auto&& member) {
         if (!result) {
            using internal_use_do_not_use::upcx_compare;
            result = upcx_compare(member(&lhs), member(&rhs));
         }
      });
      return result;
   }

   template <typename T>
   constexpr auto operator<(const T& lhs, const T& rhs) -> decltype(upcx_enable_comparison(lhs)) {
      return upcx_compare(lhs, rhs) < 0;
   }
   template <typename T>
   constexpr auto operator>(const T& lhs, const T& rhs) -> decltype(upcx_enable_comparison(lhs)) {
      return upcx_compare(lhs, rhs) > 0;
   }
   template <typename T>
   constexpr auto operator<=(const T& lhs, const T& rhs) -> decltype(upcx_enable_comparison(lhs)) {
      return upcx_compare(lhs, rhs) <= 0;
   }
   template <typename T>
   constexpr auto operator>=(const T& lhs, const T& rhs) -> decltype(upcx_enable_comparison(lhs)) {
      return upcx_compare(lhs, rhs) >= 0;
   }

}} // namespace upcx::operators
