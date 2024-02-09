
#include <iostream>
#include <assert.h>
#include <sstream>

#include "main_lib.h"

#include "utilities/io.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/exceptions.h"

#include "modules/craplog/modules/datetime.h"
#include "modules/craplog/modules/formats.h"
#include "modules/craplog/modules/logs.h"
#include "modules/craplog/modules/workers/lib.h"

#include "modules/crapview/modules/filters.h"

#include <QTimeZone>


#define T_PRINT(ARG) std::cout << "  [PASSED] " ARG "()" << std::endl;

#define REDIRECT_STDOUT()\
    std::streambuf* out{ std::cout.rdbuf() };\
    std::stringstream out_;\
    std::cout.rdbuf( out_.rdbuf() );

#define RESTORE_STDOUT()\
    std::cout.rdbuf( out );

#define REDIRECT_STDERR()\
    std::streambuf* err{ std::cerr.rdbuf() };\
    std::stringstream err_;\
    std::cerr.rdbuf( err_.rdbuf() );

#define RESTORE_STDERR()\
    std::cerr.rdbuf( err );


namespace Testing
{

void testOperators()
{
    //// DIALOGS LEVEL ////

    assert( DialogsLevel::Essential < DialogsLevel::Normal );
    assert( DialogsLevel::Essential < DialogsLevel::Explanatory );
    assert( DialogsLevel::Normal < DialogsLevel::Explanatory );
    assert( ! (DialogsLevel::Essential < DialogsLevel::Essential) );
    assert( ! (DialogsLevel::Normal < DialogsLevel::Normal) );
    assert( ! (DialogsLevel::Explanatory < DialogsLevel::Explanatory) );
    T_PRINT("DialogsLevel::operator <");

    assert( DialogsLevel::Essential >= DialogsLevel::Essential );
    assert( DialogsLevel::Normal >= DialogsLevel::Essential );
    assert( DialogsLevel::Normal >= DialogsLevel::Normal );
    assert( DialogsLevel::Explanatory >= DialogsLevel::Essential );
    assert( DialogsLevel::Explanatory >= DialogsLevel::Normal );
    assert( DialogsLevel::Explanatory >= DialogsLevel::Explanatory );
    assert( ! (DialogsLevel::Essential >= DialogsLevel::Normal) );
    assert( ! (DialogsLevel::Essential >= DialogsLevel::Explanatory) );
    assert( ! (DialogsLevel::Normal >= DialogsLevel::Explanatory) );
    T_PRINT("DialogsLevel::operator >=");

    //// FIELD DATA ////

    {
    const FieldData fd;
    assert( fd == false );
    }{
    const FieldData fd("");
    assert( fd == false );
    }{
    const FieldData fd("*");
    assert( fd == true );
    }
    T_PRINT("FieldData::operator bool");

    {
    const FieldData fd;
    assert( 0ul + fd == 0ul );
    }{
    const FieldData fd("");
    assert( 0ul + fd == 0ul );
    }{
    const FieldData fd("1");
    assert( 0ul + fd == 1ul );
    }{
    const FieldData fd("0123456789");
    assert( 10ul + fd == 20ul );
    assert( fd + 10ul != 20ul ); // fd evaluates to bool (aka 1)
    }{
    const FieldData fd1;
    const FieldData fd2;
    assert( fd1 + fd2 == 0ul );
    }{
    const FieldData fd1("");
    const FieldData fd2("");
    assert( fd1 + fd2 == 0ul );
    }{
    const FieldData fd1("");
    const FieldData fd2("123");
    assert( fd1 + fd2 == 3ul );
    }{
    const FieldData fd1("123");
    const FieldData fd2("456");
    assert( fd1 + fd2 == 6ul );
    }{
    const FieldData fd1("123");
    const FieldData fd2("456");
    assert( 4ul + fd1 + fd2 == 10ul );
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    assert( fd1 + fd2 + fd3 == 30ul );
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    assert( 70ul + fd1 + fd2 + fd3 == 100ul );
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    assert( fd1 + fd2 + fd3 + 70ul == 100ul );
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    assert( 10ul + fd1 + fd2 + fd3 + 10ul == 50ul );
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    assert( 10ul + (fd1 + fd2) + fd3 + 10ul == 50ul );
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    assert( 10ul + (fd1 + fd2) + (fd3 + 10ul) != 50ul ); // fd3 evaluates to bool (== 1)
    }{
    const FieldData fd1("0123456789");
    const FieldData fd2("0123456789");
    const FieldData fd3("0123456789");
    const FieldData fd4("0123456789");
    const FieldData fd5("0123456789");
    assert( 10ul + fd1 + 20ul + fd2 + fd3 + 10ul + fd4 + 10ul + fd5 == 100ul );
    }
    T_PRINT("FieldData::operator +");
}


void testUtilities()
{
    //// IO UTILS ////

    assert( IOutils::exists("../logdoctor") == true );
    assert( IOutils::exists("some/fancy/path") == false );
    T_PRINT("IOutils::exists");

    assert( IOutils::isFile("../logdoctor/main.cpp") == true );
    assert( IOutils::isFile("../logdoctor/tests") == false );
    T_PRINT("IOutils::isFile");

    assert( IOutils::isDir("../logdoctor/tests") == true );
    assert( IOutils::isDir("../logdoctor/main.cpp") == false );
    T_PRINT("IOutils::isDir");

    assert( IOutils::checkFile("../logdoctor/main.cpp") == true );
    assert( IOutils::checkFile("../logdoctor/tests") == false );
    T_PRINT("IOutils::checkFile");

    assert( IOutils::checkDir("../logdoctor/tests") == true );
    assert( IOutils::checkDir("../logdoctor/main.cpp") == false );
    T_PRINT("IOutils::checkDir");


    //// CHARS ////

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

    for (const char& c : numerical_chars)
        assert( CharOps::isNumeric(c) == true );
    for (const char& c : every_other_char_but(numerical_chars))
        assert( CharOps::isNumeric(c) == false );
    T_PRINT("CharOps::isNumeric");

    for (const char& c : alphabetical_chars)
        assert( CharOps::isAlphabetic(c) == true );
    for (const char& c : every_other_char_but(alphabetical_chars))
        assert( CharOps::isAlphabetic(c) == false );
    T_PRINT("CharOps::isAlphabetic");

    for (const char& c : alphanumerical_chars)
        assert( CharOps::isAlnum(c) == true );
    for (const char& c : every_other_char_but(alphanumerical_chars))
        assert( CharOps::isAlnum(c) == false );
    T_PRINT("CharOps::isAlnum");

    for (const char& c : hexadecimal_chars)
        assert( CharOps::isHex(c) == true );
    for (const char& c : every_other_char_but(hexadecimal_chars))
        assert( CharOps::isHex(c) == false );
    T_PRINT("CharOps::isHex");

    for (const char& c : ip_chars)
        assert( CharOps::isIP(c) == true );
    for (const char& c : every_other_char_but(ip_chars))
        assert( CharOps::isIP(c) == false );
    T_PRINT("CharOps::isIP");
    }


    //// STRINGS ////

    assert( StringOps::count("test test  test   test ", ' ') == 7ul );
    assert( StringOps::count("flagABCflagXYZflag", "flag") == 3ul );
    T_PRINT("StringOps::count");

    assert( StringOps::isNumeric("0123456789") == true );
    assert( StringOps::isNumeric("0123456789 ") == false );
    T_PRINT("StringOps::isNumeric");

    assert( StringOps::isAlphabetic("abcXYZ") == true );
    assert( StringOps::isAlphabetic("abc_XYZ") == false );
    T_PRINT("StringOps::isAlphabetic");

    assert( StringOps::isAlnum("testME123") == true );
    assert( StringOps::isAlnum("Fail!") == false );
    T_PRINT("StringOps::isAlnum");

    assert( StringOps::isIP("192.168.1.1") == true );
    assert( StringOps::isIP("::1") == true );
    assert( StringOps::isIP("987654321.") == true );
    assert( StringOps::isIP("abcdef:") == true );
    assert( StringOps::isIP(".: ") == false );
    T_PRINT("StringOps::isIP");

    assert( StringOps::startsWith("/path/to", '/') == true );
    assert( StringOps::startsWith(" rand", 'r') == false );
    assert( StringOps::startsWith("flag", "flag") == true );
    assert( StringOps::startsWith("flag", "lag") == false );
    T_PRINT("StringOps::startsWith");

    assert( StringOps::endsWith("space_pad ", ' ') == true );
    assert( StringOps::endsWith("rand ", 'd') == false );
    assert( StringOps::endsWith("the end", " end") == true );
    assert( StringOps::endsWith("the end", "_end") == false );
    T_PRINT("StringOps::endsWith");

    assert( StringOps::contains("abcdefedcba", "efe") == true );
    assert( StringOps::contains("this_should_fail", "SHOULD") == false );
    T_PRINT("StringOps::contains");

    assert( StringOps::strip("___test___", '_') == "test" );
    assert( StringOps::strip("test", ' ') == "test" );
    assert( StringOps::strip("    ", ' ') == "" );
    assert( StringOps::strip("", ' ') == "" );
    assert( StringOps::strip("a b ctestc", " abc") == "test" );
    assert( StringOps::strip("\n\t \ntest\r ") == "test" );
    assert( StringOps::strip("test") == "test" );
    assert( StringOps::strip(" \t\n\r") == "" );
    assert( StringOps::strip("") == "" );
    T_PRINT("StringOps::strip");

    assert( StringOps::lstrip("___test_", '_') == "test_" );
    assert( StringOps::lstrip("test", ' ') == "test" );
    assert( StringOps::lstrip("    ", ' ') == "" );
    assert( StringOps::lstrip("", ' ') == "" );
    assert( StringOps::lstrip("the three trees", " ethr") == "s" );
    assert( StringOps::lstrip(" \n\t\rtest ") == "test " );
    assert( StringOps::lstrip("test") == "test" );
    assert( StringOps::lstrip(" \t\n\r") == "" );
    assert( StringOps::lstrip("") == "" );
    T_PRINT("StringOps::lstrip");

    assert( StringOps::rstrip("_test___", '_') == "_test" );
    assert( StringOps::rstrip("test", ' ') == "test" );
    assert( StringOps::rstrip("    ", ' ') == "" );
    assert( StringOps::rstrip("", ' ') == "" );
    assert( StringOps::rstrip("testTEST", "TEST") == "test" );
    assert( StringOps::rstrip(" test\r\t\n ") == " test" );
    assert( StringOps::rstrip("test") == "test" );
    assert( StringOps::rstrip(" \t\n\r") == "" );
    assert( StringOps::rstrip("") == "" );
    T_PRINT("StringOps::rstrip");

    assert( StringOps::lstripUntil("is ok?", ' ') == "ok?" );
    assert( StringOps::lstripUntil("yes, it is", ' ', false) == " it is" );
    assert( StringOps::lstripUntil("__test", '_', false, false) == "__test" );
    assert( StringOps::lstripUntil("__test", '_', true, false) == "_test" );
    assert( StringOps::lstripUntil("__test", '_', true, true) == "test" );
    assert( StringOps::lstripUntil("   ", '_') == "   " );
    assert( StringOps::lstripUntil("   ", ' ') == "" );
    assert( StringOps::lstripUntil("", ' ') == "" );
    T_PRINT("StringOps::lstripUntil");

    {
    std::vector<std::string> v;
    std::vector<std::string> t{"ok","ok","ok"};
    StringOps::split(v, "ok\nok\nok");
    assert( v == t );
    }{
    std::vector<std::string> v;
    std::vector<std::string> t{"es","ing s","uff"};
    StringOps::split(v, "testing stuff", 't');
    assert( v == t );
    }{
    std::vector<std::string> v;
    std::vector<std::string> t{"should","work","as expected"};
    StringOps::split(v, "  should  work  as expected  ", "  ");
    assert( v == t );
    }
    T_PRINT("StringOps::split");

    {
    std::vector<std::string> v;
    std::vector<std::string> t{"ok","ok","ok"};
    StringOps::splitrip(v, " ok \n ok \n ok ");
    assert( v == t );
    }{
    std::vector<std::string> v;
    std::vector<std::string> t{"ok","ok","ok"};
    StringOps::splitrip(v, "ok.   .ok.   .ok", ' ', ".");
    assert( v == t );
    }{
    std::vector<std::string> v;
    std::vector<std::string> t{"test","passed"};
    StringOps::splitrip(v, "iftesthaspassedfi", "has", "if");
    assert( v == t );
    }
    T_PRINT("StringOps::splitrip");

    assert( StringOps::replace("replace every space", " ", "_") == "replace_every_space" );
    assert( StringOps::replace("thERR corrERRct phrasERR", "ERR", "e") == "the correct phrase" );
    T_PRINT("StringOps::replace");

    assert( StringOps::toUpper("raise me up") == "RAISE ME UP" );
    assert( StringOps::toUpper("1 2 3") == "1 2 3" );
    T_PRINT("StringOps::toUpper");

    assert( StringOps::toLower("LO-O-OW") == "lo-o-ow" );
    assert( StringOps::toLower("9 8 7") == "9 8 7" );
    T_PRINT("StringOps::toLower");


    //// VECTORS ////

    assert( VecOps::contains<int>({1,2,4,8,16,32,64}, 4) == true );
    assert( VecOps::contains<char>({'a','b','c'}, 'z') == false );
    assert( VecOps::contains<std::string>({"catch","the","flag"}, "flag") == true );
    T_PRINT("VecOps::contains");
}


void testCraplogModules()
{
    //// LOGS FORMAT FIELDS ////

    {
    // test for truth
    assert( _NO_PARSE_NEEDED & date_time_year );
    assert( _NO_PARSE_NEEDED & date_time_month );
    assert( _NO_PARSE_NEEDED & date_time_day );
    assert( _NO_PARSE_NEEDED & date_time_hour );
    assert( _NO_PARSE_NEEDED & date_time_minute );
    assert( _NO_PARSE_NEEDED & date_time_second );
    assert( _NO_PARSE_NEEDED & request_protocol );
    assert( _NO_PARSE_NEEDED & request_method );
    assert( _NO_PARSE_NEEDED & request_uri );
    assert( _NO_PARSE_NEEDED & request_query );
    assert( _NO_PARSE_NEEDED & response_code );
    assert( _NO_PARSE_NEEDED & bytes_received );
    assert( _NO_PARSE_NEEDED & bytes_sent );
    assert( _NO_PARSE_NEEDED & time_taken_ms );
    assert( _NO_PARSE_NEEDED & client );
    assert( _NO_PARSE_NEEDED & user_agent );
    assert( _NO_PARSE_NEEDED & cookie );
    assert( _NO_PARSE_NEEDED & referer );
    // test for false
    assert( !( _NO_PARSE_NEEDED & _INVALID ) );
    assert( !( _NO_PARSE_NEEDED & _DISCARDED ) );
    assert( !( _NO_PARSE_NEEDED & date_time_epoch_us ) );
    assert( !( _NO_PARSE_NEEDED & date_time_epoch_ms ) );
    assert( !( _NO_PARSE_NEEDED & date_time_epoch_s ) );
    assert( !( _NO_PARSE_NEEDED & date_time_epoch_s_ms ) );
    assert( !( _NO_PARSE_NEEDED & date_time_ncsa ) );
    assert( !( _NO_PARSE_NEEDED & date_time_iso ) );
    assert( !( _NO_PARSE_NEEDED & date_time_gmt ) );
    assert( !( _NO_PARSE_NEEDED & date_time_mcs ) );
    assert( !( _NO_PARSE_NEEDED & date_time_utc_d ) );
    assert( !( _NO_PARSE_NEEDED & date_time_utc_t ) );
    assert( !( _NO_PARSE_NEEDED & date_time_yyyymmdd ) );
    assert( !( _NO_PARSE_NEEDED & date_time_mmddyy ) );
    assert( !( _NO_PARSE_NEEDED & date_time_mdyyyy ) );
    assert( !( _NO_PARSE_NEEDED & date_time_year_short ) );
    assert( !( _NO_PARSE_NEEDED & date_time_month_str ) );
    assert( !( _NO_PARSE_NEEDED & date_time_clock_24 ) );
    assert( !( _NO_PARSE_NEEDED & date_time_clock_12 ) );
    assert( !( _NO_PARSE_NEEDED & date_time_clock_short ) );
    assert( !( _NO_PARSE_NEEDED & request_uri_query ) );
    assert( !( _NO_PARSE_NEEDED & request_full ) );
    assert( !( _NO_PARSE_NEEDED & time_taken_us ) );
    assert( !( _NO_PARSE_NEEDED & time_taken_s ) );
    assert( !( _NO_PARSE_NEEDED & time_taken_s_ms ) );
    }{
    // test for truth
    assert( _DATE_TIME & date_time_epoch_us );
    assert( _DATE_TIME & date_time_epoch_ms );
    assert( _DATE_TIME & date_time_epoch_s );
    assert( _DATE_TIME & date_time_epoch_s_ms );
    assert( _DATE_TIME & date_time_ncsa );
    assert( _DATE_TIME & date_time_iso );
    assert( _DATE_TIME & date_time_gmt );
    assert( _DATE_TIME & date_time_mcs );
    assert( _DATE_TIME & date_time_utc_d );
    assert( _DATE_TIME & date_time_utc_t );
    assert( _DATE_TIME & date_time_yyyymmdd );
    assert( _DATE_TIME & date_time_mmddyy );
    assert( _DATE_TIME & date_time_mdyyyy );
    assert( _DATE_TIME & date_time_year_short );
    assert( _DATE_TIME & date_time_month_str );
    assert( _DATE_TIME & date_time_year );
    assert( _DATE_TIME & date_time_month );
    assert( _DATE_TIME & date_time_day );
    assert( _DATE_TIME & date_time_hour );
    assert( _DATE_TIME & date_time_minute );
    assert( _DATE_TIME & date_time_second );
    assert( _DATE_TIME & date_time_clock_short );
    assert( _DATE_TIME & date_time_clock_12 );
    assert( _DATE_TIME & date_time_clock_24 );
    // test for false
    assert( !( _DATE_TIME & _INVALID ) );
    assert( !( _DATE_TIME & _DISCARDED ) );
    assert( !( _DATE_TIME & request_protocol ) );
    assert( !( _DATE_TIME & request_method ) );
    assert( !( _DATE_TIME & request_uri ) );
    assert( !( _DATE_TIME & request_query ) );
    assert( !( _DATE_TIME & request_uri_query ) );
    assert( !( _DATE_TIME & request_full ) );
    assert( !( _DATE_TIME & response_code ) );
    assert( !( _DATE_TIME & bytes_received ) );
    assert( !( _DATE_TIME & bytes_sent ) );
    assert( !( _DATE_TIME & time_taken_us ) );
    assert( !( _DATE_TIME & time_taken_ms ) );
    assert( !( _DATE_TIME & time_taken_s ) );
    assert( !( _DATE_TIME & time_taken_s_ms ) );
    assert( !( _DATE_TIME & client ) );
    assert( !( _DATE_TIME & user_agent ) );
    assert( !( _DATE_TIME & cookie ) );
    assert( !( _DATE_TIME & referer ) );
    }{
    // test for truth
    assert( _DATE_TIME_FULL & date_time_epoch_us );
    assert( _DATE_TIME_FULL & date_time_epoch_ms );
    assert( _DATE_TIME_FULL & date_time_epoch_s );
    assert( _DATE_TIME_FULL & date_time_epoch_s_ms );
    assert( _DATE_TIME_FULL & date_time_ncsa );
    assert( _DATE_TIME_FULL & date_time_iso );
    assert( _DATE_TIME_FULL & date_time_gmt );
    assert( _DATE_TIME_FULL & date_time_mcs );
    // test for false
    assert( !( _DATE_TIME_FULL & date_time_utc_d ) );
    assert( !( _DATE_TIME_FULL & date_time_utc_t ) );
    assert( !( _DATE_TIME_FULL & date_time_yyyymmdd ) );
    assert( !( _DATE_TIME_FULL & date_time_mmddyy ) );
    assert( !( _DATE_TIME_FULL & date_time_mdyyyy ) );
    assert( !( _DATE_TIME_FULL & date_time_year_short ) );
    assert( !( _DATE_TIME_FULL & date_time_month_str ) );
    assert( !( _DATE_TIME_FULL & date_time_year ) );
    assert( !( _DATE_TIME_FULL & date_time_month ) );
    assert( !( _DATE_TIME_FULL & date_time_day ) );
    assert( !( _DATE_TIME_FULL & date_time_hour ) );
    assert( !( _DATE_TIME_FULL & date_time_minute ) );
    assert( !( _DATE_TIME_FULL & date_time_second ) );
    assert( !( _DATE_TIME_FULL & date_time_clock_short ) );
    assert( !( _DATE_TIME_FULL & date_time_clock_12 ) );
    assert( !( _DATE_TIME_FULL & date_time_clock_24 ) );
    }{
    // test for truth
    assert( _DATE_TIME_EPOCH & date_time_epoch_us );
    assert( _DATE_TIME_EPOCH & date_time_epoch_ms );
    assert( _DATE_TIME_EPOCH & date_time_epoch_s );
    assert( _DATE_TIME_EPOCH & date_time_epoch_s_ms );
    // test for false
    assert( !( _DATE_TIME_EPOCH & date_time_ncsa ) );
    assert( !( _DATE_TIME_EPOCH & date_time_iso ) );
    assert( !( _DATE_TIME_EPOCH & date_time_gmt ) );
    assert( !( _DATE_TIME_EPOCH & date_time_mcs ) );
    assert( !( _DATE_TIME_EPOCH & date_time_utc_d ) );
    assert( !( _DATE_TIME_EPOCH & date_time_utc_t ) );
    assert( !( _DATE_TIME_EPOCH & date_time_yyyymmdd ) );
    assert( !( _DATE_TIME_EPOCH & date_time_mmddyy ) );
    assert( !( _DATE_TIME_EPOCH & date_time_mdyyyy ) );
    assert( !( _DATE_TIME_EPOCH & date_time_year_short ) );
    assert( !( _DATE_TIME_EPOCH & date_time_month_str ) );
    assert( !( _DATE_TIME_EPOCH & date_time_year ) );
    assert( !( _DATE_TIME_EPOCH & date_time_month ) );
    assert( !( _DATE_TIME_EPOCH & date_time_day ) );
    assert( !( _DATE_TIME_EPOCH & date_time_hour ) );
    assert( !( _DATE_TIME_EPOCH & date_time_minute ) );
    assert( !( _DATE_TIME_EPOCH & date_time_second ) );
    assert( !( _DATE_TIME_EPOCH & date_time_clock_short ) );
    assert( !( _DATE_TIME_EPOCH & date_time_clock_12 ) );
    assert( !( _DATE_TIME_EPOCH & date_time_clock_24 ) );
    }{
    // test for truth
    assert( _DATE_TIME_UTC & date_time_utc_d );
    assert( _DATE_TIME_UTC & date_time_utc_t );
    // test for false
    assert( !( _DATE_TIME_UTC & date_time_epoch_us ) );
    assert( !( _DATE_TIME_UTC & date_time_epoch_ms ) );
    assert( !( _DATE_TIME_UTC & date_time_epoch_s ) );
    assert( !( _DATE_TIME_UTC & date_time_epoch_s_ms ) );
    assert( !( _DATE_TIME_UTC & date_time_ncsa ) );
    assert( !( _DATE_TIME_UTC & date_time_iso ) );
    assert( !( _DATE_TIME_UTC & date_time_gmt ) );
    assert( !( _DATE_TIME_UTC & date_time_mcs ) );
    assert( !( _DATE_TIME_UTC & date_time_yyyymmdd ) );
    assert( !( _DATE_TIME_UTC & date_time_mmddyy ) );
    assert( !( _DATE_TIME_UTC & date_time_mdyyyy ) );
    assert( !( _DATE_TIME_UTC & date_time_year_short ) );
    assert( !( _DATE_TIME_UTC & date_time_month_str ) );
    assert( !( _DATE_TIME_UTC & date_time_year ) );
    assert( !( _DATE_TIME_UTC & date_time_month ) );
    assert( !( _DATE_TIME_UTC & date_time_day ) );
    assert( !( _DATE_TIME_UTC & date_time_hour ) );
    assert( !( _DATE_TIME_UTC & date_time_minute ) );
    assert( !( _DATE_TIME_UTC & date_time_second ) );
    assert( !( _DATE_TIME_UTC & date_time_clock_short ) );
    assert( !( _DATE_TIME_UTC & date_time_clock_12 ) );
    assert( !( _DATE_TIME_UTC & date_time_clock_24 ) );
    }{
    // test for truth
    assert( _DATE_TIME_DATE & date_time_utc_d );
    assert( _DATE_TIME_DATE & date_time_yyyymmdd );
    assert( _DATE_TIME_DATE & date_time_mmddyy );
    assert( _DATE_TIME_DATE & date_time_mdyyyy );
    // test for false
    assert( !( _DATE_TIME_DATE & date_time_epoch_us ) );
    assert( !( _DATE_TIME_DATE & date_time_epoch_ms ) );
    assert( !( _DATE_TIME_DATE & date_time_epoch_s ) );
    assert( !( _DATE_TIME_DATE & date_time_epoch_s_ms ) );
    assert( !( _DATE_TIME_DATE & date_time_ncsa ) );
    assert( !( _DATE_TIME_DATE & date_time_iso ) );
    assert( !( _DATE_TIME_DATE & date_time_gmt ) );
    assert( !( _DATE_TIME_DATE & date_time_mcs ) );
    assert( !( _DATE_TIME_DATE & date_time_utc_t ) );
    assert( !( _DATE_TIME_DATE & date_time_year_short ) );
    assert( !( _DATE_TIME_DATE & date_time_month_str ) );
    assert( !( _DATE_TIME_DATE & date_time_year ) );
    assert( !( _DATE_TIME_DATE & date_time_month ) );
    assert( !( _DATE_TIME_DATE & date_time_day ) );
    assert( !( _DATE_TIME_DATE & date_time_hour ) );
    assert( !( _DATE_TIME_DATE & date_time_minute ) );
    assert( !( _DATE_TIME_DATE & date_time_second ) );
    assert( !( _DATE_TIME_DATE & date_time_clock_short ) );
    assert( !( _DATE_TIME_DATE & date_time_clock_12 ) );
    assert( !( _DATE_TIME_DATE & date_time_clock_24 ) );
    }{
    // test for truth
    assert( _DATE_TIME_YEAR & date_time_year_short );
    assert( _DATE_TIME_YEAR & date_time_year );
    // test for false
    assert( !( _DATE_TIME_YEAR & date_time_epoch_us ) );
    assert( !( _DATE_TIME_YEAR & date_time_epoch_ms ) );
    assert( !( _DATE_TIME_YEAR & date_time_epoch_s ) );
    assert( !( _DATE_TIME_YEAR & date_time_epoch_s_ms ) );
    assert( !( _DATE_TIME_YEAR & date_time_ncsa ) );
    assert( !( _DATE_TIME_YEAR & date_time_iso ) );
    assert( !( _DATE_TIME_YEAR & date_time_gmt ) );
    assert( !( _DATE_TIME_YEAR & date_time_mcs ) );
    assert( !( _DATE_TIME_YEAR & date_time_utc_d ) );
    assert( !( _DATE_TIME_YEAR & date_time_utc_t ) );
    assert( !( _DATE_TIME_YEAR & date_time_yyyymmdd ) );
    assert( !( _DATE_TIME_YEAR & date_time_mmddyy ) );
    assert( !( _DATE_TIME_YEAR & date_time_mdyyyy ) );
    assert( !( _DATE_TIME_YEAR & date_time_month_str ) );
    assert( !( _DATE_TIME_YEAR & date_time_month ) );
    assert( !( _DATE_TIME_YEAR & date_time_day ) );
    assert( !( _DATE_TIME_YEAR & date_time_hour ) );
    assert( !( _DATE_TIME_YEAR & date_time_minute ) );
    assert( !( _DATE_TIME_YEAR & date_time_second ) );
    assert( !( _DATE_TIME_YEAR & date_time_clock_short ) );
    assert( !( _DATE_TIME_YEAR & date_time_clock_12 ) );
    assert( !( _DATE_TIME_YEAR & date_time_clock_24 ) );
    }{
    // test for truth
    assert( _DATE_TIME_MONTH & date_time_month_str );
    assert( _DATE_TIME_MONTH & date_time_month );
    // test for false
    assert( !( _DATE_TIME_MONTH & date_time_epoch_us ) );
    assert( !( _DATE_TIME_MONTH & date_time_epoch_ms ) );
    assert( !( _DATE_TIME_MONTH & date_time_epoch_s ) );
    assert( !( _DATE_TIME_MONTH & date_time_epoch_s_ms ) );
    assert( !( _DATE_TIME_MONTH & date_time_ncsa ) );
    assert( !( _DATE_TIME_MONTH & date_time_iso ) );
    assert( !( _DATE_TIME_MONTH & date_time_gmt ) );
    assert( !( _DATE_TIME_MONTH & date_time_mcs ) );
    assert( !( _DATE_TIME_MONTH & date_time_utc_d ) );
    assert( !( _DATE_TIME_MONTH & date_time_utc_t ) );
    assert( !( _DATE_TIME_MONTH & date_time_yyyymmdd ) );
    assert( !( _DATE_TIME_MONTH & date_time_mmddyy ) );
    assert( !( _DATE_TIME_MONTH & date_time_mdyyyy ) );
    assert( !( _DATE_TIME_MONTH & date_time_year_short ) );
    assert( !( _DATE_TIME_MONTH & date_time_year ) );
    assert( !( _DATE_TIME_MONTH & date_time_day ) );
    assert( !( _DATE_TIME_MONTH & date_time_hour ) );
    assert( !( _DATE_TIME_MONTH & date_time_minute ) );
    assert( !( _DATE_TIME_MONTH & date_time_second ) );
    assert( !( _DATE_TIME_MONTH & date_time_clock_short ) );
    assert( !( _DATE_TIME_MONTH & date_time_clock_12 ) );
    assert( !( _DATE_TIME_MONTH & date_time_clock_24 ) );
    }{
    // test for truth
    assert( _DATE_TIME_CLOCK & date_time_clock_short );
    assert( _DATE_TIME_CLOCK & date_time_clock_12 );
    assert( _DATE_TIME_CLOCK & date_time_clock_24 );
    // test for false
    assert( !( _DATE_TIME_CLOCK & date_time_epoch_us ) );
    assert( !( _DATE_TIME_CLOCK & date_time_epoch_ms ) );
    assert( !( _DATE_TIME_CLOCK & date_time_epoch_s ) );
    assert( !( _DATE_TIME_CLOCK & date_time_epoch_s_ms ) );
    assert( !( _DATE_TIME_CLOCK & date_time_ncsa ) );
    assert( !( _DATE_TIME_CLOCK & date_time_iso ) );
    assert( !( _DATE_TIME_CLOCK & date_time_gmt ) );
    assert( !( _DATE_TIME_CLOCK & date_time_mcs ) );
    assert( !( _DATE_TIME_CLOCK & date_time_utc_d ) );
    assert( !( _DATE_TIME_CLOCK & date_time_utc_t ) );
    assert( !( _DATE_TIME_CLOCK & date_time_yyyymmdd ) );
    assert( !( _DATE_TIME_CLOCK & date_time_mmddyy ) );
    assert( !( _DATE_TIME_CLOCK & date_time_mdyyyy ) );
    assert( !( _DATE_TIME_CLOCK & date_time_year_short ) );
    assert( !( _DATE_TIME_CLOCK & date_time_month_str ) );
    assert( !( _DATE_TIME_CLOCK & date_time_year ) );
    assert( !( _DATE_TIME_CLOCK & date_time_month ) );
    assert( !( _DATE_TIME_CLOCK & date_time_day ) );
    assert( !( _DATE_TIME_CLOCK & date_time_hour ) );
    assert( !( _DATE_TIME_CLOCK & date_time_minute ) );
    assert( !( _DATE_TIME_CLOCK & date_time_second ) );
    }{
    // test for truth
    assert( _REQUEST & request_protocol );
    assert( _REQUEST & request_method );
    assert( _REQUEST & request_uri );
    assert( _REQUEST & request_query );
    assert( _REQUEST & request_uri_query );
    assert( _REQUEST & request_full );
    // test for false
    assert( !( _REQUEST & _INVALID ) );
    assert( !( _REQUEST & _DISCARDED ) );
    assert( !( _REQUEST & date_time_epoch_us ) );
    assert( !( _REQUEST & date_time_epoch_ms ) );
    assert( !( _REQUEST & date_time_epoch_s ) );
    assert( !( _REQUEST & date_time_epoch_s_ms ) );
    assert( !( _REQUEST & date_time_ncsa ) );
    assert( !( _REQUEST & date_time_iso ) );
    assert( !( _REQUEST & date_time_gmt ) );
    assert( !( _REQUEST & date_time_mcs ) );
    assert( !( _REQUEST & date_time_utc_d ) );
    assert( !( _REQUEST & date_time_utc_t ) );
    assert( !( _REQUEST & date_time_yyyymmdd ) );
    assert( !( _REQUEST & date_time_mmddyy ) );
    assert( !( _REQUEST & date_time_mdyyyy ) );
    assert( !( _REQUEST & date_time_year_short ) );
    assert( !( _REQUEST & date_time_month_str ) );
    assert( !( _REQUEST & date_time_year ) );
    assert( !( _REQUEST & date_time_month ) );
    assert( !( _REQUEST & date_time_day ) );
    assert( !( _REQUEST & date_time_hour ) );
    assert( !( _REQUEST & date_time_minute ) );
    assert( !( _REQUEST & date_time_second ) );
    assert( !( _REQUEST & date_time_clock_short ) );
    assert( !( _REQUEST & date_time_clock_12 ) );
    assert( !( _REQUEST & date_time_clock_24 ) );
    assert( !( _REQUEST & response_code ) );
    assert( !( _REQUEST & bytes_received ) );
    assert( !( _REQUEST & bytes_sent ) );
    assert( !( _REQUEST & time_taken_us ) );
    assert( !( _REQUEST & time_taken_ms ) );
    assert( !( _REQUEST & time_taken_s ) );
    assert( !( _REQUEST & time_taken_s_ms ) );
    assert( !( _REQUEST & client ) );
    assert( !( _REQUEST & user_agent ) );
    assert( !( _REQUEST & cookie ) );
    assert( !( _REQUEST & referer ) );
    }{
    // test for truth
    assert( _RESPONSE_CODE & response_code );
    // test for false
    assert( !( _RESPONSE_CODE & _INVALID ) );
    assert( !( _RESPONSE_CODE & _DISCARDED ) );
    assert( !( _RESPONSE_CODE & date_time_epoch_us ) );
    assert( !( _RESPONSE_CODE & date_time_epoch_ms ) );
    assert( !( _RESPONSE_CODE & date_time_epoch_s ) );
    assert( !( _RESPONSE_CODE & date_time_epoch_s_ms ) );
    assert( !( _RESPONSE_CODE & date_time_ncsa ) );
    assert( !( _RESPONSE_CODE & date_time_iso ) );
    assert( !( _RESPONSE_CODE & date_time_gmt ) );
    assert( !( _RESPONSE_CODE & date_time_mcs ) );
    assert( !( _RESPONSE_CODE & date_time_utc_d ) );
    assert( !( _RESPONSE_CODE & date_time_utc_t ) );
    assert( !( _RESPONSE_CODE & date_time_yyyymmdd ) );
    assert( !( _RESPONSE_CODE & date_time_mmddyy ) );
    assert( !( _RESPONSE_CODE & date_time_mdyyyy ) );
    assert( !( _RESPONSE_CODE & date_time_year_short ) );
    assert( !( _RESPONSE_CODE & date_time_month_str ) );
    assert( !( _RESPONSE_CODE & date_time_year ) );
    assert( !( _RESPONSE_CODE & date_time_month ) );
    assert( !( _RESPONSE_CODE & date_time_day ) );
    assert( !( _RESPONSE_CODE & date_time_hour ) );
    assert( !( _RESPONSE_CODE & date_time_minute ) );
    assert( !( _RESPONSE_CODE & date_time_second ) );
    assert( !( _RESPONSE_CODE & date_time_clock_short ) );
    assert( !( _RESPONSE_CODE & date_time_clock_12 ) );
    assert( !( _RESPONSE_CODE & date_time_clock_24 ) );
    assert( !( _RESPONSE_CODE & request_protocol ) );
    assert( !( _RESPONSE_CODE & request_method ) );
    assert( !( _RESPONSE_CODE & request_uri ) );
    assert( !( _RESPONSE_CODE & request_query ) );
    assert( !( _RESPONSE_CODE & request_uri_query ) );
    assert( !( _RESPONSE_CODE & request_full ) );
    assert( !( _RESPONSE_CODE & bytes_received ) );
    assert( !( _RESPONSE_CODE & bytes_sent ) );
    assert( !( _RESPONSE_CODE & time_taken_us ) );
    assert( !( _RESPONSE_CODE & time_taken_ms ) );
    assert( !( _RESPONSE_CODE & time_taken_s ) );
    assert( !( _RESPONSE_CODE & time_taken_s_ms ) );
    assert( !( _RESPONSE_CODE & client ) );
    assert( !( _RESPONSE_CODE & user_agent ) );
    assert( !( _RESPONSE_CODE & cookie ) );
    assert( !( _RESPONSE_CODE & referer ) );
    }{
    // test for truth
    assert( _DATA_TRANSFER & bytes_received );
    assert( _DATA_TRANSFER & bytes_sent );
    // test for false
    assert( !( _DATA_TRANSFER & _INVALID ) );
    assert( !( _DATA_TRANSFER & _DISCARDED ) );
    assert( !( _DATA_TRANSFER & date_time_epoch_us ) );
    assert( !( _DATA_TRANSFER & date_time_epoch_ms ) );
    assert( !( _DATA_TRANSFER & date_time_epoch_s ) );
    assert( !( _DATA_TRANSFER & date_time_epoch_s_ms ) );
    assert( !( _DATA_TRANSFER & date_time_ncsa ) );
    assert( !( _DATA_TRANSFER & date_time_iso ) );
    assert( !( _DATA_TRANSFER & date_time_gmt ) );
    assert( !( _DATA_TRANSFER & date_time_mcs ) );
    assert( !( _DATA_TRANSFER & date_time_utc_d ) );
    assert( !( _DATA_TRANSFER & date_time_utc_t ) );
    assert( !( _DATA_TRANSFER & date_time_yyyymmdd ) );
    assert( !( _DATA_TRANSFER & date_time_mmddyy ) );
    assert( !( _DATA_TRANSFER & date_time_mdyyyy ) );
    assert( !( _DATA_TRANSFER & date_time_year_short ) );
    assert( !( _DATA_TRANSFER & date_time_month_str ) );
    assert( !( _DATA_TRANSFER & date_time_year ) );
    assert( !( _DATA_TRANSFER & date_time_month ) );
    assert( !( _DATA_TRANSFER & date_time_day ) );
    assert( !( _DATA_TRANSFER & date_time_hour ) );
    assert( !( _DATA_TRANSFER & date_time_minute ) );
    assert( !( _DATA_TRANSFER & date_time_second ) );
    assert( !( _DATA_TRANSFER & date_time_clock_short ) );
    assert( !( _DATA_TRANSFER & date_time_clock_12 ) );
    assert( !( _DATA_TRANSFER & date_time_clock_24 ) );
    assert( !( _DATA_TRANSFER & request_protocol ) );
    assert( !( _DATA_TRANSFER & request_method ) );
    assert( !( _DATA_TRANSFER & request_uri ) );
    assert( !( _DATA_TRANSFER & request_query ) );
    assert( !( _DATA_TRANSFER & request_uri_query ) );
    assert( !( _DATA_TRANSFER & request_full ) );
    assert( !( _DATA_TRANSFER & response_code ) );
    assert( !( _DATA_TRANSFER & time_taken_us ) );
    assert( !( _DATA_TRANSFER & time_taken_ms ) );
    assert( !( _DATA_TRANSFER & time_taken_s ) );
    assert( !( _DATA_TRANSFER & time_taken_s_ms ) );
    assert( !( _DATA_TRANSFER & client ) );
    assert( !( _DATA_TRANSFER & user_agent ) );
    assert( !( _DATA_TRANSFER & cookie ) );
    assert( !( _DATA_TRANSFER & referer ) );
    }{
    // test for truth
    assert( _TIME_TAKEN & time_taken_us );
    assert( _TIME_TAKEN & time_taken_ms );
    assert( _TIME_TAKEN & time_taken_s );
    assert( _TIME_TAKEN & time_taken_s_ms );
    // test for false
    assert( !( _TIME_TAKEN & _INVALID ) );
    assert( !( _TIME_TAKEN & _DISCARDED ) );
    assert( !( _TIME_TAKEN & date_time_epoch_us ) );
    assert( !( _TIME_TAKEN & date_time_epoch_ms ) );
    assert( !( _TIME_TAKEN & date_time_epoch_s ) );
    assert( !( _TIME_TAKEN & date_time_epoch_s_ms ) );
    assert( !( _TIME_TAKEN & date_time_ncsa ) );
    assert( !( _TIME_TAKEN & date_time_iso ) );
    assert( !( _TIME_TAKEN & date_time_gmt ) );
    assert( !( _TIME_TAKEN & date_time_mcs ) );
    assert( !( _TIME_TAKEN & date_time_utc_d ) );
    assert( !( _TIME_TAKEN & date_time_utc_t ) );
    assert( !( _TIME_TAKEN & date_time_yyyymmdd ) );
    assert( !( _TIME_TAKEN & date_time_mmddyy ) );
    assert( !( _TIME_TAKEN & date_time_mdyyyy ) );
    assert( !( _TIME_TAKEN & date_time_year_short ) );
    assert( !( _TIME_TAKEN & date_time_month_str ) );
    assert( !( _TIME_TAKEN & date_time_year ) );
    assert( !( _TIME_TAKEN & date_time_month ) );
    assert( !( _TIME_TAKEN & date_time_day ) );
    assert( !( _TIME_TAKEN & date_time_hour ) );
    assert( !( _TIME_TAKEN & date_time_minute ) );
    assert( !( _TIME_TAKEN & date_time_second ) );
    assert( !( _TIME_TAKEN & date_time_clock_short ) );
    assert( !( _TIME_TAKEN & date_time_clock_12 ) );
    assert( !( _TIME_TAKEN & date_time_clock_24 ) );
    assert( !( _TIME_TAKEN & request_protocol ) );
    assert( !( _TIME_TAKEN & request_method ) );
    assert( !( _TIME_TAKEN & request_uri ) );
    assert( !( _TIME_TAKEN & request_query ) );
    assert( !( _TIME_TAKEN & request_uri_query ) );
    assert( !( _TIME_TAKEN & request_full ) );
    assert( !( _TIME_TAKEN & response_code ) );
    assert( !( _TIME_TAKEN & bytes_received ) );
    assert( !( _TIME_TAKEN & bytes_sent ) );
    assert( !( _TIME_TAKEN & client ) );
    assert( !( _TIME_TAKEN & user_agent ) );
    assert( !( _TIME_TAKEN & cookie ) );
    assert( !( _TIME_TAKEN & referer ) );
    }{
    // test for truth
    assert( _CLIENT_DATA & client );
    assert( _CLIENT_DATA & user_agent );
    assert( _CLIENT_DATA & cookie );
    assert( _CLIENT_DATA & referer );
    // test for false
    assert( !( _CLIENT_DATA & _INVALID ) );
    assert( !( _CLIENT_DATA & _DISCARDED ) );
    assert( !( _CLIENT_DATA & date_time_epoch_us ) );
    assert( !( _CLIENT_DATA & date_time_epoch_ms ) );
    assert( !( _CLIENT_DATA & date_time_epoch_s ) );
    assert( !( _CLIENT_DATA & date_time_epoch_s_ms ) );
    assert( !( _CLIENT_DATA & date_time_ncsa ) );
    assert( !( _CLIENT_DATA & date_time_iso ) );
    assert( !( _CLIENT_DATA & date_time_gmt ) );
    assert( !( _CLIENT_DATA & date_time_mcs ) );
    assert( !( _CLIENT_DATA & date_time_utc_d ) );
    assert( !( _CLIENT_DATA & date_time_utc_t ) );
    assert( !( _CLIENT_DATA & date_time_yyyymmdd ) );
    assert( !( _CLIENT_DATA & date_time_mmddyy ) );
    assert( !( _CLIENT_DATA & date_time_mdyyyy ) );
    assert( !( _CLIENT_DATA & date_time_year_short ) );
    assert( !( _CLIENT_DATA & date_time_month_str ) );
    assert( !( _CLIENT_DATA & date_time_year ) );
    assert( !( _CLIENT_DATA & date_time_month ) );
    assert( !( _CLIENT_DATA & date_time_day ) );
    assert( !( _CLIENT_DATA & date_time_hour ) );
    assert( !( _CLIENT_DATA & date_time_minute ) );
    assert( !( _CLIENT_DATA & date_time_second ) );
    assert( !( _CLIENT_DATA & date_time_clock_short ) );
    assert( !( _CLIENT_DATA & date_time_clock_12 ) );
    assert( !( _CLIENT_DATA & date_time_clock_24 ) );
    assert( !( _CLIENT_DATA & request_protocol ) );
    assert( !( _CLIENT_DATA & request_method ) );
    assert( !( _CLIENT_DATA & request_uri ) );
    assert( !( _CLIENT_DATA & request_query ) );
    assert( !( _CLIENT_DATA & request_uri_query ) );
    assert( !( _CLIENT_DATA & request_full ) );
    assert( !( _CLIENT_DATA & response_code ) );
    assert( !( _CLIENT_DATA & bytes_received ) );
    assert( !( _CLIENT_DATA & bytes_sent ) );
    assert( !( _CLIENT_DATA & time_taken_us ) );
    assert( !( _CLIENT_DATA & time_taken_ms ) );
    assert( !( _CLIENT_DATA & time_taken_s ) );
    assert( !( _CLIENT_DATA & time_taken_s_ms ) );
    }{
    // test for exact matches
    assert( (_NO_PARSE_NEEDED & date_time_year) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & date_time_month) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & date_time_day) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & date_time_hour) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & date_time_minute) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & date_time_second) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & request_protocol) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & request_method) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & request_uri) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & request_query) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & response_code) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & bytes_received) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & bytes_sent) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & time_taken_ms) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & client) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & user_agent) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & cookie) == _NO_PARSE_NEEDED );
    assert( (_NO_PARSE_NEEDED & referer) == _NO_PARSE_NEEDED );
    }{
    // test for exact matches
    assert( (_DATE_TIME & date_time_epoch_us) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_epoch_ms) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_epoch_s) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_epoch_s_ms) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_ncsa) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_iso) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_gmt) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_mcs) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_utc_d) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_utc_t) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_yyyymmdd) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_mmddyy) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_mdyyyy) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_year_short) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_month_str) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_year) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_month) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_day) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_hour) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_minute) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_second) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_clock_short) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_clock_12) == _DATE_TIME );
    assert( (_DATE_TIME & date_time_clock_24) == _DATE_TIME );
    }{
    // test for exact matches
    assert( (_DATE_TIME_FULL & date_time_epoch_us) == _DATE_TIME_FULL );
    assert( (_DATE_TIME_FULL & date_time_epoch_ms) == _DATE_TIME_FULL );
    assert( (_DATE_TIME_FULL & date_time_epoch_s) == _DATE_TIME_FULL );
    assert( (_DATE_TIME_FULL & date_time_epoch_s_ms) == _DATE_TIME_FULL );
    assert( (_DATE_TIME_FULL & date_time_ncsa) == _DATE_TIME_FULL );
    assert( (_DATE_TIME_FULL & date_time_iso) == _DATE_TIME_FULL );
    assert( (_DATE_TIME_FULL & date_time_gmt) == _DATE_TIME_FULL );
    assert( (_DATE_TIME_FULL & date_time_mcs) == _DATE_TIME_FULL );
    }{
    // test for exact matches
    assert( (_DATE_TIME_EPOCH & date_time_epoch_us) == _DATE_TIME_EPOCH );
    assert( (_DATE_TIME_EPOCH & date_time_epoch_ms) == _DATE_TIME_EPOCH );
    assert( (_DATE_TIME_EPOCH & date_time_epoch_s) == _DATE_TIME_EPOCH );
    assert( (_DATE_TIME_EPOCH & date_time_epoch_s_ms) == _DATE_TIME_EPOCH );
    }{
    // test for exact matches
    assert( (_DATE_TIME_UTC & date_time_utc_d) == _DATE_TIME_UTC );
    assert( (_DATE_TIME_UTC & date_time_utc_t) == _DATE_TIME_UTC );
    }{
    // test for exact matches
    assert( (_DATE_TIME_DATE & date_time_utc_d) == _DATE_TIME_DATE );
    assert( (_DATE_TIME_DATE & date_time_yyyymmdd) == _DATE_TIME_DATE );
    assert( (_DATE_TIME_DATE & date_time_mmddyy) == _DATE_TIME_DATE );
    assert( (_DATE_TIME_DATE & date_time_mdyyyy) == _DATE_TIME_DATE );
    }{
    // test for exact matches
    assert( (_DATE_TIME_YEAR & date_time_year_short) == _DATE_TIME_YEAR );
    assert( (_DATE_TIME_YEAR & date_time_year) == _DATE_TIME_YEAR );
    }{
    // test for exact matches
    assert( (_DATE_TIME_MONTH & date_time_month_str) == _DATE_TIME_MONTH );
    assert( (_DATE_TIME_MONTH & date_time_month) == _DATE_TIME_MONTH );
    }{
    // test for exact matches
    assert( (_DATE_TIME_CLOCK & date_time_clock_short) == _DATE_TIME_CLOCK );
    assert( (_DATE_TIME_CLOCK & date_time_clock_12) == _DATE_TIME_CLOCK );
    assert( (_DATE_TIME_CLOCK & date_time_clock_24) == _DATE_TIME_CLOCK );
    }{
    // test for exact matches
    assert( (_REQUEST & request_protocol) == _REQUEST );
    assert( (_REQUEST & request_method) == _REQUEST );
    assert( (_REQUEST & request_uri) == _REQUEST );
    assert( (_REQUEST & request_query) == _REQUEST );
    assert( (_REQUEST & request_uri_query) == _REQUEST );
    assert( (_REQUEST & request_full) == _REQUEST );
    }{
    // test for exact matches
    assert( (_RESPONSE_CODE & response_code) == _RESPONSE_CODE );
    }{
    // test for exact matches
    assert( (_DATA_TRANSFER & bytes_received) == _DATA_TRANSFER );
    assert( (_DATA_TRANSFER & bytes_sent) == _DATA_TRANSFER );
    }{
    // test for exact matches
    assert( (_TIME_TAKEN & time_taken_us) == _TIME_TAKEN );
    assert( (_TIME_TAKEN & time_taken_ms) == _TIME_TAKEN );
    assert( (_TIME_TAKEN & time_taken_s) == _TIME_TAKEN );
    assert( (_TIME_TAKEN & time_taken_s_ms) == _TIME_TAKEN );
    }{
    // test for exact matches
    assert( (_CLIENT_DATA & client) == _CLIENT_DATA );
    assert( (_CLIENT_DATA & user_agent) == _CLIENT_DATA );
    assert( (_CLIENT_DATA & cookie) == _CLIENT_DATA );
    assert( (_CLIENT_DATA & referer) == _CLIENT_DATA );
    }{
    // test for fields which contain a fixed number of spaces
    assert( _MAY_HAVE_SPACES & request_full );
    assert( _MAY_HAVE_SPACES & date_time_ncsa );
    assert( _MAY_HAVE_SPACES & date_time_gmt );
    assert( _MAY_HAVE_SPACES & date_time_mcs );
    // test for fields which doesn't (supposedly) contain spaces
    assert( !( _MAY_HAVE_SPACES & date_time_epoch_us ) );
    assert( !( _MAY_HAVE_SPACES & date_time_epoch_ms ) );
    assert( !( _MAY_HAVE_SPACES & date_time_epoch_s ) );
    assert( !( _MAY_HAVE_SPACES & date_time_epoch_s_ms ) );
    assert( !( _MAY_HAVE_SPACES & date_time_iso ) );
    assert( !( _MAY_HAVE_SPACES & date_time_utc_d ) );
    assert( !( _MAY_HAVE_SPACES & date_time_utc_t ) );
    assert( !( _MAY_HAVE_SPACES & date_time_yyyymmdd ) );
    assert( !( _MAY_HAVE_SPACES & date_time_mmddyy ) );
    assert( !( _MAY_HAVE_SPACES & date_time_mdyyyy ) );
    assert( !( _MAY_HAVE_SPACES & date_time_year_short ) );
    assert( !( _MAY_HAVE_SPACES & date_time_month_str ) );
    assert( !( _MAY_HAVE_SPACES & date_time_year ) );
    assert( !( _MAY_HAVE_SPACES & date_time_month ) );
    assert( !( _MAY_HAVE_SPACES & date_time_day ) );
    assert( !( _MAY_HAVE_SPACES & date_time_hour ) );
    assert( !( _MAY_HAVE_SPACES & date_time_minute ) );
    assert( !( _MAY_HAVE_SPACES & date_time_second ) );
    assert( !( _MAY_HAVE_SPACES & date_time_clock_short ) );
    assert( !( _MAY_HAVE_SPACES & date_time_clock_12 ) );
    assert( !( _MAY_HAVE_SPACES & date_time_clock_24 ) );
    assert( !( _MAY_HAVE_SPACES & request_protocol ) );
    assert( !( _MAY_HAVE_SPACES & request_method ) );
    assert( !( _MAY_HAVE_SPACES & request_uri ) );
    assert( !( _MAY_HAVE_SPACES & request_query ) );
    assert( !( _MAY_HAVE_SPACES & request_uri_query ) );
    assert( !( _MAY_HAVE_SPACES & response_code ) );
    assert( !( _MAY_HAVE_SPACES & bytes_received ) );
    assert( !( _MAY_HAVE_SPACES & bytes_sent ) );
    assert( !( _MAY_HAVE_SPACES & time_taken_us ) );
    assert( !( _MAY_HAVE_SPACES & time_taken_ms ) );
    assert( !( _MAY_HAVE_SPACES & time_taken_s ) );
    assert( !( _MAY_HAVE_SPACES & time_taken_s_ms ) );
    assert( !( _MAY_HAVE_SPACES & client ) );
    assert( !( _MAY_HAVE_SPACES & user_agent ) );
    assert( !( _MAY_HAVE_SPACES & cookie ) );
    assert( !( _MAY_HAVE_SPACES & referer ) );
    }{
    // test for the correct amount of spaces
    assert( (_COUNT_SPACES & date_time_ncsa) == 1ul );
    assert( (_COUNT_SPACES & date_time_gmt) == 3ul );
    assert( (_COUNT_SPACES & date_time_mcs) == 4ul );
    assert( (request_full & 2) == 2 ); // useless, just a reminder is should have 2
    }
    T_PRINT("LogsFormatField");


    //// DATE-TIME ////

    {
    QTimeZone tz{ QTimeZone::systemTimeZone() };
    const int e{ 946771199 - tz.standardTimeOffset( QDateTime::fromSecsSinceEpoch(946771199) ) };
    const std::string e_str{ std::to_string( e ) };
    const std::string epochs[4]{ e_str, e_str+".000", e_str+"000", e_str+"000000" };
    std::vector<std::string> target{"2000","01","01","23","59","59"};
    assert( DateTimeOps::processDateTime("[01/Jan/2000:23:59:59 +0000]", date_time_ncsa) == target );
    assert( DateTimeOps::processDateTime("Sat Jan 01 23:59:59 2000", date_time_mcs) == target );
    assert( DateTimeOps::processDateTime("Saturday, 01-Jan-2000 23:59:59 UTC", date_time_gmt) == target );
    assert( DateTimeOps::processDateTime("2000-01-01T23:59:59+00:00", date_time_iso) == target );
    assert( DateTimeOps::processDateTime(epochs[0], date_time_epoch_s) == target );
    assert( DateTimeOps::processDateTime(epochs[1], date_time_epoch_s_ms) == target );
    assert( DateTimeOps::processDateTime(epochs[2], date_time_epoch_ms) == target );
    assert( DateTimeOps::processDateTime(epochs[3], date_time_epoch_us) == target );
    target = {"2000","01","01","","",""};
    assert( DateTimeOps::processDateTime("2000-01-01", date_time_utc_d) == target );
    assert( DateTimeOps::processDateTime("2000-01-01", date_time_yyyymmdd) == target );
    assert( DateTimeOps::processDateTime("01/01/00", date_time_mmddyy) == target );
    assert( DateTimeOps::processDateTime("1/1/2000", date_time_mdyyyy) == target );
    target = {"","","","23","59","59"};
    assert( DateTimeOps::processDateTime("23:59:59", date_time_utc_t) == target );
    assert( DateTimeOps::processDateTime("11:59:59 pm", date_time_clock_12) == target );
    assert( DateTimeOps::processDateTime("23:59:59", date_time_clock_24) == target );
    target = {"","","","23","59",""};
    assert( DateTimeOps::processDateTime("23:59", date_time_clock_short) == target );
    /*target = {"","","","PM","",""};
    assert( DateTimeOps::processDateTime("pm", date_time_clock_meridian) == target );*/
    target = {"2000","","","","",""};
    assert( DateTimeOps::processDateTime("2000", date_time_year) == target );
    target = {"","01","","","",""};
    assert( DateTimeOps::processDateTime("01", date_time_month) == target );
    target = {"","","01","","",""};
    assert( DateTimeOps::processDateTime("01", date_time_day) == target );
    target = {"","","","23","",""};
    assert( DateTimeOps::processDateTime("23", date_time_hour) == target );
    target = {"","","","","59",""};
    assert( DateTimeOps::processDateTime("59", date_time_minute) == target );
    target = {"","","","","","59"};
    assert( DateTimeOps::processDateTime("59", date_time_second) == target );
    }
    T_PRINT("DateTimeOps::processDateTime");


    //// FORMATS ////

    {
    FormatOps fo;
    LogsFormat lf;
    std::string format_string;
    std::vector<LogsFormatField> fields;
    std::vector<std::string> separators;
    // test the default string
    format_string = "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"";
    fields = {client,_DISCARDED,_DISCARDED,date_time_ncsa,request_full,response_code,_DISCARDED,referer,user_agent};
    separators = {" "," "," [","] \"","\" "," "," \"","\" \""};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final == "\"" );
    // test all simple fields
    format_string = "%%%h %% %t\t%r\n%H %m [%U%%%q} <%s> %<s %>s %O %I %T %D %a %A %b %B %e %f %k %l %L %p %P %R %S %u %v %V %% %X%%";
    fields = {client,date_time_ncsa,request_full,request_protocol,request_method,request_uri,request_query,response_code,response_code,response_code,bytes_sent,bytes_received,time_taken_s,time_taken_ms,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" % [","]\t","\n"," "," [","%","} <","> "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," % "};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial == "%" );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final == "%" );
    // test non-existing/invalid simple fields
    for ( auto& fs : {"%","%c","%d","%E","%Q","%w","%Z","%! ","%, ","%h%r"} ) {
        REDIRECT_STDOUT()
        REDIRECT_STDERR()
        try {
            std::ignore = fo.processApacheFormatString(fs);
            RESTORE_STDOUT()
            RESTORE_STDERR()
            assert( false );
        } catch (const LogFormatException& e) {}
        RESTORE_STDOUT()
        RESTORE_STDERR()
    }
    // test client related composed fields
    format_string = "%{}a %{c}a %{}h %{c}h %{Cookie}i %200{Cookie}i %{User-agent}i %302,400{User-agent}i %!200{Referer}i %,200{Referer}i %{Referer}i";
    fields = {client,client,client,client,cookie,cookie,user_agent,user_agent,referer,referer,referer};
    separators = {" "," "," "," "," "," "," "," "," "," "};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test unexisting/unsupported client related composed fields
    format_string = "%{ }a %{x}a %{NOPE}a %{ }h %{y}h %{NOPE}h %{}i %{ }i %{Random}i %{Cookies}i";
    fields = {client,client,client,client,client,client,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "," "};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test date-time composed fields
    format_string = "%{%%}t %{%n}t %{%t}t %{}t %{sec}t %{msec}t %{usec}t %{msec_frac}t %{usec_frac}t %{%a}t %{%A}t %{%b}t %{%B}t %{%c}t %{%C}t %{%d}t %{%D}t %{%e}t %{%F}t %{%g}t %{%G}t %{%h}t %{%H}t %{%I}t %{%j}t %{%k}t %{%m}t %{%M}t %{%p}t %{%r}t %{%R}t %{%S}t %{%T}t %{%u}t %{%U}t %{%V}t %{%w}t %{%W}t %{%x}t %{%X}t %{%y}t %{%Y}t %{%z}t %{%Z}t";
    fields = {date_time_ncsa,date_time_epoch_s,date_time_epoch_ms,date_time_epoch_us,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,date_time_month_str,date_time_month_str,date_time_mcs,_DISCARDED,date_time_day,date_time_mmddyy,date_time_day,date_time_yyyymmdd,_DISCARDED,_DISCARDED,date_time_month_str,date_time_hour,_DISCARDED,_DISCARDED,date_time_hour,date_time_month,date_time_minute,_DISCARDED,date_time_clock_12,date_time_clock_short,date_time_second,date_time_clock_24,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,date_time_mmddyy,date_time_clock_24,date_time_year_short,date_time_year,_DISCARDED,_DISCARDED};
    separators = {"] "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial == "% \n \t [" );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test date-time composed fields, with one field only
    format_string = "%{}t";
    fields = {date_time_ncsa};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial == "[" );
    assert( lf.fields == fields );
    assert( lf.separators.empty() );
    assert( lf.final == "]" );
    // test date-time composed fields, with many aggreagated fields
    format_string = "%{%%%Y_%m_%e%t%H@%M@%S%%}t";
    fields = {date_time_year,date_time_month,date_time_day,date_time_hour,date_time_minute,date_time_second};
    separators = {"_","_","\t","@","@"};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial == "%" );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final == "%" );
    // test date-time composed fields, with all fields aggeregated in one
    format_string = "%{%% %n %t %a %A %b %B %c %C %d %D %e %F %g %G %h %H %I %j %k %m %M %p %r %R %S %T %u %U %V %w %W %x %X %y %Y %z %Z}t";
    fields = {_DISCARDED,_DISCARDED,date_time_month_str,date_time_month_str,date_time_mcs,_DISCARDED,date_time_day,date_time_mmddyy,date_time_day,date_time_yyyymmdd,_DISCARDED,_DISCARDED,date_time_month_str,date_time_hour,_DISCARDED,_DISCARDED,date_time_hour,date_time_month,date_time_minute,_DISCARDED,date_time_clock_12,date_time_clock_short,date_time_second,date_time_clock_24,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,date_time_mmddyy,date_time_clock_24,date_time_year_short,date_time_year,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial == "% \n \t " );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test unexisting/unsupported date-time composed fields
    format_string = "%{ }t %{nope}t %{%}t %{%?}t %{%E}t %{%q}t";
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial == "  nope % %? %E %q" );
    assert( lf.fields.empty() );
    assert( lf.separators.empty() );
    assert( lf.final.empty() );
    // test invalid date-time composed fields
    for ( auto& fs : {"%{%H}t%{%M}t%{%S}t","%{%d%m%y}t"} ) {
        REDIRECT_STDOUT()
        REDIRECT_STDERR()
        try {
            std::ignore = fo.processApacheFormatString(fs);
            RESTORE_STDOUT()
            RESTORE_STDERR()
            assert( false );
        } catch (const LogFormatException& e) {}
        RESTORE_STDOUT()
        RESTORE_STDERR()
    }
    // test time taken related composed fields
    format_string = "%{}T %{s}T %{ms}T %{us}T";
    fields = {time_taken_s,time_taken_s,time_taken_ms,time_taken_us};
    separators = {" "," "," "};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test unexisting/unsupported time taken related composed fields
    format_string = "%{ }T %{%s}T %{msec}T";
    fields = {};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial == "  %s msec" );
    assert( lf.fields.empty() );
    assert( lf.separators.empty() );
    assert( lf.final.empty() );
    // test unused composed fields
    format_string = "%{}C %{}e %{}L %{}n %{}o %{}p %{canonical}p %{local}p %{remote}p %{}P %{pid}P %{tid}P %{hextid}P %{}^ti %{}^to";
    fields = {_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test unused composed fields, with random content
    format_string = "%{TEST}C %{TEST}e %{TEST}L %{TEST}n %{TEST}o %{TEST}p %{TEST}P %{TEST}^ti %{TEST}^to";
    fields = {_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," TEST TEST "," "};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test the default string with dumb logging: no characters to enclose the full request
    format_string = "%h %l %u %t %r %>s %b \"%{Referer}i\" \"%{User-agent}i\"";
    fields = {client,_DISCARDED,_DISCARDED,date_time_ncsa,request_full,response_code,_DISCARDED,referer,user_agent};
    separators = {" "," "," [","] "," "," "," \"","\" \""};
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final == "\"" );
    // test an empty string
    format_string.erase();
    lf = fo.processApacheFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields.empty() );
    assert( lf.separators.empty() );
    assert( lf.final.empty() );
    }
    T_PRINT("FormatOps::processApacheFormatString");

    {
    FormatOps fo;
    LogsFormat lf;
    std::string format_string;
    std::vector<LogsFormatField> fields;
    std::vector<std::string> separators;
    // test the default string
    format_string = "$remote_addr - $remote_user [$time_local] \"$request\" $status $bytes_sent \"$http_referer\" \"$http_user_agent\"";
    fields = {client,_DISCARDED,date_time_ncsa,request_full,response_code,bytes_sent,referer,user_agent};
    separators = {" - "," [","] \"","\" "," "," \"","\" \""};
    lf = fo.processNginxFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final == "\"" );
    // test all the considered fields
    format_string = "$remote_addr $realip_remote_addr $time_local $time_iso8601 $date_gmt $msec $request $server_protocol $request_method $request_uri $uri $query_string $status $bytes_sent $request_length $request_time $http_referer $cookie_ $http_user_agent";
    fields = {client,client,date_time_ncsa,date_time_iso,date_time_gmt,date_time_epoch_s_ms,request_full,request_protocol,request_method,request_uri_query,request_uri,request_query,response_code,bytes_sent,bytes_received,time_taken_s_ms,referer,cookie,user_agent};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = fo.processNginxFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test all the non-considered fields
    format_string = "$ancient_browser $arg_ $args $binary_remote_addr $body_bytes_sent $connection $connection_requests $connections_active $connections_reading $connections_waiting $connections_writing $content_length $content_type $date_local $document_root $document_uri $fastcgi_path_info $fastcgi_script_name $geoip_area_code $geoip_city $geoip_city_continent_code $geoip_city_country_code $geoip_city_country_code3 $geoip_city_country_name $geoip_country_code $geoip_country_code3 $geoip_country_name $geoip_dma_code $geoip_latitude $geoip_longitude $geoip_org $geoip_postal_code $geoip_region $geoip_region_name $gzip_ratio $host $hostname $http2 $http_ $https $invalid_referer $is_args $limit_rate $memcached_key $modern_browser $msie $nginx_version $pid $pipe $proxy_add_x_forwarded_for $proxy_host $proxy_port $proxy_protocol_addr $proxy_protocol_port $realip_remote_port $realpath_root $remote_port $remote_user $request_body $request_body_file $request_completion $request_filename $request_id $scheme $secure_link $secure_link_expires $sent_http_ $server_addr $server_name $server_port $session_log_binary_id $session_log_id $slice_range $spdy $spdy_request_priority $ssl_cipher $ssl_client_cert $ssl_client_fingerprint $ssl_client_i_dn $ssl_client_raw_cert $ssl_client_s_dn $ssl_client_serial $ssl_client_verify $ssl_protocol $ssl_server_name $ssl_session_id $ssl_session_reused $tcpinfo_rtt $tcpinfo_rttvar $tcpinfo_snd_cwnd $tcpinfo_rcv_space $uid_got $uid_reset $uid_set $upstream_addr $upstream_cache_status $upstream_connect_time $upstream_cookie_ $upstream_header_time $upstream_http_ $upstream_response_length $upstream_response_time $upstream_status";
    fields = {_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = fo.processNginxFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test an empty string
    format_string.erase();
    lf = fo.processNginxFormatString(format_string);
    assert( lf.initial.empty() );
    assert( lf.fields.empty() );
    assert( lf.separators.empty() );
    assert( lf.final.empty() );
    }
    T_PRINT("FormatOps::processNginxFormatString");

    {
    FormatOps fo;
    LogsFormat lf;
    std::string format_string;
    std::vector<LogsFormatField> fields;
    std::vector<std::string> separators;
    // test the default string for the W3C module
    format_string = "date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs(User-Agent) cs(Referer) sc-status sc-substatus sc-win32-status time-taken";
    fields = {date_time_utc_d,date_time_utc_t,_DISCARDED,request_method,request_uri,request_query,_DISCARDED,_DISCARDED,client,user_agent,referer,response_code,_DISCARDED,_DISCARDED,time_taken_ms};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = fo.processIisFormatString(format_string, 0);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test all the considered fields for the W3C module
    format_string = "date time cs-version cs-method cs-uri-stem cs-uri-query sc-status sc-bytes cs-bytes time-taken cs(Referer) cs(Cookie) cs(User-Agent) c-ip";
    fields = {date_time_utc_d,date_time_utc_t,request_protocol,request_method,request_uri,request_query,response_code,bytes_sent,bytes_received,time_taken_ms,referer,cookie,user_agent,client};
    separators = {" "," "," "," "," "," "," "," "," "," "," "," "," "};
    lf = fo.processIisFormatString(format_string, 0);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test all the non-considered fields for the W3C module
    format_string = "s-sitename s-computername s-ip s-port cs-username cs-host sc-substatus sc-win32-status streamid";
    fields = {_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED,_DISCARDED};
    separators = {" "," "," "," "," "," "," "," "};
    lf = fo.processIisFormatString(format_string, 0);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test an empty string for the W3C module
    format_string.erase();
    lf = fo.processIisFormatString(format_string, 0);
    assert( lf.initial.empty() );
    assert( lf.fields.empty() );
    assert( lf.separators.empty() );
    assert( lf.final.empty() );
    // test the the NCSA module
    format_string = "some random useless text";
    fields = {client,_DISCARDED,_DISCARDED,date_time_ncsa,request_full,response_code,bytes_sent};
    separators = {" "," "," [","] \"","\" "," "};
    lf = fo.processIisFormatString(format_string, 1);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test an empty string for the NCSA module
    format_string.erase();
    lf = fo.processIisFormatString(format_string, 1);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final.empty() );
    // test the the IIS module
    format_string = "some random useless text";
    fields = {client,_DISCARDED,date_time_mdyyyy,date_time_utc_t,_DISCARDED,_DISCARDED,_DISCARDED,time_taken_ms,bytes_received,bytes_sent,response_code,_DISCARDED,request_method,request_uri,request_query};
    separators = {", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", ",", "};
    lf = fo.processIisFormatString(format_string, 2);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final == "," );
    // test an empty string for the IIS module
    format_string.erase();
    lf = fo.processIisFormatString(format_string, 2);
    assert( lf.initial.empty() );
    assert( lf.fields == fields );
    assert( lf.separators == separators );
    assert( lf.final == "," );
    }
    T_PRINT("FormatOps::processIisFormatString");


    //// LOGS TYPE ////

    {
    LogsFormat lf{ "","","]",{" ","_"},{_DISCARDED,_DISCARDED,_DISCARDED},0 };
    assert( LogOps::defineFileType({"ok ok_ok]","a a_a]","TEST TEST_TEST]"}, lf) == LogType::Access );
    assert( LogOps::defineFileType({"no no no!","some thing wrong","with this file!"}, lf) == LogType::Discarded );
    assert( LogOps::defineFileType({}, lf) == LogType::Failed );
    }
    T_PRINT("LogOps::defineFileType");


    //// LOGS PARSING ////

    {
    FormatOps fo;
    LogsFormat lf;
    std::string log_line;
    {
    lf = fo.processApacheFormatString(R"(%h %l %u %t "%r" %>s %O "%{Referer}i" "%{User-agent}i")");
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET /index.php?query=x HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.year == true && *line_data.year == "2000" );
    assert( line_data.month == true && *line_data.month == "01" );
    assert( line_data.day == true && *line_data.day == "01" );
    assert( line_data.hour == true && *line_data.hour == "23" );
    assert( line_data.minute == true && *line_data.minute == "59" );
    assert( line_data.second == true && *line_data.second == "59" );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    assert( line_data.response_code == true && *line_data.response_code == "200" );
    assert( line_data.time_taken == false && *line_data.time_taken == "" );
    assert( line_data.bytes_sent == true && *line_data.bytes_sent == "1024" );
    assert( line_data.bytes_received == false && *line_data.bytes_received == "" );
    assert( line_data.client == true && *line_data.client == "192.168.1.123" );
    assert( line_data.cookie == false && *line_data.cookie == "" );
    assert( line_data.user_agent == true && *line_data.user_agent == "UserAgent/3.0 (Details stuff) Info/123" );
    assert( line_data.referrer == true && *line_data.referrer == "http://www.referrer.site" );
    }{
    // same as above but without the query
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET /index.php HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // dumb logging, without any surrounding character to enclose the request
    lf = fo.processApacheFormatString(R"(%h %l %u %t %r %>s %O "%{Referer}i" "%{User-agent}i")");
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] GET /index.php?query=x HTTP/1.1 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.year == true && *line_data.year == "2000" );
    assert( line_data.month == true && *line_data.month == "01" );
    assert( line_data.day == true && *line_data.day == "01" );
    assert( line_data.hour == true && *line_data.hour == "23" );
    assert( line_data.minute == true && *line_data.minute == "59" );
    assert( line_data.second == true && *line_data.second == "59" );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    assert( line_data.response_code == true && *line_data.response_code == "200" );
    assert( line_data.time_taken == false && *line_data.time_taken == "" );
    assert( line_data.bytes_sent == true && *line_data.bytes_sent == "1024" );
    assert( line_data.bytes_received == false && *line_data.bytes_received == "" );
    assert( line_data.client == true && *line_data.client == "192.168.1.123" );
    assert( line_data.cookie == false && *line_data.cookie == "" );
    assert( line_data.user_agent == true && *line_data.user_agent == "UserAgent/3.0 (Details stuff) Info/123" );
    assert( line_data.referrer == true && *line_data.referrer == "http://www.referrer.site" );
    }{
    // dumb logging, without any surrounding character to enclose the request and the user-agent
    lf = fo.processApacheFormatString(R"(%h %l %u %t %r %>s %O "%{Referer}i" %{User-agent}i)");
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] GET /index.php?query=x HTTP/1.1 200 1024 "http://www.referrer.site" UserAgent/3.0 (Details stuff) Info/123)";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    assert( line_data.response_code == true && *line_data.response_code == "200" );
    assert( line_data.time_taken == false && *line_data.time_taken == "" );
    assert( line_data.bytes_sent == true && *line_data.bytes_sent == "1024" );
    assert( line_data.bytes_received == false && *line_data.bytes_received == "" );
    assert( line_data.client == true && *line_data.client == "192.168.1.123" );
    assert( line_data.cookie == false && *line_data.cookie == "" );
    assert( line_data.user_agent == true && *line_data.user_agent == "UserAgent/3.0 (Details stuff) Info/123" );
    assert( line_data.referrer == true && *line_data.referrer == "http://www.referrer.site" );
    }{
    // malformed request with empty method
    lf = fo.processApacheFormatString(R"(%h %l %u %t "%r" %>s %O "%{Referer}i" "%{User-agent}i")");
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] " /index.php?query=x HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with empty protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET /index.php?query=x " 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with empty uri
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET  HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with empty method and protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] " /index.php?query=x " 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with empty method and uri
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "  HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with empty uri and protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET  " 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with all fields empty
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "  " 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with missing method
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "/index.php?query=x HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with missing protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET /index.php?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with missing uri
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with missing the uri but with the query
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET ?query=x HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with only the method
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with only the protocol
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with only the uri and the query
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "/index.php?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with only the uri
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "/index.php" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with only the query
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with every field missing
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == false && *line_data.uri == "" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with only random data
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "M4l1C10US" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == false && *line_data.protocol == "" );
    assert( line_data.method == false && *line_data.method == "" );
    assert( line_data.uri == true && *line_data.uri == "M4l1C10US" );
    assert( line_data.query == false && *line_data.query == "" );
    }{
    // malformed request with method and protocol positions swapped
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "HTTP/1.1 /index.php?query=x GET" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with method and uri positions swapped
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "/index.php?query=x GET HTTP/1.1" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with uri and protocol positions swapped
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "GET HTTP/1.1 /index.php?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }{
    // malformed request with all fields positions swapped
    log_line = R"(192.168.1.123 - - [01/Jan/2000:23:59:59 +0000] "HTTP/1.1 GET /index.php?query=x" 200 1024 "http://www.referrer.site" "UserAgent/3.0 (Details stuff) Info/123")";
    LogLineData line_data( log_line, lf );
    assert( line_data.protocol == true && *line_data.protocol == "HTTP/1.1" );
    assert( line_data.method == true && *line_data.method == "GET" );
    assert( line_data.uri == true && *line_data.uri == "/index.php" );
    assert( line_data.query == true && *line_data.query == "query=x" );
    }
    T_PRINT("LogLineData::LogLineData");
    }
}


void testCrapviewModules()
{
    //// FILTERS ////

    assert( FilterOps::parseNull("null").value() == " IS NULL" );
    assert( FilterOps::parseNull("NULL").value() == " IS NULL" );
    assert( FilterOps::parseNull("not null").value() == " IS NOT NULL" );
    assert( FilterOps::parseNull("NOT NULL").value() == " IS NOT NULL" );
    assert( FilterOps::parseNull("!NULL").value() == " IS NOT NULL" );
    assert( FilterOps::parseNull("! NULL").value() == " IS NOT NULL" );
    // try to brake
    assert( FilterOps::parseNull("  null   ").value() == " IS NULL" );
    assert( FilterOps::parseNull("  not null   ").value() == " IS NOT NULL" );
    assert( FilterOps::parseNull("not   null").value() == " IS NOT NULL" );
    assert( FilterOps::parseNull("!    null").value() == " IS NOT NULL" );
    // test for invalid filter
    assert( ! FilterOps::parseNull("").has_value() );
    assert( ! FilterOps::parseNull(" ").has_value() );
    assert( ! FilterOps::parseNull(" \t\n ").has_value() );
    assert( ! FilterOps::parseNull("123").has_value() );
    assert( ! FilterOps::parseNull("abc").has_value() );
    assert( ! FilterOps::parseNull("*").has_value() );
    assert( ! FilterOps::parseNull("null test").has_value() );
    assert( ! FilterOps::parseNull("not null test").has_value() );
    T_PRINT("FilterOps::parseNull");

    assert( FilterOps::parseTextualFilter("").value() == "" );
    assert( FilterOps::parseTextualFilter(" ").value() == "" );
    assert( FilterOps::parseTextualFilter(" \t\n ").value() == "" );
    assert( FilterOps::parseTextualFilter("null").value() == " IS NULL" );
    assert( FilterOps::parseTextualFilter("not null").value() == " IS NOT NULL" );
    assert( FilterOps::parseTextualFilter("*").value() == " IS NOT NULL" );
    assert( FilterOps::parseTextualFilter("test").value() == " LIKE 'test'" );
    assert( FilterOps::parseTextualFilter("!test").value() == " NOT LIKE 'test'" );
    assert( FilterOps::parseTextualFilter("\\!test").value() == " LIKE '!test'" );
    assert( FilterOps::parseTextualFilter(" test").value() == " LIKE 'test'" );
    assert( FilterOps::parseTextualFilter("\\ test").value() == " LIKE ' test'" );
    // try to brake
    assert( FilterOps::parseTextualFilter("  a test string ").value() == " LIKE 'a test string'" );
    assert( FilterOps::parseTextualFilter("! a test string").value() == " NOT LIKE 'a test string'" );
    assert( FilterOps::parseTextualFilter("[test 123 #!?]").value() == " LIKE '[test 123 #!?]'" );
    T_PRINT("FilterOps::parseTextualFilter");

    assert( FilterOps::parseNumericFilter("").value() == "" );
    assert( FilterOps::parseNumericFilter(" ").value() == "" );
    assert( FilterOps::parseNumericFilter(" \t\n ").value() == "" );
    assert( FilterOps::parseNumericFilter("null").value() == " IS NULL" );
    assert( FilterOps::parseNumericFilter("not null").value() == " IS NOT NULL" );
    assert( FilterOps::parseNumericFilter("123").value() == "=123" );
    assert( FilterOps::parseNumericFilter("=123").value() == "=123" );
    assert( FilterOps::parseNumericFilter("= 123").value() == "=123" );
    assert( FilterOps::parseNumericFilter("==123").value() == "==123" );
    assert( FilterOps::parseNumericFilter("== 123").value() == "==123" );
    assert( FilterOps::parseNumericFilter("eq 123").value() == "=123" );
    assert( FilterOps::parseNumericFilter("EQ 123").value() == "=123" );
    assert( FilterOps::parseNumericFilter("!123").value() == "!=123" );
    assert( FilterOps::parseNumericFilter("!=123").value() == "!=123" );
    assert( FilterOps::parseNumericFilter("!= 123").value() == "!=123" );
    assert( FilterOps::parseNumericFilter("ne 123").value() == "!=123" );
    assert( FilterOps::parseNumericFilter("NE 123").value() == "!=123" );
    assert( FilterOps::parseNumericFilter(">123").value() == ">123" );
    assert( FilterOps::parseNumericFilter("> 123").value() == ">123" );
    assert( FilterOps::parseNumericFilter("gt 123").value() == ">123" );
    assert( FilterOps::parseNumericFilter("GT 123").value() == ">123" );
    assert( FilterOps::parseNumericFilter(">=123").value() == ">=123" );
    assert( FilterOps::parseNumericFilter(">= 123").value() == ">=123" );
    assert( FilterOps::parseNumericFilter("ge 123").value() == ">=123" );
    assert( FilterOps::parseNumericFilter("GE 123").value() == ">=123" );
    assert( FilterOps::parseNumericFilter("<123").value() == "<123" );
    assert( FilterOps::parseNumericFilter("< 123").value() == "<123" );
    assert( FilterOps::parseNumericFilter("lt 123").value() == "<123" );
    assert( FilterOps::parseNumericFilter("LT 123").value() == "<123" );
    assert( FilterOps::parseNumericFilter("<=123").value() == "<=123" );
    assert( FilterOps::parseNumericFilter("<= 123").value() == "<=123" );
    assert( FilterOps::parseNumericFilter("le 123").value() == "<=123" );
    assert( FilterOps::parseNumericFilter("LE 123").value() == "<=123" );
    // try to brake
    assert( FilterOps::parseNumericFilter("   123 ").value() == "=123" );
    assert( FilterOps::parseNumericFilter("1 2 3").value() == "=123" );
    assert( FilterOps::parseNumericFilter(" =   12   3 ").value() == "=123" );
    assert( FilterOps::parseNumericFilter("= =123").value() == "==123" );
    assert( FilterOps::parseNumericFilter(" = = 1 2 3 ").value() == "==123" );
    assert( FilterOps::parseNumericFilter(" !   123 ").value() == "!=123" );
    assert( FilterOps::parseNumericFilter("! =123").value() == "!=123" );
    assert( FilterOps::parseNumericFilter("! = 123").value() == "!=123" );
    assert( FilterOps::parseNumericFilter("eq123").value() == "=123" );
    assert( FilterOps::parseNumericFilter("EQ123").value() == "=123" );
    assert( FilterOps::parseNumericFilter(" E Q 1 2 3 ").value() == "=123" );
    assert( FilterOps::parseNumericFilter("ne123").value() == "!=123" );
    assert( FilterOps::parseNumericFilter("NE123").value() == "!=123" );
    assert( FilterOps::parseNumericFilter(" N E 1 2 3 ").value() == "!=123" );
    assert( FilterOps::parseNumericFilter("gt123").value() == ">123" );
    assert( FilterOps::parseNumericFilter("GT123").value() == ">123" );
    assert( FilterOps::parseNumericFilter(" G T 1 2 3 ").value() == ">123" );
    assert( FilterOps::parseNumericFilter("ge123").value() == ">=123" );
    assert( FilterOps::parseNumericFilter("GE123").value() == ">=123" );
    assert( FilterOps::parseNumericFilter(" G E 1 2 3 ").value() == ">=123" );
    assert( FilterOps::parseNumericFilter("lt123").value() == "<123" );
    assert( FilterOps::parseNumericFilter("LT123").value() == "<123" );
    assert( FilterOps::parseNumericFilter(" L T 1 2 3 ").value() == "<123" );
    assert( FilterOps::parseNumericFilter("le123").value() == "<=123" );
    assert( FilterOps::parseNumericFilter("LE123").value() == "<=123" );
    assert( FilterOps::parseNumericFilter(" L E 1 2 3 ").value() == "<=123" );
    // test for invalid filter
    assert( ! FilterOps::parseNumericFilter("*").has_value() );
    assert( ! FilterOps::parseNumericFilter("===123").has_value() );
    assert( ! FilterOps::parseNumericFilter("!==123").has_value() );
    assert( ! FilterOps::parseNumericFilter("!>123").has_value() );
    assert( ! FilterOps::parseNumericFilter("!<123").has_value() );
    assert( ! FilterOps::parseNumericFilter(">>123").has_value() );
    assert( ! FilterOps::parseNumericFilter("=>123").has_value() );
    assert( ! FilterOps::parseNumericFilter(">==123").has_value() );
    assert( ! FilterOps::parseNumericFilter(">>=123").has_value() );
    assert( ! FilterOps::parseNumericFilter("<<123").has_value() );
    assert( ! FilterOps::parseNumericFilter("=<123").has_value() );
    assert( ! FilterOps::parseNumericFilter("<==123").has_value() );
    assert( ! FilterOps::parseNumericFilter("<<=123").has_value() );
    assert( ! FilterOps::parseNumericFilter("EQ ").has_value() );
    assert( ! FilterOps::parseNumericFilter("EQEQ 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("EQ EQ 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("NE ").has_value() );
    assert( ! FilterOps::parseNumericFilter("NEEQ 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("NE EQ 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("GT ").has_value() );
    assert( ! FilterOps::parseNumericFilter("GTEQ 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("GT EQ 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("GE ").has_value() );
    assert( ! FilterOps::parseNumericFilter("LT ").has_value() );
    assert( ! FilterOps::parseNumericFilter("LTEQ 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("LT EQ 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("LE ").has_value() );
    assert( ! FilterOps::parseNumericFilter("abc").has_value() );
    assert( ! FilterOps::parseNumericFilter("abc xyz").has_value() );
    assert( ! FilterOps::parseNumericFilter("abc 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("123 abc").has_value() );
    assert( ! FilterOps::parseNumericFilter("1a2b3c").has_value() );
    assert( ! FilterOps::parseNumericFilter("+123").has_value() );
    assert( ! FilterOps::parseNumericFilter("-123").has_value() );
    T_PRINT("FilterOps::parseNumericFilter");
}


} // namespace Testing
