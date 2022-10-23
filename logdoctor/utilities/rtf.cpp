
#include "rtf.h"


RichText::RichText()
{

}


void RichText::enrichLogs( QString &rich_content, const std::string& content, const FormatOps::LogsFormat& logs_format, TextBrowser& TB )
{
    const std::unordered_map<std::string, QString>& colors = TB.getColorScheme();
    int color_scheme = TB.getColorSchemeID();
    bool wide_lines = TB.getWideLinesUsage();
    // enrich the text
    rich_content.clear();
    rich_content.reserve( content.size() );
    rich_content += QString("<!DOCTYPE html><html><head></head><body");
    if ( color_scheme > 0 ) {
        rich_content += " style=\"background:" + colors.at("background") + "; color:" + colors.at("text") + "\"";
    }
    rich_content += ">";
    if ( wide_lines == true ) {
        rich_content += "<br/>";
    }
    QString rich_line="", class_name="";
    std::string sep, fld, fld_str, aux_sep2;
    bool missing=false;
    size_t start, stop, i, aux_start, aux_stop,
           line_size, sep_size;
    const size_t n_sep = logs_format.separators.size()-1;
    std::vector<std::string> lines;
    StringOps::splitrip( lines, content );
    for ( const std::string& line : lines ) {
        // check if the line is commented, usually from IIS logs
        if ( StringOps::startsWith( line, "#" ) && !StringOps::startsWith( logs_format.initial, "#" ) ) {
            rich_line = QString("<p>%1</p>").arg( QString::fromStdString( line ) );
            if ( wide_lines == true ) {
                rich_line += "<br/>";
            }
            rich_content.push_back( rich_line );
            continue;
        }
        i = 0;
        line_size = line.size()-1;
        rich_line = "<p>";
        // add the initial chars
        stop = logs_format.initial.size();
        rich_line += QString::fromStdString( logs_format.initial );
        while (true) {
            // color fields
            start = stop; // stop updated at the end of this loop
            if ( i <= n_sep ) {
                sep = logs_format.separators.at( i );
                stop = line.find( sep, start );
            } else if ( i == n_sep+1 ) {
                // final separator
                sep = logs_format.final;
                if ( sep.size() == 0 ) {
                    stop = line_size+1;
                } else {
                    stop = line.find( sep, start );
                    if ( stop == std::string::npos ) {
                        stop = line_size +1;
                    }
                }
            } else {
                // no more separators
                break;
            }
            if ( stop == std::string::npos ) {
                // separator not found, skip to the next one
                i++;
                stop = start;
                continue;
            }

            // get fields
            fld = logs_format.fields.at( i );
            fld_str = line.substr(start, stop-start);

            if ( i+1 <= n_sep ) {
                // not the last separator, check for mistakes
                bool ok = true;
                aux_stop = stop;

                if ( sep == " " ) {
                    // whitespace-separated-values fields
                    int c = StringOps::count( fld_str, sep ),
                        n = 0;
                    if ( fld == "request_full" ) {
                        n = 2;
                    } else if ( fld == "date_time_mcs" ) {
                        n = 4;
                    } else if ( fld == "date_time_ncsa" ) {
                        n = 1;
                    } else if ( fld == "date_time_gmt" ) {
                        n = 3;
                    }
                    if ( n > 0 && c < n ) {
                        // loop until the correct number of whitespaces is reached
                        aux_start = stop + 1;
                        while ( c < n ) {
                            aux_stop = line.find( sep, aux_start );
                            if ( aux_stop == std::string::npos ) {
                                // not found
                                ok = false;
                                break;
                            }
                            aux_start = aux_stop + 1;
                            c++;
                        }
                    }

                } else if ( fld == "user_agent" && StringOps::startsWith( sep, "\"" ) ) {
                    // atm the only support is for escaped quotes
                    if ( fld_str.back() == '\\' ) {
                        aux_start = stop + sep.size();
                        while (true) {
                            aux_stop = line.find( sep, aux_start );
                            if ( aux_stop == std::string::npos ) {
                                // not found
                                break;
                            } else if ( line.at( aux_stop-1 ) != '\\' ) {
                                // non-backslashed quotes
                                break;
                            }
                            aux_start = aux_stop + sep.size();
                        }
                    }
                }

                // finally update if needed
                if ( ok == true && aux_stop >= stop ) {
                    stop = aux_stop;
                    fld_str = StringOps::strip( line.substr(start, stop-start), " " );
                }
            }

            sep_size = sep.size(); // do not remove, holdss the corretc size to increase stop
            // color the fields
            if ( StringOps::startsWith( fld, "date_time" ) ) {
                if ( StringOps::startsWith( fld_str, "[" ) ) {
                    fld_str = fld_str.substr( 1, fld_str.size()-1 );
                    rich_line += "[";
                    if ( StringOps::endsWith( fld_str, "]" ) ) {
                        fld_str = fld_str.substr( 0, fld_str.size()-1 );
                        sep = "]" + sep;
                    }
                }
            }
            rich_line += "<b>";
            class_name = "";
            if ( color_scheme > 0 ) {
                class_name += "<span style=\"color:";
                if ( fld == "client" ) {
                    class_name += colors.at("ip");
                } else if ( fld == "port" ) {
                    class_name += colors.at("pt");
                } else if ( StringOps::startsWith( fld, "date_time" ) ) {
                    class_name += colors.at("time");
                } else if ( fld == "user_agent" ) {
                    class_name += colors.at("ua");
                } else if ( StringOps::startsWith( fld, "request" ) ) {
                    class_name += colors.at("req");
                } else if ( fld == "response_code" ) {
                    class_name += colors.at("res");
                } else {
                    class_name += colors.at("x");
                }
                class_name += "\">";
            }
            // add the class name as span
            rich_line += class_name;
            rich_line += QString::fromStdString( fld_str );
            if ( color_scheme > 0 ) {
                rich_line += "</span>";
            }
            rich_line += "</b>";
            // update the stop for the next start
            if ( missing == true ) {
                stop = aux_stop;
            } else {
                stop += sep_size;
                i++;
            }
            if ( stop > line_size ) {
                // this was the final separator
                rich_line += QString::fromStdString( logs_format.final );
                break;
            }
            if ( missing == true ) {
                missing = false;
                rich_line += QString::fromStdString( aux_sep2 );
            } else {
                rich_line += QString::fromStdString( sep );
            }
        }
        rich_line += "</p>";
        if ( wide_lines == true ) {
            rich_line += "<br/>";
        }
        rich_content.push_back( rich_line );
    }
    lines.clear();
    rich_content.push_back("</body></html>");
}



void RichText::richLogsDefault( QString& rich_str )
{
    rich_str.clear();
    rich_str += QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }hr { height: 1px; border-width: 0; }</style></head><body style=\" font-family:'Noto Sans'; font-size:16pt; font-weight:400; font-style:normal;\"><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; color:#6d6d6d;\">%1</span></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; color:#6d6d6d;\">%2</span></p></body></html>")
        .arg( RichText::tr("Select a file from the list"),
              RichText::tr("to inspect its content") );
}


void RichText::richLogsFailure( QString &rich_str )
{
    rich_str.clear();
    rich_str += QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }hr { height: 1px; border-width: 0; }</style></head><body style=\" font-family:'Noto Sans'; font-size:16pt; font-weight:400; font-style:normal;\"><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; color:#aa0000;\">%1</span></p></body></html>")
        .arg( RichText::tr("Failed to read") );
}
