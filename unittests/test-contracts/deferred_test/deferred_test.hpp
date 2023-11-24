#pragma once

#include <upcxio/upcxio.hpp>
#include <vector>

class [[upcxio::contract]] deferred_test : public upcxio::contract {
public:
   using upcxio::contract::contract;

   [[upcxio::action]]
   void defercall( upcxio::name payer, uint64_t sender_id, upcxio::name contract, uint64_t payload );

   [[upcxio::action]]
   void delayedcall( upcxio::name payer, uint64_t sender_id, upcxio::name contract,
                     uint64_t payload, uint32_t delay_sec, bool replace_existing );

   [[upcxio::action]]
   void deferfunc( uint64_t payload );
   using deferfunc_action = upcxio::action_wrapper<"deferfunc"_n, &deferred_test::deferfunc>;

   [[upcxio::action]]
   void inlinecall( upcxio::name contract, upcxio::name authorizer, uint64_t payload );

   [[upcxio::action]]
   void fail();

   [[upcxio::on_notify("upcxio::onerror")]]
   void on_error( uint128_t sender_id, upcxio::ignore<std::vector<char>> sent_trx );
};
