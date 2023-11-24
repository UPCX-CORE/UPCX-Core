#pragma once

#include <upcxio/upcxio.hpp>

class [[upcxio::contract]] ram_restrictions_test : public upcxio::contract {
public:
   struct [[upcxio::table]] data {
      uint64_t           key;
      std::vector<char>  value;

      uint64_t primary_key() const { return key; }
   };

   typedef upcxio::multi_index<"tablea"_n, data> tablea;
   typedef upcxio::multi_index<"tableb"_n, data> tableb;

public:
   using upcxio::contract::contract;

   [[upcxio::action]]
   void noop();

   [[upcxio::action]]
   void setdata( uint32_t len1, uint32_t len2, upcxio::name payer );

   [[upcxio::action]]
   void notifysetdat( upcxio::name acctonotify, uint32_t len1, uint32_t len2, upcxio::name payer );

   [[upcxio::on_notify("tester2::notifysetdat")]]
   void on_notify_setdata( upcxio::name acctonotify, uint32_t len1, uint32_t len2, upcxio::name payer );

   [[upcxio::action]]
   void senddefer( uint64_t senderid, upcxio::name payer );

   [[upcxio::action]]
   void notifydefer( upcxio::name acctonotify, uint64_t senderid, upcxio::name payer );

   [[upcxio::on_notify("tester2::notifydefer")]]
   void on_notifydefer( upcxio::name acctonotify, uint64_t senderid, upcxio::name payer );

};
