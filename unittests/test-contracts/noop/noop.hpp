#pragma once

#include <upcx/upcx.hpp>

class [[upcx::contract]] noop : public upcx::contract {
public:
   using upcx::contract::contract;

   [[upcx::action]]
   void anyaction( upcx::name                       from,
                   const upcx::ignore<std::string>& type,
                   const upcx::ignore<std::string>& data );
};
