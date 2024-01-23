#pragma once

#include <upcx/upcx.hpp>
#include <upcx/singleton.hpp>
#include <upcx/asset.hpp>

// Extacted from upcx.token contract:
namespace upcx {
   class [[upcx::contract("upcx.token")]] token : public upcx::contract {
   public:
      using upcx::contract::contract;

      [[upcx::action]]
      void transfer( upcx::name        from,
                     upcx::name        to,
                     upcx::asset       quantity,
                     const std::string& memo );
      using transfer_action = upcx::action_wrapper<"transfer"_n, &token::transfer>;
   };
}

// This contract:
class [[upcx::contract]] proxy : public upcx::contract {
public:
   proxy( upcx::name self, upcx::name first_receiver, upcx::datastream<const char*> ds );

   [[upcx::action]]
   void setowner( upcx::name owner, uint32_t delay );

   [[upcx::on_notify("upcx.token::transfer")]]
   void on_transfer( upcx::name        from,
                     upcx::name        to,
                     upcx::asset       quantity,
                     const std::string& memo );

   [[upcx::on_notify("upcx::onerror")]]
   void on_error( uint128_t sender_id, upcx::ignore<std::vector<char>> sent_trx );

   struct [[upcx::table]] config {
      upcx::name owner;
      uint32_t    delay   = 0;
      uint32_t    next_id = 0;

      UPCXLIB_SERIALIZE( config, (owner)(delay)(next_id) )
   };

   using config_singleton = upcx::singleton< "config"_n,  config >;

protected:
   config_singleton _config;
};
