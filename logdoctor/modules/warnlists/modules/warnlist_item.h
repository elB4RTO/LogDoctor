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

#endif // LOGDOCTOR__WARNLISTS__WARNLISTITEM__H
