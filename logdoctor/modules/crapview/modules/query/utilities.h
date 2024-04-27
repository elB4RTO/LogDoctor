#ifndef LOGDOCTOR__CRAPVIEW__MODULES__QUERY__UTILITIES_H
#define LOGDOCTOR__CRAPVIEW__MODULES__QUERY__UTILITIES_H


#include <QString>


namespace QueryPrivate
{

//! Converts the given string to int
/*!
    \param str The string to convert
    \return The resulting integer
    \throw VoidException
*/
int toInt( QStringView str );

//! Morphs the given variant into an int
/*!
    \param var The variant to convert
    \return The resulting integer
    \throw VoidException
*/
int toInt( const QVariant& var );


//! Morphs the given variant into a string
/*!
    \param var The variant to convert
    \return The resulting string
    \throw VoidException
*/
QString toString( const QVariant& var );


//! Returns the minute gap for the given minute with the given gap
/*!
    \param minute The minute of the hour
    \param gap The gap used to split the hour
    \return The gap index
    \throw DateTimeException
*/
int getMinuteGap( const int minute, const int gap=10 );


//! Returns the number of days for a given month
/*!
    \param year The year, used to handle leap years
    \param month The month
    \return The number of days
    \throw DateTimeException
*/
int getMonthDays( const int year, const int month );


//! Returns the number of days in a given period
/*!
    \param from_year The initial year
    \param from_month The initial month
    \param from_day The initial day
    \param to_year The final Year
    \param to_month The final month
    \param to_day The final day
    \return The number of days
    \throw DateTimeException
*/
int countDays( const int from_year, const int from_month, const int from_day, const int to_year, const int to_month, const int to_day );


//! Returns the number of months in a given period
/*!
    \param from_year The initial year
    \param from_month The initial month
    \param to_year The final Year
    \param to_month The final month
    \return The number of months in the period
*/
int countMonths( const int from_year, const int from_month, const int to_year, const int to_month ) noexcept;

} // namespace QueryPrivate


#endif // LOGDOCTOR__CRAPVIEW__MODULES__QUERY__UTILITIES_H
