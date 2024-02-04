#ifndef LOGDOCTOR__WARNLISTS__WARNLIST__H
#define LOGDOCTOR__WARNLISTS__WARNLIST__H


#include "modules/warnlists/modules/warnlist_item.h"


struct Warnlist final
{
    WarnlistItem method;
    WarnlistItem uri;
    WarnlistItem client;
    WarnlistItem user_agent;

    explicit Warnlist() noexcept;

    //! Returns a reference to the requested warnlist item
    /*!
        \throw DoNotCatchException
    */
    WarnlistItem& get( const WarnlistField field );

    //! Returns whether the requested warnlist is in use
    /*!
        \throw DoNotCatchException
    */
    inline bool isUsed( const WarnlistField field )
    { return get( field ).used; }

    //! Sets the requested warnlist in the given in-use condition
    /*!
        \throw DoNotCatchException
    */
    inline void setUsed( const WarnlistField field, const bool used )
    { get( field ).used = used; }
};

#endif // LOGDOCTOR__WARNLISTS__WARNLIST__H
