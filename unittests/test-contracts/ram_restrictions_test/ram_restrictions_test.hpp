#pragma once

#include <upcx/upcx.hpp>

class [[upcx::contract]] ram_restrictions_test : public upcx::contract {
public:
   struct [[upcx::table]] data {
      uint64_t           key;
      std::vector<char>  value;

      uint64_t primary_key() const { return key; }
   };

   typedef upcx::multi_index<"tablea"_n, data> tablea;
   typedef upcx::multi_index<"tableb"_n, data> tableb;

public:
   using upcx::contract::contract;

   [[upcx::action]]
   void noop();

   [[upcx::action]]
   void setdata( uint32_t len1, uint32_t len2, upcx::name payer );

   [[upcx::action]]
   void notifysetdat( upcx::name acctonotify, uint32_t len1, uint32_t len2, upcx::name payer );

   [[upcx::on_notify("tester2::notifysetdat")]]
   void on_notify_setdata( upcx::name acctonotify, uint32_t len1, uint32_t len2, upcx::name payer );

   [[upcx::action]]
   void senddefer( uint64_t senderid, upcx::name payer );

   [[upcx::action]]
   void notifydefer( upcx::name acctonotify, uint64_t senderid, upcx::name payer );

   [[upcx::on_notify("tester2::notifydefer")]]
   void on_notifydefer( upcx::name acctonotify, uint64_t senderid, upcx::name payer );

};
