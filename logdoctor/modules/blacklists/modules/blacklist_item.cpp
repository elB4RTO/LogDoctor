
#include "blacklist_item.h"

#include "modules/shared.h"

#include "modules/exceptions.h"

#include "utilities/bwlists.h"


const char* BlacklistItem::fieldName() const
{
    switch (this->field) {
        case BlacklistField::Client:
            return FIELDS__CLIENT.c_str();
        default:
            // should be unreachable
            throw DoNotCatchException( "Unexpected BlacklistField", std::to_string(static_cast<int>(this->field)) );
    }
}

void BlacklistItem::set( const std::vector<std::string>& new_list )
{
    this->list.clear();
    this->list.reserve( new_list.size() );
    for ( const std::string& item : new_list ) {
        this->add( item );
    }
}

void BlacklistItem::add( const std::string& item )
{
    this->list.push_back( this->sanitized( item ) );
}

void BlacklistItem::remove( const std::string& item )
{
    if ( const auto it{ std::find( this->list.cbegin(), this->list.cend(), item ) }; it != this->list.cend() ) {
        this->list.erase( it );
    } else {
        throw BWlistException( "Failed to remove the item: "+item );
    }
}

int BlacklistItem::moveUp( const std::string& item )
{
    if ( auto it{ std::find( std::next(this->list.begin()), this->list.end(), item ) }; it != this->list.end() ) {
        const int pos{ static_cast<int>( std::distance(this->list.begin(), it) ) - 1 };
        std::swap( *it, *std::prev(it) );
        return pos;
    } else {
        throw BWlistException( "Failed to move up the item: "+item );
    }
}

int BlacklistItem::moveDown( const std::string& item )
{
    if ( auto it{ std::find( this->list.begin(), std::prev(this->list.end()), item ) }; it != this->list.end() ) {
        const int pos{ static_cast<int>( std::distance(this->list.begin(), it) ) + 1 };
        std::swap( *it, *std::next(it) );
        return pos;
    } else {
        throw BWlistException( "Failed to move down the item: "+item );
    }
}

std::string BlacklistItem::sanitized(const std::string& item ) const
{
    switch (this->field) {
        case BlacklistField::Client:
            return BWutils::sanitizedClient( item );
        default:
            // should be unreachable
            throw DoNotCatchException( "Unexpected BlacklistField", std::to_string(static_cast<int>(this->field)) );
    }
}
