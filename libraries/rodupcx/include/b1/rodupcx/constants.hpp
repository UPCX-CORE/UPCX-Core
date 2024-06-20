#pragma once

#include <upcx/name.hpp>

namespace b1::rodupcx {

// kv database which stores rodupcx state, including a mirror of nodupcx state
inline constexpr upcx::name state_database{ "upcx.state" };

// account within state_database which stores state
inline constexpr upcx::name state_account{ "state" };

} // namespace b1::rodupcx
