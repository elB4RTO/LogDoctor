#ifndef RTF_H
#define RTF_H

#include "qfont.h"
#include "qstring.h"

#include "utilities/strings.h"

#include "tools/craplog/modules/formats.h"

class RichText
{
public:
    RichText();

    static QString enrichLogs( std::string content, FormatOps::LogsFormat logs_format, int color_scheme, bool wide_lines );
};

#endif // RTF_H
