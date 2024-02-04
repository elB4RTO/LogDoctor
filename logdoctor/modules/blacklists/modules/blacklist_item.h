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

    //! \throw DoNotCatchException
    const char* fieldName() const;

    //! \throw BWlistException, DoNotCatchException
    void set( const std::vector<std::string>& new_list );

    //! \throw BWlistException, DoNotCatchException
    void add( const std::string& item );

    //! \throw BWlistException
    void remove( const std::string& item );

    //! \throw BWlistException
    int moveUp( const std::string& item );

    //! \throw BWlistException
    int moveDown( const std::string& item );

private:
    //! \throw BWlistException, DoNotCatchException
    std::string sanitized( const std::string& item ) const;
};


#endif // LOGDOCTOR__BLACKLISTS__BLACKLISTITEM__H
