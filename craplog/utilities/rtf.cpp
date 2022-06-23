#include "rtf.h"

RichText::RichText()
{
    
}


QString RichText::enrichLogs( std::string content, Craplog::LogsFormat logs_format, int color_scheme )
{
    QString style;
    switch ( color_scheme ) {
        case 1:
            // breeze
            style = QString("<style>"
                    "body{background-color:#ffffff}"
                    "p{color:#9c9c9b}"
                    "span.ip{color:#644a9b}"
                    "span.pt{color:#d5bc79}"
                    "span.date{color:#d685d6}"
                    "span.ua{color:#006e28}"
                    "span.req_err{color:#54b8ff}"
                    "span.res_lev{color:#d24f4f}"
                    "<style/>");
        case 2:
            // monokai
            style = QString("<style>"
                    "body{background-color:#272822}"
                    "p{color:#706c5a}"
                    "span.ip{color:#57adbc}"
                    "span.pt{color:#c1b864}"
                    "span.date{color:#9773db}"
                    "span.ua{color:#a6e22e}"
                    "span.req_err{color:#bebeb8}"
                    "span.res_lev{color:#f92672}"
                    "<style/>");
        case 3:
            // radical
            style = QString("<style>"
                    "body{background-color:#141322}"
                    "p{color:#749295}"
                    "span.ip{color:#fda8bc}"
                    "span.pt{color:#ff85a1}"
                    "span.date{color:#7c9c9e}"
                    "span.ua{color:#42a784}"
                    "span.req_err{color:#d5358f}"
                    "span.res_lev{color:#56e8e4}"
                    "<style/>");
        default:
            style = "";
    }

    QString rich_content = QString("<html><head>%1<head/><body>")
        .arg( style );
    for ( std::string& line : StringOp::splitrip( content ) ) {
        if ( line[0] == '[' ) {
            // error logs
        }
    }
    return rich_content;
}
