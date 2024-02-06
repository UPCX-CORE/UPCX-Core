#pragma once
#include "ship_protocol.hpp"

namespace chain_types {
using namespace upcx::ship_protocol;

struct block_info {
   uint32_t               block_num = {};
   upcx::checksum256     block_id  = {};
   upcx::block_timestamp timestamp;
};

UPCX_REFLECT(block_info, block_num, block_id, timestamp);
}; // namespace chain_types