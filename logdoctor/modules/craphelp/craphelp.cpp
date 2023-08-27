
#include "craphelp.h"
#include "ui_craphelp.h"

#include "modules/exceptions.h"

#include "utilities/io.h"


Craphelp::Craphelp(QWidget *parent)
    : QWidget{ parent }
    , ui{ new Ui::Craphelp }
{
    ui->setupUi(this);
}


const std::unordered_map<std::string, QString> Craphelp::getColorScheme( const int scheme_id ) const
{
    switch ( scheme_id ) {
        case 0:
            // none
            return {
                {"background", ""},
                      {"text", ""},
                        {"h1", ""},
                        {"h3", ""},
                      {"code", ""},
                      {"link", ""} };
        case 1:
            // breeze
            return {
                {"background", "#ffffff"},
                      {"text", "#1f1c1b"},
                    {"italic", "#9c9c9b"},
                        {"h1", "#006e28"},
                        {"h3", "#54b8ff"},
                      {"code", "#644a9b"},
                      {"link", "#d24f4f"} };

        case 2:
            // monokai
            return {
                {"background", "#272822"},
                      {"text", "#d1d1cb"},
                    {"italic", "#c1b864"},
                        {"h1", "#a6e22e"},
                        {"h3", "#9773db"},
                      {"code", "#57adbc"},
                      {"link", "#f92672"} };

        case 3:
            // radical
            return {
                {"background", "#141322"},
                      {"text", "#a8c0c2"},
                    {"italic", "#fda8bc"},
                        {"h1", "#d5358f"},
                        {"h3", "#56e8e4"},
                      {"code", "#7c9c9e"},
                      {"link", "#ff85a1"} };

        default:
            // wrong, shouldn't be here
            throw GenericException( "Unexpected ColorScheme ID: "+std::to_string( scheme_id ), true ); // leave un-catched
    }
}


void Craphelp::helpLogsFormat( const std::string& path, const QFont& font, const int color_scheme_id ) const
{
    std::unordered_map<std::string, QString> color_scheme{ this->getColorScheme( color_scheme_id ) };
    std::string aux;
    IOutils::readFile( path, aux );
    QString content;
    if ( color_scheme_id == 0 ) {
        // remove the style for the colors
        while (true) {
            const size_t start{ aux.find( "background-color:" ) };
            if ( start == std::string::npos ) {
                break;
            }
            const size_t stop{ aux.find( ";\n", start ) + 2ul };
            aux.erase( start, stop-start );
        }
        while (true) {
            const size_t start{ aux.find( "color:" ) };
            if ( start == std::string::npos ) {
                break;
            }
            const size_t stop{ aux.find( ";\n", start ) + 2ul };
            aux.erase( start, stop-start );
        }
        content = QString::fromStdString( aux );

    } else {
        // replace with colors
        content = QString::fromStdString( aux )
            .replace( "$BG$",   color_scheme.at("background") )
            .replace( "$TEXT$", color_scheme.at("text") )
            .replace( "$IT$",   color_scheme.at("italic") )
            .replace( "$H1$",   color_scheme.at("h1") )
            .replace( "$H3$",   color_scheme.at("h3") )
            .replace( "$CODE$", color_scheme.at("code") )
            .replace( "$LINK$", color_scheme.at("link") );
    }
    // show the content
    this->ui->helpBrowser->setText( content );
    // apply the font
    this->ui->helpBrowser->setFont( font );
}


void Craphelp::helpLogsFormatDefault( std::string_view file_name, const QFont& font, const int color_scheme_id ) const
{
    std::unordered_map<std::string, QString> color_scheme = this->getColorScheme( color_scheme_id );
    std::string aux;
    if ( file_name == "apache_format" ) {
        this->defaultApacheFormat( aux );
    } else if ( file_name == "nginx_format" ) {
        this->defaultNginxFormat( aux );
    } else if ( file_name == "iis_format" ) {
        this->defaultIisFormat( aux );
    }
    QString content;
    if ( color_scheme_id == 0 ) {
        // remove the style for the colors
        while (true) {
            const size_t start{ aux.find( "background-color:" ) };
            if ( start == std::string::npos ) {
                break;
            }
            const size_t stop{ aux.find( ";", start ) + 1ul };
            aux.erase( start, stop-start );
        }
        while (true) {
            const size_t start{ aux.find( "color:" ) };
            if ( start == std::string::npos ) {
                break;
            }
            const size_t stop{ aux.find( ";", start ) + 1ul };
            aux.erase( start, stop-start );
        }
        content = QString::fromStdString( aux );

    } else {
        // replace with colors
        content = QString::fromStdString( aux )
            .replace( "$BG$",   color_scheme.at("background") )
            .replace( "$TEXT$", color_scheme.at("text") )
            .replace( "$IT$",   color_scheme.at("italic") )
            .replace( "$H1$",   color_scheme.at("h1") )
            .replace( "$H3$",   color_scheme.at("h3") )
            .replace( "$CODE$", color_scheme.at("code") )
            .replace( "$LINK$", color_scheme.at("link") );
    }
    // show the content
    this->ui->helpBrowser->setText( content );
    // apply the font
    this->ui->helpBrowser->setFont( font );
}



void Craphelp::defaultApacheFormat( std::string& str ) const
{
    str.append(
        "<!DOCTYPE html>"
        "<html lang=\"en_US\">"
        "  <head>"
        "    <meta charset=\"utf-8\">"
        "    <title>LogDoctor - Help</title>"
        "    <style type=\"text/css\">"
        "      body {"
        "        color: $TEXT$;"
        "        font-size: 19px;"
        "        background-color: $BG$;"
        "      }"
        "      h1 {"
        "        color: $H1$;"
        "      }"
        "      h2 {"
        "        color: $BG$;"
        "        background-color: $H1$;"
        "      }"
        "      h3 {"
        "        color: $H3$;"
        "      }"
        "      th,td {"
        "        padding: 20px;"
        "      }"
        "      code {"
        "        color: $CODE$;"
        "        font-size: 17px;"
        "        padding-left:16px"
        "      }"
        "      i {"
        "        color: $IT$;"
        "      }"
        "      a {"
        "        color: $LINK$;"
        "      }"
        "    </style>"
        "  </head>"
        "  <body>"
        "    <br>"
        "    <h1 align=\"center\">Apache2</h1>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h2 align=\"center\"><br>Access logs format string</h2>"
        "    <br>"
        "    <h3><br>Configuration file</h3>"
        "    <br>"
        "    <p>The configuration file should be located at:</p><br>"
        "      <code>/etc/apache2/apache2.conf</code>"
        "    <br>"
        "    <p>The line to configure access logs is the one starting with \"<b>LogFormat</b>\" followed by the list of fields codes.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Common logs formats</h3>"
        "    <br>"
        "    <p>Most commonly used format strings are:</p>"
        "    <br>"
        "    <ul>"
        "      <li style=\"padding-bottom:10px\">Common log format <i>(CLF)</i></li><br>"
        "        <code>LogFormat \"%h %l %u %t \\\"%r\\\" %>s %O\" common</code><br>"
        "      <br>"
        "      <li style=\"padding-bottom:10px\">Combined log format <i>(NCSA standard)</i></li><br>"
        "        <code>LogFormat \"%h %l %u %t \\\"%r\\\" %>s %O \\\"%{Referer}i\\\" \\\"%{User-agent}i\\\"\" combined</code>"
        "    </ul>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Suggested logs formats</h3>"
        "    <br>"
        "    <p>A suggested format string, to allow using the complete set of functionalities of LogDoctor, is:</p><br>"
        "      <code>LogFormat \"%{%F %T}t %H %m %U %q %&gt;s %I %O %D \\\"%{Referer}i\\\" \\\"%{Cookie}i\\\" \\\"%{User-agent}i\\\" %{c}h\" combined</code>"
        "    <br>"
        "    <p>The string above should be preferred, but alternatives can be used as well, like:</p><br>"
        "      <code>LogFormat \"%{sec}t \\\"%r\\\" %q %&lt;s %I %O %D \\\"%{Referer}i\\\" \\\"%{Cookie}i\\\" \\\"%{User-agent}i\\\" %h\" combined</code>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Note on custom format strings</h3>"
        "    <br>"
        "    <p>If you're using your own custom string, please keep in mind that parsing is not magic. When you define your own string, think about which characters can be there in a field and use separators accordingly to not conflict with the field itself.<br>"
        "    As an example: an URI (<i>%U</i>) can't contain whitespaces, so it is safe to use a space to separe this field by the previous and next one. Instead, the User-Agent (*%{User-agent}i*) may contain spaces, as well as parenthesis, brackets, dashes, etc, so it's better to pick an appropriate separator (double-quotes are a good choice, since they get escaped while logging).</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Note on control-characters</h3>"
        "    <br>"
        "    <p>Although Apache2 does support some control-characters (<i>aka</i> escape sequences), it is reccomended to <b>not use them</b> inside format strings.<br>"
        "    In particular, the <i>carriage return</i> will most-likely overwrite previous fields data, making it very difficult to understand where the current field ends (specially for fields like URIs, queries, user-agents, etc) and nearly impossible to retrieve the overwritten data, which will lead in having a wasted database, un-realistic statistics and/or crashes during execution.<br>"
        "    About the <i>new line</i> character, it has no sense to use it, if not for testing purposes. The same is true for the <i>horizontal tab</i>, for which is better to use a simple whitespace instead.<br>"
        "    The only control-characters supported by Apache2 are <b>\n</b>, <b>\t</b> and <b>\r</b>. Any other character will be ignored and treated as text.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h2 align=\"center\"><br>Access logs format fields</h2>"
        "    <br>"
        "    <h3><br>Fields considered by LogDoctor</h3>"
        "    <br>"
        "    <p>Only the following fields will be considered, meaning that only these fields' data will be stored and used for the statistics.</p>"
        "    <br>"
        "    <table border=\"1\" width=\"100%\">"
        "      <tr><th style=\"width:150px\">Code</th><th>Informations</th></tr>"
        "      <tr>"
        "        <td align=\"center\">%%</td>"
        "        <td>The percent sign character, will result in a single percent sign and treated as normal text (from both Apache and LogDoctor).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%t</td>"
        "        <td>Time the request was received, in the format <i>[DD/Mon/YYYY:hh:mm:ss ±TZ]</i>. The last number <i>(TZ)</i> indicates the timezone offset from GMT.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%{<i>FORMAT</i>}t</td>"
        "        <td>Time the request was received, in the form given by FORMAT, which should be in an extended <b>strftime</b> format.<br>"
        "        The following format tokens are supported (by LogDoctor, any other than the following will be discarded, even if valid):"
        "        <br>"
        "        <table border=\"1\" style=\"padding:16px\">"
        "          <tr><th style=\"width:100px; padding:10px\">Format</th><th style=\"padding:10px\">Description</th></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">sec</td><td style=\"padding:10px\">time since epoch, in seconds</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">msec</td><td style=\"padding:10px\">time since epoch, in milliseconds</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">usec</td><td style=\"padding:10px\">time since epoch, in microseconds</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%b</td><td style=\"padding:10px\">month name, abbreviated (same as <i>%h</i>)</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%B</td><td style=\"padding:10px\">month name</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%c</td><td style=\"padding:10px\">date and time representation</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%d</td><td style=\"padding:10px\">day number, zero padded</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%D</td><td style=\"padding:10px\">date, in the form of <i>MM/DD/YY</i></td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%e</td><td style=\"padding:10px\">day number, space padded</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%F</td><td style=\"padding:10px\">date, in the form of <i>YYYY/MM/DD</i></td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%h</td><td style=\"padding:10px\">month name, abbreviated (same as <i>%b</i>)</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%H</td><td style=\"padding:10px\">hour, in <i>24h</i> format, zero padded</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%m</td><td style=\"padding:10px\">month number, zero padded</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%M</td><td style=\"padding:10px\">minute</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%r</td><td style=\"padding:10px\">time if the day, in <i>12h</i> format, in the form of <i>HH:MM:SS AM/PM</i></td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%R</td><td style=\"padding:10px\">time of the day, in <i>HH:MM</i> format</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%S</td><td style=\"padding:10px\">second</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%T</td><td style=\"padding:10px\">ISO 8601 time, in the form of <i>HH:MM:SS</i></td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%x</td><td style=\"padding:10px\">date representation</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%X</td><td style=\"padding:10px\">time representation</td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%y</td><td style=\"padding:10px\">year, last two digits <i>(YY)</i></td></tr>"
        "          <tr><td align=\"center\" style=\"padding:10px\">%Y</td><td style=\"padding:10px\">year</td></tr>"
        "        </table>"
        "        <br>"
        "        <br>"
        "        <i>Note:</i> time formats <i>sec</i>, <i>msec</i> and <i>usec</i> can't be mixed together or with other formats."
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%r</td>"
        "        <td>First line of request, equivalent to: <i>%m %U?%q %H</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%H</td>"
        "        <td>The request protocol <i>(HTTP/v, HTTPS/v)</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%m</td>"
        "        <td>The request method <i>(GET, POST, HEAD, ...)</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%U</td>"
        "        <td>The URI path requested, not including any query string.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%q</td>"
        "        <td>Query string (if any).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%s</td>"
        "        <td>HTTP Status code at the beginning of the request (exclude redirections statuses).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%>s</td>"
        "        <td>Final HTTP Status code (in case requests have been internally redirected).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%I</td>"
        "        <td>Bytes received, including request and headers (you need to enable mod_logio to use this).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%O</td>"
        "        <td>Bytes sent, including headers (you need to enable mod_logio to use this).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%T</td>"
        "        <td>The time taken to serve the request, in seconds.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%{<i>UNIT</i>}T</td>"
        "        <td>The time taken to serve the request, in a time unit given by UNIT (only available in 2.4.13 and later).<br>"
        "        Valid units are:"
        "        <br>"
        "          <table border=\"1\" style=\"padding:16px\">"
        "            <tr><th style=\"width:100px; padding:10px\">Unit</th><th style=\"padding:10px\">Description</th></tr>"
        "            <tr><td align=\"center\" style=\"padding:10px\">s</td><td style=\"padding:10px\">seconds</td></tr>"
        "            <tr><td align=\"center\" style=\"padding:10px\">ms</td><td style=\"padding:10px\">milliseconds</td></tr>"
        "            <tr><td align=\"center\" style=\"padding:10px\">us</td><td style=\"padding:10px\">microseconds</td></tr>"
        "          </table>"
        "        </td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%D</td>"
        "        <td>The time taken to serve the request, in milliseconds.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%h</td>"
        "        <td>IP Address of the client (remote hostname).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%{c}h</td>"
        "        <td>Like %h, but always reports on the hostname of the underlying TCP connection and not any modifications to the remote hostname by modules like mod_remoteip.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">%{<i>VARNAME</i>}i</td>"
        "        <td>The contents of VARNAME: header line(s) in the request sent to the server.<br>"
        "        Supported varnames (by LogDoctor) are:"
        "        <br>"
        "          <table border=\"1\" style=\"padding:16px\">"
        "            <tr><th style=\"width:150px; padding:10px\">VarName</th><th style=\"padding:10px\">Description</th></tr>"
        "            <tr><td align=\"center\" style=\"padding:10px\">Cookie</td><td style=\"padding:10px\">cookie of the request</td></tr>"
        "            <tr><td align=\"center\" style=\"padding:10px\">Referer</td><td style=\"padding:10px\">referrer host</td></tr>"
        "            <tr><td align=\"center\" style=\"padding:10px\">User-agent</td><td style=\"padding:10px\">web-browser or bot identification string</td></tr>"
        "          </table>"
        "        </td>"
        "      </tr>"
        "    </table>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Fields discarded by LogDoctor</h3>"
        "    <br>"
        "    <p>Any field than the ones above won't be considered by LogDoctor.<br>"
        "    When generating a <i>log sample</i>, these fields will appear as '<b>DISCARDED</b>'.<br>"
        "    If you aint using logs for any other purpose, please remove unnecessary fields to make the process faster and reduce the possibility of errors.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>References</h3>"
        "    <br>"
        "    <ul>"
        "      <li>Complete list of Apache2 access logs formats:<br>"
        "        <a>https://httpd.apache.org/docs/2.4/mod/mod_log_config.html</a></li><br>"
        "      <li>Complete list of strftime formats:<br>"
        "        <a>https://cplusplus.com/reference/ctime/strftime/</a></li>"
        "    </ul>"
        "    <br>"
        "    <br>"
        "    <br>"
        "  </body>"
        "</html>"
    );
}

void Craphelp::defaultNginxFormat( std::string& str ) const
{
    str.append(
        "<!DOCTYPE html>"
        "<html lang=\"en_US\">"
        "  <head>"
        "    <meta charset=\"utf-8\">"
        "    <title>LogDoctor - Help</title>"
        "    <style type=\"text/css\">"
        "      body {"
        "        color: $TEXT$;"
        "        font-size: 19px;"
        "        background-color: $BG$;"
        "      }"
        "      h1 {"
        "        color: $H1$;"
        "      }"
        "      h2 {"
        "        color: $BG$;"
        "        background-color: $H1$;"
        "      }"
        "      h3 {"
        "        color: $H3$;"
        "      }"
        "      th,td {"
        "        padding: 20px;"
        "      }"
        "      code {"
        "        color: $CODE$;"
        "        font-size: 17px;"
        "        padding-left:16px"
        "      }"
        "      i {"
        "        color: $IT$;"
        "      }"
        "      a {"
        "        color: $LINK$;"
        "      }"
        "    </style>"
        "  </head>"
        "  <body>"
        "    <br>"
        "    <h1 align=\"center\">Nginx</h1>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h2 align=\"center\"><br>Access logs format string</h2>"
        "    <br>"
        "    <h3><br>Configuration file</h3>"
        "    <br>"
        "    <p>The configuration file should be located at:</p><br>"
        "      <code>/usr/local/etc/nginx/nginx.conf</code>"
        "    <br>"
        "    <p>The line to configure access logs is the one starting with \"<b>log_format main</b>\" followed by the list of fields codes.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Default logs formats</h3>"
        "    <br>"
        "    <p>The default logs format string is:</p>"
        "    <br>"
        "    <code>log_format main '$remote_addr - $remote_user [$time_local] \"$request\" $status $bytes_sent \"$http_referer\" \"$http_user_agent\"'</code>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Suggested logs format</h3>"
        "    <br>"
        "    <p>The suggested format string, to allow using the most of the functionalities of LogDoctor, is:</p><br>"
        "      <code>log_format main '$time_iso8601 \"$request\" $status $request_length $bytes_sent $request_time \"$http_referer\" \"$http_user_agent\" $remote_addr'</code>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Note the format strings</h3>"
        "    <br>"
        "    <p>When copy-pasting the format string, please remove any identation (if present) and just paste the resulting format string only.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Note on control-characters</h3>"
        "    <br>"
        "    <p>Although Nginx does support some control-characters (<i>aka</i> escape sequences), it is reccomended to <b>not use them</b> inside format strings.<br>"
        "    In particular, the <i>carriage return</i> will most-likely overwrite previous fields data, making it very difficult to understand where the current field ends (specially for fields like URIs, queries, user-agents, etc) and nearly impossible to retrieve the overwritten data, which will lead in having a wasted database, un-realistic statistics and/or crashes during execution.<br>"
        "    About the <i>new line</i> character, it has no sense to use it, if not for testing purposes. The same is true for the <i>horizontal tab</i>, for which is better to use a simple whitespace instead.<br>"
        "    The only control-characters supported by Nginx are <b>\n</b>, <b>\t</b> and <b>\r</b>. Any other character will be ignored and treated as text.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h2 align=\"center\"><br>Access logs format fields</h2>"
        "    <br>"
        "    <h3><br>Fields considered by LogDoctor</h3>"
        "    <br>"
        "    <p>Only the following fields will be considered, meaning that only these fields' data will be stored and used for the statistics.</p>"
        "    <br>"
        "    <table border=\"1\" width=\"100%\">"
        "      <tr><th style=\"width:150px\">Code</th><th>Informations</th></tr>"
        "      <tr>"
        "        <td align=\"center\">$time_local</td>"
        "        <td>Date and time in <i>CLF</i> (NCSA) format.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$time_iso8601</td>"
        "        <td>Date and time in <i>ISO 8601</i> format.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$msec</td>"
        "        <td>Time since epoch (in seconds, with a milliseconds resolution) at the time of the log write.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$request</td>"
        "        <td>First line of request, equivalent to: <i>$request_method $request_uri $server_protocol</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$server_protocol</td>"
        "        <td>The request protocol <i>(HTTP/v, HTTPS/v)</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$request_method</td>"
        "        <td>The request method <i>(GET, POST, HEAD, ...)</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$request_uri</td>"
        "        <td>The requested URI, including the query string (if any).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$uri</td>"
        "        <td>The URI path requested, not including the query string.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$query_string</td>"
        "        <td>Query string (if any).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$status</td>"
        "        <td>HTTP status code.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$request_length</td>"
        "        <td>Number of bytes received (including request line, header, and request body).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$bytes_sent</td>"
        "        <td>The number of bytes sent to a client.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$request_time</td>"
        "        <td>Request processing time in seconds with a milliseconds resolution: time elapsed between the first bytes were read from the client and the log write after the last bytes were sent to the client.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$http_referer</td>"
        "        <td>Referrer hostname.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$cookie_NAME</td>"
        "        <td>Cookie of the request having the given <i>NAME</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$http_user_agent</td>"
        "        <td>Web-browser or Bot identification string.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$remote_addr</td>"
        "        <td>IP Address of the visitor (client).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">$realip_remote_addr</td>"
        "        <td>Original client IP address.</td>"
        "      </tr>"
        "    </table>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Fields discarded by LogDoctor</h3>"
        "    <br>"
        "    <p>Any field than the ones above won't be considered by LogDoctor.<br>"
        "    When generating a <i>log sample</i>, these fields will appear as '<b>DISCARDED</b>'.<br>"
        "    If you aint using logs for any other purpose, please remove unnecessary fields to make the process faster and reduce the possibility of errors.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>References</h3>"
        "    <br>"
        "    <ul>"
        "      <li>Nginx logs formats:<br>"
        "        <a>https://nginx.org/en/docs/http/ngx_http_log_module.html#log_format</a><br>"
        "        <a>https://docs.nginx.com/nginx/admin-guide/monitoring/logging/#setting-up-the-access-log</a></li><br>"
        "    </ul>"
        "    <br>"
        "    <br>"
        "    <br>"
        "  </body>"
        "</html>"
    );
}

void Craphelp::defaultIisFormat( std::string& str ) const
{
    str.append(
        "<!DOCTYPE html>"
        "<html lang=\"en_US\">"
        "  <head>"
        "    <meta charset=\"utf-8\">"
        "    <title>LogDoctor - Help</title>"
        "    <style type=\"text/css\">"
        "      body {"
        "        color: $TEXT$;"
        "        font-size: 19px;"
        "        background-color: $BG$;"
        "      }"
        "      h1 {"
        "        color: $H1$;"
        "      }"
        "      h2 {"
        "        color: $BG$;"
        "        background-color: $H1$;"
        "      }"
        "      h3 {"
        "        color: $H3$;"
        "      }"
        "      th,td {"
        "        padding: 20px;"
        "      }"
        "      code {"
        "        color: $CODE$;"
        "        font-size: 17px;"
        "        padding-left:16px"
        "      }"
        "      i {"
        "        color: $IT$;"
        "      }"
        "      a {"
        "        color: $LINK$;"
        "      }"
        "    </style>"
        "  </head>"
        "  <body>"
        "    <br>"
        "    <h1 align=\"center\">IIS</h1>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h2 align=\"center\"><br>Access logs format string</h2>"
        "    <br>"
        "    <h3><br>Configuration</h3>"
        "    <br>"
        "    <p>The configuration file should be located at:</p><br>"
        "      <code>C:\\inetpub\\logs\\LogFiles\\&lt;YourSiteName&gt;</code>"
        "    <br>"
        "    <p>Access logs can be configured from the <i>IIS Manager</i>.<br>"
        "    To open the IIS Manager, go to <code>Control panel</code><i>→</i><code>Administrative tools</code><i>→</i><code></code><i>→</i><code>Internet Informations Service (IIS) Manager</code>.<br>"
        "    Once inside, click on <code>Logging</code> to edit the logs settings.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Logs format modules</h3>"
        "    <br>"
        "    <p>Available logs formats (supported by LogDoctor) are:</p>"
        "    <br>"
        "    <ul>"
        "      <li style=\"padding-bottom:10px\"><i>IIS</i></li>"
        "        <p>Comma-separated values, can't be customized further. The file name is usually something like <code>u_in&lt;DATE&gt;.log</code></p>"
        "      <br>"
        "      <li style=\"padding-bottom:10px\"><i>NCSA</i></li>"
        "        <p>Whitespace-separated values, can't be customized further. The file name is usually something like <code>u_nc&lt;DATE&gt;.log</code></p>"
        "      <br>"
        "      <li style=\"padding-bottom:10px\"><i>W3C</i></li>"
        "        <p>Whitespace-separated values, can be customized by selecting which fields to log. The file name is usually something like <code>u_ex&lt;DATE&gt;.log</code></p>"
        "    </ul>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Suggested logs format</h3>"
        "    <br>"
        "    <p>The suggested logs module is the <i>W3C</i> format, which will allow using the complete set of functionalities of LogDoctor.</p>"
        "    <p>The suggested fields to use are the following:</p><br>"
        "      <code>date</code>, <code>time</code>, <code>cs-method</code>, <code>cs-uri-stem</code>, <code>cs-uri-query</code>, <code>c-ip</code>, <code>cs-version</code>, <code>cs(User-Agent)</code>, <code>cs(Cookie)</code>, <code>cs(Referer)</code>, <code>sc-status</code>, <code>sc-bytes</code>, <code>cs-bytes</code>, <code>time-taken</code>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Logs format strings</h3>"
        "    <br>"
        "    <p>Both the <i>IIS</i> and <i>NCSA</i> modules are standard, and thus LogDoctor doesn't need the <i>format string</i> to be specified.<br>"
        "    The <i>W3C</i> module can be customized and so a <i>format string</i> must be specified. It can be found insed any log file created using this format module, searching the line starting with <code>#Fields:</code>.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h2 align=\"center\"><br>W3C logs format fields</h2>"
        "    <br>"
        "    <h3><br>Fields considered by LogDoctor</h3>"
        "    <br>"
        "    <p>Only the following fields will be considered, meaning that only these fields' data will be stored and used for the statistics.</p>"
        "    <br>"
        "    <table border=\"1\" width=\"100%\">"
        "      <tr><th style=\"width:150px\">Code</th><th>Informations</th></tr>"
        "      <tr>"
        "        <td align=\"center\">date</td>"
        "        <td>The date on which the activity occurred <i>(UTC)</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">time</td>"
        "        <td>The time at which the activity occurred <i>(UTC)</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">cs-version</td>"
        "        <td>The request protocol <i>(HTTP/v, HTTPS/v)</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">cs-method</td>"
        "        <td>The request method <i>(GET, POST, HEAD, ...)</i>.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">cs-uri-stem</td>"
        "        <td>The URI path requested, not including any query string.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">cs-uri-query</td>"
        "        <td>Query string (if any).</td>"
        "      </tr>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">sc-status</td>"
        "        <td>HTTP status code.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">cs-bytes</td>"
        "        <td>The number of bytes received and processed by the server.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">sc-bytes</td>"
        "        <td>The number of bytes sent back by the server.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">time-taken</td>"
        "        <td>Time taken to serve the request, in milliseconds</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">cs(Referer)</td>"
        "        <td>The site which provided a link to the current site.</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">cs(Cookie)</td>"
        "        <td>The content of the cookie sent or received (if any).</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">cs(User-Agent)</td>"
        "        <td>Web-browser or bot identification string</td>"
        "      </tr>"
        "      <tr>"
        "        <td align=\"center\">c-ip</td>"
        "        <td>The IP address of the client that made the request.</td>"
        "    </table>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>Fields discarded by LogDoctor</h3>"
        "    <br>"
        "    <p>Any field than the ones above won't be considered by LogDoctor.<br>"
        "    When generating a <i>log sample</i>, these fields will appear as '<b>DISCARDED</b>'.<br>"
        "    If you aint using logs for any other purpose, please remove unnecessary fields to make the process faster and reduce the possibility of errors.</p>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <br>"
        "    <h3>References</h3>"
        "    <br>"
        "    <ul>"
        "      <li>Configure IIS logging:<br>"
        "        <a style=\"color:$LINK$\">https://docs.microsoft.com/en-us/iis/manage/provisioning-and-managing-iis/configure-logging-in-iis</a></li><br>"
        "      <li>Logs format modules overview:<br>"
        "        <a style=\"color:$LINK$\">https://docs.microsoft.com/en-us/previous-versions/iis/6.0-sdk/ms525807(v=vs.90)</a></li><br>"
        "      <li>The W3C log format:<br>"
        "        <a style=\"color:$LINK$\">https://docs.microsoft.com/en-us/windows/win32/http/w3c-logging</a></li>"
        "    </ul>"
        "    <br>"
        "    <br>"
        "    <br>"
        "  </body>"
        "</html>"
    );
}

