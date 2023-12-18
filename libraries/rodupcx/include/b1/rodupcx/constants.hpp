#pragma once

#include <upcxio/name.hpp>

namespace b1::rodupcx {

// kv database which stores rodupcx state, including a mirror of nodupcx state
inline constexpr upcxio::name state_database{ "upcxio.state" };

// account within state_database which stores state
inline constexpr upcxio::name state_account{ "state" };

} // namespace b1::rodupcx
