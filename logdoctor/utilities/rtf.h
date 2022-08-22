#ifndef RTF_H
#define RTF_H

#include "qfont.h"
#include "qstring.h"

#include "utilities/strings.h"

#include "modules/tb.h"

#include "tools/craplog/modules/formats.h"


class RichText : public QObject
{
    Q_OBJECT

public:
    RichText();

    static void enrichLogs(
        QString& rich_content,
        const std::string& content,
        const FormatOps::LogsFormat& logs_format,
        TextBrowser& TB );

    static void richLogsDefault( QString& rich_str );
    static void richLogsFailure( QString& rich_str );
};

#endif // RTF_H
