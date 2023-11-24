#pragma once

#include <upcxio/upcxio.hpp>

class [[upcxio::contract]] integration_test : public upcxio::contract {
public:
   using upcxio::contract::contract;

   [[upcxio::action]]
   void store( upcxio::name from, upcxio::name to, uint64_t num );

   struct [[upcxio::table("payloads")]] payload {
      uint64_t              key;
      std::vector<uint64_t> data;

      uint64_t primary_key()const { return key; }

      EOSLIB_SERIALIZE( payload, (key)(data) )
   };

   using payloads_table = upcxio::multi_index< "payloads"_n,  payload >;

};
