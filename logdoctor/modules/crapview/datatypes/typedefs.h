#ifndef LOGDOCTOR__CRAPVIEW__DATATYPES__TYPEDEFS_H
#define LOGDOCTOR__CRAPVIEW__DATATYPES__TYPEDEFS_H


#define CRAPVIEW_DATA_TYPEDEFS\
    using stats_dates_t    = std::map<int, std::map<int, std::vector<int>>>;\
    using database_dates_t = std::map<WebServer, stats_dates_t>;


#endif // LOGDOCTOR__CRAPVIEW__DATATYPES__TYPEDEFS_H
