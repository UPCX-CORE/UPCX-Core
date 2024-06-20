#pragma once

#include <upcx/upcx.hpp>

class [[upcx::contract]] test_api_multi_index : public upcx::contract {
public:
   using upcx::contract::contract;

   [[upcx::action("s1g")]]
   void idx64_general();

   [[upcx::action("s1store")]]
   void idx64_store_only();

   [[upcx::action("s1check")]]
   void idx64_check_without_storing();

   [[upcx::action("s1findfail1")]]
   void idx64_require_find_fail();

   [[upcx::action("s1findfail2")]]
   void idx64_require_find_fail_with_msg();

   [[upcx::action("s1findfail3")]]
   void idx64_require_find_sk_fail();

   [[upcx::action("s1findfail4")]]
   void idx64_require_find_sk_fail_with_msg();

   [[upcx::action("s1pkend")]]
   void idx64_pk_iterator_exceed_end();

   [[upcx::action("s1skend")]]
   void idx64_sk_iterator_exceed_end();

   [[upcx::action("s1pkbegin")]]
   void idx64_pk_iterator_exceed_begin();

   [[upcx::action("s1skbegin")]]
   void idx64_sk_iterator_exceed_begin();

   [[upcx::action("s1pkref")]]
   void idx64_pass_pk_ref_to_other_table();

   [[upcx::action("s1skref")]]
   void idx64_pass_sk_ref_to_other_table();

   [[upcx::action("s1pkitrto")]]
   void idx64_pass_pk_end_itr_to_iterator_to();

   [[upcx::action("s1pkmodify")]]
   void idx64_pass_pk_end_itr_to_modify();

   [[upcx::action("s1pkerase")]]
   void idx64_pass_pk_end_itr_to_erase();

   [[upcx::action("s1skitrto")]]
   void idx64_pass_sk_end_itr_to_iterator_to();

   [[upcx::action("s1skmodify")]]
   void idx64_pass_sk_end_itr_to_modify();

   [[upcx::action("s1skerase")]]
   void idx64_pass_sk_end_itr_to_erase();

   [[upcx::action("s1modpk")]]
   void idx64_modify_primary_key();

   [[upcx::action("s1exhaustpk")]]
   void idx64_run_out_of_avl_pk();

   [[upcx::action("s1skcache")]]
   void idx64_sk_cache_pk_lookup();

   [[upcx::action("s1pkcache")]]
   void idx64_pk_cache_sk_lookup();

   [[upcx::action("s2g")]]
   void idx128_general();

   [[upcx::action("s2store")]]
   void idx128_store_only();

   [[upcx::action("s2check")]]
   void idx128_check_without_storing();

   [[upcx::action("s2autoinc")]]
   void idx128_autoincrement_test();

   [[upcx::action("s2autoinc1")]]
   void idx128_autoincrement_test_part1();

   [[upcx::action("s2autoinc2")]]
   void idx128_autoincrement_test_part2();

   [[upcx::action("s3g")]]
   void idx256_general();

   [[upcx::action("sdg")]]
   void idx_double_general();

   [[upcx::action("sldg")]]
   void idx_long_double_general();

};
