#pragma once

#include <upcxio/upcxio.hpp>

class [[upcxio::contract]] restrict_action_test : public upcxio::contract {
public:
   using upcxio::contract::contract;

   [[upcxio::action]]
   void noop( );

   [[upcxio::action]]
   void sendinline( upcxio::name authorizer );

   [[upcxio::action]]
   void senddefer( upcxio::name authorizer, uint32_t senderid );


   [[upcxio::action]]
   void notifyinline( upcxio::name acctonotify, upcxio::name authorizer );

   [[upcxio::action]]
   void notifydefer( upcxio::name acctonotify, upcxio::name authorizer, uint32_t senderid );

   [[upcxio::on_notify("testacc::notifyinline")]]
   void on_notify_inline( upcxio::name acctonotify, upcxio::name authorizer );

   [[upcxio::on_notify("testacc::notifydefer")]]
   void on_notify_defer( upcxio::name acctonotify, upcxio::name authorizer, uint32_t senderid );
};
