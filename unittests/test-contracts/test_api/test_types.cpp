#include <upcxio/upcxio.hpp>

#include "test_api.hpp"

void test_types::types_size() {

   upcxio_assert( sizeof(int64_t)   ==  8, "int64_t size != 8"   );
   upcxio_assert( sizeof(uint64_t)  ==  8, "uint64_t size != 8"  );
   upcxio_assert( sizeof(uint32_t)  ==  4, "uint32_t size != 4"  );
   upcxio_assert( sizeof(int32_t)   ==  4, "int32_t size != 4"   );
   upcxio_assert( sizeof(uint128_t) == 16, "uint128_t size != 16");
   upcxio_assert( sizeof(int128_t)  == 16, "int128_t size != 16" );
   upcxio_assert( sizeof(uint8_t)   ==  1, "uint8_t size != 1"   );

   upcxio_assert( sizeof(upcxio::name) ==  8, "name size !=  8");
}

void test_types::char_to_symbol() {

   upcxio_assert( upcxio::name::char_to_value('1') ==  1, "upcxio::char_to_symbol('1') !=  1" );
   upcxio_assert( upcxio::name::char_to_value('2') ==  2, "upcxio::char_to_symbol('2') !=  2" );
   upcxio_assert( upcxio::name::char_to_value('3') ==  3, "upcxio::char_to_symbol('3') !=  3" );
   upcxio_assert( upcxio::name::char_to_value('4') ==  4, "upcxio::char_to_symbol('4') !=  4" );
   upcxio_assert( upcxio::name::char_to_value('5') ==  5, "upcxio::char_to_symbol('5') !=  5" );
   upcxio_assert( upcxio::name::char_to_value('a') ==  6, "upcxio::char_to_symbol('a') !=  6" );
   upcxio_assert( upcxio::name::char_to_value('b') ==  7, "upcxio::char_to_symbol('b') !=  7" );
   upcxio_assert( upcxio::name::char_to_value('c') ==  8, "upcxio::char_to_symbol('c') !=  8" );
   upcxio_assert( upcxio::name::char_to_value('d') ==  9, "upcxio::char_to_symbol('d') !=  9" );
   upcxio_assert( upcxio::name::char_to_value('e') == 10, "upcxio::char_to_symbol('e') != 10" );
   upcxio_assert( upcxio::name::char_to_value('f') == 11, "upcxio::char_to_symbol('f') != 11" );
   upcxio_assert( upcxio::name::char_to_value('g') == 12, "upcxio::char_to_symbol('g') != 12" );
   upcxio_assert( upcxio::name::char_to_value('h') == 13, "upcxio::char_to_symbol('h') != 13" );
   upcxio_assert( upcxio::name::char_to_value('i') == 14, "upcxio::char_to_symbol('i') != 14" );
   upcxio_assert( upcxio::name::char_to_value('j') == 15, "upcxio::char_to_symbol('j') != 15" );
   upcxio_assert( upcxio::name::char_to_value('k') == 16, "upcxio::char_to_symbol('k') != 16" );
   upcxio_assert( upcxio::name::char_to_value('l') == 17, "upcxio::char_to_symbol('l') != 17" );
   upcxio_assert( upcxio::name::char_to_value('m') == 18, "upcxio::char_to_symbol('m') != 18" );
   upcxio_assert( upcxio::name::char_to_value('n') == 19, "upcxio::char_to_symbol('n') != 19" );
   upcxio_assert( upcxio::name::char_to_value('o') == 20, "upcxio::char_to_symbol('o') != 20" );
   upcxio_assert( upcxio::name::char_to_value('p') == 21, "upcxio::char_to_symbol('p') != 21" );
   upcxio_assert( upcxio::name::char_to_value('q') == 22, "upcxio::char_to_symbol('q') != 22" );
   upcxio_assert( upcxio::name::char_to_value('r') == 23, "upcxio::char_to_symbol('r') != 23" );
   upcxio_assert( upcxio::name::char_to_value('s') == 24, "upcxio::char_to_symbol('s') != 24" );
   upcxio_assert( upcxio::name::char_to_value('t') == 25, "upcxio::char_to_symbol('t') != 25" );
   upcxio_assert( upcxio::name::char_to_value('u') == 26, "upcxio::char_to_symbol('u') != 26" );
   upcxio_assert( upcxio::name::char_to_value('v') == 27, "upcxio::char_to_symbol('v') != 27" );
   upcxio_assert( upcxio::name::char_to_value('w') == 28, "upcxio::char_to_symbol('w') != 28" );
   upcxio_assert( upcxio::name::char_to_value('x') == 29, "upcxio::char_to_symbol('x') != 29" );
   upcxio_assert( upcxio::name::char_to_value('y') == 30, "upcxio::char_to_symbol('y') != 30" );
   upcxio_assert( upcxio::name::char_to_value('z') == 31, "upcxio::char_to_symbol('z') != 31" );

   for(unsigned char i = 0; i<255; i++) {
      if( (i >= 'a' && i <= 'z') || (i >= '1' || i <= '5') ) continue;
      upcxio_assert( upcxio::name::char_to_value((char)i) == 0, "upcxio::char_to_symbol() != 0" );
   }
}

void test_types::string_to_name() {
   return;
   upcxio_assert( upcxio::name("a") == "a"_n, "upcxio::string_to_name(a)" );
   upcxio_assert( upcxio::name("ba") == "ba"_n, "upcxio::string_to_name(ba)" );
   upcxio_assert( upcxio::name("cba") == "cba"_n, "upcxio::string_to_name(cba)" );
   upcxio_assert( upcxio::name("dcba") == "dcba"_n, "upcxio::string_to_name(dcba)" );
   upcxio_assert( upcxio::name("edcba") == "edcba"_n, "upcxio::string_to_name(edcba)" );
   upcxio_assert( upcxio::name("fedcba") == "fedcba"_n, "upcxio::string_to_name(fedcba)" );
   upcxio_assert( upcxio::name("gfedcba") == "gfedcba"_n, "upcxio::string_to_name(gfedcba)" );
   upcxio_assert( upcxio::name("hgfedcba") == "hgfedcba"_n, "upcxio::string_to_name(hgfedcba)" );
   upcxio_assert( upcxio::name("ihgfedcba") == "ihgfedcba"_n, "upcxio::string_to_name(ihgfedcba)" );
   upcxio_assert( upcxio::name("jihgfedcba") == "jihgfedcba"_n, "upcxio::string_to_name(jihgfedcba)" );
   upcxio_assert( upcxio::name("kjihgfedcba") == "kjihgfedcba"_n, "upcxio::string_to_name(kjihgfedcba)" );
   upcxio_assert( upcxio::name("lkjihgfedcba") == "lkjihgfedcba"_n, "upcxio::string_to_name(lkjihgfedcba)" );
   upcxio_assert( upcxio::name("mlkjihgfedcba") == "mlkjihgfedcba"_n, "upcxio::string_to_name(mlkjihgfedcba)" );
}
