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
class LogFile {
public:
    explicit LogFile(){}
    explicit LogFile
        (const bool sel,const bool used,const size_t sz,const QString& nm,const std::string& hs,const std::string& pt)
        :selected{sel},used_already{used},size_{sz},name_{nm},hash_{hs},path_{pt}{}
    bool isSelected() const   //!< Wheter the file has been selected to be use or not
        { return this->selected; }
    void setSelected()              //!< Sets the file as selected
        { this->selected |= true; }
    void setUnselected()            //!< Sets the file as unselected
        { this->selected &= false; }
    bool hasBeenUsed() const  //!< Wheter the file has been used already or not
        { return this->used_already; }
    size_t size() const     //!< The size of the file
        { return this->size_; }
    const QString& name() const     //!< The name of the file, to be displayed in the list
        { return this->name_; }
    const std::string& hash() const //!< The sha256 hash of the content
        { return this->hash_; }
    const std::string& path() const //!< The path of the file, including the file name
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
class LogsFormat {
public:
    explicit LogsFormat(){}
    explicit LogsFormat
        (const std::string& str,const std::string& itl,const std::string& fnl,const std::vector<std::string>& seps,const std::vector<std::string>& flds,const unsigned nl)
        :string{str},initial{itl},final{fnl},separators{seps},fields{flds},new_lines{nl}{}
    std::string string;                  //!< The logs format string
    std::string initial;                 //!< The initial separator
    std::string final;                   //!< The final separator
    std::vector<std::string> separators; //!< The separators in the middle
    std::vector<std::string> fields;     //!< The fields
    size_t new_lines;                    //!< The number of new lines
};


//! Hold the items of a blacklist/warnlist
struct BWlist {
    bool used;                     //!< Whether the list is set to be used or not
    std::vector<std::string> list; //!< The list of items
};


#endif // LOGDOCTOR__CRAPLOG__LIB_H
