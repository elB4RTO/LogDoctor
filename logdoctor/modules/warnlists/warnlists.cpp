
#include "warnlists.h"

#include "modules/exceptions.h"


Warnlist& Warnlists::get( const WebServer ws )
{
    switch (ws) {
        case WebServer::Apache:
            return this->apache;
        case WebServer::Nginx:
            return this->nginx;
        case WebServer::IIS:
            return this->iis;
        default:
            // should be unreachable
            throw DoNotCatchException( "Unexpected WebServer", std::to_string(static_cast<int>(ws)) );
    }
}
