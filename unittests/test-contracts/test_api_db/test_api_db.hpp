#pragma once

#include <upcxio/upcxio.hpp>

class [[upcxio::contract]] test_api_db : public upcxio::contract {
public:
   using upcxio::contract::contract;

   [[upcxio::action("pg")]]
   void primary_i64_general();

   [[upcxio::action("pl")]]
   void primary_i64_lowerbound();

   [[upcxio::action("pu")]]
   void primary_i64_upperbound();

   [[upcxio::action("s1g")]]
   void idx64_general();

   [[upcxio::action("s1l")]]
   void idx64_lowerbound();

   [[upcxio::action("s1u")]]
   void idx64_upperbound();

   [[upcxio::action("tia")]]
   void test_invalid_access( upcxio::name code, uint64_t val, uint32_t index, bool store );

   [[upcxio::action("sdnancreate")]]
   void idx_double_nan_create_fail();

   [[upcxio::action("sdnanmodify")]]
   void idx_double_nan_modify_fail();

   [[upcxio::action("sdnanlookup")]]
   void idx_double_nan_lookup_fail( uint32_t lookup_type );

   [[upcxio::action("sk32align")]]
   void misaligned_secondary_key256_tests();

};
