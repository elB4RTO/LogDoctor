
#include "rtf.h"


RichText::RichText()
{

}


QString RichText::enrichLogs(const std::string& content, const FormatOps::LogsFormat& logs_format, const TextBrowser &TB )
{
    std::unordered_map<std::string, QString> colors = TB.getColorScheme();
    int color_scheme = TB.getColorSchemeID();
    bool wide_lines = TB.getWideLinesUsage();
    // enrich the text
    QString rich_content = QString("<!DOCTYPE html><html><head></head><body");
    if ( color_scheme > 0 ) {
        rich_content += " style=\"background:" + colors.at("background") + "; color:" + colors.at("text") + "\"";
    }
    rich_content += ">";
    if ( wide_lines == true ) {
        rich_content += "<br/>";
    }
    QString rich_line="", class_name="";
    std::string sep, fld, fld_str, aux_sep1, aux_sep2, aux_fld_str;
    bool missing=false;
    int start, stop=0, i, aux_start1, aux_start2, aux_stop,
        line_size, sep_size,
        n_sep = logs_format.separators.size()-1;
    for ( const std::string& line : StringOps::splitrip( content ) ) {
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
                if ( sep == "" ) {
                    stop = line_size+1;
                } else {
                    stop = line.find( sep, start );
                    if ( stop > line_size+1 || stop < 0 ) {
                        stop = line_size +1;
                    }
                }
            } else {
                // no more separators
                break;
            }
            if ( stop > line_size+1 || stop < 0 ) {
                // separator not found, skip to the next one
                i++;
                stop = start;
                continue;
            }
            if ( i+1 <= n_sep ) {

                // tricky error messages from apache
                aux_fld_str = StringOps::strip( line.substr(start, stop-start), " " );
                if ( StringOps::startsWith( aux_fld_str, "AH0"  )
                  || StringOps::startsWith( aux_fld_str, "PHP " ) ) {
                    if ( logs_format.fields.at( i ) != "error_message" ) {
                        // this field is missing, step to the right field
                        for ( int j=i+1; j<logs_format.fields.size(); j++ ) {
                            if ( logs_format.fields.at( j ) == "error_message" ) {
                                i = j;
                                break;
                            }
                        }
                    }
                    // correct field, check if the next separators has columns. in case, update the stop position
                    int c_count = StringOps::count( aux_fld_str, ": " ),
                        c_max = 2;
                    if ( StringOps::startsWith( aux_fld_str, "AH00171" )
                      || StringOps::startsWith( aux_fld_str, "AH00163" ) ) {
                        c_max = 1;
                    }
                    if ( c_count < c_max ) {
                        // supposed to contain at least 2 columns
                        aux_stop = stop;
                        int j;
                        for ( j=0; j<c_count; j++ ) {
                            aux_stop = line.find( ": ", aux_stop );
                            if ( aux_stop != 0 || aux_stop >= line_size ) {
                                // column not not found
                                break;
                            } else {
                                // one_setp_further
                                aux_stop ++;
                            }
                        }
                        // finally update the stop to the next sep
                        if ( i <= n_sep ) {
                            sep = logs_format.separators.at( i );
                            stop = line.find( sep, aux_stop );
                        } else if ( i == n_sep+1 ) {
                            // final separator
                            sep = logs_format.final;
                            if ( sep == "" ) {
                                stop = line_size+1;
                            } else {
                                stop = line.find( sep, aux_stop );
                                if ( stop > line_size+1 || stop < 0 ) {
                                    stop = line_size +1;
                                }
                            }
                        }
                    }

                } else {

                    // not the last separator, check the possibility of missing
                    aux_sep1 = sep;
                    aux_start1 = aux_sep1.find(' ');
                    if ( aux_start1 >= 0 && aux_start1 < aux_sep1.size() ) {
                        aux_sep1 = StringOps::lstripUntil( aux_sep1, " " );
                    }
                    // iterate over following separators
                    for ( int j=i+1; j<n_sep; j++ ) {
                        aux_sep2 = logs_format.separators.at( j );
                        aux_start2 = aux_sep2.find(' ');
                        if ( aux_start2 > aux_sep2.size() || aux_start2 < 0 ) {
                            aux_start2 = stop;
                        } else {
                            aux_start2 = stop + aux_start2 + 1;
                            aux_sep2 = StringOps::lstripUntil( aux_sep2, " " );
                        }
                        // if the 2 seps are identical, skip (for uncertainty)
                        if ( aux_sep1 == aux_sep2 || aux_sep2 == "" ) {
                            continue;
                        }
                        // check if the next sep is found in the same position of the current one
                        if ( line.find( aux_sep2, aux_start2 ) == aux_start2 ) {
                            // probably the current field is missing, skip to this one
                            i = j;
                            aux_stop = aux_start2 + aux_sep2.size();
                            aux_sep2 = logs_format.separators.at( j );
                            missing = true;
                        }
                        break;
                    }
                }
            }

            sep_size = sep.size(); // do not remove, holdss the corretc size to increase stop
            // color the fields
            fld = logs_format.fields.at( i );
            fld_str = line.substr(start, stop-start);
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
                } else if ( fld == "user_agent" || fld == "source_file" ) {
                    class_name += colors.at("ua_src");
                } else if ( StringOps::startsWith( fld, "request" ) || fld == "error_message" ) {
                    class_name += colors.at("req_err");
                } else if ( fld == "response_code" || fld == "error_level" ) {
                    class_name += colors.at("res_lev");
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
        rich_line.clear();
    }
    rich_content.push_back("</body></html>");
    return rich_content;
}




QString RichText::richLogsDefault()
{
    return QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }hr { height: 1px; border-width: 0; }</style></head><body style=\" font-family:'Noto Sans'; font-size:16pt; font-weight:400; font-style:normal;\"><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; color:#6d6d6d;\">Select a file from the list</span></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; color:#6d6d6d;\">to inspect its content</span></p></body></html>");
}


QString RichText::richLogsFailure()
{
    return QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }hr { height: 1px; border-width: 0; }</style></head><body style=\" font-family:'Noto Sans'; font-size:16pt; font-weight:400; font-style:normal;\"><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:13pt;\"><br /></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; color:#aa0000;\">Failed to read</span></p></body></html>");
}
