#ifndef CRAPVIEW_H
#define CRAPVIEW_H

#include <QHash>

#include "tools/crapview/modules/query.h"

#include <string>


class Crapview
{
public:
    Crapview();

    const int getDialogLevel();
    void setDialogLevel( const int new_level );

    void setDbPath( const std::string& path );

    void refreshDates();

    const QStringList getYears( const QString& web_server, const QString& logs_type );
    const QStringList getMonths( const QString& web_server, const QString& logs_type, const QString& year );
    const QStringList getDays( const QString& web_server, const QString& logs_type, const QString& year, const QString& month );

    const QStringList getFields( const QString& tab, const QString& logs_type );


private:
    // quantity of informational dialogs to display
    int dialog_level = 2; // 0: essential, 1: usefull, 2: explanatory

    DbQuery dbQuery;

    // collection of available dates
    // { web_server : { log_type : { year : { month_str : [ days ] } } } }
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>> dates;

    // collection of available fields for tabs which needs them
    // { tab : { log_type_str : [ fields ] } }
    const QHash<QString, QHash<QString, QStringList>> fields = {
        {"Daytime", {
            {"Access", {"Protocol","Method","URI","Query","Response code","Referrer","Cookie","User-agent","Client"}},
            {"Error",  {"Level","Message","Source file","Client","Port"}} }},
        {"Relational", {
            {"Access", {"Protocol","Method","URI","Query","Response code","Time taken","Bytes sent","Bytes received","Referrer","Cookie","User-agent","Client"}},
            {"Error",  {"Level","Message","Source file","Client","Port"}} }}
    };

    // conversion between text and IDs
    const QHash<QString, int>
        WebServer_s2i = {
            {"Apache2",11}, {"Nginx",12}, {"IIS",13} },
        LogsType_s2i  = {
            {"Access",1}, {"Error",2} },
        Months_s2i = {
            {"January",1}, {"February",2}, {"March",3}, {"April",4}, {"May",5}, {"June",6},
            {"July",7}, {"August",8}, {"September",9}, {"October",10}, {"November",11}, {"December",12} };

    const QHash<int, QString>
        /*WebServer_i2s = {
            {11,"Apache2"}, {12,"Nginx"}, {13,"IIS"} },
        LogsType_i2s  = {
            {1,"Access"}, {2,"Error"} },
        LogField_i2s  = {
            {1,"year"}, {2,"month"}, {3,"day"}, {4,"hour"}, {5,"minute"}, {6,"second"},
            {10,"protocol"}, {11,"method"}, {12,"request"}, {13,"query"}, {14,"response"},
            {15,"time_taken"}, {16,"bytes_sent"}, {17,"bytes_received"}, {18,"referrer"},
            {20,"client"}, {21,"user_agent"}, {22,"cookie"},
            {30,"port"}, {31,"level"}, {32,"message"}, {33,"source_file"} },*/
        Months_i2s = {
            {1,"January"}, {2,"February"}, {3,"March"}, {4,"April"}, {5,"May"}, {6,"June"},
            {7,"July"}, {8,"August"}, {9,"September"}, {10,"October"}, {11,"November"}, {12,"December"} };
};

#endif // CRAPVIEW_H
