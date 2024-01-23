#pragma once

#include <upcx/upcx.hpp>

class [[upcx::contract]] restrict_action_test : public upcx::contract {
public:
   using upcx::contract::contract;

   [[upcx::action]]
   void noop( );

   [[upcx::action]]
   void sendinline( upcx::name authorizer );

   [[upcx::action]]
   void senddefer( upcx::name authorizer, uint32_t senderid );


   [[upcx::action]]
   void notifyinline( upcx::name acctonotify, upcx::name authorizer );

   [[upcx::action]]
   void notifydefer( upcx::name acctonotify, upcx::name authorizer, uint32_t senderid );

   [[upcx::on_notify("testacc::notifyinline")]]
   void on_notify_inline( upcx::name acctonotify, upcx::name authorizer );

   [[upcx::on_notify("testacc::notifydefer")]]
   void on_notify_defer( upcx::name acctonotify, upcx::name authorizer, uint32_t senderid );
};
