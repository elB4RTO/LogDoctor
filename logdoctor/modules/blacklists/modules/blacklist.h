#ifndef LOGDOCTOR__BLACKLISTS__BLACKLIST__H
#define LOGDOCTOR__BLACKLISTS__BLACKLIST__H


#include "modules/blacklists/modules/blacklist_item.h"


struct Blacklist final
{
    BlacklistItem client;

    explicit Blacklist() noexcept;

    //! \throw DoNotCatchException
    BlacklistItem& get( const BlacklistField field );

    //! \throw DoNotCatchException
    inline bool isUsed( const BlacklistField field )
    { return get( field ).used; }

    //! \throw DoNotCatchException
    inline void setUsed( const BlacklistField field, const bool used )
    { get( field ).used = used; }
};


#endif // LOGDOCTOR__BLACKLISTS__BLACKLIST__H
