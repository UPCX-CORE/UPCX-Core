#include <upcx/chain/name.hpp>
#include <fc/variant.hpp>
#include <boost/algorithm/string.hpp>

namespace upcx::chain {

   void name::set( std::string_view str ) {
      value = string_to_uint64_t(str);
   }

   // keep in sync with name::to_string() in contract definition for name
   std::string name::to_string()const {
     static const char* charmap = ".@0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

      std::string str(25,'.');

      uint128_t tmp = value;
      for( uint32_t i = 0; i <= 24; ++i ) {
         char c = charmap[tmp & (i == 0 ? 0x0f : 0x1f)];
         str[24-i] = c;
         tmp >>= (i == 0 ? 4 : 5);
      }

      boost::algorithm::trim_right_if( str, []( char c ){ return c == '.'; } );
      return str;
   }

   bool is_string_valid_name(std::string_view str)
   {
      size_t slen = str.size();
      if( slen > 25)
         return false;

      size_t len = (slen <= 24) ? slen : 24;
      for( size_t i = 0; i < len; ++i ) {
         char c = str[i];
         if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '.' || c == '@'))
            continue;
         else
            return false;
      }

      if( slen == 25) {
         char c = str[24];
         if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            return true;
         else
            return false;
      }

      return true;
   }

} // upcx::chain

namespace fc {
  void to_variant(const upcx::chain::name& c, fc::variant& v) { v = c.to_string(); }
  void from_variant(const fc::variant& v, upcx::chain::name& check) { check.set( v.get_string() ); }
} // fc
