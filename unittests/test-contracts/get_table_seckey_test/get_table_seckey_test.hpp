/**
 *  @file
 *  @copyright defined in upcx/LICENSE
 */
#pragma once

#include <upcxio/upcxio.hpp>
#include <upcxio/crypto.hpp>

using namespace upcxio;

class [[upcxio::contract]] get_table_seckey_test : public upcxio::contract {
    public:
    using upcxio::contract::contract;

    // Number object
    struct [[upcxio::table]] numobj {
        uint64_t        key;
        uint64_t        sec64;
        uint128_t       sec128;
        double          secdouble;
        long double     secldouble;
        name            nm;

        uint64_t primary_key() const { return key; }
        uint64_t sec64_key() const { return sec64; }
        uint128_t sec128_key() const { return sec128; }
        double secdouble_key() const { return secdouble; }
        long double secldouble_key() const { return secldouble; }
        uint64_t name_key() const { return nm.value; }
    };

    typedef upcxio::multi_index< "numobjs"_n, numobj,
                                indexed_by<"bysec1"_n, const_mem_fun<numobj, uint64_t, &numobj::sec64_key>>,
                                indexed_by<"bysec2"_n, const_mem_fun<numobj, uint128_t, &numobj::sec128_key>>,
                                indexed_by<"bysec3"_n, const_mem_fun<numobj, double, &numobj::secdouble_key>>,
                                indexed_by<"bysec4"_n, const_mem_fun<numobj, long double, &numobj::secldouble_key>>,
                                indexed_by<"byname"_n, const_mem_fun<numobj, uint64_t, &numobj::name_key>>
                                > numobjs;

   [[upcxio::action]]
   void addnumobj(uint64_t input, std::string nm);
};
