#pragma once

#include <upcxio/upcxio.hpp>
#include <upcxio/singleton.hpp>
#include <upcxio/asset.hpp>

// Extacted from upcxio.token contract:
namespace upcxio {
   class [[upcxio::contract("upcxio.token")]] token : public upcxio::contract {
   public:
      using upcxio::contract::contract;

      [[upcxio::action]]
      void transfer( upcxio::name        from,
                     upcxio::name        to,
                     upcxio::asset       quantity,
                     const std::string& memo );
      using transfer_action = upcxio::action_wrapper<"transfer"_n, &token::transfer>;
   };
}

// This contract:
class [[upcxio::contract]] proxy : public upcxio::contract {
public:
   proxy( upcxio::name self, upcxio::name first_receiver, upcxio::datastream<const char*> ds );

   [[upcxio::action]]
   void setowner( upcxio::name owner, uint32_t delay );

   [[upcxio::on_notify("upcxio.token::transfer")]]
   void on_transfer( upcxio::name        from,
                     upcxio::name        to,
                     upcxio::asset       quantity,
                     const std::string& memo );

   [[upcxio::on_notify("upcxio::onerror")]]
   void on_error( uint128_t sender_id, upcxio::ignore<std::vector<char>> sent_trx );

   struct [[upcxio::table]] config {
      upcxio::name owner;
      uint32_t    delay   = 0;
      uint32_t    next_id = 0;

      EOSLIB_SERIALIZE( config, (owner)(delay)(next_id) )
   };

   using config_singleton = upcxio::singleton< "config"_n,  config >;

protected:
   config_singleton _config;
};
