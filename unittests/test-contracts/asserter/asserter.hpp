#pragma once

#include <upcxio/upcxio.hpp>

class [[upcxio::contract]] asserter : public upcxio::contract {
public:
   using upcxio::contract::contract;

   [[upcxio::action]]
   void procassert( int8_t condition, std::string message );

   [[upcxio::action]]
   void provereset();
};
