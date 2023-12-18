#include <b1/rodupcx/embedded_rodupcx.h>
#include <b1/rodupcx/rodupcx.hpp>
#include <fc/scoped_exit.hpp>

struct rodupcx_error_s {
   const char* msg = "no error";
   std::string buffer;

   bool set(const char* m) {
      try {
         buffer = m;
         msg    = buffer.c_str();
      } catch (...) { msg = "error storing error message"; }
      return false;
   }
};

struct rodupcx_context_s : b1::rodupcx::rodupcx_context {};

struct rodupcx_db_partition_s {
   std::shared_ptr<b1::rodupcx::rodupcx_db_partition> obj;
};

struct rodupcx_db_snapshot_s : b1::rodupcx::rodupcx_db_snapshot {
   using rodupcx_db_snapshot::rodupcx_db_snapshot;
};

struct rodupcx_filter_s : b1::rodupcx::rodupcx_filter {
   using rodupcx_filter::rodupcx_filter;
};

struct rodupcx_query_handler_s : b1::rodupcx::rodupcx_query_handler {
   using rodupcx_query_handler::rodupcx_query_handler;
};

extern "C" rodupcx_error* rodupcx_create_error() {
   try {
      return std::make_unique<rodupcx_error>().release();
   } catch (...) { return nullptr; }
}

extern "C" void rodupcx_destroy_error(rodupcx_error* error) { std::unique_ptr<rodupcx_error>{ error }; }

extern "C" const char* rodupcx_get_error(rodupcx_error* error) {
   if (!error)
      return "error is null";
   return error->msg;
}

template <typename T, typename F>
auto handle_exceptions(rodupcx_error* error, T errval, F f) noexcept -> decltype(f()) {
   if (!error)
      return errval;
   try {
      return f();
   } catch (std::exception& e) {
      error->set(e.what());
      return errval;
   } catch (...) {
      error->set("unknown exception");
      return errval;
   }
}

extern "C" rodupcx_context* rodupcx_create() {
   try {
      return std::make_unique<rodupcx_context>().release();
   } catch (...) { return nullptr; }
}

extern "C" void rodupcx_destroy(rodupcx_context* context) { std::unique_ptr<rodupcx_context>{ context }; }

extern "C" rodupcx_bool rodupcx_open_db(rodupcx_error* error, rodupcx_context* context, const char* path,
                                      rodupcx_bool create_if_missing, int num_threads, int max_open_files) {
   return handle_exceptions(error, false, [&] {
      if (!context)
         return error->set("context is null");
      if (!path)
         return error->set("path is null");
      if (context->db)
         return error->set("a database is already open on this context");
      context->db = std::make_shared<b1::chain_kv::database>(
            path, create_if_missing, num_threads ? std::make_optional(num_threads) : std::nullopt,
            max_open_files ? std::make_optional(max_open_files) : std::nullopt);
      return true;
   });
}

extern "C" rodupcx_db_partition* rodupcx_create_partition(rodupcx_error* error, rodupcx_context* context,
                                                        const char* prefix, uint32_t prefix_size) {
   return handle_exceptions(error, nullptr, [&]() -> rodupcx_db_partition* {
      if (!context)
         return error->set("context is null"), nullptr;
      if (!prefix)
         return error->set("prefix is null"), nullptr;
      if (!context->db)
         return error->set("database wasn't opened"), nullptr;
      auto p = std::make_unique<rodupcx_db_partition>();
      p->obj = std::make_shared<b1::rodupcx::rodupcx_db_partition>(context->db,
                                                                 std::vector<char>{ prefix, prefix + prefix_size });
      return p.release();
   });
}

extern "C" void rodupcx_destroy_partition(rodupcx_db_partition* partition) {
   std::unique_ptr<rodupcx_db_partition>{ partition };
}

extern "C" rodupcx_db_snapshot* rodupcx_create_snapshot(rodupcx_error* error, rodupcx_db_partition* partition,
                                                      rodupcx_bool persistent) {
   return handle_exceptions(error, nullptr, [&]() -> rodupcx_db_snapshot* {
      if (!partition)
         return error->set("partition is null"), nullptr;
      return std::make_unique<rodupcx_db_snapshot>(partition->obj, persistent).release();
   });
}

extern "C" void rodupcx_destroy_snapshot(rodupcx_db_snapshot* snapshot) {
   std::unique_ptr<rodupcx_db_snapshot>{ snapshot };
}

extern "C" rodupcx_bool rodupcx_refresh_snapshot(rodupcx_error* error, rodupcx_db_snapshot* snapshot) {
   return handle_exceptions(error, false, [&]() {
      if (!snapshot)
         return error->set("snapshot is null");
      snapshot->refresh();
      return true;
   });
}

template <typename F>
void with_result(const char* data, uint64_t size, F f) {
   upcxio::input_stream          bin{ data, data + size };
   upcxio::ship_protocol::result result;
   from_bin(result, bin);
   auto* result_v0 = std::get_if<upcxio::ship_protocol::get_blocks_result_v0>(&result);
   if (result_v0)
      return f(*result_v0);

   auto* result_v1 = std::get_if<upcxio::ship_protocol::get_blocks_result_v1>(&result);
   if (result_v1)
      return f(*result_v1);

   throw std::runtime_error("expected a get_blocks_result_v0 or get_blocks_result_v1");
}

extern "C" rodupcx_bool rodupcx_start_block(rodupcx_error* error, rodupcx_db_snapshot* snapshot, const char* data,
                                          uint64_t size) {
   return handle_exceptions(error, false, [&]() {
      if (!snapshot)
         return error->set("snapshot is null");
      with_result(data, size, [&](auto& result) { snapshot->start_block(result); });
      return true;
   });
}

extern "C" rodupcx_bool rodupcx_end_block(rodupcx_error* error, rodupcx_db_snapshot* snapshot, const char* data,
                                        uint64_t size, bool force_write) {
   return handle_exceptions(error, false, [&]() {
      if (!snapshot)
         return error->set("snapshot is null");
      with_result(data, size, [&](auto& result) { snapshot->end_block(result, force_write); });
      return true;
   });
}

extern "C" rodupcx_bool rodupcx_write_block_info(rodupcx_error* error, rodupcx_db_snapshot* snapshot, const char* data,
                                               uint64_t size) {
   return handle_exceptions(error, false, [&]() {
      if (!snapshot)
         return error->set("snapshot is null");
      with_result(data, size, [&](auto& result) { snapshot->write_block_info(result); });
      return true;
   });
}

extern "C" rodupcx_bool rodupcx_write_deltas(rodupcx_error* error, rodupcx_db_snapshot* snapshot, const char* data,
                                           uint64_t size, rodupcx_bool (*shutdown)(void*), void* shutdown_arg) {
   return handle_exceptions(error, false, [&]() {
      if (!snapshot)
         return error->set("snapshot is null");
      with_result(data, size, [&](auto& result) {
         snapshot->write_deltas(result, [=]() -> bool {
            if (shutdown)
               return shutdown(shutdown_arg);
            else
               return false;
         });
      });
      return true;
   });
}

extern "C" rodupcx_filter* rodupcx_create_filter(rodupcx_error* error, uint64_t name, const char* wasm_filename) {
   return handle_exceptions(error, nullptr, [&]() -> rodupcx_filter* { //
      return std::make_unique<rodupcx_filter>(upcxio::name{ name }, wasm_filename).release();
   });
}

extern "C" void rodupcx_destroy_filter(rodupcx_filter* filter) { std::unique_ptr<rodupcx_filter>{ filter }; }

extern "C" rodupcx_bool rodupcx_run_filter(rodupcx_error* error, rodupcx_db_snapshot* snapshot, rodupcx_filter* filter,
                                         const char* data, uint64_t size,
                                         rodupcx_bool (*push_data)(void* arg, const char* data, uint64_t size),
                                         void* push_data_arg) {
   return handle_exceptions(error, false, [&]() {
      if (!snapshot)
         return error->set("snapshot is null");
      if (!filter)
         return error->set("filter is null");
      with_result(data, size, [&](auto& result) {
         filter->process(*snapshot, result, { data, data + size }, [&](const char* data, uint64_t size) {
            if (push_data && !push_data(push_data_arg, data, size))
               throw std::runtime_error("push_data returned false");
         });
      });
      return true;
   });
}

extern "C" rodupcx_query_handler* rodupcx_create_query_handler(rodupcx_error* error, rodupcx_db_partition* partition,
                                                             uint32_t max_console_size, uint32_t wasm_cache_size,
                                                             uint64_t max_exec_time_ms, const char* contract_dir) {
   return handle_exceptions(error, nullptr, [&]() -> rodupcx_query_handler* {
      if (!partition)
         return error->set("partition is null"), nullptr;
      auto shared_state              = std::make_shared<b1::rodupcx::wasm_ql::shared_state>(partition->obj->db);
      shared_state->max_console_size = max_console_size;
      shared_state->wasm_cache_size  = wasm_cache_size;
      shared_state->max_exec_time_ms = max_exec_time_ms;
      shared_state->contract_dir     = contract_dir ? contract_dir : "";
      return std::make_unique<rodupcx_query_handler>(partition->obj, shared_state).release();
   });
}

void rodupcx_destroy_query_handler(rodupcx_query_handler* handler) { std::unique_ptr<rodupcx_query_handler>{ handler }; }

rodupcx_bool rodupcx_query_transaction(rodupcx_error* error, rodupcx_query_handler* handler, rodupcx_db_snapshot* snapshot,
                                     const char* data, uint64_t size, char** result, uint64_t* result_size) {
   return handle_exceptions(error, false, [&]() {
      if (!handler)
         return error->set("handler is null");
      if (!result)
         return error->set("result is null");
      if (!result_size)
         return error->set("result_size is null");
      *result      = nullptr;
      *result_size = 0;

      std::vector<std::vector<char>> memory;
      upcxio::input_stream            s{ data, size };
      auto trx = upcxio::from_bin<upcxio::ship_protocol::packed_transaction>(s);

      auto                                    thread_state = handler->state_cache.get_state();
      upcxio::ship_protocol::transaction_trace tt;
      if (snapshot->snap) {
         tt = query_send_transaction(*thread_state, snapshot->partition->contract_kv_prefix, trx,
                                     snapshot->snap->snapshot(), memory, true);
      } else {
         tt = query_send_transaction(*thread_state, snapshot->partition->contract_kv_prefix, trx, nullptr, memory,
                                     true);
      }

      handler->state_cache.store_state(std::move(thread_state));

      upcxio::size_stream ss;
      upcxio::to_bin(tt, ss);
      *result = (char*)malloc(ss.size);
      if (!result)
         throw std::bad_alloc();
      auto free_on_except = fc::make_scoped_exit([&]{
         free(*result);
         *result = nullptr;
      });
      upcxio::fixed_buf_stream fbs(*result, ss.size);
      to_bin(tt, fbs);
      if (fbs.pos != fbs.end) {
         upcxio::check(false, upcxio::convert_stream_error(upcxio::stream_error::underrun));
      }
      *result_size = ss.size;
      free_on_except.cancel();
      return true;
   });
}

void rodupcx_free_result(char* result) {
   if (result)
      free(result);
}
