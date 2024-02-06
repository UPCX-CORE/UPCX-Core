#pragma once

#include <upcx/chain/types.hpp>
#include <upcx/chain/webassembly/upcx-vm-oc/upcx-vm-oc.h>

#include <exception>

#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>

#include <vector>
#include <list>

namespace upcx { namespace chain {

class apply_context;

namespace upcxvmoc {

using control_block = upcx_vm_oc_control_block;

struct no_offset{};
struct code_offset {
   size_t offset; 
};    
struct intrinsic_ordinal { 
   size_t ordinal; 
};

using upcxvmoc_optional_offset_or_import_t = std::variant<no_offset, code_offset, intrinsic_ordinal>;

struct code_descriptor {
   digest_type code_hash;
   uint8_t vm_version;
   uint8_t codegen_version;
   size_t code_begin;
   upcxvmoc_optional_offset_or_import_t start;
   unsigned apply_offset;
   int starting_memory_pages;
   size_t initdata_begin;
   unsigned initdata_size;
   unsigned initdata_prologue_size;
};

enum upcxvmoc_exitcode : int {
   UPCXVMOC_EXIT_CLEAN_EXIT = 1,
   UPCXVMOC_EXIT_CHECKTIME_FAIL,
   UPCXVMOC_EXIT_SEGV,
   UPCXVMOC_EXIT_EXCEPTION
};

}}}

FC_REFLECT(upcx::chain::upcxvmoc::no_offset, );
FC_REFLECT(upcx::chain::upcxvmoc::code_offset, (offset));
FC_REFLECT(upcx::chain::upcxvmoc::intrinsic_ordinal, (ordinal));
FC_REFLECT(upcx::chain::upcxvmoc::code_descriptor, (code_hash)(vm_version)(codegen_version)(code_begin)(start)(apply_offset)(starting_memory_pages)(initdata_begin)(initdata_size)(initdata_prologue_size));

#define UPCXVMOC_INTRINSIC_INIT_PRIORITY __attribute__((init_priority(198)))
