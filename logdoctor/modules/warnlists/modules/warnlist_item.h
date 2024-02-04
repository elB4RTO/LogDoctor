#ifndef LOGDOCTOR__WARNLISTS__WARNLISTITEM__H
#define LOGDOCTOR__WARNLISTS__WARNLISTITEM__H


#include "modules/warnlists/modules/lib.h"

#include <vector>
#include <string>


struct WarnlistItem final
{
    bool used;
    WarnlistField field;
    std::vector<std::string> list;

    WarnlistItem( const WarnlistField fld ) noexcept
        : used{false}, field{fld} {}
    WarnlistItem( const bool use, const WarnlistField fld ) noexcept
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

#endif // LOGDOCTOR__WARNLISTS__WARNLISTITEM__H
