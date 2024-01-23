#include <upcx/chain/webassembly/upcx-vm-oc.hpp>
#include <upcx/chain/wasm_upcx_constraints.hpp>
#include <upcx/chain/wasm_upcx_injection.hpp>
#include <upcx/chain/apply_context.hpp>
#include <upcx/chain/exceptions.hpp>
#include <upcx/chain/global_property_object.hpp>

#include <vector>
#include <iterator>

namespace upcx { namespace chain { namespace webassembly { namespace upcxvmoc {

class upcxvmoc_instantiated_module : public wasm_instantiated_module_interface {
   public:
      upcxvmoc_instantiated_module(const digest_type& code_hash, const uint8_t& vm_version, upcxvmoc_runtime& wr) :
         _code_hash(code_hash),
         _vm_version(vm_version),
         _upcxvmoc_runtime(wr)
      {

      }

      ~upcxvmoc_instantiated_module() {
         _upcxvmoc_runtime.cc.free_code(_code_hash, _vm_version);
      }

      void apply(apply_context& context) override {
         const code_descriptor* const cd = _upcxvmoc_runtime.cc.get_descriptor_for_code_sync(_code_hash, _vm_version);
         UPCX_ASSERT(cd, wasm_execution_error, "UPCX VM OC instantiation failed");

         _upcxvmoc_runtime.exec.execute(*cd, _upcxvmoc_runtime.mem, context);
      }

      const digest_type              _code_hash;
      const uint8_t                  _vm_version;
      upcxvmoc_runtime&               _upcxvmoc_runtime;
};

upcxvmoc_runtime::upcxvmoc_runtime(const boost::filesystem::path data_dir, const upcxvmoc::config& upcxvmoc_config, const chainbase::database& db)
   : cc(data_dir, upcxvmoc_config, db), exec(cc), mem(wasm_constraints::maximum_linear_memory/wasm_constraints::wasm_page_size) {
}

upcxvmoc_runtime::~upcxvmoc_runtime() {
}

std::unique_ptr<wasm_instantiated_module_interface> upcxvmoc_runtime::instantiate_module(const char* code_bytes, size_t code_size, std::vector<uint8_t> initial_memory,
                                                                                        const digest_type& code_hash, const uint8_t& vm_type, const uint8_t& vm_version) {

   return std::make_unique<upcxvmoc_instantiated_module>(code_hash, vm_type, *this);
}

//never called. UPCX VM OC overrides upcx_exit to its own implementation
void upcxvmoc_runtime::immediately_exit_currently_running_module() {}

}}}}
