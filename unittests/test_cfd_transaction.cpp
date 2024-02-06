#include "test_cfd_transaction.hpp"
#include <contracts.hpp>

using namespace upcx::chain::literals;

std::vector<upcx::chain::signed_block_ptr> deploy_test_api(upcx::testing::tester& chain) {
   std::vector<upcx::chain::signed_block_ptr> result;
   chain.create_account("testapi"_n);
   chain.create_account("dummy"_n);
   result.push_back(chain.produce_block());
   chain.set_code("testapi"_n, upcx::testing::contracts::test_api_wasm());
   result.push_back(chain.produce_block());
   return result;
}

upcx::chain::transaction_trace_ptr push_test_cfd_transaction(upcx::testing::tester& chain) {
   cf_action                        cfa;
   upcx::chain::signed_transaction trx;
   upcx::chain::action             act({}, cfa);
   trx.context_free_actions.push_back(act);
   trx.context_free_data.emplace_back(fc::raw::pack<uint32_t>(100));
   trx.context_free_data.emplace_back(fc::raw::pack<uint32_t>(200));
   // add a normal action along with cfa
   dummy_action         da = {DUMMY_ACTION_DEFAULT_A, DUMMY_ACTION_DEFAULT_B, DUMMY_ACTION_DEFAULT_C};
   upcx::chain::action act1(
       std::vector<upcx::chain::permission_level>{{"testapi"_n, upcx::chain::config::active_name}}, da);
   trx.actions.push_back(act1);
   chain.set_transaction_headers(trx);
   // run normal passing case
   auto sigs = trx.sign(chain.get_private_key("testapi"_n, "active"), chain.control->get_chain_id());
   return chain.push_transaction(trx);
}
