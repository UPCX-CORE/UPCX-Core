#pragma once

#include <upcx/upcx.hpp>
#include <vector>

class [[upcx::contract]] deferred_test : public upcx::contract {
public:
   using upcx::contract::contract;

   [[upcx::action]]
   void defercall( upcx::name payer, uint64_t sender_id, upcx::name contract, uint64_t payload );

   [[upcx::action]]
   void delayedcall( upcx::name payer, uint64_t sender_id, upcx::name contract,
                     uint64_t payload, uint32_t delay_sec, bool replace_existing );

   [[upcx::action]]
   void deferfunc( uint64_t payload );
   using deferfunc_action = upcx::action_wrapper<"deferfunc"_n, &deferred_test::deferfunc>;

   [[upcx::action]]
   void inlinecall( upcx::name contract, upcx::name authorizer, uint64_t payload );

   [[upcx::action]]
   void fail();

   [[upcx::on_notify("upcx::onerror")]]
   void on_error( uint128_t sender_id, upcx::ignore<std::vector<char>> sent_trx );
};
