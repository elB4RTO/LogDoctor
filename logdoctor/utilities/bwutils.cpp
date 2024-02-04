
#include "utilities/bwlists.h"

#include "utilities/strings.h"

#include "modules/exceptions.h"

#include <QUrl>


namespace BWutils
{

std::string sanitizedMethod( const std::string& item )
{
    const std::string sanitized_item{ StringOps::strip( item ) };
    if ( ! StringOps::isAlphabetic( sanitized_item ) ) {
        // only letters allowed
        throw BWlistException("Invalid Method");
    }
    return StringOps::toUpper( sanitized_item );
}


std::string sanitizedUri( const std::string& item )
{
    const std::string sanitized_item{ StringOps::lstrip( item ) };
    if ( sanitized_item.empty() ) {
        throw BWlistException("Invalid URI");
    }
    return QUrl::toPercentEncoding(
        QString::fromStdString( sanitized_item ),
        "/#&?=+"
    ).toStdString();
}


std::string sanitizedClient( const std::string& item )
{
    const std::string sanitized_item{ StringOps::strip( item ) };
    if ( ! StringOps::isIP( sanitized_item ) ) {
        // only IPv4/IPv6 allowed
        throw BWlistException("Invalid Client");
    }
    return sanitized_item;
}


std::string sanitizedUserAgent( std::string_view item )
{
    return StringOps::replace( item, R"(")", R"(\")" );
}

} // namespace BWutils
