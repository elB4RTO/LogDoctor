#ifndef RTF_H
#define RTF_H

#include "qfont.h"
#include "qstring.h"

#include "utilities/strings.h"

class RichText
{
public:
    RichText();

    static QString enrichLogs( std::string content, int logs_format, int color_scheme, bool wide_lines );
};

#endif // RICHTEXT_H
