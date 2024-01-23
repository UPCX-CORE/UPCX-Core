#include <upcx.boot/upcx.boot.hpp>
#include <upcx/privileged.hpp>

namespace upcxboot {

void boot::onerror( ignore<uint128_t>, ignore<std::vector<char>> ) {
   check( false, "the onerror action cannot be called directly" );
}

void boot::activate( const upcx::checksum256& feature_digest ) {
   require_auth( get_self() );
   upcx::preactivate_feature( feature_digest );
}

void boot::reqactivated( const upcx::checksum256& feature_digest ) {
   check( upcx::is_feature_activated( feature_digest ), "protocol feature is not activated" );
}

}
