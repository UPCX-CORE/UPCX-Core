#pragma once

#include <upcxio/state_history/types.hpp>
#include <upcxio/chain/combined_database.hpp>

namespace upcxio {
namespace state_history {

std::vector<table_delta> create_deltas(const chain::combined_database& db, bool full_snapshot);

} // namespace state_history
} // namespace upcxio
