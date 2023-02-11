#ifndef RTF_H
#define RTF_H

#include <QFont>
#include <QString>

#include "modules/tb.h"
#include "modules/craplog/modules/formats.h"


//! RichText
/*!
    Utilities for the TextBrowser
*/

class RichText : public QObject
{
    Q_OBJECT

public:
    explicit RichText();

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
        TextBrowser& TB
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

#endif // RTF_H
