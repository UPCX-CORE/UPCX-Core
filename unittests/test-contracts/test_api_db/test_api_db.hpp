#pragma once

#include <upcx/upcx.hpp>

class [[upcx::contract]] test_api_db : public upcx::contract {
public:
   using upcx::contract::contract;

   [[upcx::action("pg")]]
   void primary_i64_general();

   [[upcx::action("pl")]]
   void primary_i64_lowerbound();

   [[upcx::action("pu")]]
   void primary_i64_upperbound();

   [[upcx::action("s1g")]]
   void idx64_general();

   [[upcx::action("s1l")]]
   void idx64_lowerbound();

   [[upcx::action("s1u")]]
   void idx64_upperbound();

   [[upcx::action("tia")]]
   void test_invalid_access( upcx::name code, uint64_t val, uint32_t index, bool store );

   [[upcx::action("sdnancreate")]]
   void idx_double_nan_create_fail();

   [[upcx::action("sdnanmodify")]]
   void idx_double_nan_modify_fail();

   [[upcx::action("sdnanlookup")]]
   void idx_double_nan_lookup_fail( uint32_t lookup_type );

   [[upcx::action("sk32align")]]
   void misaligned_secondary_key256_tests();

};
