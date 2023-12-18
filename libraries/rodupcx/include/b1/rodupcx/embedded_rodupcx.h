#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rodupcx_error_s         rodupcx_error;
typedef struct rodupcx_context_s       rodupcx_context;
typedef struct rodupcx_db_partition_s  rodupcx_db_partition;
typedef struct rodupcx_db_snapshot_s   rodupcx_db_snapshot;
typedef struct rodupcx_filter_s        rodupcx_filter;
typedef struct rodupcx_query_handler_s rodupcx_query_handler;
typedef int                           rodupcx_bool;

// Create an error object. If multiple threads use an error object, then they must synchronize it. Returns NULL on
// failure.
rodupcx_error* rodupcx_create_error();

// Destroy an error object. This is a no-op if error == NULL.
void rodupcx_destroy_error(rodupcx_error* error);

// Get last error in this object. Never returns NULL. The error object owns the returned string.
const char* rodupcx_get_error(rodupcx_error* error);

// Create a context. Returns NULL on failure.
rodupcx_context* rodupcx_create();

// Destroy a context. It is undefined behavior if the context is used between threads without synchronization, or if any
// other objects currently exist for this context. This is a no-op if context == NULL.
void rodupcx_destroy(rodupcx_context* context);

// Open database. num_threads is the target number of rocksdb background threads; use 0 for default. max_open_files is
// the max number of open rocksdb files; 0 to make this unlimited.
//
// It is undefined behavior if the context is used between threads without synchronization. Returns false on error.
rodupcx_bool rodupcx_open_db(rodupcx_error* error, rodupcx_context* context, const char* path,
                           rodupcx_bool create_if_missing, int num_threads, int max_open_files);

// Create or open a database partition. It is undefined behavior if more than 1 partition is opened for a given prefix,
// if any partitions have overlapping prefixes, or if the context is used between threads without synchronization.
// Returns NULL on failure.
rodupcx_db_partition* rodupcx_create_partition(rodupcx_error* error, rodupcx_context* context, const char* prefix,
                                             uint32_t prefix_size);

// Destroy a partition. It is undefined behavior if the partition is used between threads without synchronization. This
// is a no-op if partition == NULL.
void rodupcx_destroy_partition(rodupcx_db_partition* partition);

// Create a database snapshot. Snapshots isolate changes from each other. All database reads and writes happen through
// snapshots. Snapshot objects may safely outlive partition objects.
//
// A single partition supports any number of simultaneous non-persistent snapshots, but only a single persistent
// snapshot at any time. persistent and non-persistent may coexist. Only persistent snapshots make permanent changes to
// the database.
//
// Each snapshot may be used by a different thread, even if they're created from a single partition.
//
// It is undefined behavior if more than 1 persistent snapshot exists on a partition, or if the partition is used
// between threads without synchronization. Returns NULL on failure.
rodupcx_db_snapshot* rodupcx_create_snapshot(rodupcx_error* error, rodupcx_db_partition* partition, rodupcx_bool persistent);

// Destroy a snapshot. It is undefined behavior if the snapshot is used between threads without synchronization. This is
// a no-op if snapshot == NULL.
void rodupcx_destroy_snapshot(rodupcx_db_snapshot* snapshot);

// Refresh a snapshot so it may read recently-written database changes. This operation is invalid if the snapshot is
// persistent. It is undefined behavior if the snapshot is used between threads without synchronization.
rodupcx_bool rodupcx_refresh_snapshot(rodupcx_error* error, rodupcx_db_snapshot* snapshot);

// Start writing a block. Aborts any block in progress and rolls back reversible blocks if needed. `data` must be the
// serialized `result` type defined by the state-history plugin's ABI. Currently only supports `get_blocks_result_v0`.
// It is undefined behavior if the snapshot is used between threads without synchronization.
rodupcx_bool rodupcx_start_block(rodupcx_error* error, rodupcx_db_snapshot* snapshot, const char* data, uint64_t size);

// Finish writing a block. `data` must be the serialized `result` type defined by the state-history plugin's ABI.
// Currently only supports `get_blocks_result_v0`. If `force_write` is true, then the data will become immediately
// available to newly-created or newly-refreshed snapshots to read. If `force_write` is false, then the write may be
// delayed until a future rodupcx_end_block call. It is undefined behavior if the snapshot is used between threads
// without synchronization.
rodupcx_bool rodupcx_end_block(rodupcx_error* error, rodupcx_db_snapshot* snapshot, const char* data, uint64_t size,
                             bool force_write);

// Write block info. `data` must be the serialized `result` type defined by the state-history plugin's ABI. Currently
// only supports `get_blocks_result_v0`. If `rodupcx_write_block_info` returns false, the snapshot will be in an
// inconsistent state; call `start_block` to abandon the current write and start another. It is undefined behavior if
// the snapshot is used between threads without synchronization.
rodupcx_bool rodupcx_write_block_info(rodupcx_error* error, rodupcx_db_snapshot* snapshot, const char* data, uint64_t size);

// Write state-history deltas to a block. `data` must be the serialized `result` type defined by the state-history
// plugin's ABI. Currently only supports `get_blocks_result_v0`. If `shutdown` isn't null, then `rodupcx_write_deltas`
// may call it during long operations. If `shutdown` returns true, then `rodupcx_write_deltas` abandons the writes. If
// `rodupcx_write_deltas` returns false, the snapshot will be in an inconsistent state; call `start_block` to abandon the
// current write and start another. It is undefined behavior if the snapshot is used between threads without
// synchronization.
rodupcx_bool rodupcx_write_deltas(rodupcx_error* error, rodupcx_db_snapshot* snapshot, const char* data, uint64_t size,
                                rodupcx_bool (*shutdown)(void*), void* shutdown_arg);

// Create a filter. Returns NULL on failure.
rodupcx_filter* rodupcx_create_filter(rodupcx_error* error, uint64_t name, const char* wasm_filename);

// Destroy a filter. It is undefined behavior if the filter is used between threads without synchronization. This is a
// no-op if filter == NULL.
void rodupcx_destroy_filter(rodupcx_filter* filter);

// Run filter. data must be the serialized `result` type defined by the state-history plugin's ABI. The `push_data`
// callback receives data from the filter's `push_data` intrinsic. If `push_data` is null, then the intrinsic is a
// no-op.
//
// If `rodupcx_run_filter` returns false, the snapshot will be in an inconsistent state; call `start_block` to abandon
// the current write and start another. It is undefined behavior if `snapshot` or `filter` is used between threads
// without synchronization.
rodupcx_bool rodupcx_run_filter(rodupcx_error* error, rodupcx_db_snapshot* snapshot, rodupcx_filter* filter,
                              const char* data, uint64_t size,
                              rodupcx_bool (*push_data)(void* arg, const char* data, uint64_t size),
                              void* push_data_arg);

// Create a query handler. This object manages pools of resources for running queries simultaneously.
//
// Query handlers may safely outlive partition objects. It is undefined behavior if the partition is used between
// threads without synchronization. Returns NULL on failure.
//
// TODO: remove partition arg; redundant with snapshot in rodupcx_query_transaction
rodupcx_query_handler* rodupcx_create_query_handler(rodupcx_error* error, rodupcx_db_partition* partition,
                                                  uint32_t max_console_size, uint32_t wasm_cache_size,
                                                  uint64_t max_exec_time_ms, const char* contract_dir);

// Destroy a query handler. It is undefined behavior if the handler is used between threads without synchronization.
// This is a no-op if handler == NULL.
void rodupcx_destroy_query_handler(rodupcx_query_handler* handler);

// Run a query. data is a serialized ship_protocol::packed_transaction. Returns false on serious error and sets *result
// to NULL and *result_size to 0. Otherwise, sets *result and *result_size to memory containing a serialized
// ship_protocol::transaction_trace. If the query failed, the error result will be in the transaction trace. Caller must
// use rodupcx_free_result to free the memory.
//
// It is safe to use the same handler from multiple threads if:
// * The return from rodupcx_create_query_handler happens-before any calls to rodupcx_query_transaction
// * The return from all rodupcx_query_transaction calls happens-before the call to rodupcx_destroy_query_handler
//
// It is undefined behavior if `snapshot` is used between threads without synchronization.
rodupcx_bool rodupcx_query_transaction(rodupcx_error* error, rodupcx_query_handler* handler, rodupcx_db_snapshot* snapshot,
                                     const char* data, uint64_t size, char** result, uint64_t* result_size);

// Frees memory from rodupcx_query_transaction. Does nothing if result == NULL.
void rodupcx_free_result(char* result);

#ifdef __cplusplus
}
#endif
