#ifndef LOGDOCTOR__BLACKLISTS__BLACKLISTITEM__H
#define LOGDOCTOR__BLACKLISTS__BLACKLISTITEM__H


#include "modules/blacklists/modules/lib.h"

#include <vector>
#include <string>


struct BlacklistItem final
{
    bool used;
    BlacklistField field;
    std::vector<std::string> list;

    BlacklistItem( const BlacklistField fld ) noexcept
        : used{false}, field{fld} {}
    BlacklistItem( const bool use, const BlacklistField fld ) noexcept
        : used{use}, field{fld} {}

    //! Returns the name of the field to which the list is associated
    /*!
        The name is translatable
        \throw DoNotCatchException
    */
    const char* fieldName() const;

    //! Replaces the current list with the one provided
    /*!
        All the items will be sanitized before actually
        becoming part of the list
        \throw BWlistException, DoNotCatchException
    */
    void set( const std::vector<std::string>& new_list );

    //! Adds the provided item to the list
    /*!
        The item will be sanitized before actually
        being added to the list
        \throw BWlistException, DoNotCatchException
    */
    void add( const std::string& item );

    //! Remove the requested item from the list
    /*!
        \throw BWlistException
    */
    void remove( const std::string& item );

    //! Moves the requested item one position up in the list
    /*!
        \throw BWlistException
    */
    int moveUp( const std::string& item );

    //! Moves the requested item one position down in the list
    /*!
        \throw BWlistException
    */
    int moveDown( const std::string& item );

private:
    //! Returns a sanitized item
    /*!
        \throw BWlistException, DoNotCatchException
    */
    std::string sanitized( const std::string& item ) const;
};


#endif // LOGDOCTOR__BLACKLISTS__BLACKLISTITEM__H
