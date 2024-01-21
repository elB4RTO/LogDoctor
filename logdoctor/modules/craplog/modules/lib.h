#ifndef LOGDOCTOR__CRAPLOG__LIB_H
#define LOGDOCTOR__CRAPLOG__LIB_H


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
    : __int8 {
#else
    : __INT8_TYPE__ {
#endif
    Failed    = -1, //!< Failed to determine the type
    Discarded =  0, //!< Not a valid file, will be discarded
    Access    =  1  //!< Valid access logs file type
};


//! Holds informations about a log file
struct LogFile final {
    explicit LogFile() noexcept = default;
    explicit LogFile
        (const bool sel,const bool used,const size_t sz,const QString& nm,const std::string& hs,const std::string& pt) noexcept
        :selected{sel},used_already{used},size_{sz},name_{nm},hash_{hs},path_{pt}{}
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


//! Holds informations about a log format
struct LogsFormat final {
    explicit LogsFormat() noexcept = default;
    explicit LogsFormat
        (const std::string& str,std::string&& itl,std::string&& fnl,std::vector<std::string>&& seps,std::vector<std::string>&& flds,const unsigned nl) noexcept
        :string{str},initial{std::move(itl)},final{std::move(fnl)},separators{std::move(seps)},fields{std::move(flds)},new_lines{nl}{}
    std::string string;                  //!< The logs format string
    std::string initial;                 //!< The initial separator
    std::string final;                   //!< The final separator
    std::vector<std::string> separators; //!< The separators in the middle
    std::vector<std::string> fields;     //!< The fields
    size_t new_lines;                    //!< The number of new lines
};


//! Hold the items of a blacklist/warnlist
struct BWlist final {
    bool used;                     //!< Whether the list is set to be used or not
    std::vector<std::string> list; //!< The list of items
};


#endif // LOGDOCTOR__CRAPLOG__LIB_H
