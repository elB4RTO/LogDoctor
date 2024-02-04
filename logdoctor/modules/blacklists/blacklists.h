#ifndef LOGDOCTOR__BLACKLISTS__H
#define LOGDOCTOR__BLACKLISTS__H


#include "main_lib.h"

#include "modules/blacklists/modules/blacklist.h"

#include <QMetaType>


struct Blacklists final
{
    Blacklist apache;
    Blacklist nginx;
    Blacklist iis;

    explicit Blacklists() noexcept = default;

    //! \throw DoNotCatchException
    inline bool isUsed( const WebServer ws, const BlacklistField fld )
    { return get( ws ).isUsed( fld ); }

    //! \throw DoNotCatchException
    inline void setUsed( const WebServer ws, const BlacklistField fld, const bool used )
    { get( ws ).setUsed( fld, used ); }

    //! \throw DoNotCatchException
    const Blacklist& getConst( const WebServer ws ) const;

    //! \throw DoNotCatchException
    Blacklist& get( const WebServer ws );

    //! \throw DoNotCatchException
    inline BlacklistItem& get( const WebServer ws, const BlacklistField fld )
    { return get( ws ).get( fld ); }

    //! \throw DoNotCatchException
    inline std::vector<std::string>& getList( const WebServer ws, const BlacklistField fld )
    { return get( ws, fld ).list; }

    //! \throw DoNotCatchException
    inline const std::vector<std::string>& getListConst( const WebServer ws, const BlacklistField fld )
    { return get( ws, fld ).list; }

    //! \throw BWlistException, DoNotCatchException
    inline void setList( const WebServer ws, const BlacklistField fld, const std::vector<std::string>& list )
    { get( ws, fld ).set( list ); }

    //! \throw BWlistException, DoNotCatchException
    inline void clearList( const WebServer ws, const BlacklistField fld )
    { getList( ws, fld ).clear(); }

    //! \throw BWlistException, DoNotCatchException
    inline void addItem( const WebServer ws, const BlacklistField fld, const std::string& item )
    { get( ws, fld ).add( item ); }

    //! \throw BWlistException
    inline void removeItem( const WebServer ws, const BlacklistField fld, const std::string& item )
    { get( ws, fld ).remove( item ); }

    //! \throw BWlistException
    inline int moveUpItem( const WebServer ws, const BlacklistField fld, const std::string& item )
    { return get( ws, fld ).moveUp( item ); }

    //! \throw BWlistException
    inline int moveDownItem( const WebServer ws, const BlacklistField fld, const std::string& item )
    { return get( ws, fld ).moveDown( item ); }
};

Q_DECLARE_METATYPE(Blacklists)


#endif // LOGDOCTOR__BLACKLISTS__H
