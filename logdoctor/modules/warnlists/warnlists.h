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

    //! Returns whether the requested warnlist is in use
    /*!
        \throw DoNotCatchException
    */
    inline bool isUsed( const WebServer ws, const WarnlistField fld )
    { return get( ws ).isUsed( fld ); }

    //! Sets the requested warnlist in the given in-use condition
    /*!
        \throw DoNotCatchException
    */
    inline void setUsed( const WebServer ws, const WarnlistField fld, const bool used )
    { get( ws ).setUsed( fld, used ); }

    //! Returns a reference to the requested warnlist
    /*!
        \throw DoNotCatchException
    */
    Warnlist& get( const WebServer ws );

    //! Returns a reference to the requested warnlist
    /*!
        \throw DoNotCatchException
    */
    inline WarnlistItem& get( const WebServer ws, const WarnlistField fld )
    { return get( ws ).get( fld ); }

    //! Returns a reference to the requested warnlist
    /*!
        \throw DoNotCatchException
    */
    inline std::vector<std::string>& getList( const WebServer ws, const WarnlistField fld )
    { return get( ws, fld ).list; }

    //! Returns a const reference to the requested warnlist
    /*!
        \throw DoNotCatchException
    */
    inline const std::vector<std::string>& getListConst( const WebServer ws, const WarnlistField fld )
    { return get( ws, fld ).list; }

    //! Replaces the requested warnlist with the one provided
    /*!
        \throw BWlistException, DoNotCatchException
    */
    inline void setList( const WebServer ws, const WarnlistField fld, const std::vector<std::string>& list )
    { get( ws, fld ).set( list ); }

    //! Clears the requested warnlist
    /*!
        \throw BWlistException, DoNotCatchException
    */
    inline void clearList( const WebServer ws, const WarnlistField fld )
    { getList( ws, fld ).clear(); }

    //! Adds the provided item to the requested warnlist
    /*!
        \throw BWlistException, DoNotCatchException
    */
    inline void addItem( const WebServer ws, const WarnlistField fld, const std::string& item )
    { get( ws, fld ).add( item ); }

    //! Remove the requested item from the requested warnlist
    /*!
        \throw BWlistException
    */
    inline void removeItem( const WebServer ws, const WarnlistField fld, const std::string& item )
    { get( ws, fld ).remove( item ); }

    //! Moves the requested item one position up in the requested warnlist
    /*!
        \throw BWlistException
    */
    inline int moveUpItem( const WebServer ws, const WarnlistField fld, const std::string& item )
    { return get( ws, fld ).moveUp( item ); }

    //! Moves the requested item one position down in the requested warnlist
    /*!
        \throw BWlistException
    */
    inline int moveDownItem( const WebServer ws, const WarnlistField fld, const std::string& item )
    { return get( ws, fld ).moveDown( item ); }
};


#endif // LOGDOCTOR__WARNLISTS__H
