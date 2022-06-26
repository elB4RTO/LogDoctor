
#include "rtf.h"


RichText::RichText()
{

}


QString RichText::enrichLogs( std::string content, int logs_format, int color_scheme, bool wide_lines )
{

    std::unordered_map<std::string, QString> style;
    switch ( color_scheme ) {
        case 1:
            // breeze
            style.emplace("background","#ffffff");
            style.emplace("text","#9c9c9b");
            style.emplace("x","#1f1c1b");
            style.emplace("ip","#644a9b");
            style.emplace("pt","#d5bc79");
            style.emplace("time","#d685d6");
            style.emplace("ua_src","#006e28");
            style.emplace("req_err","#54b8ff");
            style.emplace("res_lev","#d24f4f");
            break;
        case 2:
            // monokai
            style.emplace("background","#272822");
            style.emplace("text","#706c5a");
            style.emplace("x","#d1d1cb");
            style.emplace("ip","#57adbc");
            style.emplace("pt","#c1b864");
            style.emplace("time","#9773db");
            style.emplace("ua_src","#a6e22e");
            style.emplace("req_err","#bebeb8");
            style.emplace("res_lev","#f92672");
            break;
        case 3:
            // radical
            style.emplace("background","#141322");
            style.emplace("text","#749295");
            style.emplace("x","#7c9c9e");
            style.emplace("ip","#fda8bc");
            style.emplace("pt","#ff85a1");
            style.emplace("time","#a8c0c2");
            style.emplace("ua_src","#42a784");
            style.emplace("req_err","#d5358f");
            style.emplace("res_lev","#56e8e4");
            break;
        default:
            // no style
            break;
    }
    QString rich_content = QString("<!DOCTYPE html><html><head></head><body");
    if ( color_scheme > 0 ) {
        rich_content += " style=\"background:" + style["background"] + "; color:" + style["text"] + "\"";
    }
    rich_content += ">";
    if ( wide_lines == true ) {
        rich_content += "<br/>";
    }
    QString rich_line="", class_name="";
    std::string sep, fld;
    int start=0, stop=0, i=0;
    int line_size;
    int n_sep = format.separators.size()-1;
    for ( std::string& line : StringOps::splitrip( content ) ) {
        i = 0;
        line_size = line.size()-1;
        rich_line = "<p>";
        // add the initial chars
        stop = format.initial.size();
        rich_line += QString::fromStdString( format.initial );
        while (true) {
            // color fields
            if ( i <= n_sep ) {
                sep = format.separators[i];
            } else if ( i == n_sep+1 ) {
                // final separator
                sep = format.final;
            } else {
                // no more separators
                break;
            }
            start = stop; // stop updated at the end of this loop
            stop = line.find( sep, start );
            if ( stop > line_size || stop < 0 ) {
                // separator not found, skip to the next one
                i++;
                stop = start;
                continue;
            }
            // color the fields
            rich_line += "<b>";
            class_name = "";
            if ( color_scheme > 0 ) {
                fld = format.fields[i];
                class_name += "<span style=\"color:";
                if ( fld == "client" ) {
                    class_name += style["ip"];
                } else if ( fld == "port" ) {
                    class_name += style["pt"];
                } else if ( fld == "date_time" ) {
                    class_name += style["time"];
                } else if ( fld == "user_agent" || fld == "source_file" ) {
                    class_name += style["ua_src"];
                } else if ( fld == "request" || fld == "error_message" ) {
                    class_name += style["req_err"];
                } else if ( fld == "response_code" || fld == "error_level" ) {
                    class_name += style["res_lev"];
                } else {
                    class_name += style["x"];
                }
                class_name += "\">";
            }
            // add the class name as span
            rich_line += class_name;
            rich_line += QString::fromStdString( line.substr(start, stop-start) );
            if ( color_scheme > 0 ) {
                rich_line += "</span>";
            }
            rich_line += "</b>";
            // update the stop for the next start
            stop = stop + sep.size();
            if ( stop > line_size ) {
                // this was the final separator
                rich_line += QString::fromStdString( format.final );
                break;
            }
            rich_line += QString::fromStdString( sep );
            i++;
        }
        rich_line += "</p>";
        if ( wide_lines == true ) {
            rich_line += "<br/>";
        }
        rich_content.push_back( rich_line );
        rich_line.clear();
    }
    rich_content.push_back("</body></html>");
    return rich_content;
}
