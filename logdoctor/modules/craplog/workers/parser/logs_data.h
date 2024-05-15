#ifndef LOGDOCTOR__CRAPLOG__WORKERS__PARSER__LOGS_DATA_H
#define LOGDOCTOR__CRAPLOG__WORKERS__PARSER__LOGS_DATA_H


#include <string>
#include <vector>


#ifdef _MSC_VER
enum LogsFormatField : uint32_t;
#else
enum LogsFormatField : __uint32_t;
#endif

struct LogsFormat;


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
    FieldData(const FieldData &) = delete;
    FieldData &operator=(const FieldData &) = delete;

    //! Returns whether the field contains data
    inline operator bool() const noexcept
    { return this->is_set; }
    //! Returns a reference to the field's data
    inline const std::string& operator *() const noexcept
    { return this->data; }
    //! Returns the sum of the field's data size with another field's data size
    inline size_t operator +(const FieldData& rhs) const noexcept
    { return this->data.size() + rhs.data.size(); }
    //! Returns the sum of the field's data size with the given size
    friend inline size_t operator +(const size_t lhs, const FieldData& rhs) noexcept;

private:
    bool is_set;
    std::string data;
};


inline size_t operator +(const size_t lhs, const FieldData& rhs) noexcept
{
    return lhs + rhs.data.size();
}



//! LogLineData
/*!
    Holds the data of a single log line
*/
struct LogLineData final
{
    //! \throw LogParserException
    LogLineData(const std::string& line, const LogsFormat& logs_format);
    ~LogLineData() noexcept = default;
    LogLineData(LogLineData&& other) noexcept = default;
    LogLineData& operator=(LogLineData&& rhs) noexcept = delete;
    LogLineData(const LogLineData &) = delete;
    LogLineData &operator=(const LogLineData &) = delete;

    //! Returns the total size of all the fields' data
    size_t size() const noexcept;

    // date and time
    FieldData year;
    FieldData month;
    FieldData day;
    FieldData hour;
    FieldData minute;
    FieldData second;
    // request
    FieldData protocol;
    FieldData method;
    FieldData uri;
    FieldData query;
    // server
    FieldData response_code;
    FieldData time_taken;
    FieldData bytes_sent;
    FieldData bytes_received;
    // client
    FieldData client;
    FieldData cookie;
    FieldData user_agent;
    FieldData referrer;

private:
    //! Returns a reference to the field data corresponding to the given field identifier
    FieldData& data(const LogsFormatField id);

    void storeUriQuery(std::string&& str) noexcept;

    void storeMalformedRequestOneSpace(std::string&& str) noexcept;
    void storeMalformedRequestMultiSpace(std::string&& str) noexcept;

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


#endif // LOGDOCTOR__CRAPLOG__WORKERS__PARSER__LOGS_DATA_H
