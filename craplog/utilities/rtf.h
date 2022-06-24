#ifndef RTF_H
#define RTF_H

#include "qfont.h"
#include "qstring.h"

#include "utilities/strings.h"
#include "tools/craplog.h"

class RichText
{
public:
    RichText();

    static QString enrichLogs( std::string content, Craplog::LogsFormat, int color_scheme );
};

#endif // RICHTEXT_H
