#pragma once

#include <upcx/upcx.hpp>

class [[upcx::contract]] integration_test : public upcx::contract {
public:
   using upcx::contract::contract;

   [[upcx::action]]
   void store( upcx::name from, upcx::name to, uint64_t num );

   struct [[upcx::table("payloads")]] payload {
      uint64_t              key;
      std::vector<uint64_t> data;

      uint64_t primary_key()const { return key; }

      UPCXLIB_SERIALIZE( payload, (key)(data) )
   };

   using payloads_table = upcx::multi_index< "payloads"_n,  payload >;

};
