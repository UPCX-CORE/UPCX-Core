#pragma once

#include <upcxio/upcxio.hpp>

class [[upcxio::contract]] payloadless : public upcxio::contract {
public:
   using upcxio::contract::contract;

   [[upcxio::action]]
   void doit();
};
