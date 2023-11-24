#pragma once

#include <upcxio/upcxio.hpp>

class [[upcxio::contract]] noop : public upcxio::contract {
public:
   using upcxio::contract::contract;

   [[upcxio::action]]
   void anyaction( upcxio::name                       from,
                   const upcxio::ignore<std::string>& type,
                   const upcxio::ignore<std::string>& data );
};
