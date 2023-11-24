#pragma once
#include <fc/variant.hpp>
#include <upcxio/trace_api/trace.hpp>
#include <upcxio/chain/abi_def.hpp>
#include <upcxio/chain/protocol_feature_activation.hpp>

namespace upcxio { namespace trace_api {

   using data_log_entry = std::variant<
      block_trace_v0,
      block_trace_v1,
      block_trace_v2
   >;

}}
