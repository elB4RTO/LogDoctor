#include "rtf.h"

RichText::RichText()
{

}


QString RichText::enrichLogs( std::string content, Craplog::LogsFormat format, int color_scheme )
{
    QString style;
    switch ( color_scheme ) {
        case 1:
            // breeze
            style = QString("<style>"
                    "body{background-color:#ffffff}"
                    "p{color:#9c9c9b}"
                    "span.x{color:#1f1c1b}"
                    "span.ip{color:#644a9b}"
                    "span.pt{color:#d5bc79}"
                    "span.time{color:#d685d6}"
                    "span.ua_src{color:#006e28}"
                    "span.req_err{color:#54b8ff}"
                    "span.res_lev{color:#d24f4f}"
                    "/<style>");
        case 2:
            // monokai
            style = QString("<style>"
                    "body{background-color:#272822}"
                    "p{color:#706c5a}"
                    "span.x{color:#d1d1cb}"
                    "span.ip{color:#57adbc}"
                    "span.pt{color:#c1b864}"
                    "span.time{color:#9773db}"
                    "span.ua_src{color:#a6e22e}"
                    "span.req_err{color:#bebeb8}"
                    "span.res_lev{color:#f92672}"
                    "</style>");
        case 3:
            // radical
            style = QString("<style>"
                    "body{background-color:#141322}"
                    "p{color:#749295}"
                    "span.x{color:#7c9c9e}"
                    "span.ip{color:#fda8bc}"
                    "span.pt{color:#ff85a1}"
                    "span.time{color:#a8c0c2}"
                    "span.ua_src{color:#42a784}"
                    "span.req_err{color:#d5358f}"
                    "span.res_lev{color:#56e8e4}"
                    "</style>");
        default:
            style = "";
    }

    QString rich_content = QString("<!DOCTYPE html><html><head>%1</head><body>")
        .arg( style );
    QString rich_line="", class_name="";
    std::string sep, fld;
    int start=0, stop=0, i=0;
    int line_size;
    int n_sep = format.separators.size()-1;
    for ( std::string& line : StringOps::splitrip( content ) ) {
        i = 0;
        line_size = line.size()-1;
        rich_line += "<p>";
        // add the initial chars
        stop = format.initial.size();
        rich_line += QString::fromStdString( format.initial );
        while (true) {
            // color fields
            if ( i <= n_sep ) {
                sep = format.separators[i];
            } else {
                sep = format.final;
            }
            start = stop;
            stop = line.find( sep, start );
            if ( stop > line_size ) {
                // separator not found, skip to the next one
                i++;
                if ( i > n_sep+1 ) {
                    // break if no one left
                    break;
                }
                continue;
            }
            // color the fields
            class_name.clear();
            fld = format.fields[i];
            if ( color_scheme > 0 ) {
                class_name += "<span class=\"";
                if ( fld == "client" ) {
                    class_name += "ip";
                } else if ( fld == "port" ) {
                    class_name += "pt";
                } else if ( fld == "date_time" ) {
                    class_name += "time";
                } else if ( fld == "user_agent" || fld == "source_file" ) {
                    class_name += "ua_src";
                } else if ( fld == "request" || fld == "error_message" ) {
                    class_name = "req_err";
                } else if ( fld == "response_code" || fld == "error_level" ) {
                    class_name += "res_lev";
                } else {
                    class_name += "x";
                }
                class_name += "\">";
            }
            // add the class name as apan
            rich_line += QString::fromStdString( line.substr(start, stop-start) );
            if ( color_scheme > 0 ) {
                rich_line += "</span>";
            }
            // update the stop for the next start
            stop = stop + sep.size();
            if ( stop >= line_size ) {
                break;
            }
        }
        rich_line += "</p>";
        rich_content.push_back( rich_line );
        rich_line.clear();
    }
    rich_content.push_back("</body></html>");
    return rich_content;
}
