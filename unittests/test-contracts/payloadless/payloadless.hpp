#pragma once

#include <upcx/upcx.hpp>

class [[upcx::contract]] payloadless : public upcx::contract {
public:
   using upcx::contract::contract;

   [[upcx::action]]
   void doit();
};
