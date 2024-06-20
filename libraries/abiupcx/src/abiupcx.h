// copyright defined in abiupcx/LICENSE.txt

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct abiupcx_context_s abiupcx_context;
typedef int abiupcx_bool;

// Create a context. The context holds all memory allocated by functions in this header. Returns null on failure.
abiupcx_context* abiupcx_create();

// Destroy a context.
void abiupcx_destroy(abiupcx_context* context);

// Get last error. Never returns null. The context owns the returned string.
const char* abiupcx_get_error(abiupcx_context* context);

// Get generated binary. The context owns the returned memory. Functions return null on error; use abiupcx_get_error to
// retrieve error.
int abiupcx_get_bin_size(abiupcx_context* context);
const char* abiupcx_get_bin_data(abiupcx_context* context);

// Convert generated binary to hex. The context owns the returned string. Returns null on error; use abiupcx_get_error to
// retrieve error.
const char* abiupcx_get_bin_hex(abiupcx_context* context);

// Name conversion. The context owns the returned memory. Functions return null on error; use abiupcx_get_error to
// retrieve error.
uint64_t abiupcx_string_to_name(abiupcx_context* context, const char* str);
const char* abiupcx_name_to_string(abiupcx_context* context, uint64_t name);

// Set abi (JSON format). Returns false on error.
abiupcx_bool abiupcx_set_abi(abiupcx_context* context, uint64_t contract, const char* abi);

// Set abi (binary format). Returns false on error.
abiupcx_bool abiupcx_set_abi_bin(abiupcx_context* context, uint64_t contract, const char* data, size_t size);

// Set abi (hex format). Returns false on error.
abiupcx_bool abiupcx_set_abi_hex(abiupcx_context* context, uint64_t contract, const char* hex);

// Get the type name for an action. The context owns the returned memory. Returns null on error; use abiupcx_get_error
// to retrieve error.
const char* abiupcx_get_type_for_action(abiupcx_context* context, uint64_t contract, uint64_t action);

// Get the type name for a table. The context owns the returned memory. Returns null on error; use abiupcx_get_error
// to retrieve error.
const char* abiupcx_get_type_for_table(abiupcx_context* context, uint64_t contract, uint64_t table);

// Get the definition for a kv table in json. The context owns the returned memory. Returns null on error; use
// abiupcx_get_error to retrieve error.
const char* abiupcx_get_kv_table_def(abiupcx_context* context, uint64_t contract, uint64_t table);

// Get the type name for an action_result. The context owns the returned memory. Returns null on error; use
// abiupcx_get_error to retrieve error.
const char* abiupcx_get_type_for_action_result(abiupcx_context* context, uint64_t contract, uint64_t action_result);

// Convert json to binary. Use abiupcx_get_bin_* to retrieve result. Returns false on error.
abiupcx_bool abiupcx_json_to_bin(abiupcx_context* context, uint64_t contract, const char* type, const char* json);

// Convert json to binary. Allow json field reordering. Use abiupcx_get_bin_* to retrieve result. Returns false on error.
abiupcx_bool abiupcx_json_to_bin_reorderable(abiupcx_context* context, uint64_t contract, const char* type,
                                           const char* json);

// Convert binary to json. The context owns the returned string. Returns null on error; use abiupcx_get_error to retrieve
// error.
const char* abiupcx_bin_to_json(abiupcx_context* context, uint64_t contract, const char* type, const char* data,
                               size_t size);

// Convert hex to json. The context owns the returned memory. Returns null on error; use abiupcx_get_error to retrieve
// error.
const char* abiupcx_hex_to_json(abiupcx_context* context, uint64_t contract, const char* type, const char* hex);

// Convert abi json to bin, Use abiupcx_get_bin_* to retrieve result. Returns false on error.
abiupcx_bool abiupcx_abi_json_to_bin(abiupcx_context* context, const char* json);

// Convert abi bin to json, The context.result_str has the result, Returns null on error; use abiupcx_get_error to
// retrieve
const char* abiupcx_abi_bin_to_json(abiupcx_context* context, const char* abi_bin_data, const size_t abi_bin_data_size);

#ifdef __cplusplus
}
#endif
