
#include "printables.h"

#include "modules/shared.h"

#include <QString>


namespace PrintSec
{

QString printableSize( const size_t bytes ) noexcept
{
    std::string size_sfx{" B"};
    double size{ static_cast<double>(bytes) };
    if (size > 1024.0) {
        size /= 1024.0;
        size_sfx = " KiB";
        if (size > 1024.0) {
            size /= 1024.0;
            size_sfx = " MiB";
        }
    }
    // cut decimals depending on how big the floor is
    std::string size_str{ std::to_string( size ) };
    size_t cut_index{ size_str.find('.')+1 };
    if ( cut_index == 0ul ) { // catches the overflow of +1 of the above
        cut_index = size_str.find(',')+1;
    }
    size_t n_decimals{ 3ul };
    if ( size >= 100.0 ) {
        -- n_decimals;
        if ( size >= 1000.0 ) {
            -- n_decimals;
            if ( size >= 10000.0 ) {
                -- n_decimals;
                -- cut_index; // no decimals, no "dot"
            }
        }
    }
    if ( cut_index >= 1ul ) {
        cut_index += n_decimals;
        if ( cut_index >= size_str.size() ) {
            cut_index = size_str.size()-1ul;
        }
    }
    return QString::fromStdString( size_str.substr(0ul,cut_index) + size_sfx );
}


QString printableSpeed( const double bytes, const double secs_ ) noexcept
{
    std::string speed_sfx{" B/s"};
    const double secs{ ( secs_ > 0.0 ) ? secs_ : ( secs_ < 0.0 ) ? -secs_ : 0.1 };
    double speed{ bytes / secs };
    if (speed > 1024.0) {
        speed /= 1024.0;
        speed_sfx = " KiB/s";
        if (speed > 1024.0) {
            speed /= 1024.0;
            speed_sfx = " MiB/s";
        }
    }
    // cut decimals depending on how big the floor is
    std::string speed_str{ std::to_string( speed ) };
    size_t cut_index{ speed_str.find('.')+1ul };
    if ( cut_index == 0ul ) { // catches the overflow of +1 of the above
        cut_index = speed_str.find(',')+1ul;
    }
    size_t n_decimals{ 3ul };
    if ( speed >= 100.0 ) {
        -- n_decimals;
        if ( speed >= 1000.0 ) {
            -- n_decimals;
            if ( speed >= 10000.0 ) {
                -- n_decimals;
                -- cut_index; // no decimals, no "dot"
            }
        }
    }
    if ( cut_index >= 1ul ) {
        cut_index += n_decimals;
        if ( cut_index >= speed_str.size() ) {
            cut_index = speed_str.size()-1ul;
        }
    }
    return QString::fromStdString( speed_str.substr(0ul,cut_index) + speed_sfx );
}


QString printableTime( const unsigned seconds ) noexcept
{
    const unsigned mins{ seconds / 60u };
    const unsigned secs{ seconds - (mins*60u) };
    return QString("%1:%2").arg(
        (mins<10u)
            ? QString("0%1").arg( mins )
            : QString::number( mins ),
        (secs<10u)
            ? QString("0%1").arg( secs )
            : QString::number( secs )
    );
}


QString printableTime( const int hour, const int minute, const int second ) noexcept
{
    return QString("%1:%2:%3").arg(
        (hour<10)
            ? QString("0%1").arg( hour )
            : QString::number( hour ),
        (minute<10)
            ? QString("0%1").arg( minute )
            : QString::number( minute ),
        (second<10)
            ? QString("0%1").arg( second )
            : QString::number( second )
    );
}


QString printableDate( const QString& year, const int month, const QString& day ) noexcept
{
    return QString("%1-%2-%3").arg(
        year,
        (month<10)
            ? QString("0%1").arg( month )
            : QString::number( month ),
        (day.size()<2)
            ? QString("0%1").arg( day )
            : day
    );
}


QString printableDate( const int year, const int month, const int day ) noexcept
{
    return QString("%1-%2-%3").arg(
        QString::number( year ),
        (month<10)
            ? QString("0%1").arg( month )
            : QString::number( month ),
        (day<10)
            ? QString("0%1").arg( day )
            : QString::number( day )
    );
}


QString printableBool( const int value ) noexcept
{
    return value == 0
         ? TR::tr( BOOLS__FALSE.c_str() )
         : TR::tr( BOOLS__TRUE.c_str()  );
}

} // namespace PrintSec
