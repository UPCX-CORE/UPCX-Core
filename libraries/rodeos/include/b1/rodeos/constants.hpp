#pragma once

#include <upcxio/name.hpp>

namespace b1::rodeos {

// kv database which stores rodeos state, including a mirror of nodeos state
inline constexpr upcxio::name state_database{ "upcxio.state" };

// account within state_database which stores state
inline constexpr upcxio::name state_account{ "state" };

} // namespace b1::rodeos
