#ifndef QUERY_H
#define QUERY_H

#include <QMessageBox>
#include <QString>
#include <QHash>

#include <vector>
#include <unordered_map>


const QHash<int, QString>
    TYPES = {
        {1,QMessageBox::tr("Access")}, {2,QMessageBox::tr("Error")} },
    FIELDS = {
        {10,QMessageBox::tr("Protocol")},   {11,QMessageBox::tr("Method")},
        {12,QMessageBox::tr("URI")},        {13,QMessageBox::tr("Query")},      {14,QMessageBox::tr("Response code")},
        {15,QMessageBox::tr("Time taken")}, {16,QMessageBox::tr("Bytes sent")}, {17,QMessageBox::tr("Bytes received")},
        {18,QMessageBox::tr("Referrer")},   {22,QMessageBox::tr("Cookie")},
        {20,QMessageBox::tr("Client")},     {21,QMessageBox::tr("User-agent")},
        {30,QMessageBox::tr("Port")},
        {31,QMessageBox::tr("Level")}, {32,QMessageBox::tr("Message")}, {33,QMessageBox::tr("Source file")} },
    MONTHS = {
        {1, QMessageBox::tr("January")},  {2, QMessageBox::tr("February")},  {3, QMessageBox::tr("March")},
        {4, QMessageBox::tr("April")},    {5, QMessageBox::tr("May")},       {6, QMessageBox::tr("June")},
        {7, QMessageBox::tr("July")},     {8, QMessageBox::tr("August")},    {9, QMessageBox::tr("September")},
        {10,QMessageBox::tr("October")},  {11,QMessageBox::tr("November")},  {12,QMessageBox::tr("December")} };


class DbQuery
{
public:
    DbQuery();

    void setDialogLevel( const int new_level );

    void setDbPath( const std::string& path );
    //const std::string getDbPath( const int web_server );

    void refreshDates( std::tuple<bool, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>>>& result );

    void getItemsCount(
        std::tuple<bool, std::vector<std::tuple<QString, int>>>& result,
        const QString& web_server, const QString& log_type,
        const QString& year,       const QString& month,    const QString& day,
        const QString& log_field );

    void getDaytimeCounts(
        std::tuple<bool, std::unordered_map<int, std::unordered_map<int, int>>>& result,
        const QString& web_server, const QString& log_type,
        const QString& from_year_, const QString& from_month_, const QString& from_day_,
        const QString& to_year_,   const QString& to_month_,   const QString& to_day_,
        const QString& log_field_, const QString& field_filter );

    void getRelationalCountsDay(
        std::tuple<bool, std::vector<int>>& result,
        const QString& web_server,   const QString& log_type,
        const QString& year_,        const QString& month_,         const QString& day_,
        const QString& log_field_1_, const QString& field_filter_1,
        const QString& log_field_2_, const QString& field_filter_2 );

    void getRelationalCountsPeriod(
        std::tuple<bool, std::vector<int>>& result,
        const QString& web_server,   const QString& log_type,
        const QString& from_year_,   const QString& from_month_,    const QString& from_day_,
        const QString& to_year_,     const QString& to_month_,      const QString& to_day_,
        const QString& log_field_1_, const QString& field_filter_1,
        const QString& log_field_2_, const QString& field_filter_2 );

private:
    // quantity of informational dialogs to display
    int dialog_level = 2; // 0: essential, 1: usefull, 2: explanatory

    std::string db_path;
    QString db_name;

    const QHash<QString, QString>
        LogFields_to_DbFields = {
            {FIELDS.value(10), "protocol"},
            {FIELDS.value(11), "method"},
            {FIELDS.value(12), "request"},
            {FIELDS.value(13), "query"},
            {FIELDS.value(14), "response"},
            {FIELDS.value(15), "time_taken"},
            {FIELDS.value(16), "bytes_sent"},
            {FIELDS.value(17), "bytes_received"},
            {FIELDS.value(18), "referrer"},
            {FIELDS.value(20), "client"},
            {FIELDS.value(21), "user_agent"},
            {FIELDS.value(22), "cookie"},
            {FIELDS.value(30), "port"},
            {FIELDS.value(31), "level"},
            {FIELDS.value(32), "message"},
            {FIELDS.value(33), "source_file"}
        };

    const QHash<QString, int>
        Months_s2i = {
            {MONTHS.value(1),1},   {MONTHS.value(2),2},   {MONTHS.value(3),3},
            {MONTHS.value(4),4},   {MONTHS.value(5),5},   {MONTHS.value(6),6},
            {MONTHS.value(7),7},   {MONTHS.value(8),8},   {MONTHS.value(9),9},
            {MONTHS.value(10),10}, {MONTHS.value(11),11}, {MONTHS.value(12),12} };

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
