#pragma once
#include <b1/chain_kv/chain_kv.hpp>
#include <b1/rodupcx/filter.hpp>
#include <b1/rodupcx/wasm_ql.hpp>
#include <upcx/ship_protocol.hpp>
#include <functional>

namespace b1::rodupcx {

static constexpr char undo_prefix_byte        = 0x01;
static constexpr char contract_kv_prefix_byte = 0x02;

struct rodupcx_context {
   std::shared_ptr<chain_kv::database> db;
};

struct rodupcx_db_partition {
   const std::shared_ptr<chain_kv::database> db;
   const std::vector<char>                   undo_prefix;
   const std::vector<char>                   contract_kv_prefix;

   // todo: move rocksdb::ManagedSnapshot to here to prevent optimization in cloner from
   //       defeating non-persistent snapshots.

   rodupcx_db_partition(std::shared_ptr<chain_kv::database> db, const std::vector<char>& prefix)
       : db{ std::move(db) }, //
         undo_prefix{ [&] {
            auto x = prefix;
            x.push_back(undo_prefix_byte);
            return x;
         }() },
         contract_kv_prefix{ [&] {
            auto x = prefix;
            x.push_back(contract_kv_prefix_byte);
            return x;
         }() } {}
};

struct rodupcx_db_snapshot {
   std::shared_ptr<rodupcx_db_partition>    partition       = {};
   std::shared_ptr<chain_kv::database>     db              = {};
   std::optional<chain_kv::undo_stack>     undo_stack      = {}; // only if persistent
   std::optional<rocksdb::ManagedSnapshot> snap            = {}; // only if !persistent
   std::optional<chain_kv::write_session>  write_session   = {};
   upcx::checksum256                      chain_id        = {};
   uint32_t                                head            = 0;
   upcx::checksum256                      head_id         = {};
   uint32_t                                irreversible    = 0;
   upcx::checksum256                      irreversible_id = {};
   uint32_t                                first           = 0;
   std::optional<uint32_t>                 writing_block   = {};

   rodupcx_db_snapshot(std::shared_ptr<rodupcx_db_partition> partition, bool persistent);

   void refresh();
   void end_write(bool write_fill);
   void start_block(const upcx::ship_protocol::get_blocks_result_base& result);
   void end_block(const upcx::ship_protocol::get_blocks_result_base& result, bool force_write);
   void check_write(const upcx::ship_protocol::get_blocks_result_base& result);
   void write_block_info(const upcx::ship_protocol::get_blocks_result_v0& result);
   void write_block_info(const upcx::ship_protocol::get_blocks_result_v1& result);
   void write_deltas(const upcx::ship_protocol::get_blocks_result_v0& result, std::function<bool()> shutdown);
   void write_deltas(const upcx::ship_protocol::get_blocks_result_v1& result, std::function<bool()> shutdown);

 private:
   void write_block_info(uint32_t block_num, const upcx::checksum256& id,
                         const upcx::ship_protocol::signed_block_header& block);
   void write_deltas(uint32_t block_num, upcx::opaque<std::vector<upcx::ship_protocol::table_delta>> deltas, std::function<bool()> shutdown);
   void write_fill_status();
};

struct rodupcx_filter {
   upcx::name                           name         = {};
   std::unique_ptr<filter::backend_t>    backend      = {};
   std::unique_ptr<filter::filter_state> filter_state = {};

   rodupcx_filter(upcx::name name, const std::string& wasm_filename);

   void process(rodupcx_db_snapshot& snapshot, const upcx::ship_protocol::get_blocks_result_base& result,
                upcx::input_stream bin, const std::function<void(const char* data, uint64_t size)>& push_data);
};

struct rodupcx_query_handler {
   std::shared_ptr<rodupcx_db_partition>               partition;
   const std::shared_ptr<const wasm_ql::shared_state> shared_state;
   wasm_ql::thread_state_cache                        state_cache;

   rodupcx_query_handler(std::shared_ptr<rodupcx_db_partition>         partition,
                        std::shared_ptr<const wasm_ql::shared_state> shared_state);
   rodupcx_query_handler(const rodupcx_query_handler&) = delete;
};

} // namespace b1::rodupcx
