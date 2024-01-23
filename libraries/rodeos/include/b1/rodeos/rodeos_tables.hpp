#pragma once

#include <b1/rodupcx/callbacks/kv.hpp>
#include <b1/rodupcx/constants.hpp>
#include <upcx/ship_protocol.hpp>
#include <upcx/to_key.hpp>

namespace upcx {
using b1::rodupcx::kv_environment;
}

#include <upcx/key_value.hpp>

namespace b1::rodupcx {

using account           = upcx::ship_protocol::account;
using account_metadata  = upcx::ship_protocol::account_metadata;
using code              = upcx::ship_protocol::code;
using contract_index128 = upcx::ship_protocol::contract_index128;
using contract_index64  = upcx::ship_protocol::contract_index64;
using contract_row      = upcx::ship_protocol::contract_row;
using contract_table    = upcx::ship_protocol::contract_table;
using global_property   = upcx::ship_protocol::global_property;
using key_value         = upcx::ship_protocol::key_value;
using key_value_v0      = upcx::ship_protocol::key_value_v0;
using producer_schedule = upcx::ship_protocol::producer_schedule;
using table_delta       = upcx::ship_protocol::table_delta;
using table_delta_v1    = upcx::ship_protocol::table_delta_v1;

struct fill_status_v0 {
   upcx::checksum256 chain_id        = {};
   uint32_t           head            = {};
   upcx::checksum256 head_id         = {};
   uint32_t           irreversible    = {};
   upcx::checksum256 irreversible_id = {};
   uint32_t           first           = {};
};

UPCX_REFLECT(fill_status_v0, chain_id, head, head_id, irreversible, irreversible_id, first)

using fill_status = std::variant<fill_status_v0>;

inline bool operator==(const fill_status_v0& a, fill_status_v0& b) {
   return std::tie(a.head, a.head_id, a.irreversible, a.irreversible_id, a.first) ==
          std::tie(b.head, b.head_id, b.irreversible, b.irreversible_id, b.first);
}

inline bool operator!=(const fill_status_v0& a, fill_status_v0& b) { return !(a == b); }

using fill_status_sing = upcx::kv_singleton<fill_status, upcx::name{ "fill.status" }, state_database>;

struct block_info_v0 {
   uint32_t                         num                = {};
   upcx::checksum256               id                 = {};
   upcx::block_timestamp           timestamp          = {};
   upcx::name                      producer           = {};
   uint16_t                         confirmed          = {};
   upcx::checksum256               previous           = {};
   upcx::checksum256               transaction_mroot  = {};
   upcx::checksum256               action_mroot       = {};
   uint32_t                         schedule_version   = {};
   std::optional<producer_schedule> new_producers      = {};
   upcx::signature                 producer_signature = {};
};

UPCX_REFLECT(block_info_v0, num, id, timestamp, producer, confirmed, previous, transaction_mroot, action_mroot,
              schedule_version, new_producers, producer_signature)

using block_info = std::variant<block_info_v0>;

// todo: move out of "state"?
struct block_info_kv : upcx::kv_table<block_info> {
   index<uint32_t> primary_index{ upcx::name{ "primary" }, [](const auto& var) {
                                    return std::visit([](const auto& obj) { return obj.num; }, *var);
                                 } };

   index<upcx::checksum256> id_index{ upcx::name{ "id" }, [](const auto& var) {
                                         return std::visit([](const auto& obj) { return obj.id; }, *var);
                                      } };

   block_info_kv(upcx::kv_environment environment) : upcx::kv_table<block_info>{ std::move(environment) } {
      init(state_account, upcx::name{ "block.info" }, state_database, primary_index, id_index);
   }
};

struct global_property_kv : upcx::kv_table<global_property> {
   index<std::vector<char>> primary_index{ upcx::name{ "primary" },
                                           [](const auto& var) { return std::vector<char>{}; } };

   global_property_kv(upcx::kv_environment environment) : upcx::kv_table<global_property>{ std::move(environment) } {
      init(state_account, upcx::name{ "global.prop" }, state_database, primary_index);
   }
};

struct account_kv : upcx::kv_table<account> {
   index<upcx::name> primary_index{ upcx::name{ "primary" }, [](const auto& var) {
                                       return std::visit([](const auto& obj) { return obj.name; }, *var);
                                    } };

   account_kv(upcx::kv_environment environment) : upcx::kv_table<account>{ std::move(environment) } {
      init(state_account, upcx::name{ "account" }, state_database, primary_index);
   }
};

struct account_metadata_kv : upcx::kv_table<account_metadata> {
   index<upcx::name> primary_index{ upcx::name{ "primary" }, [](const auto& var) {
                                       return std::visit([](const auto& obj) { return obj.name; }, *var);
                                    } };

   account_metadata_kv(upcx::kv_environment environment)
       : upcx::kv_table<account_metadata>{ std::move(environment) } {
      init(state_account, upcx::name{ "account.meta" }, state_database, primary_index);
   }
};

struct code_kv : upcx::kv_table<code> {
   index<std::tuple<const uint8_t&, const uint8_t&, const upcx::checksum256&>> primary_index{
      upcx::name{ "primary" },
      [](const auto& var) {
         return std::visit([](const auto& obj) { return std::tie(obj.vm_type, obj.vm_version, obj.code_hash); }, *var);
      }
   };

   code_kv(upcx::kv_environment environment) : upcx::kv_table<code>{ std::move(environment) } {
      init(state_account, upcx::name{ "code" }, state_database, primary_index);
   }
};

struct contract_table_kv : upcx::kv_table<contract_table> {
   index<std::tuple<const upcx::name&, const upcx::name&, const upcx::name&>> primary_index{
      upcx::name{ "primary" },
      [](const auto& var) {
         return std::visit([](const auto& obj) { return std::tie(obj.code, obj.table, obj.scope); }, *var);
      }
   };

   contract_table_kv(upcx::kv_environment environment) : upcx::kv_table<contract_table>{ std::move(environment) } {
      init(state_account, upcx::name{ "contract.tab" }, state_database, primary_index);
   }
};

struct contract_row_kv : upcx::kv_table<contract_row> {
   using PT = typename std::tuple<const upcx::name&, const upcx::name&, const upcx::name&, const uint64_t&>;
   index<PT> primary_index{ upcx::name{ "primary" }, [](const auto& var) {
                              return std::visit(
                                    [](const auto& obj) {
                                       return std::tie(obj.code, obj.table, obj.scope, obj.primary_key);
                                    },
                                    *var);
                           } };

   contract_row_kv(upcx::kv_environment environment) : upcx::kv_table<contract_row>{ std::move(environment) } {
      init(state_account, upcx::name{ "contract.row" }, state_database, primary_index);
   }
};

struct contract_index64_kv : upcx::kv_table<contract_index64> {
   using PT = typename std::tuple<const upcx::name&, const upcx::name&, const upcx::name&, const uint64_t&>;
   index<PT> primary_index{ upcx::name{ "primary" }, [](const auto& var) {
                              return std::visit(
                                    [](const auto& obj) {
                                       return std::tie(obj.code, obj.table, obj.scope, obj.primary_key);
                                    },
                                    *var);
                           } };
   using ST = typename std::tuple<const upcx::name&, const upcx::name&, const upcx::name&, const uint64_t&,
                                  const uint64_t&>;
   index<ST> secondary_index{ upcx::name{ "secondary" }, [](const auto& var) {
                                return std::visit(
                                      [](const auto& obj) {
                                         return std::tie(obj.code, obj.table, obj.scope, obj.secondary_key,
                                                         obj.primary_key);
                                      },
                                      *var);
                             } };

   contract_index64_kv(upcx::kv_environment environment)
       : upcx::kv_table<contract_index64>{ std::move(environment) } {
      init(state_account, upcx::name{ "contract.i1" }, state_database, primary_index, secondary_index);
   }
};

struct contract_index128_kv : upcx::kv_table<contract_index128> {
   using PT = typename std::tuple<const upcx::name&, const upcx::name&, const upcx::name&, const uint64_t&>;
   index<PT> primary_index{ upcx::name{ "primary" }, [](const auto& var) {
                              return std::visit(
                                    [](const auto& obj) {
                                       return std::tie(obj.code, obj.table, obj.scope, obj.primary_key);
                                    },
                                    *var);
                           } };
   using ST = typename std::tuple<const upcx::name&, const upcx::name&, const upcx::name&, const __uint128_t&,
                                  const uint64_t&>;
   index<ST> secondary_index{ upcx::name{ "secondary" }, [](const auto& var) {
                                return std::visit(
                                      [](const auto& obj) {
                                         return std::tie(obj.code, obj.table, obj.scope, obj.secondary_key,
                                                         obj.primary_key);
                                      },
                                      *var);
                             } };

   contract_index128_kv(upcx::kv_environment environment)
       : upcx::kv_table<contract_index128>{ std::move(environment) } {
      init(state_account, upcx::name{ "contract.i2" }, state_database, primary_index, secondary_index);
   }
};

template <typename Table, typename D, typename F>
void store_delta_typed(upcx::kv_environment environment, D& delta, bool bypass_preexist_check, F f) {
   Table table{ environment };
   for (auto& row : delta.rows) {
      f();
      auto obj = upcx::from_bin<typename Table::value_type>(row.data);
      if (row.present)
         table.put(obj);
      else
         table.erase(obj);
   }
}

template <typename D, typename F>
void store_delta_kv(upcx::kv_environment environment, D& delta, F f) {
   for (auto& row : delta.rows) {
      f();
      auto  obj  = upcx::from_bin<key_value>(row.data);
      auto& obj0 = std::get<key_value_v0>(obj);
#warning uncomment this when we remove the database logic on rodupcx
      /*
      if (row.present)
         environment.kv_set(obj0.database.value, obj0.contract.value, obj0.key.pos, obj0.key.remaining(),
                            obj0.value.pos, obj0.value.remaining());
      else
         environment.kv_erase(obj0.database.value, obj0.contract.value, obj0.key.pos, obj0.key.remaining());
      */
   }
}

template <typename D, typename F>
inline void store_delta(upcx::kv_environment environment, D& delta, bool bypass_preexist_check, F f) {
   if (delta.name == "global_property")
      store_delta_typed<global_property_kv>(environment, delta, bypass_preexist_check, f);
   if (delta.name == "account")
      store_delta_typed<account_kv>(environment, delta, bypass_preexist_check, f);
   if (delta.name == "account_metadata")
      store_delta_typed<account_metadata_kv>(environment, delta, bypass_preexist_check, f);
   if (delta.name == "code")
      store_delta_typed<code_kv>(environment, delta, bypass_preexist_check, f);
   if (delta.name == "contract_table")
      store_delta_typed<contract_table_kv>(environment, delta, bypass_preexist_check, f);
   if (delta.name == "contract_row")
      store_delta_typed<contract_row_kv>(environment, delta, bypass_preexist_check, f);
   if (delta.name == "contract_index64")
      store_delta_typed<contract_index64_kv>(environment, delta, bypass_preexist_check, f);
   if (delta.name == "contract_index128")
      store_delta_typed<contract_index128_kv>(environment, delta, bypass_preexist_check, f);
   if (delta.name == "key_value")
      store_delta_kv(environment, delta, f);
}

inline void store_deltas(upcx::kv_environment environment, std::vector<table_delta>& deltas,
                         bool bypass_preexist_check) {
   for (auto& delta : deltas) //
      std::visit([&](auto& delta_any_v) { store_delta(environment, delta_any_v, bypass_preexist_check, [] {}); }, delta);
}

} // namespace b1::rodupcx
