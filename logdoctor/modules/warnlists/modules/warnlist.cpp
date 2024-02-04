
#include "warnlist.h"

#include "modules/exceptions.h"


Warnlist::Warnlist() noexcept
    : method{     WarnlistField::Method    }
    , uri{        WarnlistField::Uri       }
    , client{     WarnlistField::Client    }
    , user_agent{ WarnlistField::UserAgent }
{

}

WarnlistItem& Warnlist::get( const WarnlistField field )
{
    switch (field) {
        case WarnlistField::Method:
            return this->method;
        case WarnlistField::Uri:
            return this->uri;
        case WarnlistField::Client:
            return this->client;
        case WarnlistField::UserAgent:
            return this->user_agent;
        default:
            throw DoNotCatchException( "Unexpected WarnlistField" );
    }
}
