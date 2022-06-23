#ifndef RTF_H
#define RTF_H

#include "qfont.h"
#include "qstring.h"

class RichText
{
public:
    RichText();
    
    static QString enrichLogs( std::string content, int color_scheme=0 );
};

#endif // RICHTEXT_H
