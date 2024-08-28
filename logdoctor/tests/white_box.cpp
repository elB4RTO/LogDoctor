
#include "white_box.h"

#include "lib.h"

#include "main_lib.h"

#include "workarounds/ranges_enumerate.h"
#include "workarounds/ranges_join.h"
#include "workarounds/ranges_zip.h"

#include "utilities/io.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/exceptions.h"

#include "modules/craplog/modules/formats/parse.h"
#include "modules/craplog/utilities/datetime.h"
#include "modules/craplog/utilities/logs.h"
#include "modules/craplog/workers/parser/logs_data.h"

#include "modules/crapview/utilities/filters.h"

#include <QTimeZone>


namespace Testing
{

bool testOperators()
{
    T_INIT("operators")

    //// DIALOGS LEVEL ////

    T_TEST_START("DialogsLevel::operator <")
    T_ASSERT( DialogsLevel::Essential < DialogsLevel::Normal );
    T_ASSERT( DialogsLevel::Essential < DialogsLevel::Explanatory );
    T_ASSERT( DialogsLevel::Normal < DialogsLevel::Explanatory );
    T_ASSERT_NOT( DialogsLevel::Essential < DialogsLevel::Essential );
    T_ASSERT_NOT( DialogsLevel::Normal < DialogsLevel::Normal );
    T_ASSERT_NOT( DialogsLevel::Explanatory < DialogsLevel::Explanatory );
    T_TEST_RESULT()

    T_TEST_START("DialogsLevel::operator >=")
    T_ASSERT( DialogsLevel::Essential >= DialogsLevel::Essential );
    T_ASSERT( DialogsLevel::Normal >= DialogsLevel::Essential );
    T_ASSERT( DialogsLevel::Normal >= DialogsLevel::Normal );
    T_ASSERT( DialogsLevel::Explanatory >= DialogsLevel::Essential );
    T_ASSERT( DialogsLevel::Explanatory >= DialogsLevel::Normal );
    T_ASSERT( DialogsLevel::Explanatory >= DialogsLevel::Explanatory );
    T_ASSERT_NOT( DialogsLevel::Essential >= DialogsLevel::Normal );
    T_ASSERT_NOT( DialogsLevel::Essential >= DialogsLevel::Explanatory );
    T_ASSERT_NOT( DialogsLevel::Normal >= DialogsLevel::Explanatory );
    T_TEST_RESULT()

    //// FIELD DATA ////

    T_TEST_START("FieldData::operator bool")
    {
    const FieldData fd;
    T_ASSERT( fd == false );
    }{
    const FieldData fd("");
    T_ASSERT( fd == false );
    }{
    const FieldData fd("*");
    T_ASSERT( fd == true );
    }
    T_TEST_RESULT()

    T_TEST_START("FieldData::operator +")
    {
    const FieldData fd;
    #ifdef _MSC_VER
    T_ASSERT( operator+(0ul, fd) == 0ul );
    #else
    T_ASSERT( 0ul + fd == 0ul );
    #endif
    }{
    const FieldData fd("");
    #ifdef _MSC_VER
    T_ASSERT( operator+(0ul, fd) == 0ul );
    #else
    T_ASSERT( 0ul + fd == 0ul );
    #endif
    }{
    const FieldData fd("1");
    #ifdef _MSC_VER
    T_ASSERT(operator+(0ul, fd) == 1ul );
    #else
    T_ASSERT( 0ul + fd == 1ul );
    #endif
    }{
    const FieldData fd("0123456789");
    #ifdef _MSC_VER
    T_ASSERT( operator+(10ul, fd) == 20ul );
    #else
    T_ASSERT( 10ul + fd == 20ul );
    #endif
    T_ASSERT( fd + 10ul != 20ul ); // fd evaluates to bool (aka 1)
    }{
    const FieldData fd1;
    const FieldData fd2;
    T_ASSERT( fd1 + fd2 == 0ul );
    }{
    const FieldData fd1("");
    const FieldData fd2("");
    T_ASSERT( fd1 + fd2 == 0ul );
    }{
    const FieldData fd1("");
    const FieldData fd2("123");
    T_ASSERT( fd1 + fd2 == 3ul );
    }{
    const FieldData fd1("123");
    const FieldData fd2("456");
    T_ASSERT( fd1 + fd2 == 6ul );
    }{
    const FieldData fd1("123");
    const FieldData fd2("456");
    #ifdef _MSC_VER
    T_ASSERT( operator+(4ul, fd1) + fd2 == 10ul );
    #else
    T_ASSERT( 4ul + fd1 + fd2 == 10ul );
    #endif
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    T_ASSERT( fd1 + fd2 + fd3 == 30ul );
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    #ifdef _MSC_VER
    T_ASSERT( operator+(70ul, fd1) + fd2 + fd3 == 100ul );
    #else
    T_ASSERT( 70ul + fd1 + fd2 + fd3 == 100ul );
    #endif
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    T_ASSERT( fd1 + fd2 + fd3 + 70ul == 100ul );
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    #ifdef _MSC_VER
    T_ASSERT( operator+(10ul, fd1) + fd2 + fd3 + 10ul == 50ul );
    #else
    T_ASSERT( 10ul + fd1 + fd2 + fd3 + 10ul == 50ul );
    #endif
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    T_ASSERT( 10ul + (fd1 + fd2) + fd3 + 10ul == 50ul );
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    T_ASSERT( 10ul + (fd1 + fd2) + (fd3 + 10ul) != 50ul ); // fd3 evaluates to bool (== 1)
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    const FieldData fd4("0123456789");
    const FieldData fd5("0123456789");
    #ifdef _MSC_VER
    T_ASSERT( operator+(10ul, fd1) + 20ul + fd2 + fd3 + 10ul + fd4 + 10ul + fd5 == 100ul );
    #else
    T_ASSERT( 10ul + fd1 + 20ul + fd2 + fd3 + 10ul + fd4 + 10ul + fd5 == 100ul );
    #endif
    }
    T_TEST_RESULT()

    T_FIN()
}


bool testUtilities()
{
    T_INIT("utilities")

    //// IO UTILS ////

    T_TEST_START("IOutils::exists")
    T_EXPECT( IOutils::exists("../logdoctor"), true );
    T_EXPECT( IOutils::exists("some/fancy/path"), false );
    T_TEST_RESULT()

    T_TEST_START("IOutils::isFile")
    T_EXPECT( IOutils::isFile("../logdoctor/main.cpp"), true );
    T_EXPECT( IOutils::isFile("../logdoctor/tests"), false );
    T_TEST_RESULT()

    T_TEST_START("IOutils::isDir")
    T_EXPECT( IOutils::isDir("../logdoctor/tests"), true );
    T_EXPECT( IOutils::isDir("../logdoctor/main.cpp"), false );
    T_TEST_RESULT()

    T_TEST_START("IOutils::checkFile")
    T_EXPECT( IOutils::checkFile("../logdoctor/main.cpp"), true );
    T_EXPECT( IOutils::checkFile("../logdoctor/tests"), false );
    T_TEST_RESULT()

    T_TEST_START("IOutils::checkDir")
    T_EXPECT( IOutils::checkDir("../logdoctor/tests"), true );
    T_EXPECT( IOutils::checkDir("../logdoctor/main.cpp"), false );
    T_TEST_RESULT()


    //// CHARS ////

    T_TEST_START("CharOps::isNumeric")
    {
    const auto every_other_char_but = [](const std::vector<char>& in)->std::vector<char>{
        std::vector<char> out( 128-in.size() );
        for (char c{0}; c<127; ++c) {
            if (const auto it = std::find(in.cbegin(),in.cend(),c); it == in.cend()) {
                out.push_back( c );
            }
        }
        out.push_back(127);
        out.shrink_to_fit();
        return out;
    };
    const std::vector<char> numerical_chars
        {'0','1','2','3','4','5','6','7','8','9'};
    const std::vector<char> alphabetical_chars
        {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
         'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    const std::vector<char> alphanumerical_chars
        {'0','1','2','3','4','5','6','7','8','9',
         'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
         'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    const std::vector<char> hexadecimal_chars
        {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'};
    const std::vector<char> ip_chars
        {'.',':','0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'};

    for (const char& c : numerical_chars) {
        T_ASSERT( CharOps::isNumeric(c) );
    }
    for (const char& c : every_other_char_but(numerical_chars)) {
        T_ASSERT_NOT( CharOps::isNumeric(c) );
    }
    T_TEST_RESULT()

    T_TEST_START("CharOps::isAlphabetic")
    for (const char& c : alphabetical_chars) {
        T_ASSERT( CharOps::isAlphabetic(c) );
    }
    for (const char& c : every_other_char_but(alphabetical_chars)) {
        T_ASSERT_NOT( CharOps::isAlphabetic(c) );
    }
    T_TEST_RESULT()

    T_TEST_START("CharOps::isAlnum")
    for (const char& c : alphanumerical_chars) {
        T_ASSERT( CharOps::isAlnum(c) );
    }
    for (const char& c : every_other_char_but(alphanumerical_chars)) {
        T_ASSERT_NOT( CharOps::isAlnum(c) );
    }
    T_TEST_RESULT()

    T_TEST_START("CharOps::isHex")
    for (const char& c : hexadecimal_chars) {
        T_ASSERT( CharOps::isHex(c) );
    }
    for (const char& c : every_other_char_but(hexadecimal_chars)) {
        T_ASSERT_NOT( CharOps::isHex(c) );
    }
    T_TEST_RESULT()

    T_TEST_START("CharOps::isIP")
    for (const char& c : ip_chars) {
        T_ASSERT( CharOps::isIP(c) );
    }
    for (const char& c : every_other_char_but(ip_chars)) {
        T_ASSERT_NOT( CharOps::isIP(c) );
    }
    T_TEST_RESULT()
    }


    //// STRINGS ////

    T_TEST_START("StringOps::count")
    T_EXPECT( StringOps::count("test test  test   test ", ' '), 7ul );
    T_EXPECT( StringOps::count("flagABCflagXYZflag", "flag"), 3ul );
    T_TEST_RESULT()

    T_TEST_START("StringOps::isNumeric")
    T_ASSERT( StringOps::isNumeric("0123456789") );
    T_ASSERT_NOT( StringOps::isNumeric("0123456789 ") );
    T_TEST_RESULT()

    T_TEST_START("StringOps::isAlphabetic")
    T_ASSERT( StringOps::isAlphabetic("abcXYZ") );
    T_ASSERT_NOT( StringOps::isAlphabetic("abc_XYZ") );
    T_TEST_RESULT()

    T_TEST_START("StringOps::isAlnum")
    T_ASSERT( StringOps::isAlnum("testME123") );
    T_ASSERT_NOT( StringOps::isAlnum("Fail!") );
    T_TEST_RESULT()

    T_TEST_START("StringOps::isIP")
    T_ASSERT( StringOps::isIP("192.168.1.1") );
    T_ASSERT( StringOps::isIP("::1") );
    T_ASSERT( StringOps::isIP("987654321.") );
    T_ASSERT( StringOps::isIP("abcdef:") );
    T_ASSERT_NOT( StringOps::isIP(".: ") );
    T_TEST_RESULT()

    T_TEST_START("StringOps::startsWith")
    T_ASSERT( StringOps::startsWith("/path/to", '/') );
    T_ASSERT( StringOps::startsWith("flag", "flag") );
    T_ASSERT_NOT( StringOps::startsWith(" rand", 'r') );
    T_ASSERT_NOT( StringOps::startsWith("flag", "lag") );
    T_TEST_RESULT()

    T_TEST_START("StringOps::endsWith")
    T_ASSERT( StringOps::endsWith("space_pad ", ' ') );
    T_ASSERT( StringOps::endsWith("the end", " end") );
    T_ASSERT_NOT( StringOps::endsWith("rand ", 'd') );
    T_ASSERT_NOT( StringOps::endsWith("the end", "_end") );
    T_TEST_RESULT()

    T_TEST_START("StringOps::contains")
    T_ASSERT( StringOps::contains("abcdefedcba", "efe") );
    T_ASSERT_NOT( StringOps::contains("this_should_fail", "SHOULD") );
    T_TEST_RESULT()

    T_TEST_START("StringOps::strip")
    T_EXPECT( StringOps::strip("___test___", '_'), "test" );
    T_EXPECT( StringOps::strip("test", ' '), "test" );
    T_EXPECT( StringOps::strip("    ", ' '), "" );
    T_EXPECT( StringOps::strip("", ' '), "" );
    T_EXPECT( StringOps::strip("a b ctestc", " abc"), "test" );
    T_EXPECT( StringOps::strip("\n\t \ntest\r "), "test" );
    T_EXPECT( StringOps::strip("test"), "test" );
    T_EXPECT( StringOps::strip(" \t\n\r"), "" );
    T_EXPECT( StringOps::strip(""), "" );
    T_TEST_RESULT()

    T_TEST_START("StringOps::lstrip")
    T_EXPECT( StringOps::lstrip("___test_", '_'), "test_" );
    T_EXPECT( StringOps::lstrip("test", ' '), "test" );
    T_EXPECT( StringOps::lstrip("    ", ' '), "" );
    T_EXPECT( StringOps::lstrip("", ' '), "" );
    T_EXPECT( StringOps::lstrip("the three trees", " ethr"), "s" );
    T_EXPECT( StringOps::lstrip(" \n\t\rtest "), "test " );
    T_EXPECT( StringOps::lstrip("test"), "test" );
    T_EXPECT( StringOps::lstrip(" \t\n\r"), "" );
    T_EXPECT( StringOps::lstrip(""), "" );
    T_TEST_RESULT()

    T_TEST_START("StringOps::rstrip")
    T_EXPECT( StringOps::rstrip("_test___", '_'), "_test" );
    T_EXPECT( StringOps::rstrip("test", ' '), "test" );
    T_EXPECT( StringOps::rstrip("    ", ' '), "" );
    T_EXPECT( StringOps::rstrip("", ' '), "" );
    T_EXPECT( StringOps::rstrip("testTEST", "TEST"), "test" );
    T_EXPECT( StringOps::rstrip(" test\r\t\n "), " test" );
    T_EXPECT( StringOps::rstrip("test"), "test" );
    T_EXPECT( StringOps::rstrip(" \t\n\r"), "" );
    T_EXPECT( StringOps::rstrip(""), "" );
    T_TEST_RESULT()

    T_TEST_START("StringOps::lstripUntil")
    T_EXPECT( StringOps::lstripUntil("is ok?", ' '), "ok?" );
    T_EXPECT( StringOps::lstripUntil("yes, it is", ' ', false), " it is" );
    T_EXPECT( StringOps::lstripUntil("__test", '_', false, false), "__test" );
    T_EXPECT( StringOps::lstripUntil("__test", '_', true, false), "_test" );
    T_EXPECT( StringOps::lstripUntil("__test", '_', true, true), "test" );
    T_EXPECT( StringOps::lstripUntil("   ", '_'), "   " );
    T_EXPECT( StringOps::lstripUntil("   ", ' '), "" );
    T_EXPECT( StringOps::lstripUntil("", ' '), "" );
    T_TEST_RESULT()

    T_TEST_START("StringOps::split")
    {
    std::vector<std::string> v;
    std::vector<std::string> t{"ok","ok","ok"};
    StringOps::split(v, "ok\nok\nok");
    T_ASSERT( v == t );
    }{
    std::vector<std::string> v;
    std::vector<std::string> t{"es","ing s","uff"};
    StringOps::split(v, "testing stuff", 't');
    T_ASSERT( v == t );
    }{
    std::vector<std::string> v;
    std::vector<std::string> t{"should","work","as expected"};
    StringOps::split(v, "  should  work  as expected  ", "  ");
    T_ASSERT( v == t );
    }
    T_TEST_RESULT()

    T_TEST_START("StringOps::splitrip")
    {
    std::vector<std::string> v;
    std::vector<std::string> t{"ok","ok","ok"};
    StringOps::splitrip(v, " ok \n ok \n ok ");
    T_ASSERT( v == t );
    }{
    std::vector<std::string> v;
    std::vector<std::string> t{"ok","ok","ok"};
    StringOps::splitrip(v, "ok.   .ok.   .ok", ' ', ".");
    T_ASSERT( v == t );
    }{
    std::vector<std::string> v;
    std::vector<std::string> t{"test","passed"};
    StringOps::splitrip(v, "iftesthaspassedfi", "has", "if");
    T_ASSERT( v == t );
    }
    T_TEST_RESULT()

    T_TEST_START("StringOps::replace")
    T_EXPECT( StringOps::replace("replace every space", " ", "_"), "replace_every_space" );
    T_EXPECT( StringOps::replace("thERR corrERRct phrasERR", "ERR", "e"), "the correct phrase" );
    T_TEST_RESULT()

    T_TEST_START("StringOps::toUpper")
    T_EXPECT( StringOps::toUpper("raise me up"), "RAISE ME UP" );
    T_EXPECT( StringOps::toUpper("1 2 3"), "1 2 3" );
    T_TEST_RESULT()

    T_TEST_START("StringOps::toLower")
    T_EXPECT( StringOps::toLower("LO-O-OW"), "lo-o-ow" );
    T_EXPECT( StringOps::toLower("9 8 7"), "9 8 7" );
    T_TEST_RESULT()


    //// VECTORS ////

    T_TEST_START("VecOps::contains")
    T_ASSERT( VecOps::contains<int>({1,2,4,8,16,32,64}, 4) );
    T_ASSERT_NOT( VecOps::contains<int>({1,2,4,8,16,32,64}, 33) );
    T_ASSERT( VecOps::contains<char>({'a','b','c'}, 'a') );
    T_ASSERT_NOT( VecOps::contains<char>({'a','b','c'}, 'z') );
    T_ASSERT( VecOps::contains<std::string>({"catch","the","flag"}, "flag") );
    T_ASSERT_NOT( VecOps::contains<std::string>({"catch","the","flag"}, "flac") );
    T_TEST_RESULT()

    T_FIN()
}


bool testWorkarounds()
{
    T_INIT("workarounds")

    //// CONCEPTS ////

    T_TEST_START("IsStdArray")
    {
    using Array = std::array<int,3>;
    T_ASSERT( IsStdArray<Array> );
    }{
    using Vector = std::vector<int>;
    T_ASSERT_NOT( IsStdArray<Vector> );
    }
    T_TEST_RESULT()

    //// RANGES ////

    T_TEST_START("Workarounds::enumerate")
    {
    const auto equality{ [](EnumeratedArray<std::array<int,3>>& enumerated, const std::array<std::tuple<int,int>,3>& fake_enumerated) -> bool {
        std::size_t i{ 0ul };
        for (const auto& tpl : enumerated) {
            if ( tpl != fake_enumerated.at(i) ) return false;
            ++i;
        }
        return true;
    }};
    std::array<int,3> a{ 10,20,30 };
    const std::array<std::tuple<int,int>,3> t{ std::make_tuple(0,10),std::make_tuple(1,20),std::make_tuple(2,30) };
    EnumeratedArray v{ Workarounds::enumerate( a ) };
    T_ASSERT( equality( v, t ) );
    }
    T_TEST_RESULT()

    T_TEST_START("Workarounds::join")
    {
    const auto equality{ [](const auto& flattened, const auto& fake_flattened) -> bool {
        if ( flattened.size() != fake_flattened.size() ) {
            return false;
        }
        const std::size_t max{ flattened.size() };
        for ( std::size_t i{0ul}; i<max; ++i ) {
            if ( *flattened.at(i) != fake_flattened.at(i) ) {
                return false;
            }
        }
        return true;
    }};
    {
    std::vector<std::vector<int>> nv{ {0}, {1,2,3}, {4,5}, {6,7,8}, {9} };
    const std::vector<int> e{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const std::vector<const int*> v{ Workarounds::join( nv ) };
    T_ASSERT( equality( v, e ) );
    }{
    std::vector<std::vector<std::vector<int>>> mnv{ { {0,1}, {2,3} }, { {4,5} }, { {6}, {7}, {8}, {9} } };
    const std::vector<std::vector<int>> ne{ {0,1}, {2,3}, {4,5}, {6}, {7}, {8}, {9} };
    const std::vector<const std::vector<int>*> nv{ Workarounds::join( mnv ) };
    T_ASSERT( equality( nv, ne ) );
    const std::vector<int> e{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const std::vector<const int*> v{ Workarounds::join( nv ) };
    T_ASSERT( equality( v, e ) );
    }
    }
    T_TEST_RESULT()

    T_TEST_START("Workarounds::zip")
    {
    const auto equality{ [](ZippedArrays<std::array<int,3>>& zipped, std::array<std::tuple<int,int>,3>& fake_zipped) -> bool {
        std::size_t i{ 0ul };
        for (const auto& tpl : zipped) {
            if ( tpl != fake_zipped.at(i) ) return false;
            ++i;
        }
        return true;
    }};
    std::array<int,3> a1{ 1,2,3 };
    std::array<int,3> a2{ 10,20,30 };
    std::array<std::tuple<int,int>,3> t{ std::make_tuple(1,10),std::make_tuple(2,20),std::make_tuple(3,30) };
    ZippedArrays v{ Workarounds::zip( a1, a2 ) };
    T_ASSERT( equality( v, t ) );
    }
    T_TEST_RESULT()

    T_FIN()
}


bool testCraplogModules()
{
    T_INIT("craplog modules")

    //// LOGS FORMAT FIELDS ////

    T_TEST_START("LogsFormatField")
    {
    // test for truth
    T_ASSERT( _NO_PARSE_NEEDED & date_time_year );
    T_ASSERT( _NO_PARSE_NEEDED & date_time_month );
    T_ASSERT( _NO_PARSE_NEEDED & date_time_day );
    T_ASSERT( _NO_PARSE_NEEDED & date_time_hour );
    T_ASSERT( _NO_PARSE_NEEDED & date_time_minute );
    T_ASSERT( _NO_PARSE_NEEDED & date_time_second );
    T_ASSERT( _NO_PARSE_NEEDED & request_protocol );
    T_ASSERT( _NO_PARSE_NEEDED & request_method );
    T_ASSERT( _NO_PARSE_NEEDED & request_uri );
    T_ASSERT( _NO_PARSE_NEEDED & request_query );
    T_ASSERT( _NO_PARSE_NEEDED & response_code );
    T_ASSERT( _NO_PARSE_NEEDED & bytes_received );
    T_ASSERT( _NO_PARSE_NEEDED & bytes_sent );
    T_ASSERT( _NO_PARSE_NEEDED & time_taken_ms );
    T_ASSERT( _NO_PARSE_NEEDED & client );
    T_ASSERT( _NO_PARSE_NEEDED & user_agent );
    T_ASSERT( _NO_PARSE_NEEDED & cookie );
    T_ASSERT( _NO_PARSE_NEEDED & referer );
    // test for false
    T_ASSERT_NOT( _NO_PARSE_NEEDED & _INVALID );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & _DISCARDED );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_epoch_us );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_epoch_ms );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_epoch_s );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_epoch_s_ms );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_ncsa );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_iso );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_gmt );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_mcs );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_utc_d );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_utc_t );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_yyyymmdd );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_mmddyy );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_mdyyyy );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_year_short );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_month_str );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_clock_24 );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_clock_12 );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & date_time_clock_short );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & request_uri_query );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & request_full );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & time_taken_us );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & time_taken_s );
    T_ASSERT_NOT( _NO_PARSE_NEEDED & time_taken_s_ms );
    }{
    // test for truth
    T_ASSERT( _DATE_TIME & date_time_epoch_us );
    T_ASSERT( _DATE_TIME & date_time_epoch_ms );
    T_ASSERT( _DATE_TIME & date_time_epoch_s );
    T_ASSERT( _DATE_TIME & date_time_epoch_s_ms );
    T_ASSERT( _DATE_TIME & date_time_ncsa );
    T_ASSERT( _DATE_TIME & date_time_iso );
    T_ASSERT( _DATE_TIME & date_time_gmt );
    T_ASSERT( _DATE_TIME & date_time_mcs );
    T_ASSERT( _DATE_TIME & date_time_utc_d );
    T_ASSERT( _DATE_TIME & date_time_utc_t );
    T_ASSERT( _DATE_TIME & date_time_yyyymmdd );
    T_ASSERT( _DATE_TIME & date_time_mmddyy );
    T_ASSERT( _DATE_TIME & date_time_mdyyyy );
    T_ASSERT( _DATE_TIME & date_time_year_short );
    T_ASSERT( _DATE_TIME & date_time_month_str );
    T_ASSERT( _DATE_TIME & date_time_year );
    T_ASSERT( _DATE_TIME & date_time_month );
    T_ASSERT( _DATE_TIME & date_time_day );
    T_ASSERT( _DATE_TIME & date_time_hour );
    T_ASSERT( _DATE_TIME & date_time_minute );
    T_ASSERT( _DATE_TIME & date_time_second );
    T_ASSERT( _DATE_TIME & date_time_clock_short );
    T_ASSERT( _DATE_TIME & date_time_clock_12 );
    T_ASSERT( _DATE_TIME & date_time_clock_24 );
    // test for false
    T_ASSERT_NOT( _DATE_TIME & _INVALID );
    T_ASSERT_NOT( _DATE_TIME & _DISCARDED );
    T_ASSERT_NOT( _DATE_TIME & request_protocol );
    T_ASSERT_NOT( _DATE_TIME & request_method );
    T_ASSERT_NOT( _DATE_TIME & request_uri );
    T_ASSERT_NOT( _DATE_TIME & request_query );
    T_ASSERT_NOT( _DATE_TIME & request_uri_query );
    T_ASSERT_NOT( _DATE_TIME & request_full );
    T_ASSERT_NOT( _DATE_TIME & response_code );
    T_ASSERT_NOT( _DATE_TIME & bytes_received );
    T_ASSERT_NOT( _DATE_TIME & bytes_sent );
    T_ASSERT_NOT( _DATE_TIME & time_taken_us );
    T_ASSERT_NOT( _DATE_TIME & time_taken_ms );
    T_ASSERT_NOT( _DATE_TIME & time_taken_s );
    T_ASSERT_NOT( _DATE_TIME & time_taken_s_ms );
    T_ASSERT_NOT( _DATE_TIME & client );
    T_ASSERT_NOT( _DATE_TIME & user_agent );
    T_ASSERT_NOT( _DATE_TIME & cookie );
    T_ASSERT_NOT( _DATE_TIME & referer );
    }{
    // test for truth
    T_ASSERT( _DATE_TIME_FULL & date_time_epoch_us );
    T_ASSERT( _DATE_TIME_FULL & date_time_epoch_ms );
    T_ASSERT( _DATE_TIME_FULL & date_time_epoch_s );
    T_ASSERT( _DATE_TIME_FULL & date_time_epoch_s_ms );
    T_ASSERT( _DATE_TIME_FULL & date_time_ncsa );
    T_ASSERT( _DATE_TIME_FULL & date_time_iso );
    T_ASSERT( _DATE_TIME_FULL & date_time_gmt );
    T_ASSERT( _DATE_TIME_FULL & date_time_mcs );
    // test for false
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_utc_d );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_utc_t );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_yyyymmdd );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_mmddyy );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_mdyyyy );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_year_short );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_month_str );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_year );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_month );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_day );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_hour );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_minute );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_second );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_clock_short );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_clock_12 );
    T_ASSERT_NOT( _DATE_TIME_FULL & date_time_clock_24 );
    }{
    // test for truth
    T_ASSERT( _DATE_TIME_EPOCH & date_time_epoch_us );
    T_ASSERT( _DATE_TIME_EPOCH & date_time_epoch_ms );
    T_ASSERT( _DATE_TIME_EPOCH & date_time_epoch_s );
    T_ASSERT( _DATE_TIME_EPOCH & date_time_epoch_s_ms );
    // test for false
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_ncsa );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_iso );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_gmt );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_mcs );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_utc_d );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_utc_t );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_yyyymmdd );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_mmddyy );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_mdyyyy );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_year_short );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_month_str );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_year );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_month );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_day );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_hour );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_minute );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_second );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_clock_short );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_clock_12 );
    T_ASSERT_NOT( _DATE_TIME_EPOCH & date_time_clock_24 );
    }{
    // test for truth
    T_ASSERT( _DATE_TIME_UTC & date_time_utc_d );
    T_ASSERT( _DATE_TIME_UTC & date_time_utc_t );
    // test for false
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_epoch_us );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_epoch_ms );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_epoch_s );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_epoch_s_ms );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_ncsa );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_iso );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_gmt );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_mcs );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_yyyymmdd );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_mmddyy );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_mdyyyy );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_year_short );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_month_str );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_year );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_month );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_day );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_hour );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_minute );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_second );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_clock_short );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_clock_12 );
    T_ASSERT_NOT( _DATE_TIME_UTC & date_time_clock_24 );
    }{
    // test for truth
    T_ASSERT( _DATE_TIME_DATE & date_time_utc_d );
    T_ASSERT( _DATE_TIME_DATE & date_time_yyyymmdd );
    T_ASSERT( _DATE_TIME_DATE & date_time_mmddyy );
    T_ASSERT( _DATE_TIME_DATE & date_time_mdyyyy );
    // test for false
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_epoch_us );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_epoch_ms );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_epoch_s );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_epoch_s_ms );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_ncsa );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_iso );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_gmt );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_mcs );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_utc_t );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_year_short );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_month_str );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_year );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_month );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_day );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_hour );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_minute );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_second );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_clock_short );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_clock_12 );
    T_ASSERT_NOT( _DATE_TIME_DATE & date_time_clock_24 );
    }{
    // test for truth
    T_ASSERT( _DATE_TIME_YEAR & date_time_year_short );
    T_ASSERT( _DATE_TIME_YEAR & date_time_year );
    // test for false
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_epoch_us );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_epoch_ms );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_epoch_s );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_epoch_s_ms );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_ncsa );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_iso );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_gmt );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_mcs );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_utc_d );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_utc_t );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_yyyymmdd );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_mmddyy );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_mdyyyy );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_month_str );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_month );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_day );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_hour );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_minute );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_second );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_clock_short );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_clock_12 );
    T_ASSERT_NOT( _DATE_TIME_YEAR & date_time_clock_24 );
    }{
    // test for truth
    T_ASSERT( _DATE_TIME_MONTH & date_time_month_str );
    T_ASSERT( _DATE_TIME_MONTH & date_time_month );
    // test for false
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_epoch_us );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_epoch_ms );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_epoch_s );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_epoch_s_ms );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_ncsa );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_iso );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_gmt );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_mcs );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_utc_d );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_utc_t );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_yyyymmdd );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_mmddyy );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_mdyyyy );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_year_short );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_year );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_day );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_hour );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_minute );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_second );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_clock_short );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_clock_12 );
    T_ASSERT_NOT( _DATE_TIME_MONTH & date_time_clock_24 );
    }{
    // test for truth
    T_ASSERT( _DATE_TIME_CLOCK & date_time_clock_short );
    T_ASSERT( _DATE_TIME_CLOCK & date_time_clock_12 );
    T_ASSERT( _DATE_TIME_CLOCK & date_time_clock_24 );
    // test for false
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_epoch_us );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_epoch_ms );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_epoch_s );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_epoch_s_ms );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_ncsa );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_iso );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_gmt );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_mcs );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_utc_d );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_utc_t );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_yyyymmdd );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_mmddyy );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_mdyyyy );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_year_short );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_month_str );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_year );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_month );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_day );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_hour );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_minute );
    T_ASSERT_NOT( _DATE_TIME_CLOCK & date_time_second );
    }{
    // test for truth
    T_ASSERT( _REQUEST & request_protocol );
    T_ASSERT( _REQUEST & request_method );
    T_ASSERT( _REQUEST & request_uri );
    T_ASSERT( _REQUEST & request_query );
    T_ASSERT( _REQUEST & request_uri_query );
    T_ASSERT( _REQUEST & request_full );
    // test for false
    T_ASSERT_NOT( _REQUEST & _INVALID );
    T_ASSERT_NOT( _REQUEST & _DISCARDED );
    T_ASSERT_NOT( _REQUEST & date_time_epoch_us );
    T_ASSERT_NOT( _REQUEST & date_time_epoch_ms );
    T_ASSERT_NOT( _REQUEST & date_time_epoch_s );
    T_ASSERT_NOT( _REQUEST & date_time_epoch_s_ms );
    T_ASSERT_NOT( _REQUEST & date_time_ncsa );
    T_ASSERT_NOT( _REQUEST & date_time_iso );
    T_ASSERT_NOT( _REQUEST & date_time_gmt );
    T_ASSERT_NOT( _REQUEST & date_time_mcs );
    T_ASSERT_NOT( _REQUEST & date_time_utc_d );
    T_ASSERT_NOT( _REQUEST & date_time_utc_t );
    T_ASSERT_NOT( _REQUEST & date_time_yyyymmdd );
    T_ASSERT_NOT( _REQUEST & date_time_mmddyy );
    T_ASSERT_NOT( _REQUEST & date_time_mdyyyy );
    T_ASSERT_NOT( _REQUEST & date_time_year_short );
    T_ASSERT_NOT( _REQUEST & date_time_month_str );
    T_ASSERT_NOT( _REQUEST & date_time_year );
    T_ASSERT_NOT( _REQUEST & date_time_month );
    T_ASSERT_NOT( _REQUEST & date_time_day );
    T_ASSERT_NOT( _REQUEST & date_time_hour );
    T_ASSERT_NOT( _REQUEST & date_time_minute );
    T_ASSERT_NOT( _REQUEST & date_time_second );
    T_ASSERT_NOT( _REQUEST & date_time_clock_short );
    T_ASSERT_NOT( _REQUEST & date_time_clock_12 );
    T_ASSERT_NOT( _REQUEST & date_time_clock_24 );
    T_ASSERT_NOT( _REQUEST & response_code );
    T_ASSERT_NOT( _REQUEST & bytes_received );
    T_ASSERT_NOT( _REQUEST & bytes_sent );
    T_ASSERT_NOT( _REQUEST & time_taken_us );
    T_ASSERT_NOT( _REQUEST & time_taken_ms );
    T_ASSERT_NOT( _REQUEST & time_taken_s );
    T_ASSERT_NOT( _REQUEST & time_taken_s_ms );
    T_ASSERT_NOT( _REQUEST & client );
    T_ASSERT_NOT( _REQUEST & user_agent );
    T_ASSERT_NOT( _REQUEST & cookie );
    T_ASSERT_NOT( _REQUEST & referer );
    }{
    // test for truth
    T_ASSERT( _RESPONSE_CODE & response_code );
    // test for false
    T_ASSERT_NOT( _RESPONSE_CODE & _INVALID );
    T_ASSERT_NOT( _RESPONSE_CODE & _DISCARDED );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_epoch_us );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_epoch_ms );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_epoch_s );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_epoch_s_ms );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_ncsa );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_iso );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_gmt );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_mcs );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_utc_d );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_utc_t );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_yyyymmdd );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_mmddyy );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_mdyyyy );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_year_short );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_month_str );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_year );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_month );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_day );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_hour );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_minute );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_second );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_clock_short );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_clock_12 );
    T_ASSERT_NOT( _RESPONSE_CODE & date_time_clock_24 );
    T_ASSERT_NOT( _RESPONSE_CODE & request_protocol );
    T_ASSERT_NOT( _RESPONSE_CODE & request_method );
    T_ASSERT_NOT( _RESPONSE_CODE & request_uri );
    T_ASSERT_NOT( _RESPONSE_CODE & request_query );
    T_ASSERT_NOT( _RESPONSE_CODE & request_uri_query );
    T_ASSERT_NOT( _RESPONSE_CODE & request_full );
    T_ASSERT_NOT( _RESPONSE_CODE & bytes_received );
    T_ASSERT_NOT( _RESPONSE_CODE & bytes_sent );
    T_ASSERT_NOT( _RESPONSE_CODE & time_taken_us );
    T_ASSERT_NOT( _RESPONSE_CODE & time_taken_ms );
    T_ASSERT_NOT( _RESPONSE_CODE & time_taken_s );
    T_ASSERT_NOT( _RESPONSE_CODE & time_taken_s_ms );
    T_ASSERT_NOT( _RESPONSE_CODE & client );
    T_ASSERT_NOT( _RESPONSE_CODE & user_agent );
    T_ASSERT_NOT( _RESPONSE_CODE & cookie );
    T_ASSERT_NOT( _RESPONSE_CODE & referer );
    }{
    // test for truth
    T_ASSERT( _DATA_TRANSFER & bytes_received );
    T_ASSERT( _DATA_TRANSFER & bytes_sent );
    // test for false
    T_ASSERT_NOT( _DATA_TRANSFER & _INVALID );
    T_ASSERT_NOT( _DATA_TRANSFER & _DISCARDED );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_epoch_us );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_epoch_ms );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_epoch_s );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_epoch_s_ms );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_ncsa );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_iso );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_gmt );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_mcs );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_utc_d );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_utc_t );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_yyyymmdd );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_mmddyy );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_mdyyyy );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_year_short );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_month_str );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_year );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_month );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_day );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_hour );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_minute );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_second );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_clock_short );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_clock_12 );
    T_ASSERT_NOT( _DATA_TRANSFER & date_time_clock_24 );
    T_ASSERT_NOT( _DATA_TRANSFER & request_protocol );
    T_ASSERT_NOT( _DATA_TRANSFER & request_method );
    T_ASSERT_NOT( _DATA_TRANSFER & request_uri );
    T_ASSERT_NOT( _DATA_TRANSFER & request_query );
    T_ASSERT_NOT( _DATA_TRANSFER & request_uri_query );
    T_ASSERT_NOT( _DATA_TRANSFER & request_full );
    T_ASSERT_NOT( _DATA_TRANSFER & response_code );
    T_ASSERT_NOT( _DATA_TRANSFER & time_taken_us );
    T_ASSERT_NOT( _DATA_TRANSFER & time_taken_ms );
    T_ASSERT_NOT( _DATA_TRANSFER & time_taken_s );
    T_ASSERT_NOT( _DATA_TRANSFER & time_taken_s_ms );
    T_ASSERT_NOT( _DATA_TRANSFER & client );
    T_ASSERT_NOT( _DATA_TRANSFER & user_agent );
    T_ASSERT_NOT( _DATA_TRANSFER & cookie );
    T_ASSERT_NOT( _DATA_TRANSFER & referer );
    }{
    // test for truth
    T_ASSERT( _TIME_TAKEN & time_taken_us );
    T_ASSERT( _TIME_TAKEN & time_taken_ms );
    T_ASSERT( _TIME_TAKEN & time_taken_s );
    T_ASSERT( _TIME_TAKEN & time_taken_s_ms );
    // test for false
    T_ASSERT_NOT( _TIME_TAKEN & _INVALID );
    T_ASSERT_NOT( _TIME_TAKEN & _DISCARDED );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_epoch_us );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_epoch_ms );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_epoch_s );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_epoch_s_ms );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_ncsa );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_iso );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_gmt );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_mcs );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_utc_d );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_utc_t );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_yyyymmdd );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_mmddyy );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_mdyyyy );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_year_short );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_month_str );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_year );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_month );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_day );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_hour );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_minute );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_second );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_clock_short );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_clock_12 );
    T_ASSERT_NOT( _TIME_TAKEN & date_time_clock_24 );
    T_ASSERT_NOT( _TIME_TAKEN & request_protocol );
    T_ASSERT_NOT( _TIME_TAKEN & request_method );
    T_ASSERT_NOT( _TIME_TAKEN & request_uri );
    T_ASSERT_NOT( _TIME_TAKEN & request_query );
    T_ASSERT_NOT( _TIME_TAKEN & request_uri_query );
    T_ASSERT_NOT( _TIME_TAKEN & request_full );
    T_ASSERT_NOT( _TIME_TAKEN & response_code );
    T_ASSERT_NOT( _TIME_TAKEN & bytes_received );
    T_ASSERT_NOT( _TIME_TAKEN & bytes_sent );
    T_ASSERT_NOT( _TIME_TAKEN & client );
    T_ASSERT_NOT( _TIME_TAKEN & user_agent );
    T_ASSERT_NOT( _TIME_TAKEN & cookie );
    T_ASSERT_NOT( _TIME_TAKEN & referer );
    }{
    // test for truth
    T_ASSERT( _CLIENT_DATA & client );
    T_ASSERT( _CLIENT_DATA & user_agent );
    T_ASSERT( _CLIENT_DATA & cookie );
    T_ASSERT( _CLIENT_DATA & referer );
    // test for false
    T_ASSERT_NOT( _CLIENT_DATA & _INVALID );
    T_ASSERT_NOT( _CLIENT_DATA & _DISCARDED );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_epoch_us );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_epoch_ms );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_epoch_s );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_epoch_s_ms );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_ncsa );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_iso );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_gmt );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_mcs );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_utc_d );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_utc_t );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_yyyymmdd );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_mmddyy );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_mdyyyy );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_year_short );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_month_str );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_year );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_month );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_day );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_hour );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_minute );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_second );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_clock_short );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_clock_12 );
    T_ASSERT_NOT( _CLIENT_DATA & date_time_clock_24 );
    T_ASSERT_NOT( _CLIENT_DATA & request_protocol );
    T_ASSERT_NOT( _CLIENT_DATA & request_method );
    T_ASSERT_NOT( _CLIENT_DATA & request_uri );
    T_ASSERT_NOT( _CLIENT_DATA & request_query );
    T_ASSERT_NOT( _CLIENT_DATA & request_uri_query );
    T_ASSERT_NOT( _CLIENT_DATA & request_full );
    T_ASSERT_NOT( _CLIENT_DATA & response_code );
    T_ASSERT_NOT( _CLIENT_DATA & bytes_received );
    T_ASSERT_NOT( _CLIENT_DATA & bytes_sent );
    T_ASSERT_NOT( _CLIENT_DATA & time_taken_us );
    T_ASSERT_NOT( _CLIENT_DATA & time_taken_ms );
    T_ASSERT_NOT( _CLIENT_DATA & time_taken_s );
    T_ASSERT_NOT( _CLIENT_DATA & time_taken_s_ms );
    }{
    // test for exact matches
    T_EXPECT( (_NO_PARSE_NEEDED & date_time_year), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & date_time_month), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & date_time_day), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & date_time_hour), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & date_time_minute), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & date_time_second), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & request_protocol), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & request_method), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & request_uri), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & request_query), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & response_code), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & bytes_received), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & bytes_sent), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & time_taken_ms), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & client), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & user_agent), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & cookie), _NO_PARSE_NEEDED );
    T_EXPECT( (_NO_PARSE_NEEDED & referer), _NO_PARSE_NEEDED );
    }{
    // test for exact matches
    T_EXPECT( (_DATE_TIME & date_time_epoch_us), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_epoch_ms), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_epoch_s), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_epoch_s_ms), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_ncsa), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_iso), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_gmt), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_mcs), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_utc_d), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_utc_t), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_yyyymmdd), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_mmddyy), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_mdyyyy), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_year_short), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_month_str), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_year), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_month), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_day), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_hour), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_minute), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_second), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_clock_short), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_clock_12), _DATE_TIME );
    T_EXPECT( (_DATE_TIME & date_time_clock_24), _DATE_TIME );
    }{
    // test for exact matches
    T_EXPECT( (_DATE_TIME_FULL & date_time_epoch_us), _DATE_TIME_FULL );
    T_EXPECT( (_DATE_TIME_FULL & date_time_epoch_ms), _DATE_TIME_FULL );
    T_EXPECT( (_DATE_TIME_FULL & date_time_epoch_s), _DATE_TIME_FULL );
    T_EXPECT( (_DATE_TIME_FULL & date_time_epoch_s_ms), _DATE_TIME_FULL );
    T_EXPECT( (_DATE_TIME_FULL & date_time_ncsa), _DATE_TIME_FULL );
    T_EXPECT( (_DATE_TIME_FULL & date_time_iso), _DATE_TIME_FULL );
    T_EXPECT( (_DATE_TIME_FULL & date_time_gmt), _DATE_TIME_FULL );
    T_EXPECT( (_DATE_TIME_FULL & date_time_mcs), _DATE_TIME_FULL );
    }{
    // test for exact matches
    T_EXPECT( (_DATE_TIME_EPOCH & date_time_epoch_us), _DATE_TIME_EPOCH );
    T_EXPECT( (_DATE_TIME_EPOCH & date_time_epoch_ms), _DATE_TIME_EPOCH );
    T_EXPECT( (_DATE_TIME_EPOCH & date_time_epoch_s), _DATE_TIME_EPOCH );
    T_EXPECT( (_DATE_TIME_EPOCH & date_time_epoch_s_ms), _DATE_TIME_EPOCH );
    }{
    // test for exact matches
    T_EXPECT( (_DATE_TIME_UTC & date_time_utc_d), _DATE_TIME_UTC );
    T_EXPECT( (_DATE_TIME_UTC & date_time_utc_t), _DATE_TIME_UTC );
    }{
    // test for exact matches
    T_EXPECT( (_DATE_TIME_DATE & date_time_utc_d), _DATE_TIME_DATE );
    T_EXPECT( (_DATE_TIME_DATE & date_time_yyyymmdd), _DATE_TIME_DATE );
    T_EXPECT( (_DATE_TIME_DATE & date_time_mmddyy), _DATE_TIME_DATE );
    T_EXPECT( (_DATE_TIME_DATE & date_time_mdyyyy), _DATE_TIME_DATE );
    }{
    // test for exact matches
    T_EXPECT( (_DATE_TIME_YEAR & date_time_year_short), _DATE_TIME_YEAR );
    T_EXPECT( (_DATE_TIME_YEAR & date_time_year), _DATE_TIME_YEAR );
    }{
    // test for exact matches
    T_EXPECT( (_DATE_TIME_MONTH & date_time_month_str), _DATE_TIME_MONTH );
    T_EXPECT( (_DATE_TIME_MONTH & date_time_month), _DATE_TIME_MONTH );
    }{
    // test for exact matches
    T_EXPECT( (_DATE_TIME_CLOCK & date_time_clock_short), _DATE_TIME_CLOCK );
    T_EXPECT( (_DATE_TIME_CLOCK & date_time_clock_12), _DATE_TIME_CLOCK );
    T_EXPECT( (_DATE_TIME_CLOCK & date_time_clock_24), _DATE_TIME_CLOCK );
    }{
    // test for exact matches
    T_EXPECT( (_REQUEST & request_protocol), _REQUEST );
    T_EXPECT( (_REQUEST & request_method), _REQUEST );
    T_EXPECT( (_REQUEST & request_uri), _REQUEST );
    T_EXPECT( (_REQUEST & request_query), _REQUEST );
    T_EXPECT( (_REQUEST & request_uri_query), _REQUEST );
    T_EXPECT( (_REQUEST & request_full), _REQUEST );
    }{
    // test for exact matches
    T_EXPECT( (_RESPONSE_CODE & response_code), _RESPONSE_CODE );
    }{
    // test for exact matches
    T_EXPECT( (_DATA_TRANSFER & bytes_received), _DATA_TRANSFER );
    T_EXPECT( (_DATA_TRANSFER & bytes_sent), _DATA_TRANSFER );
    }{
    // test for exact matches
    T_EXPECT( (_TIME_TAKEN & time_taken_us), _TIME_TAKEN );
    T_EXPECT( (_TIME_TAKEN & time_taken_ms), _TIME_TAKEN );
    T_EXPECT( (_TIME_TAKEN & time_taken_s), _TIME_TAKEN );
    T_EXPECT( (_TIME_TAKEN & time_taken_s_ms), _TIME_TAKEN );
    }{
    // test for exact matches
    T_EXPECT( (_CLIENT_DATA & client), _CLIENT_DATA );
    T_EXPECT( (_CLIENT_DATA & user_agent), _CLIENT_DATA );
    T_EXPECT( (_CLIENT_DATA & cookie), _CLIENT_DATA );
    T_EXPECT( (_CLIENT_DATA & referer), _CLIENT_DATA );
    }{
    // test for fields which contain a fixed number of spaces
    T_ASSERT( _MAY_HAVE_SPACES & request_full );
    T_ASSERT( _MAY_HAVE_SPACES & date_time_ncsa );
    T_ASSERT( _MAY_HAVE_SPACES & date_time_gmt );
    T_ASSERT( _MAY_HAVE_SPACES & date_time_mcs );
    // test for fields which doesn't (supposedly) contain spaces
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_epoch_us );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_epoch_ms );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_epoch_s );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_epoch_s_ms );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_iso );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_utc_d );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_utc_t );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_yyyymmdd );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_mmddyy );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_mdyyyy );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_year_short );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_month_str );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_year );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_month );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_day );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_hour );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_minute );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_second );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_clock_short );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_clock_12 );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & date_time_clock_24 );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & request_protocol );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & request_method );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & request_uri );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & request_query );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & request_uri_query );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & response_code );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & bytes_received );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & bytes_sent );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & time_taken_us );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & time_taken_ms );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & time_taken_s );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & time_taken_s_ms );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & client );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & user_agent );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & cookie );
    T_ASSERT_NOT( _MAY_HAVE_SPACES & referer );
    }{
    // test for the correct amount of spaces
    T_EXPECT( (_COUNT_SPACES & date_time_ncsa), 1ul );
    T_EXPECT( (_COUNT_SPACES & date_time_gmt), 3ul );
    T_EXPECT( (_COUNT_SPACES & date_time_mcs), 4ul );
    T_EXPECT( (request_full & 2), 2 ); // useless, just a reminder is should have 2
    }
    T_TEST_RESULT()


    //// DATE-TIME ////

    T_TEST_START("DateTimeOps::processDateTime")
    {
    QTimeZone tz{ QTimeZone::systemTimeZone() };
    const int e{ 946771199 - tz.standardTimeOffset( QDateTime::fromSecsSinceEpoch(946771199) ) };
    const std::string e_str{ std::to_string( e ) };
    const std::string epochs[4]{ e_str, e_str+".000", e_str+"000", e_str+"000000" };
    std::vector<std::string> target{"2000","01","01","23","59","59"};
    T_EXPECT( DateTimeOps::processDateTime("[01/Jan/2000:23:59:59 +0000]", date_time_ncsa), target );
    T_EXPECT( DateTimeOps::processDateTime("Sat Jan 01 23:59:59 2000", date_time_mcs), target );
    T_EXPECT( DateTimeOps::processDateTime("Saturday, 01-Jan-2000 23:59:59 UTC", date_time_gmt), target );
    T_EXPECT( DateTimeOps::processDateTime("2000-01-01T23:59:59+00:00", date_time_iso), target );
    T_EXPECT( DateTimeOps::processDateTime(epochs[0], date_time_epoch_s), target );
    T_EXPECT( DateTimeOps::processDateTime(epochs[1], date_time_epoch_s_ms), target );
    T_EXPECT( DateTimeOps::processDateTime(epochs[2], date_time_epoch_ms), target );
    T_EXPECT( DateTimeOps::processDateTime(epochs[3], date_time_epoch_us), target );
    target = {"2000","01","01","","",""};
    T_EXPECT( DateTimeOps::processDateTime("2000-01-01", date_time_utc_d), target );
    T_EXPECT( DateTimeOps::processDateTime("2000-01-01", date_time_yyyymmdd), target );
    T_EXPECT( DateTimeOps::processDateTime("01/01/00", date_time_mmddyy), target );
    T_EXPECT( DateTimeOps::processDateTime("1/1/2000", date_time_mdyyyy), target );
    target = {"","","","23","59","59"};
    T_EXPECT( DateTimeOps::processDateTime("23:59:59", date_time_utc_t), target );
    T_EXPECT( DateTimeOps::processDateTime("11:59:59 pm", date_time_clock_12), target );
    T_EXPECT( DateTimeOps::processDateTime("23:59:59", date_time_clock_24), target );
    target = {"","","","23","59",""};
    T_EXPECT( DateTimeOps::processDateTime("23:59", date_time_clock_short), target );
    /*target = {"","","","PM","",""};
    T_EXPECT( DateTimeOps::processDateTime("pm", date_time_clock_meridian), target );*/
    target = {"2000","","","","",""};
    T_EXPECT( DateTimeOps::processDateTime("2000", date_time_year), target );
    target = {"","01","","","",""};
    T_EXPECT( DateTimeOps::processDateTime("01", date_time_month), target );
    target = {"","","01","","",""};
    T_EXPECT( DateTimeOps::processDateTime("01", date_time_day), target );
    target = {"","","","23","",""};
    T_EXPECT( DateTimeOps::processDateTime("23", date_time_hour), target );
    target = {"","","","","59",""};
    T_EXPECT( DateTimeOps::processDateTime("59", date_time_minute), target );
    target = {"","","","","","59"};
    T_EXPECT( DateTimeOps::processDateTime("59", date_time_second), target );
    }
    T_TEST_RESULT()


    //// FORMATS ////

    T_TEST_START("FormatOps::Private::parseApacheFormatString")
    {
    LogsFormat lf;
    std::string format_string;
    std::vector<LogsFormatField> fields;
    std::vector<std::string> separators;
    // test the default string
    format_string = "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"";
    fields = {client,_DISCARDED,_DISCARDED,date_time_ncsa,request_full,response_code,_DISCARDED,referer,user_agent};
    separators = {" "," "," [","] \"","\" "," "," \"","\" \""};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final == "\"" );
    // test all simple fields
    format_string = "%%%h %% %t\t%r\n%H %m [%U%%%q} <%s> %<s %>s %O %I %T %D %a %A %b %B %e %f %k %l %L %p %P %R %S %u %v %V %% %X%%";
    fields = {client,date_time_ncsa,request_full,request_protocol,request_method,request_uri,request_query,response_code,response_code,response_code,bytes_sent,bytes_received,time_taken_s,time_taken_ms,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" % [","]\t","\n"," "," [","%","} <","> "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," % "};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial == "%" );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final == "%" );
    // test non-existing/invalid simple fields
    for ( auto& fs : {"%","%c","%d","%E","%Q","%w","%Z","%! ","%, ","%h%r"} ) {
        REDIRECT_STDOUT()
        REDIRECT_STDERR()
        try {
            std::ignore = FormatOps::Private::parseApacheFormatString(fs);
            RESTORE_STDOUT()
            RESTORE_STDERR()
            T_ASSERT( false );
        } catch (const LogFormatException& e) {}
        RESTORE_STDOUT()
        RESTORE_STDERR()
    }
    // test client related composed fields
    format_string = "%{}a %{c}a %{}h %{c}h %{Cookie}i %200{Cookie}i %{User-agent}i %302,400{User-agent}i %!200{Referer}i %,200{Referer}i %{Referer}i";
    fields = {client,client,client,client,cookie,cookie,user_agent,user_agent,referer,referer,referer};
    separators = {" "," "," "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test unexisting/unsupported client related composed fields
    format_string = "%{ }a %{x}a %{NOPE}a %{ }h %{y}h %{NOPE}h %{}i %{ }i %{Random}i %{Cookies}i";
    fields = {client,client,client,client,client,client,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test date-time composed fields
    format_string = "%{%%}t %{%n}t %{%t}t %{}t %{sec}t %{msec}t %{usec}t %{msec_frac}t %{usec_frac}t %{%a}t %{%A}t %{%b}t %{%B}t %{%c}t %{%C}t %{%d}t %{%D}t %{%e}t %{%F}t %{%g}t %{%G}t %{%h}t %{%H}t %{%I}t %{%j}t %{%k}t %{%m}t %{%M}t %{%p}t %{%r}t %{%R}t %{%S}t %{%T}t %{%u}t %{%U}t %{%V}t %{%w}t %{%W}t %{%x}t %{%X}t %{%y}t %{%Y}t %{%z}t %{%Z}t";
    fields = {date_time_ncsa,date_time_epoch_s,date_time_epoch_ms,date_time_epoch_us,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,date_time_month_str,date_time_month_str,date_time_mcs,_DISCARDED,date_time_day,date_time_mmddyy,date_time_day,date_time_yyyymmdd,_DISCARDED,_DISCARDED,date_time_month_str,date_time_hour,_DISCARDED,_DISCARDED,date_time_hour,date_time_month,date_time_minute,_DISCARDED,date_time_clock_12,date_time_clock_short,date_time_second,date_time_clock_24,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,date_time_mmddyy,date_time_clock_24,date_time_year_short,date_time_year,_DISCARDED,_DISCARDED};
    separators = {"] "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial == "% \n \t [" );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test date-time composed fields, with one field only
    format_string = "%{}t";
    fields = {date_time_ncsa};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial == "[" );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators.empty() );
    T_ASSERT( lf.final == "]" );
    // test date-time composed fields, with many aggreagated fields
    format_string = "%{%%%Y_%m_%e%t%H@%M@%S%%}t";
    fields = {date_time_year,date_time_month,date_time_day,date_time_hour,date_time_minute,date_time_second};
    separators = {"_","_","\t","@","@"};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial == "%" );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final == "%" );
    // test date-time composed fields, with all fields aggeregated in one
    format_string = "%{%% %n %t %a %A %b %B %c %C %d %D %e %F %g %G %h %H %I %j %k %m %M %p %r %R %S %T %u %U %V %w %W %x %X %y %Y %z %Z}t";
    fields = {_DISCARDED,_DISCARDED,date_time_month_str,date_time_month_str,date_time_mcs,_DISCARDED,date_time_day,date_time_mmddyy,date_time_day,date_time_yyyymmdd,_DISCARDED,_DISCARDED,date_time_month_str,date_time_hour,_DISCARDED,_DISCARDED,date_time_hour,date_time_month,date_time_minute,_DISCARDED,date_time_clock_12,date_time_clock_short,date_time_second,date_time_clock_24,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,date_time_mmddyy,date_time_clock_24,date_time_year_short,date_time_year,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial == "% \n \t " );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test unexisting/unsupported date-time composed fields
    format_string = "%{ }t %{nope}t %{%}t %{%?}t %{%E}t %{%q}t";
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial == "  nope % %? %E %q" );
    T_ASSERT( lf.fields.empty() );
    T_ASSERT( lf.separators.empty() );
    T_ASSERT( lf.final.empty() );
    // test invalid date-time composed fields
    for ( auto& fs : {"%{%H}t%{%M}t%{%S}t","%{%d%m%y}t"} ) {
        REDIRECT_STDOUT()
        REDIRECT_STDERR()
        try {
            std::ignore = FormatOps::Private::parseApacheFormatString(fs);
            RESTORE_STDOUT()
            RESTORE_STDERR()
            T_ASSERT( false );
        } catch (const LogFormatException& e) {}
        RESTORE_STDOUT()
        RESTORE_STDERR()
    }
    // test time taken related composed fields
    format_string = "%{}T %{s}T %{ms}T %{us}T";
    fields = {time_taken_s,time_taken_s,time_taken_ms,time_taken_us};
    separators = {" "," "," "};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test unexisting/unsupported time taken related composed fields
    format_string = "%{ }T %{%s}T %{msec}T";
    fields = {};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial == "  %s msec" );
    T_ASSERT( lf.fields.empty() );
    T_ASSERT( lf.separators.empty() );
    T_ASSERT( lf.final.empty() );
    // test unused composed fields
    format_string = "%{}C %{}e %{}L %{}n %{}o %{}p %{canonical}p %{local}p %{remote}p %{}P %{pid}P %{tid}P %{hextid}P %{}^ti %{}^to";
    fields = {_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test unused composed fields, with random content
    format_string = "%{TEST}C %{TEST}e %{TEST}L %{TEST}n %{TEST}o %{TEST}p %{TEST}P %{TEST}^ti %{TEST}^to";
    fields = {_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," TEST TEST "," "};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test the default string with dumb logging: no characters to enclose the full request
    format_string = "%h %l %u %t %r %>s %b \"%{Referer}i\" \"%{User-agent}i\"";
    fields = {client,_DISCARDED,_DISCARDED,date_time_ncsa,request_full,response_code,_DISCARDED,referer,user_agent};
    separators = {" "," "," [","] "," "," "," \"","\" \""};
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final == "\"" );
    // test an empty string
    format_string.erase();
    lf = FormatOps::Private::parseApacheFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields.empty() );
    T_ASSERT( lf.separators.empty() );
    T_ASSERT( lf.final.empty() );
    }
    T_TEST_RESULT()

    T_TEST_START("FormatOps::Private::parseNginxFormatString")
    {
    LogsFormat lf;
    std::string format_string;
    std::vector<LogsFormatField> fields;
    std::vector<std::string> separators;
    // test the default string
    format_string = "$remote_addr - $remote_user [$time_local] \"$request\" $status $bytes_sent \"$http_referer\" \"$http_user_agent\"";
    fields = {client,_DISCARDED,date_time_ncsa,request_full,response_code,bytes_sent,referer,user_agent};
    separators = {" - "," [","] \"","\" "," "," \"","\" \""};
    lf = FormatOps::Private::parseNginxFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final == "\"" );
    // test all the considered fields
    format_string = "$remote_addr $realip_remote_addr $time_local $time_iso8601 $date_gmt $msec $request $server_protocol $request_method $request_uri $uri $query_string $status $bytes_sent $request_length $request_time $http_referer $cookie_ $http_user_agent";
    fields = {client,client,date_time_ncsa,date_time_iso,date_time_gmt,date_time_epoch_s_ms,request_full,request_protocol,request_method,request_uri_query,request_uri,request_query,response_code,bytes_sent,bytes_received,time_taken_s_ms,referer,cookie,user_agent};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseNginxFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test all the non-considered fields
    format_string = "$ancient_browser $arg_ $args $binary_remote_addr $body_bytes_sent $connection $connection_requests $connections_active $connections_reading $connections_waiting $connections_writing $content_length $content_type $date_local $document_root $document_uri $fastcgi_path_info $fastcgi_script_name $geoip_area_code $geoip_city $geoip_city_continent_code $geoip_city_country_code $geoip_city_country_code3 $geoip_city_country_name $geoip_country_code $geoip_country_code3 $geoip_country_name $geoip_dma_code $geoip_latitude $geoip_longitude $geoip_org $geoip_postal_code $geoip_region $geoip_region_name $gzip_ratio $host $hostname $http2 $http_ $https $invalid_referer $is_args $limit_rate $memcached_key $modern_browser $msie $nginx_version $pid $pipe $proxy_add_x_forwarded_for $proxy_host $proxy_port $proxy_protocol_addr $proxy_protocol_port $realip_remote_port $realpath_root $remote_port $remote_user $request_body $request_body_file $request_completion $request_filename $request_id $scheme $secure_link $secure_link_expires $sent_http_ $server_addr $server_name $server_port $session_log_binary_id $session_log_id $slice_range $spdy $spdy_request_priority $ssl_cipher $ssl_client_cert $ssl_client_fingerprint $ssl_client_i_dn $ssl_client_raw_cert $ssl_client_s_dn $ssl_client_serial $ssl_client_verify $ssl_protocol $ssl_server_name $ssl_session_id $ssl_session_reused $tcpinfo_rtt $tcpinfo_rttvar $tcpinfo_snd_cwnd $tcpinfo_rcv_space $uid_got $uid_reset $uid_set $upstream_addr $upstream_cache_status $upstream_connect_time $upstream_cookie_ $upstream_header_time $upstream_http_ $upstream_response_length $upstream_response_time $upstream_status";
    fields = {_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseNginxFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test an empty string
    format_string.erase();
    lf = FormatOps::Private::parseNginxFormatString(format_string);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields.empty() );
    T_ASSERT( lf.separators.empty() );
    T_ASSERT( lf.final.empty() );
    }
    T_TEST_RESULT()

    T_TEST_START("FormatOps::Private::parseIisFormatString")
    {
    LogsFormat lf;
    std::string format_string;
    std::vector<LogsFormatField> fields;
    std::vector<std::string> separators;
    // test the default string for the W3C module
    format_string = "date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs(User-Agent) cs(Referer) sc-status sc-substatus sc-win32-status time-taken";
    fields = {date_time_utc_d,date_time_utc_t,_DISCARDED,request_method,request_uri,request_query,_DISCARDED,_DISCARDED,client,user_agent,referer,response_code,_DISCARDED,_DISCARDED,time_taken_ms};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseIisFormatString(format_string, IISLogsModule::W3C);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test all the considered fields for the W3C module
    format_string = "date time cs-version cs-method cs-uri-stem cs-uri-query sc-status sc-bytes cs-bytes time-taken cs(Referer) cs(Cookie) cs(User-Agent) c-ip";
    fields = {date_time_utc_d,date_time_utc_t,request_protocol,request_method,request_uri,request_query,response_code,bytes_sent,bytes_received,time_taken_ms,referer,cookie,user_agent,client};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseIisFormatString(format_string, IISLogsModule::W3C);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test all the non-considered fields for the W3C module
    format_string = "s-sitename s-computername s-ip s-port cs-username cs-host sc-substatus sc-win32-status streamid";
    fields = {_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "};
    lf = FormatOps::Private::parseIisFormatString(format_string, IISLogsModule::W3C);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test an empty string for the W3C module
    format_string.erase();
    lf = FormatOps::Private::parseIisFormatString(format_string, IISLogsModule::W3C);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields.empty() );
    T_ASSERT( lf.separators.empty() );
    T_ASSERT( lf.final.empty() );
    // test the the NCSA module
    format_string = "some random useless text";
    fields = {client,_DISCARDED,_DISCARDED,date_time_ncsa,request_full,response_code,bytes_sent};
    separators = {" "," "," [","] \"","\" "," "};
    lf = FormatOps::Private::parseIisFormatString(format_string, IISLogsModule::NCSA);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test an empty string for the NCSA module
    format_string.erase();
    lf = FormatOps::Private::parseIisFormatString(format_string, IISLogsModule::NCSA);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final.empty() );
    // test the the IIS module
    format_string = "some random useless text";
    fields = {client,_DISCARDED,date_time_mdyyyy,date_time_utc_t,_DISCARDED,_DISCARDED,_DISCARDED,time_taken_ms,bytes_received,bytes_sent,response_code,_DISCARDED,request_method,request_uri,request_query};
    separators = {", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", "};
    lf = FormatOps::Private::parseIisFormatString(format_string, IISLogsModule::IIS);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final == "," );
    // test an empty string for the IIS module
    format_string.erase();
    lf = FormatOps::Private::parseIisFormatString(format_string, IISLogsModule::IIS);
    T_ASSERT( lf.initial.empty() );
    T_ASSERT( lf.fields == fields );
    T_ASSERT( lf.separators == separators );
    T_ASSERT( lf.final == "," );
    }
    T_TEST_RESULT()


    //// LOGS TYPE ////

    T_TEST_START("LogOps::defineFileType")
    {
    LogsFormat lf{ "","","]",{" ","_"},{_DISCARDED,_DISCARDED,_DISCARDED},0 };
    T_EXPECT( LogOps::defineFileType({"ok ok_ok]","a a_a]","TEST TEST_TEST]"}, lf), LogType::Access );
    T_EXPECT( LogOps::defineFileType({"no no no!","some thing wrong","with this file!"}, lf), LogType::Discarded );
    T_EXPECT( LogOps::defineFileType({}, lf), LogType::Failed );
    }
    T_TEST_RESULT()


    //// LOGS PARSING ////

    T_TEST_START("LogLineData::LogLineData")
    {
    LogsFormat lf;
    std::string log_line;
    {
    lf = FormatOps::Private::parseApacheFormatString(R"(%h %l %u %t "%r" %>s %O "%{Referer}i" "%{User-agent}i")");
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET /index.php?query=x HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.year == true && *line_data.year == "2000" );
    T_ASSERT( line_data.month == true && *line_data.month == "01" );
    T_ASSERT( line_data.day == true && *line_data.day == "01" );
    T_ASSERT( line_data.hour == true && *line_data.hour == "23" );
    T_ASSERT( line_data.minute == true && *line_data.minute == "59" );
    T_ASSERT( line_data.second == true && *line_data.second == "59" );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    T_ASSERT( line_data.response_code == true && *line_data.response_code == "200" );
    T_ASSERT( line_data.time_taken == false && *line_data.time_taken == "" );
    T_ASSERT( line_data.bytes_sent == true && *line_data.bytes_sent == "1024" );
    T_ASSERT( line_data.bytes_received == false && *line_data.bytes_received == "" );
    T_ASSERT( line_data.client == true && *line_data.client == "192.168.1.123" );
    T_ASSERT( line_data.cookie == false && *line_data.cookie == "" );
    T_ASSERT( line_data.user_agent == true && *line_data.user_agent == "UserAgent/3.0 (Details stuff) Info/123" );
    T_ASSERT( line_data.referrer == true && *line_data.referrer == "http://www.referrer.site" );
    }{
    // same as above but without the query
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET /index.php HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // dumb logging, without any surrounding character to enclose the request
    lf = FormatOps::Private::parseApacheFormatString(R"(%h %l %u %t %r %>s %O "%{Referer}i" "%{User-agent}i")");
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] GET /index.php?query=x HTTP/1.1 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.year == true && *line_data.year == "2000" );
    T_ASSERT( line_data.month == true && *line_data.month == "01" );
    T_ASSERT( line_data.day == true && *line_data.day == "01" );
    T_ASSERT( line_data.hour == true && *line_data.hour == "23" );
    T_ASSERT( line_data.minute == true && *line_data.minute == "59" );
    T_ASSERT( line_data.second == true && *line_data.second == "59" );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    T_ASSERT( line_data.response_code == true && *line_data.response_code == "200" );
    T_ASSERT( line_data.time_taken == false && *line_data.time_taken == "" );
    T_ASSERT( line_data.bytes_sent == true && *line_data.bytes_sent == "1024" );
    T_ASSERT( line_data.bytes_received == false && *line_data.bytes_received == "" );
    T_ASSERT( line_data.client == true && *line_data.client == "192.168.1.123" );
    T_ASSERT( line_data.cookie == false && *line_data.cookie == "" );
    T_ASSERT( line_data.user_agent == true && *line_data.user_agent == "UserAgent/3.0 (Details stuff) Info/123" );
    T_ASSERT( line_data.referrer == true && *line_data.referrer == "http://www.referrer.site" );
    }{
    // dumb logging, without any surrounding character to enclose the request and the user-agent
    lf = FormatOps::Private::parseApacheFormatString(R"(%h %l %u %t %r %>s %O "%{Referer}i" %{User-agent}i)");
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] GET /index.php?query=x HTTP/1.1 200 1024 "http://www.referrer.site" UserAgent/3.0 (Details stuff) Info/123)";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    T_ASSERT( line_data.response_code == true && *line_data.response_code == "200" );
    T_ASSERT( line_data.time_taken == false && *line_data.time_taken == "" );
    T_ASSERT( line_data.bytes_sent == true && *line_data.bytes_sent == "1024" );
    T_ASSERT( line_data.bytes_received == false && *line_data.bytes_received == "" );
    T_ASSERT( line_data.client == true && *line_data.client == "192.168.1.123" );
    T_ASSERT( line_data.cookie == false && *line_data.cookie == "" );
    T_ASSERT( line_data.user_agent == true && *line_data.user_agent == "UserAgent/3.0 (Details stuff) Info/123" );
    T_ASSERT( line_data.referrer == true && *line_data.referrer == "http://www.referrer.site" );
    }{
    // malformed request with empty method
    lf = FormatOps::Private::parseApacheFormatString(R"(%h %l %u %t "%r" %>s %O "%{Referer}i" "%{User-agent}i")");
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] " /index.php?query=x HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with empty protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET /index.php?query=x " 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with empty uri
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET  HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with empty method and protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] " /index.php?query=x " 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with empty method and uri
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "  HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with empty uri and protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET  " 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with all fields empty
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "  " 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with missing method
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "/index.php?query=x HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with missing protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET /index.php?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with missing uri
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with missing the uri but with the query
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET ?query=x HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with only the method
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with only the protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with only the uri and the query
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "/index.php?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with only the uri
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "/index.php" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with only the query
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with every field missing
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == false && *line_data.uri == "" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with only random data
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "M4l1C10US" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == false && *line_data.protocol == "" );
    T_ASSERT( line_data.method == false && *line_data.method == "" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "M4l1C10US" );
    T_ASSERT( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with method and protocol positions swapped
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "HTTP/1.1 /index.php?query=x GET" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with method and uri positions swapped
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "/index.php?query=x GET HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with uri and protocol positions swapped
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET HTTP/1.1 /index.php?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with all fields positions swapped
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "HTTP/1.1 GET /index.php?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    T_ASSERT( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    T_ASSERT( line_data.method == true && *line_data.method == "GET" );
    T_ASSERT( line_data.uri == true && *line_data.uri == "/index.php" );
    T_ASSERT( line_data.query == true && *line_data.query == "query=x" );
    }
    }
    T_TEST_RESULT()

    T_FIN()
}


bool testCrapviewModules()
{
    T_INIT("crapview modules")

    //// FILTERS ////

    T_TEST_START("FilterOps::parseNull")
    T_EXPECT( FilterOps::parseNull("null").value(), " IS NULL" );
    T_EXPECT( FilterOps::parseNull("NULL").value(), " IS NULL" );
    T_EXPECT( FilterOps::parseNull("not null").value(), " IS NOT NULL" );
    T_EXPECT( FilterOps::parseNull("NOT NULL").value(), " IS NOT NULL" );
    T_EXPECT( FilterOps::parseNull("!NULL").value(), " IS NOT NULL" );
    T_EXPECT( FilterOps::parseNull("! NULL").value(), " IS NOT NULL" );
    // try to brake
    T_EXPECT( FilterOps::parseNull("  null   ").value(), " IS NULL" );
    T_EXPECT( FilterOps::parseNull("  not null   ").value(), " IS NOT NULL" );
    T_EXPECT( FilterOps::parseNull("not   null").value(), " IS NOT NULL" );
    T_EXPECT( FilterOps::parseNull("!    null").value(), " IS NOT NULL" );
    // test for invalid filter
    T_ASSERT_NOT( FilterOps::parseNull("").has_value() );
    T_ASSERT_NOT( FilterOps::parseNull(" ").has_value() );
    T_ASSERT_NOT( FilterOps::parseNull(" \t\n ").has_value() );
    T_ASSERT_NOT( FilterOps::parseNull("123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNull("abc").has_value() );
    T_ASSERT_NOT( FilterOps::parseNull("*").has_value() );
    T_ASSERT_NOT( FilterOps::parseNull("null test").has_value() );
    T_ASSERT_NOT( FilterOps::parseNull("not null test").has_value() );
    T_TEST_RESULT()

    T_TEST_START("FilterOps::parseTextualFilter")
    T_EXPECT( FilterOps::parseTextualFilter("").value(), "" );
    T_EXPECT( FilterOps::parseTextualFilter(" ").value(), "" );
    T_EXPECT( FilterOps::parseTextualFilter(" \t\n ").value(), "" );
    T_EXPECT( FilterOps::parseTextualFilter("null").value(), " IS NULL" );
    T_EXPECT( FilterOps::parseTextualFilter("not null").value(), " IS NOT NULL" );
    T_EXPECT( FilterOps::parseTextualFilter("*").value(), " IS NOT NULL" );
    T_EXPECT( FilterOps::parseTextualFilter("test").value(), " LIKE 'test'" );
    T_EXPECT( FilterOps::parseTextualFilter("!test").value(), " NOT LIKE 'test'" );
    T_EXPECT( FilterOps::parseTextualFilter("\\!test").value(), " LIKE '!test'" );
    T_EXPECT( FilterOps::parseTextualFilter(" test").value(), " LIKE 'test'" );
    T_EXPECT( FilterOps::parseTextualFilter("\\ test").value(), " LIKE ' test'" );
    // try to brake
    T_EXPECT( FilterOps::parseTextualFilter("  a test string ").value(), " LIKE 'a test string'" );
    T_EXPECT( FilterOps::parseTextualFilter("! a test string").value(), " NOT LIKE 'a test string'" );
    T_EXPECT( FilterOps::parseTextualFilter("[test 123 #!?]").value(), " LIKE '[test 123 #!?]'" );
    T_TEST_RESULT()

    T_TEST_START("FilterOps::parseNumericFilter")
    T_EXPECT( FilterOps::parseNumericFilter("").value(), "" );
    T_EXPECT( FilterOps::parseNumericFilter(" ").value(), "" );
    T_EXPECT( FilterOps::parseNumericFilter(" \t\n ").value(), "" );
    T_EXPECT( FilterOps::parseNumericFilter("null").value(), " IS NULL" );
    T_EXPECT( FilterOps::parseNumericFilter("not null").value(), " IS NOT NULL" );
    T_EXPECT( FilterOps::parseNumericFilter("123").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter("=123").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter("= 123").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter("==123").value(), "==123" );
    T_EXPECT( FilterOps::parseNumericFilter("== 123").value(), "==123" );
    T_EXPECT( FilterOps::parseNumericFilter("eq 123").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter("EQ 123").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter("!123").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter("!=123").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter("!= 123").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter("ne 123").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter("NE 123").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter(">123").value(), ">123" );
    T_EXPECT( FilterOps::parseNumericFilter("> 123").value(), ">123" );
    T_EXPECT( FilterOps::parseNumericFilter("gt 123").value(), ">123" );
    T_EXPECT( FilterOps::parseNumericFilter("GT 123").value(), ">123" );
    T_EXPECT( FilterOps::parseNumericFilter(">=123").value(), ">=123" );
    T_EXPECT( FilterOps::parseNumericFilter(">= 123").value(), ">=123" );
    T_EXPECT( FilterOps::parseNumericFilter("ge 123").value(), ">=123" );
    T_EXPECT( FilterOps::parseNumericFilter("GE 123").value(), ">=123" );
    T_EXPECT( FilterOps::parseNumericFilter("<123").value(), "<123" );
    T_EXPECT( FilterOps::parseNumericFilter("< 123").value(), "<123" );
    T_EXPECT( FilterOps::parseNumericFilter("lt 123").value(), "<123" );
    T_EXPECT( FilterOps::parseNumericFilter("LT 123").value(), "<123" );
    T_EXPECT( FilterOps::parseNumericFilter("<=123").value(), "<=123" );
    T_EXPECT( FilterOps::parseNumericFilter("<= 123").value(), "<=123" );
    T_EXPECT( FilterOps::parseNumericFilter("le 123").value(), "<=123" );
    T_EXPECT( FilterOps::parseNumericFilter("LE 123").value(), "<=123" );
    // try to brake
    T_EXPECT( FilterOps::parseNumericFilter("   123 ").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter("1 2 3").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter(" =   12   3 ").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter("= =123").value(), "==123" );
    T_EXPECT( FilterOps::parseNumericFilter(" = = 1 2 3 ").value(), "==123" );
    T_EXPECT( FilterOps::parseNumericFilter(" !   123 ").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter("! =123").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter("! = 123").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter("eq123").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter("EQ123").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter(" E Q 1 2 3 ").value(), "=123" );
    T_EXPECT( FilterOps::parseNumericFilter("ne123").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter("NE123").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter(" N E 1 2 3 ").value(), "!=123" );
    T_EXPECT( FilterOps::parseNumericFilter("gt123").value(), ">123" );
    T_EXPECT( FilterOps::parseNumericFilter("GT123").value(), ">123" );
    T_EXPECT( FilterOps::parseNumericFilter(" G T 1 2 3 ").value(), ">123" );
    T_EXPECT( FilterOps::parseNumericFilter("ge123").value(), ">=123" );
    T_EXPECT( FilterOps::parseNumericFilter("GE123").value(), ">=123" );
    T_EXPECT( FilterOps::parseNumericFilter(" G E 1 2 3 ").value(), ">=123" );
    T_EXPECT( FilterOps::parseNumericFilter("lt123").value(), "<123" );
    T_EXPECT( FilterOps::parseNumericFilter("LT123").value(), "<123" );
    T_EXPECT( FilterOps::parseNumericFilter(" L T 1 2 3 ").value(), "<123" );
    T_EXPECT( FilterOps::parseNumericFilter("le123").value(), "<=123" );
    T_EXPECT( FilterOps::parseNumericFilter("LE123").value(), "<=123" );
    T_EXPECT( FilterOps::parseNumericFilter(" L E 1 2 3 ").value(), "<=123" );
    // test for invalid filter
    T_ASSERT_NOT( FilterOps::parseNumericFilter("*").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("===123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("!==123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("!>123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("!<123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter(">>123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("=>123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter(">==123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter(">>=123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("<<123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("=<123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("<==123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("<<=123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("EQ ").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("EQEQ 123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("EQ EQ 123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("NE ").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("NEEQ 123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("NE EQ 123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("GT ").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("GTEQ 123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("GT EQ 123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("GE ").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("LT ").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("LTEQ 123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("LT EQ 123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("LE ").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("abc").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("abc xyz").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("abc 123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("123 abc").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("1a2b3c").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("+123").has_value() );
    T_ASSERT_NOT( FilterOps::parseNumericFilter("-123").has_value() );
    T_TEST_RESULT()

    T_FIN()
}


} // namespace Testing
