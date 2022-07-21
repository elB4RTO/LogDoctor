#ifndef QUERY_H
#define QUERY_H

#include <QString>

#include <vector>
#include <unordered_map>


class DbQuery
{
public:
    DbQuery();

    void setDialogLevel( const int new_level );

    void setDbPath( const std::string& path );
    //const std::string getDbPath( const int web_server );

    const std::tuple<const bool, const std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>>> refreshDates();

    /*const std::vector<QString> getYears( const QString& web_server, const QString& log_type );
    const std::vector<QString> getMonths( const QString& web_server, const QString& log_type, const QString& year );
    const std::vector<QString> getDays( const QString& web_server, const QString& log_type, const QString& year, const QString& month );*/

private:
    // quantity of informational dialogs to display
    int dialog_level = 2; // 0: essential, 1: usefull, 2: explanatory

    std::string db_path;

    /*const std::unordered_map<std::string, int>
        WebServer_str2int = {
            {"Apache2",11}, {"Nginx",12}, {"IIS",13} },
        LogsType_str2int  = {
            {"Access",1}, {"Error",2} },
        LogField_str2int  = {
            {"year",1}, {"month",2}, {"day",3}, {"hour",4}, {"minute",5}, {"second",6},
            {"protocol",10}, {"method",11}, {"request",12}, {"query",13}, {"response",14},
            {"time_taken",15}, {"bytes_sent",16}, {"bytes_received",17}, {"referrer",18},
            {"client",20}, {"user_agent",21}, {"cookie",22},
            {"port",30}, {"level",31}, {"message",32}, {"source_file",33} },
        Months_str2int    = {
            {"January",1}, {"February",2}, {"March",3}, {"April",4}, {"May",5}, {"June",6},
            {"July",7}, {"August",8}, {"September",9}, {"October",10}, {"November",11}, {"December",12} };

    const std::unordered_map<int, QString>
        WebServer_int2str = {
            {11,"Apache2"}, {12,"Nginx"}, {13,"IIS"} },
        LogsType_int2str  = {
            {1,"Access"}, {2,"Error"} },
        LogField_int2str  = {
            {1,"year"}, {2,"month"}, {3,"day"}, {4,"hour"}, {5,"minute"}, {6,"second"},
            {10,"protocol"}, {11,"method"}, {12,"request"}, {13,"query"}, {14,"response"},
            {15,"time_taken"}, {16,"bytes_sent"}, {17,"bytes_received"}, {18,"referrer"},
            {20,"client"}, {21,"user_agent"}, {22,"cookie"},
            {30,"port"}, {31,"level"}, {32,"message"}, {33,"source_file"} },
        Months_int2str    = {
            {1,"January"}, {2,"February"}, {3,"March"}, {4,"April"}, {5,"May"}, {6,"June"},
            {7,"July"}, {8,"August"}, {9,"September"}, {10,"October"}, {11,"November"}, {12,"December"} };*/
};

#endif // QUERY_H
