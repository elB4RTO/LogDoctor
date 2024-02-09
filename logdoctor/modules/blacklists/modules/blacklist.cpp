
#include "blacklist.h"

#include "modules/exceptions.h"


Blacklist::Blacklist() noexcept
    : client{ BlacklistField::Client }
{

}

BlacklistItem& Blacklist::get( const BlacklistField field )
{
    switch (field) {
        case BlacklistField::Client:
            return this->client;
        default:
            throw DoNotCatchException( "Unexpected BlacklistField", std::to_string(static_cast<int>(field)) );
    }
}
