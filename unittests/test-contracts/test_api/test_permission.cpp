#include <limits>

#include <upcx/action.hpp>
#include <upcx/upcx.hpp>
#include <upcx/permission.hpp>
#include <upcx/print.hpp>
#include <upcx/serialize.hpp>

#include "test_api.hpp"



struct check_auth_msg {
   upcx::name                    account;
   upcx::name                    permission;
   std::vector<upcx::public_key> pubkeys;

   UPCXLIB_SERIALIZE( check_auth_msg, (account)(permission)(pubkeys)  )
};

void test_permission::check_authorization( uint64_t receiver, uint64_t code, uint64_t action ) {
   (void)code;
   (void)action;
   using namespace upcx;

   auto self = receiver;
   auto params = unpack_action_data<check_auth_msg>();
   auto packed_pubkeys = pack(params.pubkeys);
   int64_t res64 = upcx::check_permission_authorization( params.account,
                                                     params.permission,
                                                     packed_pubkeys.data(), packed_pubkeys.size(),
                                                     (const char*)0,        0,
                                                     microseconds{ std::numeric_limits<int64_t>::max() }
                                                   );

   auto itr = db_lowerbound_i64( self, self, self, 1 );
   if(itr == -1) {
      db_store_i64( self, self, self, 1, &res64, sizeof(int64_t) );
   } else {
      db_update_i64( itr, self, &res64, sizeof(int64_t) );
   }
}

struct test_permission_last_used_msg {
   upcx::name account;
   upcx::name permission;
   int64_t     last_used_time;

   UPCXLIB_SERIALIZE( test_permission_last_used_msg, (account)(permission)(last_used_time) )
};

void test_permission::test_permission_last_used( uint64_t /* receiver */, uint64_t code, uint64_t action ) {
   (void)code;
   (void)action;
   using namespace upcx;

   auto params = unpack_action_data<test_permission_last_used_msg>();

   time_point msec{ microseconds{params.last_used_time}};
   upcx_assert( upcx::get_permission_last_used(params.account, params.permission) == msec, "unexpected last used permission time" );
}

void test_permission::test_account_creation_time( uint64_t /* receiver */, uint64_t code, uint64_t action ) {
   (void)code;
   (void)action;
   using namespace upcx;

   auto params = unpack_action_data<test_permission_last_used_msg>();

   time_point msec{ microseconds{params.last_used_time}};
   upcx_assert( upcx::get_account_creation_time(params.account) == msec, "unexpected account creation time" );
}
