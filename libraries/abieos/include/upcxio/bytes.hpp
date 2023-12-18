#pragma once

#include "from_json.hpp"
#include "to_json.hpp"
#include "operators.hpp"
#include <vector>

namespace upcxio {

struct bytes {
   std::vector<char> data;
};

UPCXIO_REFLECT(bytes, data);
UPCXIO_COMPARE(bytes);

template <typename S>
void from_json(bytes& obj, S& stream) {
   return upcxio::from_json_hex(obj.data, stream);
}

template <typename S>
void to_json(const bytes& obj, S& stream) {
   return upcxio::to_json_hex(obj.data.data(), obj.data.size(), stream);
}

} // namespace upcxio
