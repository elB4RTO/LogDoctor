
#include "blacklists.h"

#include "modules/exceptions.h"


const Blacklist& Blacklists::getConst( const WebServer ws ) const
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
            throw DoNotCatchException( "Unexpected WebServer" );
    }
}

Blacklist& Blacklists::get( const WebServer ws )
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
            throw DoNotCatchException( "Unexpected WebServer" );
    }
}
