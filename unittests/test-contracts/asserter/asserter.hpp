#pragma once

#include <upcx/upcx.hpp>

class [[upcx::contract]] asserter : public upcx::contract {
public:
   using upcx::contract::contract;

   [[upcx::action]]
   void procassert( int8_t condition, std::string message );

   [[upcx::action]]
   void provereset();
};
