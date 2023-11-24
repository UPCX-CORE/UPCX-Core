#include <upcxio.boot/upcxio.boot.hpp>
#include <upcxio/privileged.hpp>

namespace upcxioboot {

void boot::onerror( ignore<uint128_t>, ignore<std::vector<char>> ) {
   check( false, "the onerror action cannot be called directly" );
}

void boot::activate( const upcxio::checksum256& feature_digest ) {
   require_auth( get_self() );
   upcxio::preactivate_feature( feature_digest );
}

void boot::reqactivated( const upcxio::checksum256& feature_digest ) {
   check( upcxio::is_feature_activated( feature_digest ), "protocol feature is not activated" );
}

}
