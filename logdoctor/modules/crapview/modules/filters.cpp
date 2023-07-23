
#include "filters.h"

#include "utilities/strings.h"

#include <QStringList>


namespace FilterOps
{

const std::optional<QString> parseNull( const QString& filter_str, const bool to_clean )
{
    std::optional<QString> result;
    const QString aux{ ( to_clean )
                       ? filter_str.simplified().toUpper()
                       : filter_str };
    if ( !aux.isEmpty() ) { // here an empty string is considered invalid
        if ( aux == "NULL" ) {
            result.emplace( "NULL" );
        } else if ( aux == "NOT NULL" || aux == "! NULL" || aux == "!NULL" ) {
            result.emplace( "NOT NULL" );
        }
    }
    return result;
}


const std::optional<QString> parseBooleanFilter( const QString& filter_str )
{
    using opt_t = std::optional<QString>;
    opt_t result;
    if ( filter_str.isEmpty() ) {
        // an empty filter is not invalid
        result.emplace("");
        return result;
    }
    QString str{ filter_str.simplified().toUpper() };
    // check if the filter is NULL or NOT NULL
    if ( opt_t aux=parseNull( str, false ); aux.has_value() ) {
        return aux;
    }
    // normalize the string
    if ( str.startsWith("==") ) {
        str.remove(0, 2);
    } else if ( str.startsWith("=") ) {
        str.remove(0, 1);
    } else if ( str.startsWith("!=") ) {
        str.replace("!=", "NOT ");
    } else if ( str.startsWith("!") ) {
        str.replace("!", "NOT ");
    }
    str = str.replace("0", "FALSE").replace("1", "TRUE").simplified();
    // apply if valid
    if ( str == "TRUE" || str == "NOT FALSE" ) {
        result.emplace("= 1");
    } else if ( str == "FALSE" || str == "NOT TRUE" ) {
        result.emplace("= 0");
    }
    return result;
}


const std::optional<QString> parseNumericFilter( const QString& filter_str )
{
    using opt_t = std::optional<QString>;
    opt_t result;
    if ( filter_str.isEmpty() ) {
        // an empty filter is not invalid
        result.emplace("");
        return result;
    }
    // check if the filter is NULL or NOT NULL
    if ( opt_t aux=parseNull( filter_str ); aux.has_value() ) {
        return aux;
    }
    // normalize the comparison operator
    QString aux{ filter_str.simplified() };
    if ( aux.startsWith("==") ) {
        aux.replace("==", "= ");
    } else if ( aux.startsWith("=") ) {
        aux.replace("=", "= ");
    } else if ( aux.startsWith("!=") ) {
        aux.replace("!=", "!= ");
    } else if ( aux.startsWith("!") ) {
        aux.replace("!", "!= ");
    } else if ( aux.startsWith("<=") ) {
        aux.replace("<=", "<= ");
    } else if ( aux.startsWith("<") ) {
        aux.replace("<", "< ");
    } else if ( aux.startsWith(">=") ) {
        aux.replace(">=", ">= ");
    } else if ( aux.startsWith(">") ) {
        aux.replace(">", "> ");
    } else if ( aux.startsWith("eq ") ) {
        aux.replace("eq ", "= ");
    } else if ( aux.startsWith("ne ") ) {
        aux.replace("ne ", "!= ");
    } else if ( aux.startsWith("lt ") ) {
        aux.replace("lt ", "< ");
    } else if ( aux.startsWith("le ") ) {
        aux.replace("le ", "<= ");
    } else if ( aux.startsWith("gt ") ) {
        aux.replace("gt ", "> ");
    } else if ( aux.startsWith("ge ") ) {
        aux.replace("ge ", ">= ");
    }
    const std::string str{ aux.simplified().toStdString() };
    if ( StringOps::isNumeric( str ) ) {
        // string is numeric, no need to check further
        result.emplace( QString("= %1").arg(str.c_str()) );
        return result;
    }
    // a valid string is only composed by a comparison operator followed by a number
    size_t i{ 0ul };
    if ( char c=str.at(i); c == '=' || c == '!' || c == '<' || c == '>' ) {
        i ++;
        if ( str.at(i) == '=' ) {
            i ++;
        }
        if ( str.at(i) == ' ' ) {
            i ++;
        }
        if ( !StringOps::isNumeric( str.substr(i) ) ) {
            return result;
        }
        result.emplace( QString::fromStdString( str ) );
    }
    return result;
}


const std::optional<QString> parseTextualFilter( const QString& filter_str )
{
    using opt_t =std::optional<QString>;
    opt_t result;
    if ( filter_str.isEmpty() ) {
        // an empty filter is not invalid
        result.emplace("");
        return result;
    }
    // check if the filter is NULL or NOT NULL
    if ( opt_t aux=parseNull( filter_str ); aux.has_value() ) {
        return aux;
    }
    if ( filter_str == "*" ) {
        result.emplace("NOT NULL");
    } else {
        result.emplace( filter_str.trimmed() );
    }
    return result;
}

} // namespace FilterOps
