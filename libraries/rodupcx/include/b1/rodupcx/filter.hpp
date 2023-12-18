#pragma once

#include <b1/rodupcx/callbacks/basic.hpp>
#include <b1/rodupcx/callbacks/chaindb.hpp>
#include <b1/rodupcx/callbacks/compiler_builtins.hpp>
#include <b1/rodupcx/callbacks/console.hpp>
#include <b1/rodupcx/callbacks/filter.hpp>
#include <b1/rodupcx/callbacks/memory.hpp>
#include <b1/rodupcx/callbacks/unimplemented.hpp>
#include <b1/rodupcx/callbacks/unimplemented_filter.hpp>

// todo: configure limits
// todo: timeout
namespace b1::rodupcx::filter {

struct callbacks;
using rhf_t     = registered_host_functions<callbacks>;
using backend_t = upcxio::vm::backend<rhf_t, upcxio::vm::jit>;

struct filter_state : b1::rodupcx::data_state<backend_t>, b1::rodupcx::console_state, b1::rodupcx::filter_callback_state {
   upcxio::vm::wasm_allocator wa = {};
};

struct callbacks : b1::rodupcx::chaindb_callbacks<callbacks>,
                   b1::rodupcx::compiler_builtins_callbacks<callbacks>,
                   b1::rodupcx::console_callbacks<callbacks>,
                   b1::rodupcx::context_free_system_callbacks<callbacks>,
                   b1::rodupcx::data_callbacks<callbacks>,
                   b1::rodupcx::db_callbacks<callbacks>,
                   b1::rodupcx::filter_callbacks<callbacks>,
                   b1::rodupcx::memory_callbacks<callbacks>,
                   b1::rodupcx::unimplemented_callbacks<callbacks>,
                   b1::rodupcx::unimplemented_filter_callbacks<callbacks> {
   filter::filter_state&      filter_state;
   b1::rodupcx::chaindb_state& chaindb_state;
   b1::rodupcx::db_view_state& db_view_state;

   callbacks(filter::filter_state& filter_state, b1::rodupcx::chaindb_state& chaindb_state,
             b1::rodupcx::db_view_state& db_view_state)
       : filter_state{ filter_state }, chaindb_state{ chaindb_state }, db_view_state{ db_view_state } {}

   auto& get_state() { return filter_state; }
   auto& get_filter_callback_state() { return filter_state; }
   auto& get_chaindb_state() { return chaindb_state; }
   auto& get_db_view_state() { return db_view_state; }
};

inline void register_callbacks() {
   b1::rodupcx::chaindb_callbacks<callbacks>::register_callbacks<rhf_t>();
   b1::rodupcx::compiler_builtins_callbacks<callbacks>::register_callbacks<rhf_t>();
   b1::rodupcx::console_callbacks<callbacks>::register_callbacks<rhf_t>();
   b1::rodupcx::context_free_system_callbacks<callbacks>::register_callbacks<rhf_t>();
   b1::rodupcx::data_callbacks<callbacks>::register_callbacks<rhf_t>();
   b1::rodupcx::db_callbacks<callbacks>::register_callbacks<rhf_t>();
   b1::rodupcx::filter_callbacks<callbacks>::register_callbacks<rhf_t>();
   b1::rodupcx::memory_callbacks<callbacks>::register_callbacks<rhf_t>();
   b1::rodupcx::unimplemented_callbacks<callbacks>::register_callbacks<rhf_t>();
   b1::rodupcx::unimplemented_filter_callbacks<callbacks>::register_callbacks<rhf_t>();
}

} // namespace b1::rodupcx::filter
