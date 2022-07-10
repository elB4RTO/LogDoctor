#ifndef RTF_H
#define RTF_H

#include "qfont.h"
#include "qstring.h"

#include "utilities/strings.h"

#include "modules/tb.h"

#include "tools/craplog/modules/formats.h"


class RichText
{
public:
    RichText();

    static const QString enrichLogs(
        const std::string& content,
        const FormatOps::LogsFormat& logs_format,
        const TextBrowser& TB );

    static const QString richLogsDefault();
    static const QString richLogsFailure();
};

#endif // RTF_H
