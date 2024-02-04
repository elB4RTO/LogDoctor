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

    //! Returns whether the requested blacklist is in use
    /*!
        \throw DoNotCatchException
    */
    inline bool isUsed( const WebServer ws, const BlacklistField fld )
    { return get( ws ).isUsed( fld ); }

    //! Sets the requested blacklist in the given in-use condition
    /*!
        \throw DoNotCatchException
    */
    inline void setUsed( const WebServer ws, const BlacklistField fld, const bool used )
    { get( ws ).setUsed( fld, used ); }

    //! Returns a const reference to the requested blacklist
    /*!
        \throw DoNotCatchException
    */
    const Blacklist& getConst( const WebServer ws ) const;

    //! Returns a reference to the requested blacklist
    /*!
        \throw DoNotCatchException
    */
    Blacklist& get( const WebServer ws );

    //! Returns a reference to the requested blacklist
    /*!
        \throw DoNotCatchException
    */
    inline BlacklistItem& get( const WebServer ws, const BlacklistField fld )
    { return get( ws ).get( fld ); }

    //! Returns a reference to the requested blacklist
    /*!
        \throw DoNotCatchException
    */
    inline std::vector<std::string>& getList( const WebServer ws, const BlacklistField fld )
    { return get( ws, fld ).list; }

    //! Returns a const reference to the requested blacklist
    /*!
        \throw DoNotCatchException
    */
    inline const std::vector<std::string>& getListConst( const WebServer ws, const BlacklistField fld )
    { return get( ws, fld ).list; }

    //! Replaces the requested blacklist with the one provided
    /*!
        \throw BWlistException, DoNotCatchException
    */
    inline void setList( const WebServer ws, const BlacklistField fld, const std::vector<std::string>& list )
    { get( ws, fld ).set( list ); }

    //! Clears the requested blacklist
    /*!
        \throw BWlistException, DoNotCatchException
    */
    inline void clearList( const WebServer ws, const BlacklistField fld )
    { getList( ws, fld ).clear(); }

    //! Adds the provided item to the requested blacklist
    /*!
        \throw BWlistException, DoNotCatchException
    */
    inline void addItem( const WebServer ws, const BlacklistField fld, const std::string& item )
    { get( ws, fld ).add( item ); }

    //! Remove the requested item from the requested blacklist
    /*!
        \throw BWlistException
    */
    inline void removeItem( const WebServer ws, const BlacklistField fld, const std::string& item )
    { get( ws, fld ).remove( item ); }

    //! Moves the requested item one position up in the requested blacklist
    /*!
        \throw BWlistException
    */
    inline int moveUpItem( const WebServer ws, const BlacklistField fld, const std::string& item )
    { return get( ws, fld ).moveUp( item ); }

    //! Moves the requested item one position down in the requested blacklist
    /*!
        \throw BWlistException
    */
    inline int moveDownItem( const WebServer ws, const BlacklistField fld, const std::string& item )
    { return get( ws, fld ).moveDown( item ); }
};

Q_DECLARE_METATYPE(Blacklists)


#endif // LOGDOCTOR__BLACKLISTS__H
