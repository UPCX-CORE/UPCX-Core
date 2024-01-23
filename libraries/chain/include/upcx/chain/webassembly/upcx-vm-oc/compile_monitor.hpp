#pragma once

#include <upcx/chain/webassembly/upcx-vm-oc/config.hpp>

#include <boost/asio/local/datagram_protocol.hpp>
#include <upcx/chain/webassembly/upcx-vm-oc/ipc_helpers.hpp>

namespace upcx { namespace chain { namespace upcxvmoc {

wrapped_fd get_connection_to_compile_monitor(int cache_fd);

}}}