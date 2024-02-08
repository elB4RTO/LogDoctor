#ifndef LOGDOCTOR__CRAPLOG__MODULES__LIB_H
#define LOGDOCTOR__CRAPLOG__MODULES__LIB_H


#include <QString>
#include <QMetaType>

#include <vector>


//! Enumerates log file types
/*!
    File types used to decide whether a file should be considered valid or not
    \see LogOps::defineFileType()
*/
enum class LogType
#ifdef _MSC_VER
    : __int8
#else
    : __INT8_TYPE__
#endif
{
    Failed    = -1, //!< Failed to determine the type
    Discarded =  0, //!< Not a valid file, will be discarded
    Access    =  1  //!< Valid access logs file type
};


//! Holds informations about a log file
struct LogFile final
{
    explicit LogFile() noexcept = default;
    explicit LogFile(const bool sel,const bool used,const size_t sz,const QString& nm,const std::string& hs,const std::string& pt) noexcept
        :selected{sel},used_already{used},size_{sz},name_{nm},hash_{hs},path_{pt}{}
    LogFile(LogFile&& other) noexcept = default;
    LogFile& operator=(LogFile&& other) noexcept = default;
    LogFile(const LogFile& other) noexcept = default;
    LogFile& operator=(const LogFile& other) noexcept = default;
    //! Wheter the file has been selected to be used or not
    inline bool isSelected() const noexcept
        { return this->selected; }
    //! Sets the file as selected
    inline void setSelected() noexcept
        { this->selected |= true; }
    //! Sets the file as unselected
    inline void setUnselected() noexcept
        { this->selected &= false; }
    //! Wheter the file has been used already or not
    inline bool hasBeenUsed() const noexcept
        { return this->used_already; }
    //! The size of the file
    inline size_t size() const noexcept
        { return this->size_; }
    //! The name of the file, to be displayed in the list
    inline const QString& name() const noexcept
        { return this->name_; }
    //! The sha256 hash of the content
    inline const std::string& hash() const noexcept
        { return this->hash_; }
    //! The path of the file, including the file name
    inline const std::string& path() const noexcept
        { return this->path_; }
private:
    bool selected;
    bool used_already;
    size_t size_;
    QString name_;
    std::string hash_;
    std::string path_;
};
Q_DECLARE_METATYPE( LogFile )



//! Identifies a log field
enum LogsFormatField : uint32_t {
    // not a considered field
    _INVALID              = 0x00000001, // 00000000'00000000'00000000'00000001
    // not a considered field
    _DISCARDED            = 0x00000000, // 00000000'00000000'00000000'00000000
    // no need to parse
    _NO_PARSE_NEEDED      = 0x80000000, // 10000000'00000000'00000000'00000000
    // no need to parse
    _MAY_HAVE_SPACES      = 0x01000000, // 00000001'00000000'00000000'00000000
    _COUNT_SPACES         = 0x01000000, // 00000000'00000000'00000000'11111111
    // response
    _RESPONSE_CODE        = 0x00000100, // 00000000'00000000'00000001'00000000
    response_code         = 0x80000101, // 10000000'00000000'00000001'00000001
    // data transfer
    _DATA_TRANSFER        = 0x00000200, // 00000000'00000000'00000010'00000000
    bytes_received        = 0x80000201, // 10000000'00000000'00000010'00000001
    bytes_sent            = 0x80000202, // 10000000'00000000'00000010'00000010
    // performance
    _TIME_TAKEN           = 0x00000400, // 00000000'00000000'00000100'00000000
    time_taken_us         = 0x00000401, // 00000000'00000000'00000100'00000001
    time_taken_ms         = 0x80000402, // 10000000'00000000'00000100'00000010
    time_taken_s          = 0x00000404, // 00000000'00000000'00000100'00000100
    time_taken_s_ms       = 0x00000406, // 00000000'00000000'00000100'00000110
    // client data
    _CLIENT_DATA          = 0x00000800, // 00000000'00000000'00001000'00000000
    client                = 0x80000801, // 10000000'00000000'00001000'00000001
    user_agent            = 0x80000802, // 10000000'00000000'00001000'00000010
    cookie                = 0x80000804, // 10000000'00000000'00001000'00000100
    referer               = 0x80000808, // 10000000'00000000'00001000'00001000
    // request
    _REQUEST              = 0x00001000, // 00000000'00000000'00010000'00000000
    request_protocol      = 0x80001001, // 10000000'00000000'00010000'00000001
    request_method        = 0x80001002, // 10000000'00000000'00010000'00000010
    request_uri           = 0x80001004, // 10000000'00000000'00010000'00000100
    request_query         = 0x80001008, // 10000000'00000000'00010000'00001000
    request_uri_query     = 0x0000100c, // 00000000'00000000'00010000'00001100
    request_full          = 0x0100100f, // 00000001'00000000'00010000'00001111
    // date-time
    _DATE_TIME            = 0x00800000, // 00000000'10000000'00000000'00000000
    _DATE_TIME_FULL       = 0x00400000, // 00000000'01000000'00000000'00000000
    _DATE_TIME_EPOCH      = 0x00200000, // 00000000'00100000'00000000'00000000
    _DATE_TIME_UTC        = 0x00100000, // 00000000'00010000'00000000'00000000
    _DATE_TIME_DATE       = 0x00080000, // 00000000'00001000'00000000'00000000
    _DATE_TIME_YEAR       = 0x00040000, // 00000000'00000100'00000000'00000000
    _DATE_TIME_MONTH      = 0x00020000, // 00000000'00000010'00000000'00000000
    _DATE_TIME_CLOCK      = 0x00010000, // 00000000'00000001'00000000'00000000
    date_time_epoch_us    = 0x00e00001, // 00000000'11100000'00000000'00000001
    date_time_epoch_ms    = 0x00e00002, // 00000000'11100000'00000000'00000010
    date_time_epoch_s     = 0x00e00004, // 00000000'11100000'00000000'00000100
    date_time_epoch_s_ms  = 0x00e00006, // 00000000'11100000'00000000'00000110
    date_time_ncsa        = 0x01c00001, // 00000001'11000000'00000000'00000001
    date_time_iso         = 0x00c00002, // 00000000'11000000'00000000'00000010
    date_time_gmt         = 0x01c00003, // 00000001'11000000'00000000'00000011
    date_time_mcs         = 0x01c00004, // 00000001'11000000'00000000'00000100
    date_time_utc_d       = 0x00d00002, // 00000000'11011000'00000000'00000001
    date_time_utc_t       = 0x00d00001, // 00000000'11010000'00000000'00000010
    date_time_yyyymmdd    = 0x00880001, // 00000000'10001000'00000000'00000001
    date_time_mmddyy      = 0x00880002, // 00000000'10001000'00000000'00000010
    date_time_mdyyyy      = 0x00880004, // 00000000'10001000'00000000'00000100
    date_time_year_short  = 0x00840001, // 00000000'10000100'00000000'00000001
    date_time_month_str   = 0x00820002, // 00000000'10000010'00000000'00000010
    date_time_year        = 0x80840001, // 10000000'10000100'00000000'00000001
    date_time_month       = 0x80820002, // 10000000'10000010'00000000'00000010
    date_time_day         = 0x80800004, // 10000000'10000000'00000000'00000100
    date_time_hour        = 0x80800008, // 10000000'10000000'00000000'00001000
    date_time_minute      = 0x80800010, // 10000000'10000000'00000000'00010000
    date_time_second      = 0x80800020, // 10000000'10000000'00000000'00100000
    date_time_clock_short = 0x00810018, // 00000000'10000001'00000000'00011000
    date_time_clock_12    = 0x00810038, // 00000000'10000001'00000000'00111000
    date_time_clock_24    = 0x008100b8, // 00000000'10000001'00000000'10111000
};


//! Holds informations about a log format
struct LogsFormat final
{
    explicit LogsFormat() noexcept = default;
    explicit LogsFormat
        (const std::string& str,std::string&& itl,std::string&& fnl,std::vector<std::string>&& seps,std::vector<LogsFormatField>&& flds,const unsigned nl) noexcept
        :string{str},initial{std::move(itl)},final{std::move(fnl)},separators{std::move(seps)},fields{std::move(flds)},new_lines{nl}{}
    std::string string;                  //!< The logs format string
    std::string initial;                 //!< The initial separator
    std::string final;                   //!< The final separator
    std::vector<std::string> separators; //!< The separators in the middle
    std::vector<LogsFormatField> fields; //!< The fields
    size_t new_lines;                    //!< The number of new lines
};


#endif // LOGDOCTOR__CRAPLOG__MODULES__LIB_H
