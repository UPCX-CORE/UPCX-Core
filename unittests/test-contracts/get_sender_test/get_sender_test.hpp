#pragma once

#include <upcxio/upcxio.hpp>

class [[upcxio::contract]] get_sender_test : public upcxio::contract {
public:
   using upcxio::contract::contract;

   [[upcxio::action]]
   void assertsender( upcxio::name expected_sender );
   using assertsender_action = upcxio::action_wrapper<"assertsender"_n, &get_sender_test::assertsender>;

   [[upcxio::action]]
   void sendinline( upcxio::name to, upcxio::name expected_sender );

   [[upcxio::action]]
   void notify( upcxio::name to, upcxio::name expected_sender, bool send_inline );

   [[upcxio::on_notify("*::notify")]]
   void on_notify( upcxio::name to, upcxio::name expected_sender, bool send_inline );

};
