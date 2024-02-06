#pragma once

#include <upcx/state_history/types.hpp>
#include <upcx/chain/combined_database.hpp>

namespace upcx {
namespace state_history {

std::vector<table_delta> create_deltas(const chain::combined_database& db, bool full_snapshot);

} // namespace state_history
} // namespace upcx
