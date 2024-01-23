#pragma once

#include <upcx/upcx.hpp>

class [[upcx::contract]] get_sender_test : public upcx::contract {
public:
   using upcx::contract::contract;

   [[upcx::action]]
   void assertsender( upcx::name expected_sender );
   using assertsender_action = upcx::action_wrapper<"assertsender"_n, &get_sender_test::assertsender>;

   [[upcx::action]]
   void sendinline( upcx::name to, upcx::name expected_sender );

   [[upcx::action]]
   void notify( upcx::name to, upcx::name expected_sender, bool send_inline );

   [[upcx::on_notify("*::notify")]]
   void on_notify( upcx::name to, upcx::name expected_sender, bool send_inline );

};
