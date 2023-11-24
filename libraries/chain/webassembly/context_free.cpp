#include <upcxio/chain/webassembly/interface.hpp>
#include <upcxio/chain/apply_context.hpp>

namespace upcxio { namespace chain { namespace webassembly {
   int32_t interface::get_context_free_data( uint32_t index, legacy_span<char> buffer) const {
      return context.get_context_free_data( index, buffer.data(), buffer.size() );
   }
}}} // ns upcxio::chain::webassembly
