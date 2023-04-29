#ifndef PRINTSEC_H
#define PRINTSEC_H

class QString;


namespace PrintSec
{

//! Formats the size including the suffix, for display purposes
/*!
    \param bytes The size in bytes
    \return The string to be displayed
*/
const QString printableSize(
    const unsigned bytes
);


//! Formats the speed including the suffix, for display purposes
/*!
    \param bytes The size in bytes
    \param seconds The number of seconds
    \return The string to be displayed
*/
const QString printableSpeed(
    const float& bytes, const float& seconds
);


//! Formats the time, for display purposes
/*!
    \param seconds The number of seconds
    \return The string to be displayed
*/
const QString printableTime(
    const unsigned seconds
);


//! Returns a string of the given time in the format HH:MM:SS
/*!
    \param hour The hour
    \param minut The minute
    \param second The second
    \return The printable time
*/
const QString printableTime(
    const int hour, const int minute, const int second
);


//! Returns a string of the given date in the format YYYY-MM-DD
/*!
    \overload const QString printableDate(const int& year, const int& month, const int& day)
    \param year The year
    \param month The month
    \param day The day
    \return The printable date
*/
const QString printableDate(
    const QString& year, const int month, const QString& day
);


//! Returns a string of the given date in the format YYYY-MM-DD
/*!
    \param year The year (4 digits number)
    \param month The month
    \param day The day
    \return The printable date
*/
const QString printableDate(
    const int year, const int month, const int day
);


//! Returns a string corresponding to the given value
/*!
    Used to convert numeric booleans [0,1] to string [FALSE,TRUE]
    \param value The value to convert
    \return The corresponding printable string
*/
const QString printableBool(
    const int value
);

} // namespace PrintSec

#endif // PRINTSEC_H
