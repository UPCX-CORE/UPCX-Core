#pragma once

#include "from_json.hpp"
#include "to_json.hpp"
#include "operators.hpp"
#include <vector>

namespace upcx {

struct bytes {
   std::vector<char> data;
};

UPCX_REFLECT(bytes, data);
UPCX_COMPARE(bytes);

template <typename S>
void from_json(bytes& obj, S& stream) {
   return upcx::from_json_hex(obj.data, stream);
}

template <typename S>
void to_json(const bytes& obj, S& stream) {
   return upcx::to_json_hex(obj.data.data(), obj.data.size(), stream);
}

} // namespace upcx
