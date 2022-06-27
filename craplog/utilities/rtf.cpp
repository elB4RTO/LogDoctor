
#include "rtf.h"

using std::string, std::unordered_map;


RichText::RichText()
{

}


QString RichText::enrichLogs( string content, FormatOps::LogsFormat logs_format, TextBrowser TB )
{
    std::unordered_map<std::string, QString> colors = TB.getColorScheme();
    int color_scheme = TB.getColorSchemeID();
    bool wide_lines = TB.getWideLinesUsage();
    // enrich the text
    QString rich_content = QString("<!DOCTYPE html><html><head></head><body");
    if ( color_scheme > 0 ) {
        rich_content += " style=\"background:" + colors["background"] + "; color:" + colors["text"] + "\"";
    }
    rich_content += ">";
    if ( wide_lines == true ) {
        rich_content += "<br/>";
    }
    QString rich_line="", class_name="";
    string sep, fld;
    int start=0, stop=0, i=0;
    int line_size;
    int n_sep = logs_format.separators.size()-1;
    for ( string& line : StringOps::splitrip( content ) ) {
        i = 0;
        line_size = line.size()-1;
        rich_line = "<p>";
        // add the initial chars
        stop = logs_format.initial.size();
        rich_line += QString::fromStdString( logs_format.initial );
        while (true) {
            // color fields
            if ( i <= n_sep ) {
                sep = logs_format.separators[i];
            } else if ( i == n_sep+1 ) {
                // final separator
                sep = logs_format.final;
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
            if ( i+1 < logs_format.separators.size() ) {
                // not the last separator, check the possibility of missing
                if ( line.find( logs_format.separators[i+1], start-1 ) == start
                  && logs_format.separators[i+1] != logs_format.separators[i] ) {
                    // current field missing, skip to the next one
                    i++;
                    stop = start;
                    continue;
                }
            }
            // color the fields
            rich_line += "<b>";
            class_name = "";
            if ( color_scheme > 0 ) {
                fld = logs_format.fields[i];
                class_name += "<span style=\"color:";
                if ( fld == "client" ) {
                    class_name += colors["ip"];
                } else if ( fld == "port" ) {
                    class_name += colors["pt"];
                } else if ( fld == "date_time" ) {
                    class_name += colors["time"];
                } else if ( fld == "user_agent" || fld == "source_file" ) {
                    class_name += colors["ua_src"];
                } else if ( fld == "request" || fld == "error_message" ) {
                    class_name += colors["req_err"];
                } else if ( fld == "response_code" || fld == "error_level" ) {
                    class_name += colors["res_lev"];
                } else {
                    class_name += colors["x"];
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
                rich_line += QString::fromStdString( logs_format.final );
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
