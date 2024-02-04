#ifndef LOGDOCTOR__BLACKLISTS__BLACKLIST__H
#define LOGDOCTOR__BLACKLISTS__BLACKLIST__H


#include "modules/blacklists/modules/blacklist_item.h"


struct Blacklist final
{
    BlacklistItem client;

    explicit Blacklist() noexcept;

    //! Returns a reference to the requested blacklist item
    /*!
        \throw DoNotCatchException
    */
    BlacklistItem& get( const BlacklistField field );

    //! Returns whether the requested blacklist is in use
    /*!
        \throw DoNotCatchException
    */
    inline bool isUsed( const BlacklistField field )
    { return get( field ).used; }

    //! Sets the requested blacklist in the given in-use condition
    /*!
        \throw DoNotCatchException
    */
    inline void setUsed( const BlacklistField field, const bool used )
    { get( field ).used = used; }
};


#endif // LOGDOCTOR__BLACKLISTS__BLACKLIST__H
