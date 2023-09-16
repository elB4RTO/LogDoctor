#ifndef LOGDOCTOR__CRAPVIEW__FILTERS_H
#define LOGDOCTOR__CRAPVIEW__FILTERS_H


#include <optional>

class QString;


//! FilterOps
/*!
    Operations on query filters
*/
namespace FilterOps
{

//! Parses a filter for NULL or NOT NULL final values
/*!
    Auxiliar function.
    Returns a value only if the string can be reduce to 'NULL' or 'NOT NULL',
    in any other case an empty object is returned.
    \param field_str The string to parse
    \param to_clean Whether the filter_str should be cleaned before parsing (trimmed and uppercased)
    \return The resulting filter to apply to the query, if valid
*/
std::optional<QString> parseNull( const QString& filter_str, const bool to_clean=true );

//! Parses a filter for a database field with boolean type
/*!
    Boolean filters are not locale-dependant,
    meaning that English syntax must be used: 'TRUE', 'FALSE'.
    This filter is case-insensitive.
    \param field_str The given filter
    \return The resulting filter to apply to the query, if valid
*/
std::optional<QString> parseBooleanFilter( const QString& filter_str );

//! Parses a filter for a log field with integer type
/*!
    The filter can be composed by anumber or by a comparison operator followed by a number.
    If the filter is just a number (N), it will be resolved to '= N'.
    Valid comparison operators are: '=' or '==', '!' or '!=', '<', '<=', '>', '>='.
    Literal comparison operators can be use as well: 'eq', 'ne', 'lt', 'le', 'gt', 'ge'.
    \param field_str The given filter
    \return The resulting filter to apply to the query, if valid
*/
std::optional<QString> parseNumericFilter( const QString& filter_str );

//! Parses a filter for a log field with text type
/*!
    Any filter not resolving to NULL or NOT NULL will be used as is.
    If the filter is exactly '*', il will be resolved as NOT NULL.
    \param field_str The given filter
    \return The resulting filter to apply to the query, if valid
*/
std::optional<QString> parseTextualFilter( const QString& filter_str );

}


#endif // LOGDOCTOR__CRAPVIEW__FILTERS_H
