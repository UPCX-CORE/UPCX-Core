#include <boost/test/unit_test.hpp>
#include <upcx/chain/name.hpp>

using namespace upcx::chain;

BOOST_AUTO_TEST_SUITE(name_test)

static constexpr uint64_t u64min = std::numeric_limits<uint64_t>::min(); // 0ULL
static constexpr uint64_t u64max = std::numeric_limits<uint64_t>::max(); // 18446744073709551615ULL

BOOST_AUTO_TEST_CASE(ctor_test) {
try {

   BOOST_TEST( name{}.to_uint64_t() == u64min );

   //// constexpr explicit name(uint64_t);
   BOOST_TEST( name{u64min}.to_uint64_t() == u64min );
   BOOST_TEST( name{1ULL}.to_uint64_t() == 1ULL );
   BOOST_TEST( name{u64max}.to_uint64_t() == u64max );

   //// explicit name(string_view);
   // Note:
   // These are the exact `uint64_t` value representations of the given string
   BOOST_TEST( name{"1"}.to_uint64_t() == 576460752303423488ULL );
   BOOST_TEST( name{"5"}.to_uint64_t() == 2882303761517117440ULL );
   BOOST_TEST( name{"a"}.to_uint64_t() == 3458764513820540928ULL );
   BOOST_TEST( name{"z"}.to_uint64_t() == 17870283321406128128ULL );

   BOOST_TEST( name{"abc"}.to_uint64_t() == 3589368903014285312ULL );
   BOOST_TEST( name{"123"}.to_uint64_t() == 614178399182651392ULL );

   BOOST_TEST( name{".abc"}.to_uint64_t() == 112167778219196416ULL );
   BOOST_TEST( name{".........abc"}.to_uint64_t() == 102016ULL );
   BOOST_TEST( name{"123."}.to_uint64_t() == 614178399182651392ULL );
   BOOST_TEST( name{"123........."}.to_uint64_t() == 614178399182651392ULL );
   BOOST_TEST( name{".a.b.c.1.2.3."}.to_uint64_t() == 108209673814966320ULL );

   BOOST_TEST( name{"abc.123"}.to_uint64_t() == 3589369488740450304ULL );
   BOOST_TEST( name{"123.abc"}.to_uint64_t() == 614181822271586304ULL );

   BOOST_TEST( name{"12345abcdefgd"}.to_uint64_t() == 614251623682315983ULL );
   BOOST_TEST( name{"hijklmnopqrsd"}.to_uint64_t() == 7754926748989239183ULL );
   BOOST_TEST( name{"tuvwxyz.1234d"}.to_uint64_t() == 14895601873741973071ULL );

   BOOST_TEST( name{"111111111111d"}.to_uint64_t() == 595056260442243615ULL );
   BOOST_TEST( name{"555555555555d"}.to_uint64_t() == 2975281302211218015ULL );
   BOOST_TEST( name{"aaaaaaaaaaaad"}.to_uint64_t() == 3570337562653461615ULL );
   BOOST_TEST( name{"zzzzzzzzzzzzd"}.to_uint64_t() == u64max );

   BOOST_CHECK_THROW( name{"-1"}, name_type_exception );
   BOOST_CHECK_THROW( name{"0"}, name_type_exception );
   BOOST_CHECK_THROW( name{"6"}, name_type_exception );
   BOOST_CHECK_THROW( name{"111111111111k"}, name_type_exception );
   BOOST_CHECK_THROW( name{"zzzzzzzzzzzzk"}, name_type_exception );
   BOOST_CHECK_THROW( name{"12345abcdefghd"}, name_type_exception );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE(char_to_symbol_test) {
try {
   // --------------------------------------------
   // static constexpr uint8_t char_to_value(char);
   char c{'.'};
   uint8_t expected_value{}; // Will increment to the expected correct value in the set [0,32);
   BOOST_TEST( char_to_symbol(c) == expected_value );
   ++expected_value;

   for(c = '1'; c <= '5'; ++c) {
      BOOST_TEST( char_to_symbol(c) == expected_value );
      ++expected_value;
   }

   for(c = 'a'; c <= 'z'; ++c) {
      BOOST_TEST( char_to_symbol(c) == expected_value );
      ++expected_value;
   }

   BOOST_CHECK_THROW( char_to_symbol(char{'-'}), name_type_exception );
   BOOST_CHECK_THROW( char_to_symbol(char{'/'}), name_type_exception );
   BOOST_CHECK_THROW( char_to_symbol(char{'6'}), name_type_exception );
   BOOST_CHECK_THROW( char_to_symbol(char{'A'}), name_type_exception );
   BOOST_CHECK_THROW( char_to_symbol(char{'Z'}), name_type_exception );
   BOOST_CHECK_THROW( char_to_symbol(char{'`'}), name_type_exception );
   BOOST_CHECK_THROW( char_to_symbol(char{'{'}), name_type_exception );
} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE(to_string_test) {
try {
   // -------------------------------
   // std::string to_string()const
   BOOST_TEST( name{"1"}.to_string() == "1" );
   BOOST_TEST( name{"5"}.to_string() == "5" );
   BOOST_TEST( name{"a"}.to_string() == "a" );
   BOOST_TEST( name{"z"}.to_string() == "z" );

   BOOST_TEST( name{"abc"}.to_string() == "abc" );
   BOOST_TEST( name{"123"}.to_string() == "123" );

   BOOST_TEST( name{".abc"}.to_string() == ".abc" );
   BOOST_TEST( name{".........abc"}.to_string() == ".........abc" );
   BOOST_TEST( name{"123."}.to_string() == "123" );
   BOOST_TEST( name{"123........."}.to_string() == "123" );
   BOOST_TEST( name{".a.b.c.1.2.3."}.to_string() == ".a.b.c.1.2.3" );

   BOOST_TEST( name{"abc.123"}.to_string() == "abc.123" );
   BOOST_TEST( name{"123.abc"}.to_string() == "123.abc" );

   BOOST_TEST( name{"12345abcdefgd"}.to_string() == "12345abcdefgd" );
   BOOST_TEST( name{"hijklmnopqrsd"}.to_string() == "hijklmnopqrsd" );
   BOOST_TEST( name{"tuvwxyz.1234d"}.to_string() == "tuvwxyz.1234d" );

   BOOST_TEST( name{"111111111111d"}.to_string() == "111111111111d" );
   BOOST_TEST( name{"555555555555d"}.to_string() == "555555555555d" );
   BOOST_TEST( name{"aaaaaaaaaaaad"}.to_string() == "aaaaaaaaaaaad" );
   BOOST_TEST( name{"zzzzzzzzzzzzd"}.to_string() == "zzzzzzzzzzzzd" );
   BOOST_TEST( name{""}.to_string() == "" );
   BOOST_TEST( name{"e"}.to_string() == "e" );
   BOOST_TEST( name{"eo"}.to_string() == "eo" );
   BOOST_TEST( name{"upcx"}.to_string() == "upcx" );
   BOOST_TEST( name{"upcxi"}.to_string() == "upcxi" );
   BOOST_TEST( name{"upcx"}.to_string() == "upcx" );
   BOOST_TEST( name{"upcxa"}.to_string() == "upcxa" );
   BOOST_TEST( name{"upcxac"}.to_string() == "upcxac" );
   BOOST_TEST( name{"upcxacc"}.to_string() == "upcxacc" );
   BOOST_TEST( name{"upcxacco"}.to_string() == "upcxacco" );
   BOOST_TEST( name{"upcxaccou"}.to_string() == "upcxaccou" );
   BOOST_TEST( name{"upcxaccoun"}.to_string() == "upcxaccoun" );
   BOOST_TEST( name{"upcxaccount"}.to_string() == "upcxaccount" );
   BOOST_TEST( name{"upcxaccountd"}.to_string() == "upcxaccountd" );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE(operators_test) {
try {
      // ---------------------------------------
   // constexpr explicit operator bool()const
   // Note that I must be explicit about calling the operator because it is defined as `explicit`
   BOOST_TEST( name{0}.operator bool() == false );
   BOOST_TEST( !name{0}.operator bool() == true );
   BOOST_TEST( name{0}.empty() == true );
   BOOST_TEST( name{0}.good() == false );
   BOOST_TEST( name{1}.operator bool() == true );
   BOOST_TEST( !name{1}.operator bool() == false );
   BOOST_TEST( name{1}.empty() == false );
   BOOST_TEST( name{1}.good() == true );

   BOOST_TEST( name{""}.operator bool() == false );
   BOOST_TEST( !name{""}.operator bool() == true );
   BOOST_TEST( name{""}.empty() == true );
   BOOST_TEST( name{""}.good() == false );
   BOOST_TEST( name{"1"}.operator bool() == true );
   BOOST_TEST( !name{"1"}.operator bool() == false );
   BOOST_TEST( name{"1"}.empty() == false );
   BOOST_TEST( name{"1"}.good() == true );

   // ----------------------------------------------------------
   // friend constexpr bool operator == ( const name& a, const name& b )
   BOOST_TEST( name{"1"} == name{"1"} );
   BOOST_TEST( name{"5"} == name{"5"} );
   BOOST_TEST( name{"a"} == name{"a"} );
   BOOST_TEST( name{"z"} == name{"z"} );

   BOOST_TEST( name{"abc"} == name{"abc"} );
   BOOST_TEST( name{"123"} == name{"123"} );

   BOOST_TEST( name{".abc"} == name{".abc"} );
   BOOST_TEST( name{".........abc"} == name{".........abc"} );
   BOOST_TEST( name{"123."} == name{"123"} );
   BOOST_TEST( name{"123........."} == name{"123"} );
   BOOST_TEST( name{".a.b.c.1.2.3."} == name{".a.b.c.1.2.3"} );

   BOOST_TEST( name{"abc.123"} == name{"abc.123"} );
   BOOST_TEST( name{"123.abc"} == name{"123.abc"} );

   BOOST_TEST( name{"12345abcdefgd"} == name{"12345abcdefgd"} );
   BOOST_TEST( name{"hijklmnopqrsd"} == name{"hijklmnopqrsd"} );
   BOOST_TEST( name{"tuvwxyz.1234d"} == name{"tuvwxyz.1234d"} );

   BOOST_TEST( name{"111111111111d"} == name{"111111111111d"} );
   BOOST_TEST( name{"555555555555d"} == name{"555555555555d"} );
   BOOST_TEST( name{"aaaaaaaaaaaad"} == name{"aaaaaaaaaaaad"} );
   BOOST_TEST( name{"zzzzzzzzzzzzd"} == name{"zzzzzzzzzzzzd"} );

   // -----------------------------------------------------------
   // friend constexpr bool operator != ( const name& a, const name& b )
   BOOST_TEST( name{"1"} != name{} );
   BOOST_TEST( name{"5"} != name{} );
   BOOST_TEST( name{"a"} != name{} );
   BOOST_TEST( name{"z"} != name{} );

   BOOST_TEST( name{"abc"} != name{} );
   BOOST_TEST( name{"123"} != name{} );

   BOOST_TEST( name{".abc"} != name{} );
   BOOST_TEST( name{".........abc"} != name{} );
   BOOST_TEST( name{"123."} != name{} );
   BOOST_TEST( name{"123........."} != name{} );
   BOOST_TEST( name{".a.b.c.1.2.3."} != name{} );

   BOOST_TEST( name{"abc.123"} != name{} );
   BOOST_TEST( name{"123.abc"} != name{} );

   BOOST_TEST( name{"12345abcdefgd"} != name{} );
   BOOST_TEST( name{"hijklmnopqrsd"} != name{} );
   BOOST_TEST( name{"tuvwxyz.1234d"} != name{} );

   BOOST_TEST( name{"111111111111d"} != name{} );
   BOOST_TEST( name{"555555555555d"} != name{} );
   BOOST_TEST( name{"aaaaaaaaaaaad"} != name{} );
   BOOST_TEST( name{"zzzzzzzzzzzzd"} != name{} );

   // ---------------------------------------------------------
   // friend constexpr bool operator < ( const name& a, const name& b )
   BOOST_TEST( name{} < name{"1"} );
   BOOST_TEST( name{"4"} < name{"5"} );
   BOOST_TEST( name{"1"} < name{"a"} );
   BOOST_TEST( name{"y"} < name{"z"} );

   BOOST_TEST( name{"aaa"} < name{"abc"} );
   BOOST_TEST( name{"122"} < name{"123"} );

   BOOST_TEST( name{".111"} < name{".abc"} );
   BOOST_TEST( name{"."} < name{".........abc"} );
   BOOST_TEST( name{"122."} < name{"123."} );
   BOOST_TEST( name{"122."} < name{"123........."} );
   BOOST_TEST( name{".1"} < name{".a.b.c.1.2.3."} );

   BOOST_TEST( name{"abb"} < name{"abc.123"} );
   BOOST_TEST( name{"123.aaa"} < name{"123.abc"} );

   BOOST_TEST( name{"12345abcdefga"} < name{"12345abcdefgd"} );
   BOOST_TEST( name{"hijklmnopqrsh"} < name{"hijklmnopqrsd"} );
   BOOST_TEST( name{"tuvwxyz.1234d"} < name{"tuvwxyz.1234d"} );

   BOOST_TEST( name{"111111111111a"} < name{"111111111111d"} );
   BOOST_TEST( name{"555555555555b"} < name{"555555555555d"} );
   BOOST_TEST( name{"aaaaaaaaaaaac"} < name{"aaaaaaaaaaaad"} );
   BOOST_TEST( name{"zzzzzzzzzzzzc"} < name{"zzzzzzzzzzzzd"} );

   // ---------------------------------------------------------
   // friend constexpr bool operator <= ( const name& a, const name& b )
   BOOST_TEST( name{} <= name{"1"} );
   BOOST_TEST( name{"4"} <= name{"5"} );
   BOOST_TEST( name{"1"} <= name{"a"} );
   BOOST_TEST( name{"y"} <= name{"z"} );

   BOOST_TEST( name{"aaa"} <= name{"abc"} );
   BOOST_TEST( name{"122"} <= name{"123"} );

   BOOST_TEST( name{".111"} <= name{".abc"} );
   BOOST_TEST( name{"."} <= name{".........abc"} );
   BOOST_TEST( name{"122."} <= name{"123."} );
   BOOST_TEST( name{"123."} <= name{"123........."} );
   BOOST_TEST( name{".1"} <= name{".a.b.c.1.2.3."} );

   BOOST_TEST( name{"abb"} <= name{"abc.123"} );
   BOOST_TEST( name{"123.aaa"} <= name{"123.abc"} );

   BOOST_TEST( name{"12345abcdefga"} <= name{"12345abcdefgd"} );
   BOOST_TEST( name{"hijklmnopqrsh"} <= name{"hijklmnopqrsd"} );
   BOOST_TEST( name{"tuvwxyz.1234d"} <= name{"tuvwxyz.1234d"} );

   BOOST_TEST( name{"111111111111a"} <= name{"111111111111d"} );
   BOOST_TEST( name{"555555555555b"} <= name{"555555555555d"} );
   BOOST_TEST( name{"aaaaaaaaaaaad"} <= name{"aaaaaaaaaaaad"} );
   BOOST_TEST( name{"zzzzzzzzzzzzi"} <= name{"zzzzzzzzzzzzd"} );

      // ---------------------------------------------------------
   // friend constexpr bool operator < ( const name& a, const name& b )
   BOOST_TEST( name{"1"} > name{""} );
   BOOST_TEST( name{"5"} > name{"1"} );
   BOOST_TEST( name{"a"} > name{"5"} );
   BOOST_TEST( name{"z"} > name{"a"} );
   BOOST_TEST( name{"z1"} > name{"z"} );

   BOOST_TEST( name{"abd"} > name{"abc"} );
   BOOST_TEST( name{"124"} > name{"123"} );

   BOOST_TEST( name{".zzz"} > name{".abc"} );
   BOOST_TEST( name{".........z"} > name{".........abc"} );
   BOOST_TEST( name{"124."} > name{"123."} );
   BOOST_TEST( name{"124........."} > name{"123........."} );
   BOOST_TEST( name{".a.b.z.1.2.3."} > name{".a.b.c.1.2.3."} );

   BOOST_TEST( name{"abz.123"} > name{"abc.123"} );
   BOOST_TEST( name{"124.abc"} > name{"123.abc"} );

   BOOST_TEST( name{"13345abcdefgd"} > name{"12345abcdefgd"} );
   BOOST_TEST( name{"zijklmnopqrsd"} > name{"hijklmnopqrsd"} );
   BOOST_TEST( name{"tuvwxyz.1235d"} > name{"tuvwxyz.1234d"} );

   BOOST_TEST( name{"zzzzzzzzzzzzd"} > name{"111111111111d"} );
   BOOST_TEST( name{"zzzzzzzzzzzzd"} > name{"555555555555d"} );
   BOOST_TEST( name{"zzzzzzzzzzzzd"} > name{"aaaaaaaaaaaad"} );

         // ---------------------------------------------------------
   // friend constexpr bool operator < ( const name& a, const name& b )
   BOOST_TEST( name{"1"} >= name{""} );
   BOOST_TEST( name{"5"} >= name{"1"} );
   BOOST_TEST( name{"a"} >= name{"5"} );
   BOOST_TEST( name{"z"} >= name{"a"} );
   BOOST_TEST( name{"z1"} >= name{"z"} );

   BOOST_TEST( name{"abd"} >= name{"abc"} );
   BOOST_TEST( name{"124"} >= name{"123"} );

   BOOST_TEST( name{".zzz"} >= name{".abc"} );
   BOOST_TEST( name{".........z"} >= name{".........abc"} );
   BOOST_TEST( name{"124."} >= name{"123."} );
   BOOST_TEST( name{"124........."} >= name{"123........."} );
   BOOST_TEST( name{".a.b.z.1.2.3."} >= name{".a.b.c.1.2.3."} );

   BOOST_TEST( name{"abz.123"} >= name{"abc.123"} );
   BOOST_TEST( name{"124.abc"} >= name{"123.abc"} );

   BOOST_TEST( name{"13345abcdefgd"} >= name{"12345abcdefgd"} );
   BOOST_TEST( name{"zijklmnopqrsd"} >= name{"hijklmnopqrsd"} );
   BOOST_TEST( name{"tuvwxyz.1235d"} >= name{"tuvwxyz.1234d"} );

   BOOST_TEST( name{"zzzzzzzzzzzzd"} >= name{"111111111111d"} );
   BOOST_TEST( name{"zzzzzzzzzzzzd"} >= name{"555555555555d"} );
   BOOST_TEST( name{"zzzzzzzzzzzzd"} >= name{"zzzzzzzzzzzzd"} );

   BOOST_TEST( name{"1"} == 576460752303423488ULL );
   BOOST_TEST( name{"5"} == 2882303761517117440ULL );
   BOOST_TEST( name{"a"} == 3458764513820540928ULL );
   BOOST_TEST( name{"z"} == 17870283321406128128ULL );

   BOOST_TEST( name{"abc"} == 3589368903014285312ULL );
   BOOST_TEST( name{"123"} == 614178399182651392ULL );

   BOOST_TEST( name{".abc"} == 112167778219196416ULL );
   BOOST_TEST( name{".........abc"} == 102016ULL );
   BOOST_TEST( name{"123."} == 614178399182651392ULL );
   BOOST_TEST( name{"123........."} == 614178399182651392ULL );
   BOOST_TEST( name{".a.b.c.1.2.3."} == 108209673814966320ULL );

   BOOST_TEST( name{"abc.123"} == 3589369488740450304ULL );
   BOOST_TEST( name{"123.abc"} == 614181822271586304ULL );

   BOOST_TEST( name{"12345abcdefgd"} == 614251623682315983ULL );
   BOOST_TEST( name{"hijklmnopqrsd"} == 7754926748989239183ULL );
   BOOST_TEST( name{"tuvwxyz.1234d"} == 14895601873741973071ULL );

   BOOST_TEST( name{"111111111111d"} == 595056260442243615ULL );
   BOOST_TEST( name{"555555555555d"} == 2975281302211218015ULL );
   BOOST_TEST( name{"aaaaaaaaaaaad"} == 3570337562653461615ULL );
   BOOST_TEST( name{"zzzzzzzzzzzzd"} == u64max );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE(n_operator_test) {
try {

   // ------------------------------------
   // ""_n operator
   BOOST_TEST( name{} == ""_n );

   BOOST_TEST( name{"1"} == "1"_n );
   BOOST_TEST( name{"5"} == "5"_n );
   BOOST_TEST( name{"a"} == "a"_n );
   BOOST_TEST( name{"z"} == "z"_n );

   BOOST_TEST( name{"abc"} == "abc"_n );
   BOOST_TEST( name{"123"} == "123"_n );

   BOOST_TEST( name{".abc"} == ".abc"_n );
   BOOST_TEST( name{".........abc"} == ".........abc"_n );
   BOOST_TEST( name{"123."} == "123."_n );
   BOOST_TEST( name{"123........."} == "123........."_n );
   BOOST_TEST( name{".a.b.c.1.2.3."} == ".a.b.c.1.2.3."_n );

   BOOST_TEST( name{"abc.123"} == "abc.123"_n );
   BOOST_TEST( name{"123.abc"} == "123.abc"_n );

   BOOST_TEST( name{"12345abcdefgd"} == "12345abcdefgd"_n );
   BOOST_TEST( name{"hijklmnopqrsd"} == "hijklmnopqrsd"_n );
   BOOST_TEST( name{"tuvwxyz.1234d"} == "tuvwxyz.1234d"_n );

   BOOST_TEST( name{"111111111111d"} == "111111111111d"_n );
   BOOST_TEST( name{"555555555555d"} == "555555555555d"_n );
   BOOST_TEST( name{"aaaaaaaaaaaad"} == "aaaaaaaaaaaad"_n );
   BOOST_TEST( name{"zzzzzzzzzzzzd"} == "zzzzzzzzzzzzd"_n );
} FC_LOG_AND_RETHROW() }


BOOST_AUTO_TEST_CASE(is_string_valid_name_test) {
try {
   BOOST_TEST( is_string_valid_name("") );
   BOOST_TEST( is_string_valid_name("1") );
   BOOST_TEST( is_string_valid_name("5") );
   BOOST_TEST( is_string_valid_name("a") );
   BOOST_TEST( is_string_valid_name("z") );
   BOOST_TEST( is_string_valid_name("abc") );
   BOOST_TEST( is_string_valid_name("123") );
   BOOST_TEST( is_string_valid_name(".abc") );
   BOOST_TEST( is_string_valid_name(".........abc") );
   BOOST_TEST( is_string_valid_name("123.") );
   BOOST_TEST( is_string_valid_name("123.........") );
   BOOST_TEST( is_string_valid_name(".a.b.c.1.2.3.") );

   BOOST_TEST( is_string_valid_name("abc.123") );
   BOOST_TEST( is_string_valid_name("123.abc") );

   BOOST_TEST( is_string_valid_name("12345abcdefgd") );
   BOOST_TEST( is_string_valid_name("hijklmnopqrsd") );
   BOOST_TEST( is_string_valid_name("tuvwxyz.1234d") );
   BOOST_TEST( is_string_valid_name("111111111111d") );
   BOOST_TEST( is_string_valid_name("555555555555d") );
   BOOST_TEST( is_string_valid_name("aaaaaaaaaaaad") );
   BOOST_TEST( is_string_valid_name("zzzzzzzzzzzzd") );

   BOOST_TEST( is_string_valid_name("-1") == false );
   BOOST_TEST( is_string_valid_name("0") == false );
   BOOST_TEST( is_string_valid_name("6") == false );
   BOOST_TEST( is_string_valid_name("abcde12345abd") == true );
   BOOST_TEST( is_string_valid_name("abcde12345abk") == false );
   BOOST_TEST( is_string_valid_name("abcdeabcde1234") == false );
} FC_LOG_AND_RETHROW() }


BOOST_AUTO_TEST_SUITE_END()
