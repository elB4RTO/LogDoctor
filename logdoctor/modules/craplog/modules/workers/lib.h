#ifndef LOGDOCTOR__CRAPLOG__WORKERS__LIB_H
#define LOGDOCTOR__CRAPLOG__WORKERS__LIB_H


#include <QMetaType>


struct LogsFormat;


//! Signals which dialog to show
enum class WorkerDialog {
    errGeneric,
    errDirNotExists,
    errFailedDefiningLogType,
    errFailedParsingLogs,
    errDatabaseFailedOpening,
    errDatabaseFailedExecuting,
    warnFileNotReadable,
    warnEmptyFile,
};

Q_DECLARE_METATYPE(WorkerDialog)



//! FieldData
/*!
    Holds the data of a single field from a log line
    \see LogLineData
*/
struct FieldData final
{
    FieldData() noexcept
        : is_set{false} {}
    explicit FieldData(std::string&& data) noexcept
        : is_set{!data.empty()}, data{std::move(data)} {}
    ~FieldData() noexcept = default;
    FieldData(FieldData&& other) noexcept = default;
    FieldData& operator=(FieldData&& rhs) noexcept = default;
    Q_DISABLE_COPY(FieldData)

    inline operator bool() const noexcept
        { return this->is_set; }
    inline const std::string& operator *() const noexcept
        { return this->data; }

    inline size_t operator +(const FieldData& rhs) const noexcept
        { return this->data.size() + rhs.data.size(); }

private:
    bool is_set;
    std::string data;
};


inline size_t operator +(const size_t lhs, const FieldData& rhs) noexcept
{
    return lhs + (*rhs).size();
}



//! LogLineData
/*!
    Holds the data of a single log line
*/
struct LogLineData final
{
    LogLineData(const std::string& line, const LogsFormat& logs_format);
    ~LogLineData() noexcept = default;
    LogLineData(LogLineData&& other) noexcept = default;
    LogLineData& operator=(LogLineData&& rhs) noexcept = delete;
    Q_DISABLE_COPY(LogLineData)

    size_t size() const noexcept;

    // date and time
    FieldData year;             // 1
    FieldData month;            // 2
    FieldData day;              // 3
    FieldData hour;             // 4
    FieldData minute;           // 5
    FieldData second;           // 6
    // request
    FieldData protocol;         // 10
    FieldData method;           // 11
    FieldData uri;              // 12
    FieldData query;            // 13
    // server
    FieldData response_code;    // 14
    FieldData time_taken;       // 15
    FieldData bytes_sent;       // 16
    FieldData bytes_received;   // 17
    // client
    FieldData client;           // 20
    FieldData cookie;           // 22
    FieldData user_agent;       // 21
    FieldData referrer;         // 18

private:
    FieldData& data(const int& id);

    void storeUriQuery(std::string&& str) noexcept;

    void storeMalformedRequestOneSpace(std::string&& str) noexcept;
    void storeMalformedRequestMultiSpace(std::string&& str) noexcept;

    inline static const std::unordered_map<std::string, int> field2id{
        // date-time
        {"date_time_year",        1},
        {"date_time_month",       2},
        {"date_time_day",         3},
        {"date_time_hour",        4},
        {"date_time_minute",      5},
        {"date_time_second",      6},
        {"date_time_ncsa",        0},
        {"date_time_iso",         0},
        {"date_time_mcs",         0},
        {"date_time_gmt",         0},
        {"date_time_utc_d",       0},
        {"date_time_utc_t",       0},
        {"date_time_epoch_s",     0},
        {"date_time_epoch_s.ms",  0},
        {"date_time_epoch_ms",    0},
        {"date_time_epoch_us",    0},
        {"date_time_YYYYMMDD",    0},
        {"date_time_MMDDYY",      0},
        {"date_time_MDYYYY",      0},
        {"date_time_year_short",  0},
        {"date_time_month_str",   0},
        {"date_time_clock_12",    0},
        {"date_time_clock_24",    0},
        {"date_time_clock_short", 0},
        // request
        {"request_protocol",     10},
        {"request_method",       11},
        {"request_uri",          12},
        {"request_query",        13},
        {"response_code",        14},
        {"request_full",          0},
        // performance
        {"time_taken_ms",        15},
        {"time_taken_us",         0},
        {"time_taken_s.ms",       0},
        {"time_taken_s",          0},
        {"bytes_sent",           16},
        {"bytes_received",       17},
        // referer
        {"referer",              18},
        // client data
        {"client",               20},
        {"user_agent",           21},
        {"cookie",               22}
    };

    inline static const std::vector<std::string> valid_methods{
        "GET",
        "POST",
        "HEAD",
        "PUT",
        "DELETE",
        "OPTIONS",
        "CONNECT",
        "TRACE",
        "PATCH"
    };

    inline static const std::vector<std::string> valid_protocols{
        "HTTP/0.9",
        "HTTP/1.0",
        "HTTP/1.1",
        "HTTP/2",
        "HTTP/3"
    };
};


#endif // LOGDOCTOR__CRAPLOG__WORKERS__LIB_H
