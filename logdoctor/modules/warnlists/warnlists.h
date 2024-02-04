#ifndef LOGDOCTOR__WARNLISTS__H
#define LOGDOCTOR__WARNLISTS__H


#include "main_lib.h"

#include "modules/warnlists/modules/warnlist.h"


struct Warnlists final
{
    Warnlist apache;
    Warnlist nginx;
    Warnlist iis;

    explicit Warnlists() noexcept = default;

    //! \throw DoNotCatchException
    inline bool isUsed( const WebServer ws, const WarnlistField fld )
    { return get( ws ).isUsed( fld ); }

    //! \throw DoNotCatchException
    inline void setUsed( const WebServer ws, const WarnlistField fld, const bool used )
    { get( ws ).setUsed( fld, used ); }

    //! \throw DoNotCatchException
    Warnlist& get( const WebServer ws );

    //! \throw DoNotCatchException
    inline WarnlistItem& get( const WebServer ws, const WarnlistField fld )
    { return get( ws ).get( fld ); }

    //! \throw DoNotCatchException
    inline std::vector<std::string>& getList( const WebServer ws, const WarnlistField fld )
    { return get( ws, fld ).list; }

    //! \throw DoNotCatchException
    inline const std::vector<std::string>& getListConst( const WebServer ws, const WarnlistField fld )
    { return get( ws, fld ).list; }

    //! \throw BWlistException, DoNotCatchException
    inline void setList( const WebServer ws, const WarnlistField fld, const std::vector<std::string>& list )
    { get( ws, fld ).set( list ); }

    //! \throw BWlistException, DoNotCatchException
    inline void clearList( const WebServer ws, const WarnlistField fld )
    { getList( ws, fld ).clear(); }

    //! \throw BWlistException, DoNotCatchException
    inline void addItem( const WebServer ws, const WarnlistField fld, const std::string& item )
    { get( ws, fld ).add( item ); }

    //! \throw BWlistException
    inline void removeItem( const WebServer ws, const WarnlistField fld, const std::string& item )
    { get( ws, fld ).remove( item ); }

    //! \throw BWlistException
    inline int moveUpItem( const WebServer ws, const WarnlistField fld, const std::string& item )
    { return get( ws, fld ).moveUp( item ); }

    //! \throw BWlistException
    inline int moveDownItem( const WebServer ws, const WarnlistField fld, const std::string& item )
    { return get( ws, fld ).moveDown( item ); }
};


#endif // LOGDOCTOR__WARNLISTS__H
