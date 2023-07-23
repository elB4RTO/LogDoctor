
#include <iostream>
#include <assert.h>

#include "utilities/io.h"
#include "utilities/strings.h"
#include "utilities/vectors.h"

#include "modules/craplog/modules/datetime.h"
#include "modules/craplog/modules/formats.h"
#include "modules/craplog/modules/logs.h"

#include "modules/crapview/modules/filters.h"

#include <QTimeZone>


#define T_PRINT(ARG) std::cout << "  [PASSED] " ARG "()" << std::endl;


namespace Testing
{

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


    //// STRINGS ////

    assert( StringOps::count("test test  test   test ", ' ') == 7ul );
    assert( StringOps::count("flagABCflagXYZflag", "flag") == 3ul );
    T_PRINT("StringOps::count");

    assert( StringOps::isNumeric('0') == true );
    assert( StringOps::isNumeric(' ') == false );
    assert( StringOps::isNumeric("0123456789") == true );
    assert( StringOps::isNumeric("0123456789 ") == false );
    T_PRINT("StringOps::isNumeric");

    assert( StringOps::isAlphabetic('a') == true );
    assert( StringOps::isAlphabetic('.') == false );
    assert( StringOps::isAlphabetic("abcXYZ") == true );
    assert( StringOps::isAlphabetic("abc_XYZ") == false );
    T_PRINT("StringOps::isAlphabetic");

    assert( StringOps::isAlnum('X') == true );
    assert( StringOps::isAlnum('7') == true );
    assert( StringOps::isAlnum('-') == false );
    assert( StringOps::isAlnum("testME123") == true );
    assert( StringOps::isAlnum("Fail!") == false );
    T_PRINT("StringOps::isAlnum");

    assert( StringOps::isHex('6') == true );
    assert( StringOps::isHex('a') == true );
    assert( StringOps::isHex('F') == true );
    assert( StringOps::isHex('g') == false );
    T_PRINT("StringOps::isHex");

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

    assert( StringOps::strip("   ok ", ' ') == "ok" );
    assert( StringOps::strip("a b ctestc", " abc") == "test" );
    assert( StringOps::strip("\n\t \ntest\r ") == "test" );
    T_PRINT("StringOps::strip");

    assert( StringOps::lstrip("___ok_", '_') == "ok_" );
    assert( StringOps::lstrip("the three trees", " ethr") == "s" );
    assert( StringOps::lstrip(" \n\t\rtest ") == "test " );
    T_PRINT("StringOps::lstrip");

    assert( StringOps::rstrip("_ok___", '_') == "_ok" );
    assert( StringOps::rstrip("testTEST", "TEST") == "test" );
    assert( StringOps::rstrip(" test\r\t\n ") == " test" );
    T_PRINT("StringOps::rstrip");

    assert( StringOps::lstripUntil("is ok?", ' ') == "ok?" );
    assert( StringOps::lstripUntil("yes, it is", ' ', false) == " it is" );
    assert( StringOps::lstripUntil("__fine", '_', true, false) == "_fine" );
    assert( StringOps::lstripUntil("\t\t\t\t\t\t\t\t\tx", '\t') == "x" );
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
    //// DATE-TIME ////

    {
    QTimeZone tz{ QTimeZone::systemTimeZone() };
    int e{ 946771199 - tz.standardTimeOffset( QDateTime::fromSecsSinceEpoch(946771199) ) };
    std::string e_str{ std::to_string( e ) };
    std::string epochs[4]{ e_str, e_str+".000", e_str+"000", e_str+"000000" };
    std::vector<std::string> target{"2000","01","01","23","59","59"};
    assert( DateTimeOps::processDateTime("[01/Jan/2000:23:59:59 +0000]", "ncsa") == target );
    assert( DateTimeOps::processDateTime("Sat Jan 01 23:59:59 2000", "mcs") == target );
    assert( DateTimeOps::processDateTime("Saturday, 01-Jan-2000 23:59:59 UTC", "gmt") == target );
    assert( DateTimeOps::processDateTime("2000-01-01T23:59:59+00:00", "iso") == target );
    assert( DateTimeOps::processDateTime(epochs[0], "epoch_s") == target );
    assert( DateTimeOps::processDateTime(epochs[1], "epoch_s.ms") == target );
    assert( DateTimeOps::processDateTime(epochs[2], "epoch_ms") == target );
    assert( DateTimeOps::processDateTime(epochs[3], "epoch_us") == target );
    target = {"2000","01","01","","",""};
    assert( DateTimeOps::processDateTime("2000-01-01", "utc_d") == target );
    assert( DateTimeOps::processDateTime("2000-01-01", "YYYYMMDD") == target );
    assert( DateTimeOps::processDateTime("01/01/00", "MMDDYY") == target );
    assert( DateTimeOps::processDateTime("1/1/2000", "MDYYYY") == target );
    target = {"","","","23","59","59"};
    assert( DateTimeOps::processDateTime("23:59:59", "utc_t") == target );
    assert( DateTimeOps::processDateTime("11:59:59 pm", "clock_12") == target );
    assert( DateTimeOps::processDateTime("23:59:59", "clock_24") == target );
    target = {"","","","23","59",""};
    assert( DateTimeOps::processDateTime("23:59", "clock_short") == target );
    target = {"","","","PM","",""};
    assert( DateTimeOps::processDateTime("pm", "clock_meridian") == target );
    target = {"2000","","","","",""};
    assert( DateTimeOps::processDateTime("2000", "year") == target );
    target = {"","01","","","",""};
    assert( DateTimeOps::processDateTime("01", "month") == target );
    target = {"","","01","","",""};
    assert( DateTimeOps::processDateTime("01", "day") == target );
    target = {"","","","23","",""};
    assert( DateTimeOps::processDateTime("23", "hour") == target );
    target = {"","","","","59",""};
    assert( DateTimeOps::processDateTime("59", "minute") == target );
    target = {"","","","","","59"};
    assert( DateTimeOps::processDateTime("59", "second") == target );
    }
    T_PRINT("DateTimeOps::processDateTime");


    //// FORMATS ////

    FormatOps fo;

    {
    std::vector<std::string> f{"client","NONE","NONE","date_time_ncsa","request_full","response_code","NONE","referer","user_agent"};
    assert( fo.processApacheFormatString("%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\"").fields == f );
    }
    T_PRINT("FormatOps::processApacheFormatString");

    {
    std::vector<std::string> f{"client","NONE","date_time_ncsa","request_full","response_code","bytes_sent","referer","user_agent"};
    assert( fo.processNginxFormatString("$remote_addr - $remote_user [$time_local] \"$request\" $status $bytes_sent \"$http_referer\" \"$http_user_agent\"").fields == f );
    }
    T_PRINT("FormatOps::processNginxFormatString");

    {
    std::vector<std::string> f{"date_time_utc_d","date_time_utc_t","NONE","request_method","request_uri","request_query","NONE","NONE","client","user_agent","referer","response_code","NONE","NONE","time_taken_ms"};
    assert( fo.processIisFormatString("date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs(User-Agent) cs(Referer) sc-status sc-substatus sc-win32-status time-taken", 0).fields == f );
    f = {"client","NONE","NONE","date_time_ncsa","request_full","response_code","bytes_sent"};
    assert( fo.processIisFormatString("", 1).fields == f );
    f = {"client","NONE","date_time_MDYYYY","date_time_utc_t","NONE","NONE","NONE","time_taken_ms","bytes_received","bytes_sent","response_code","NONE","request_method","request_uri","request_query"};
    assert( fo.processIisFormatString("", 2).fields == f );
    }
    T_PRINT("FormatOps::processIisFormatString");


    //// LOGS ////

    {
    LogsFormat lf{ "","","]",{" ","_"},{"","",""},0 };
    assert( LogOps::defineFileType({"ok ok_ok]","a a_a]","TEST TEST_TEST]"}, lf) == LogType::Access );
    assert( LogOps::defineFileType({"no no no!","some thing wrong","with this file!"}, lf) == LogType::Discarded );
    assert( LogOps::defineFileType({}, lf) == LogType::Failed );
    }
    T_PRINT("LogOps::defineFileType");
}


void testCrapviewModules()
{
    //// FILTERS ////

    assert( FilterOps::parseNull("  null   ").value() == "NULL" );
    assert( FilterOps::parseNull("  not null   ").value() == "NOT NULL" );
    assert( FilterOps::parseNull("not   null").value() == "NOT NULL" );
    assert( FilterOps::parseNull("!    null").value() == "NOT NULL" );
    assert( FilterOps::parseNull("null").value() == "NULL" );
    assert( FilterOps::parseNull("NULL").value() == "NULL" );
    assert( FilterOps::parseNull("not null").value() == "NOT NULL" );
    assert( FilterOps::parseNull("NOT NULL").value() == "NOT NULL" );
    assert( FilterOps::parseNull("!NULL").value() == "NOT NULL" );
    assert( FilterOps::parseNull("! NULL").value() == "NOT NULL" );
    assert( ! FilterOps::parseNull("").has_value() );
    assert( ! FilterOps::parseNull("123").has_value() );
    assert( ! FilterOps::parseNull("abc").has_value() );
    assert( ! FilterOps::parseNull("*").has_value() );
    assert( ! FilterOps::parseNull("null test").has_value() );
    assert( ! FilterOps::parseNull("not null test").has_value() );
    T_PRINT("FilterOps::parseNull");

    assert( FilterOps::parseTextualFilter("").value() == "" );
    assert( FilterOps::parseTextualFilter("null").value() == "NULL" );
    assert( FilterOps::parseTextualFilter("not null").value() == "NOT NULL" );
    assert( FilterOps::parseTextualFilter("*").value() == "NOT NULL" );
    assert( FilterOps::parseTextualFilter("  a test string ").value() == "a test string" );
    assert( FilterOps::parseTextualFilter("[test 123 #!?]").value() == "[test 123 #!?]" );
    T_PRINT("FilterOps::parseTextualFilter");

    assert( FilterOps::parseNumericFilter("").value() == "" );
    assert( FilterOps::parseNumericFilter("null").value() == "NULL" );
    assert( FilterOps::parseNumericFilter("not null").value() == "NOT NULL" );
    assert( FilterOps::parseNumericFilter("   123 ").value() == "= 123" );
    assert( FilterOps::parseNumericFilter(" =   123 ").value() == "= 123" );
    assert( FilterOps::parseNumericFilter(" !   123 ").value() == "!= 123" );
    assert( FilterOps::parseNumericFilter("123").value() == "= 123" );
    assert( FilterOps::parseNumericFilter("=123").value() == "= 123" );
    assert( FilterOps::parseNumericFilter("= 123").value() == "= 123" );
    assert( FilterOps::parseNumericFilter("==123").value() == "= 123" );
    assert( FilterOps::parseNumericFilter("== 123").value() == "= 123" );
    assert( FilterOps::parseNumericFilter("eq 123").value() == "= 123" );
    assert( FilterOps::parseNumericFilter("!123").value() == "!= 123" );
    assert( FilterOps::parseNumericFilter("!=123").value() == "!= 123" );
    assert( FilterOps::parseNumericFilter("!= 123").value() == "!= 123" );
    assert( FilterOps::parseNumericFilter("ne 123").value() == "!= 123" );
    assert( FilterOps::parseNumericFilter(">123").value() == "> 123" );
    assert( FilterOps::parseNumericFilter("> 123").value() == "> 123" );
    assert( FilterOps::parseNumericFilter("gt 123").value() == "> 123" );
    assert( FilterOps::parseNumericFilter(">=123").value() == ">= 123" );
    assert( FilterOps::parseNumericFilter(">= 123").value() == ">= 123" );
    assert( FilterOps::parseNumericFilter("ge 123").value() == ">= 123" );
    assert( FilterOps::parseNumericFilter("<123").value() == "< 123" );
    assert( FilterOps::parseNumericFilter("< 123").value() == "< 123" );
    assert( FilterOps::parseNumericFilter("lt 123").value() == "< 123" );
    assert( FilterOps::parseNumericFilter("<=123").value() == "<= 123" );
    assert( FilterOps::parseNumericFilter("<= 123").value() == "<= 123" );
    assert( FilterOps::parseNumericFilter("le 123").value() == "<= 123" );
    // test for invalid filter
    assert( ! FilterOps::parseNumericFilter("*").has_value() );
    assert( ! FilterOps::parseNumericFilter("= =123").has_value() );
    assert( ! FilterOps::parseNumericFilter("= = 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("===123").has_value() );
    assert( ! FilterOps::parseNumericFilter("=== 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("eq123").has_value() );
    assert( ! FilterOps::parseNumericFilter("!>123").has_value() );
    assert( ! FilterOps::parseNumericFilter("!> 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("!<123").has_value() );
    assert( ! FilterOps::parseNumericFilter("!< 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("! =123").has_value() );
    assert( ! FilterOps::parseNumericFilter("! = 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("!==123").has_value() );
    assert( ! FilterOps::parseNumericFilter("!== 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("ne123").has_value() );
    assert( ! FilterOps::parseNumericFilter(">>123").has_value() );
    assert( ! FilterOps::parseNumericFilter(">> 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("gt123").has_value() );
    assert( ! FilterOps::parseNumericFilter("=>123").has_value() );
    assert( ! FilterOps::parseNumericFilter("=> 123").has_value() );
    assert( ! FilterOps::parseNumericFilter(">==123").has_value() );
    assert( ! FilterOps::parseNumericFilter(">== 123").has_value() );
    assert( ! FilterOps::parseNumericFilter(">>=123").has_value() );
    assert( ! FilterOps::parseNumericFilter(">>= 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("ge123").has_value() );
    assert( ! FilterOps::parseNumericFilter("<<123").has_value() );
    assert( ! FilterOps::parseNumericFilter("<< 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("lt123").has_value() );
    assert( ! FilterOps::parseNumericFilter("=<123").has_value() );
    assert( ! FilterOps::parseNumericFilter("=< 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("<==123").has_value() );
    assert( ! FilterOps::parseNumericFilter("<== 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("<<=123").has_value() );
    assert( ! FilterOps::parseNumericFilter("<<= 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("le123").has_value() );
    assert( ! FilterOps::parseNumericFilter("eq ").has_value() );
    assert( ! FilterOps::parseNumericFilter("abc").has_value() );
    assert( ! FilterOps::parseNumericFilter("abc xyz").has_value() );
    assert( ! FilterOps::parseNumericFilter("abc 123").has_value() );
    assert( ! FilterOps::parseNumericFilter("123 abc").has_value() );
    assert( ! FilterOps::parseNumericFilter("1a2b3c").has_value() );
    assert( ! FilterOps::parseNumericFilter("123 456").has_value() );
    assert( ! FilterOps::parseNumericFilter("+123").has_value() );
    assert( ! FilterOps::parseNumericFilter("-123").has_value() );
    T_PRINT("FilterOps::parseNumericFilter");

    assert( FilterOps::parseBooleanFilter("").value() == "" );
    assert( FilterOps::parseBooleanFilter("null").value() == "NULL" );
    assert( FilterOps::parseBooleanFilter("not null").value() == "NOT NULL" );
    // test for true
    assert( FilterOps::parseBooleanFilter("1").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("true").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("TRUE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("=true").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("=TRUE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("= true").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("= TRUE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("==true").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("==TRUE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("== true").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("== TRUE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("!=false").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("!=FALSE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("!= false").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("!= FALSE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("!false").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("!FALSE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("! false").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("! FALSE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("!    false").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("!    FALSE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("not false").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("NOT FALSE").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("not   false").value() == "= 1" );
    assert( FilterOps::parseBooleanFilter("NOT   FALSE").value() == "= 1" );
    // test for false
    assert( FilterOps::parseBooleanFilter("0").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("false").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("FALSE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("=false").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("=FALSE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("= false").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("= FALSE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("==false").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("==FALSE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("== false").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("== FALSE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("!=true").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("!=TRUE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("!= true").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("!= TRUE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("!true").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("!TRUE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("! true").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("! TRUE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("!    true").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("!    TRUE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("not true").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("NOT TRUE").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("not   true").value() == "= 0" );
    assert( FilterOps::parseBooleanFilter("NOT   TRUE").value() == "= 0" );
    // test for invalid filter
    assert( ! FilterOps::parseBooleanFilter("*").has_value() );
    assert( ! FilterOps::parseBooleanFilter("2").has_value() );
    assert( ! FilterOps::parseBooleanFilter("0 1").has_value() );
    assert( ! FilterOps::parseBooleanFilter("abc").has_value() );
    assert( ! FilterOps::parseBooleanFilter("true false").has_value() );
    T_PRINT("FilterOps::parseBooleanFilter");
}


} // namespace Testing
