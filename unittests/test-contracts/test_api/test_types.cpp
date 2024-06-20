#include <upcx/upcx.hpp>

#include "test_api.hpp"

void test_types::types_size() {

   upcx_assert( sizeof(int64_t)   ==  8, "int64_t size != 8"   );
   upcx_assert( sizeof(uint64_t)  ==  8, "uint64_t size != 8"  );
   upcx_assert( sizeof(uint32_t)  ==  4, "uint32_t size != 4"  );
   upcx_assert( sizeof(int32_t)   ==  4, "int32_t size != 4"   );
   upcx_assert( sizeof(uint128_t) == 16, "uint128_t size != 16");
   upcx_assert( sizeof(int128_t)  == 16, "int128_t size != 16" );
   upcx_assert( sizeof(uint8_t)   ==  1, "uint8_t size != 1"   );

   upcx_assert( sizeof(upcx::name) ==  8, "name size !=  8");
}

void test_types::char_to_symbol() {

   upcx_assert( upcx::name::char_to_value('1') ==  1, "upcx::char_to_symbol('1') !=  1" );
   upcx_assert( upcx::name::char_to_value('2') ==  2, "upcx::char_to_symbol('2') !=  2" );
   upcx_assert( upcx::name::char_to_value('3') ==  3, "upcx::char_to_symbol('3') !=  3" );
   upcx_assert( upcx::name::char_to_value('4') ==  4, "upcx::char_to_symbol('4') !=  4" );
   upcx_assert( upcx::name::char_to_value('5') ==  5, "upcx::char_to_symbol('5') !=  5" );
   upcx_assert( upcx::name::char_to_value('a') ==  6, "upcx::char_to_symbol('a') !=  6" );
   upcx_assert( upcx::name::char_to_value('b') ==  7, "upcx::char_to_symbol('b') !=  7" );
   upcx_assert( upcx::name::char_to_value('c') ==  8, "upcx::char_to_symbol('c') !=  8" );
   upcx_assert( upcx::name::char_to_value('d') ==  9, "upcx::char_to_symbol('d') !=  9" );
   upcx_assert( upcx::name::char_to_value('e') == 10, "upcx::char_to_symbol('e') != 10" );
   upcx_assert( upcx::name::char_to_value('f') == 11, "upcx::char_to_symbol('f') != 11" );
   upcx_assert( upcx::name::char_to_value('g') == 12, "upcx::char_to_symbol('g') != 12" );
   upcx_assert( upcx::name::char_to_value('h') == 13, "upcx::char_to_symbol('h') != 13" );
   upcx_assert( upcx::name::char_to_value('i') == 14, "upcx::char_to_symbol('i') != 14" );
   upcx_assert( upcx::name::char_to_value('j') == 15, "upcx::char_to_symbol('j') != 15" );
   upcx_assert( upcx::name::char_to_value('k') == 16, "upcx::char_to_symbol('k') != 16" );
   upcx_assert( upcx::name::char_to_value('l') == 17, "upcx::char_to_symbol('l') != 17" );
   upcx_assert( upcx::name::char_to_value('m') == 18, "upcx::char_to_symbol('m') != 18" );
   upcx_assert( upcx::name::char_to_value('n') == 19, "upcx::char_to_symbol('n') != 19" );
   upcx_assert( upcx::name::char_to_value('o') == 20, "upcx::char_to_symbol('o') != 20" );
   upcx_assert( upcx::name::char_to_value('p') == 21, "upcx::char_to_symbol('p') != 21" );
   upcx_assert( upcx::name::char_to_value('q') == 22, "upcx::char_to_symbol('q') != 22" );
   upcx_assert( upcx::name::char_to_value('r') == 23, "upcx::char_to_symbol('r') != 23" );
   upcx_assert( upcx::name::char_to_value('s') == 24, "upcx::char_to_symbol('s') != 24" );
   upcx_assert( upcx::name::char_to_value('t') == 25, "upcx::char_to_symbol('t') != 25" );
   upcx_assert( upcx::name::char_to_value('u') == 26, "upcx::char_to_symbol('u') != 26" );
   upcx_assert( upcx::name::char_to_value('v') == 27, "upcx::char_to_symbol('v') != 27" );
   upcx_assert( upcx::name::char_to_value('w') == 28, "upcx::char_to_symbol('w') != 28" );
   upcx_assert( upcx::name::char_to_value('x') == 29, "upcx::char_to_symbol('x') != 29" );
   upcx_assert( upcx::name::char_to_value('y') == 30, "upcx::char_to_symbol('y') != 30" );
   upcx_assert( upcx::name::char_to_value('z') == 31, "upcx::char_to_symbol('z') != 31" );

   for(unsigned char i = 0; i<255; i++) {
      if( (i >= 'a' && i <= 'z') || (i >= '1' || i <= '5') ) continue;
      upcx_assert( upcx::name::char_to_value((char)i) == 0, "upcx::char_to_symbol() != 0" );
   }
}

void test_types::string_to_name() {
   return;
   upcx_assert( upcx::name("a") == "a"_n, "upcx::string_to_name(a)" );
   upcx_assert( upcx::name("ba") == "ba"_n, "upcx::string_to_name(ba)" );
   upcx_assert( upcx::name("cba") == "cba"_n, "upcx::string_to_name(cba)" );
   upcx_assert( upcx::name("dcba") == "dcba"_n, "upcx::string_to_name(dcba)" );
   upcx_assert( upcx::name("edcba") == "edcba"_n, "upcx::string_to_name(edcba)" );
   upcx_assert( upcx::name("fedcba") == "fedcba"_n, "upcx::string_to_name(fedcba)" );
   upcx_assert( upcx::name("gfedcba") == "gfedcba"_n, "upcx::string_to_name(gfedcba)" );
   upcx_assert( upcx::name("hgfedcba") == "hgfedcba"_n, "upcx::string_to_name(hgfedcba)" );
   upcx_assert( upcx::name("ihgfedcba") == "ihgfedcba"_n, "upcx::string_to_name(ihgfedcba)" );
   upcx_assert( upcx::name("jihgfedcba") == "jihgfedcba"_n, "upcx::string_to_name(jihgfedcba)" );
   upcx_assert( upcx::name("kjihgfedcba") == "kjihgfedcba"_n, "upcx::string_to_name(kjihgfedcba)" );
   upcx_assert( upcx::name("lkjihgfedcba") == "lkjihgfedcba"_n, "upcx::string_to_name(lkjihgfedcba)" );
   upcx_assert( upcx::name("mlkjihgfedcba") == "mlkjihgfedcba"_n, "upcx::string_to_name(mlkjihgfedcba)" );
}
