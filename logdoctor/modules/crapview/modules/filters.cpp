
#include "filters.h"

#include "utilities/strings.h"

#include <QRegularExpression>
#include <QStringList>


namespace FilterOps
{

std::optional<QString> parseNull( const QString& filter_str, const bool to_clean ) noexcept
{
    std::optional<QString> result;
    const QString aux{ ( to_clean )
                       ? filter_str.simplified().toUpper()
                       : filter_str };
    if ( !aux.isEmpty() ) { // here an empty string is considered invalid
        if ( aux == "NULL" || aux == "NOT NULL" ) {
            result.emplace( QStringLiteral(" IS %1").arg( aux ) );
        } else if ( aux == "! NULL" || aux == "!NULL" ) {
            result.emplace( QStringLiteral(" IS NOT NULL") );
        }
    }
    return result;
}


std::optional<QString> parseNumericFilter( const QString& filter_str ) noexcept
{
    using opt_t = std::optional<QString>;

    // a valid string is only composed by a comparison operator followed by a number
    static const QRegularExpression expected("^(=|==|!=|<|<=|>=|>)[0-9]+$");
    // remove every space from the filter
    static const QRegularExpression spaces(R"(\s*)");

    if ( filter_str.isEmpty() ) {
        // an empty filter is not invalid
        return {filter_str};
    }

    // check if the filter is NULL or NOT NULL
    if ( const opt_t aux{ parseNull( filter_str ) }; aux.has_value() ) {
        return aux;
    }

    if ( expected.matchView( filter_str ).hasMatch() ) {
        return {filter_str};
    }

    opt_t result;
    // normalize the comparison operator
    QString aux{ filter_str.toUpper().remove(spaces) };
    if ( aux.isEmpty() ) {
        return {aux};

    } else if ( StringOps::isNumeric( aux ) ) {
        aux.prepend( QChar('=') );

    } else if ( aux.at(0) == QChar('!') && aux.at(1) != QChar('=') ) {
        aux.insert(1, QChar('='));

    } else if ( aux.startsWith(QLatin1String("EQ")) ) {
        aux.replace(0, 2, QChar('='));

    } else if ( aux.startsWith(QLatin1String("NE")) ) {
        aux.replace(0, 2, QChar('=')).prepend(QChar('!'));

    } else if ( aux.startsWith(QLatin1String("LT")) ) {
        aux.replace(0, 2, QChar('<'));

    } else if ( aux.startsWith(QLatin1String("LE")) ) {
        aux.replace(0, 2, QChar('=')).prepend(QChar('<'));

    } else if ( aux.startsWith(QLatin1String("GT")) ) {
        aux.replace(0, 2, QChar('>'));

    } else if ( aux.startsWith(QLatin1String("GE")) ) {
        aux.replace(0, 2, QChar('=')).prepend(QChar('>'));
    }

    // final check
    if ( expected.matchView( aux ).hasMatch() ) {
        result.emplace( aux );
    }
    return result;
}


std::optional<QString> parseTextualFilter( const QString& filter_str ) noexcept
{
    using opt_t =std::optional<QString>;

    if ( filter_str.isEmpty() ) {
        // an empty filter is not invalid
        return {filter_str};
    }

    // check if the filter is NULL or NOT NULL
    if ( const opt_t aux{ parseNull( filter_str ) }; aux.has_value() ) {
        return aux;
    }

    opt_t result;
    QString aux{ filter_str.trimmed() };
    if ( aux.isEmpty() ) {
        return {aux};

    } else if ( aux == "*" ) {
        result.emplace(QStringLiteral(" IS NOT NULL"));

    } else {
        if ( aux.startsWith(QChar('!')) ) {
            result.emplace( QStringLiteral(" NOT LIKE '%1'").arg( aux.removeFirst().trimmed().replace(QChar('\''),QLatin1String("''")) ) );
        } else {
            if ( aux.startsWith(QChar('\\')) ) {
                aux.removeFirst();
            }
            result.emplace( QStringLiteral(" LIKE '%1'").arg( aux.replace(QChar('\''),QLatin1String("''")) ) );
        }
    }
    return result;
}

} // namespace FilterOps
