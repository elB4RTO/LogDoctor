
#include "printables.h"

#include "modules/shared.h"

#include <QString>


namespace PrintSec
{

const QString printableSize( const unsigned bytes )
{
    std::string size_sfx{" B"};
    float size{ static_cast<float>(bytes) };
    if (size > 1024.f) {
        size /= 1024.f;
        size_sfx = " KiB";
        if (size > 1024.f) {
            size /= 1024.f;
            size_sfx = " MiB";
        }
    }
    // cut decimals depending on how big the floor is
    std::string size_str{ std::to_string( size ) };
    size_t cut_index{ size_str.find('.')+1 };
    if ( cut_index == 0 ) { // catches the overflow of +1 of the above
        cut_index = size_str.find(',')+1;
    }
    size_t n_decimals{ 3 };
    if ( size >= 100 ) {
        n_decimals --;
        if ( size >= 1000 ) {
            n_decimals --;
            if ( size >= 10000 ) {
                n_decimals --;
                cut_index --; // no decimals, no "dot"
            }
        }
    }
    if ( cut_index >= 1 ) {
        cut_index += n_decimals;
        if ( cut_index >= size_str.size() ) {
            cut_index = size_str.size()-1;
        }
    }
    return QString::fromStdString( size_str.substr(0,cut_index) + size_sfx );
}


const QString printableSpeed( const float& bytes, const float& secs_ )
{
    std::string speed_sfx{" B/s"};
    const float secs{ ( secs_ > .0f ) ? secs_ : ( secs_ < .0f ) ? -secs_ : .1f };
    float speed{ bytes / secs };
    if (speed > 1024.f) {
        speed /= 1024.f;
        speed_sfx = " KiB/s";
        if (speed > 1024.f) {
            speed /= 1024.f;
            speed_sfx = " MiB/s";
        }
    }
    // cut decimals depending on how big the floor is
    std::string speed_str{ std::to_string( speed ) };
    size_t cut_index{ speed_str.find('.')+1 };
    if ( cut_index == 0 ) { // catches the overflow of +1 of the above
        cut_index = speed_str.find(',')+1;
    }
    size_t n_decimals{ 3 };
    if ( speed >= 100 ) {
        n_decimals --;
        if ( speed >= 1000 ) {
            n_decimals --;
            if ( speed >= 10000 ) {
                n_decimals --;
                cut_index --; // no decimals, no "dot"
            }
        }
    }
    if ( cut_index >= 1 ) {
        cut_index += n_decimals;
        if ( cut_index >= speed_str.size() ) {
            cut_index = speed_str.size()-1;
        }
    }
    return QString::fromStdString( speed_str.substr(0,cut_index) + speed_sfx );
}


const QString printableTime( const unsigned seconds )
{
    const unsigned mins{ seconds / 60 };
    const unsigned secs{ seconds - (mins*60) };
    return QString("%1:%2").arg(
        (mins<10)
            ? QString("0%1").arg( mins )
            : QString::number( mins ),
        (secs<10)
            ? QString("0%1").arg( secs )
            : QString::number( secs )
    );
}


const QString printableTime( const int hour, const int minute, const int second )
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


const QString printableDate( const QString& year, const int month, const QString& day )
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


const QString printableDate( const int year, const int month, const int day )
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


const QString printableBool( const int value )
{
    if ( value == 0 ) {
        return TR::tr( BOOLS__FALSE.c_str() );
    } else {
        return TR::tr( BOOLS__TRUE.c_str() );
    }
}

} // namespace PrintSec
