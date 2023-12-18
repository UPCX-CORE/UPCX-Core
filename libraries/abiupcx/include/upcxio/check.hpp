/**
 *  @file
 *  @copyright defined in upcx/LICENSE
 */
#pragma once

#ifdef __upcxio_cdt__
#include <cstdint>
namespace upcxio {
namespace internal_use_do_not_use {
extern "C" {
__attribute__((upcxio_wasm_import, noreturn))
void upcxio_assert_message(uint32_t, const char*, uint32_t);
__attribute__((upcxio_wasm_import, noreturn))
void upcxio_assert(uint32_t, const char*);
__attribute__((upcxio_wasm_import, noreturn))
void upcxio_assert_code(uint32_t, uint64_t);
}
}
}
#else
#include <stdexcept>
#endif

#include <string>
#include <string_view>

namespace upcxio {

/**
 *  @defgroup system System
 *  @ingroup core
 *  @brief Defines wrappers over upcxio_assert
 */


struct upcxio_error : std::exception {
   explicit upcxio_error(uint64_t code) {}
};

namespace detail {
   [[noreturn]] inline void assert_or_throw(std::string_view msg) {
#ifdef __upcxio_cdt__
         internal_use_do_not_use::upcxio_assert_message(false, msg.data(), msg.size());
#else
         throw std::runtime_error(std::string(msg));
#endif
   }
   [[noreturn]] inline void assert_or_throw(const char* msg) {
#ifdef __upcxio_cdt__
         internal_use_do_not_use::upcxio_assert(false, msg);
#else
         throw std::runtime_error(msg);
#endif
   }
   [[noreturn]] inline void assert_or_throw(std::string&& msg) {
#ifdef __upcxio_cdt__
         internal_use_do_not_use::upcxio_assert_message(false, msg.c_str(), msg.size());
#else
         throw std::runtime_error(std::move(msg));
#endif
   }
   [[noreturn]] inline void assert_or_throw(uint64_t code) {
#ifdef __upcxio_cdt__
         internal_use_do_not_use::upcxio_assert_code(false, code);
#else
         throw std::runtime_error(std::to_string(code));
#endif
   }
} // ns upcxio::detail

/**
 *  Assert if the predicate fails and use the supplied message.
 *
 *  @ingroup system
 *
 *  Example:
 *  @code
 *  upcxio::check(a == b, "a does not equal b");
 *  @endcode
 */
inline void check(bool pred, std::string_view msg) {
   if (!pred)
      upcxio::detail::assert_or_throw(msg);
}

/**
 *  Assert if the predicate fails and use the supplied message.
 *
 *  @ingroup system
 *
 *  Example:
 *  @code
 *  upcxio::check(a == b, "a does not equal b");
 *  @endcode
 */
inline void check(bool pred, const char* msg) {
   if (!pred)
      upcxio::detail::assert_or_throw(msg);
}

/**
 *  Assert if the predicate fails and use the supplied message.
 *
 *  @ingroup system
 *
 *  Example:
 *  @code
 *  upcxio::check(a == b, "a does not equal b");
 *  @endcode
 */
inline void check(bool pred, const std::string& msg) {
   if (!pred)
      upcxio::detail::assert_or_throw(std::string_view{msg.c_str(), msg.size()});
}

/**
 *  Assert if the predicate fails and use the supplied message.
 *
 *  @ingroup system
 *
 *  Example:
 *  @code
 *  upcxio::check(a == b, "a does not equal b");
 *  @endcode
 */
inline void check(bool pred, std::string&& msg) {
   if (!pred)
      upcxio::detail::assert_or_throw(std::move(msg));
}

/**
 *  Assert if the predicate fails and use a subset of the supplied message.
 *
 *  @ingroup system
 *
 *  Example:
 *  @code
 *  const char* msg = "a does not equal b b does not equal a";
 *  upcxio::check(a == b, "a does not equal b", 18);
 *  @endcode
 */
inline void check(bool pred, const char* msg, size_t n) {
   if (!pred)
      upcxio::detail::assert_or_throw(std::string_view{msg, n});
}

/**
 *  Assert if the predicate fails and use a subset of the supplied message.
 *
 *  @ingroup system
 *
 *  Example:
 *  @code
 *  std::string msg = "a does not equal b b does not equal a";
 *  upcxio::check(a == b, msg, 18);
 *  @endcode
 */
inline void check(bool pred, const std::string& msg, size_t n) {
   if (!pred)
      upcxio::detail::assert_or_throw(msg.substr(0, n));
}

/**
 *  Assert if the predicate fails and use the supplied error code.
 *
 *  @ingroup system
 *
 *  Example:
 *  @code
 *  upcxio::check(a == b, 13);
 *  @endcode
 */
inline void check(bool pred, uint64_t code) {
   if (!pred)
      upcxio::detail::assert_or_throw(code);
}
} // namespace upcxio
