#ifndef LOGDOCTOR__UTILITIES__RTF_H
#define LOGDOCTOR__UTILITIES__RTF_H


#include <QObject>

#include <string>

struct LogsFormat;
class TextBrowser;
class QString;


//! RichText
/*!
    Utilities for the TextBrowser
*/
class RichText final : public QObject
{
    Q_OBJECT

public:

    //! Enriches the content of a log file with HTML/CSS code
    /*!
        \param rich_content Will hold the final enriched content
        \param content The plain text content from a log file
        \param logs_format The logs format to use
        \param TB The TextBrowser instance
    */
    static void enrichLogs(
        QString& rich_content,
        const std::string& content,
        const LogsFormat& logs_format,
        const TextBrowser& TB
    );

    //! Provides the default string
    /*!
        \param rich_str Will hold the string
    */
    static void richLogsDefault( QString& rich_str );

    //! Provides the failure string
    /*!
        \param rich_str Will hold the string
    */
    static void richLogsFailure( QString& rich_str );
};


#endif // LOGDOCTOR__UTILITIES__RTF_H
