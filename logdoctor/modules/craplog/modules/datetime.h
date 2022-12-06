#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <vector>


//! DateTimeOps
/*!
    Operations for the dates
*/
class DateTimeOps
{
public:
    DateTimeOps();

    //! Returns a standardized list of items representing the given date and time
    /*!
        \param datetime The given date and time string
        \param  format The format of the given string
        \throw DateTimeException
        \return The list of items
    */
    static const std::vector<std::string> processDateTime( const std::string& datetime, const std::string& format );

private:

    //! Converts a month from the short-name to the number
    /*!
        \param month The short-name of the month
        \return The month number in the calendar
        \throw DateTimeException
        \see processDateTime
    */
    static const std::string convertMonth( const std::string& month );

};

#endif // DATETIME_H
