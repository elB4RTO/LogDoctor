#ifndef LOGDOCTOR__SHARED_H
#define LOGDOCTOR__SHARED_H


#include <QObject>


//! TR
/*!
    Fictitious class for the translations
*/
class TR : public QObject
{
    Q_OBJECT
public:
    explicit TR( QObject* parent=nullptr ):QObject{parent}{}
};


const std::string BOOLS__FALSE { TR::tr("FALSE").toStdString() };
const std::string BOOLS__TRUE  { TR::tr("TRUE").toStdString()  };

const std::string WORDS__DATE { TR::tr("Date").toStdString() };
const std::string WORDS__TIME { TR::tr("Time").toStdString() };

const std::string FIELDS__WARNING        { TR::tr("Warning").toStdString()  };
const std::string FIELDS__PROTOCOL       { TR::tr("Protocol").toStdString() };
const std::string FIELDS__METHOD         { TR::tr("Method").toStdString()   };
const std::string FIELDS__URI            { TR::tr("URI").toStdString()      };
const std::string FIELDS__QUERY          { TR::tr("Query").toStdString()    };
const std::string FIELDS__RESPONSE_CODE  { TR::tr("Response code").toStdString()  };
const std::string FIELDS__TIME_TAKEN     { TR::tr("Time taken").toStdString()     };
const std::string FIELDS__BYTES_SENT     { TR::tr("Bytes sent").toStdString()     };
const std::string FIELDS__BYTES_RECEIVED { TR::tr("Bytes received").toStdString() };
const std::string FIELDS__REFERRER       { TR::tr("Referrer").toStdString()   };
const std::string FIELDS__COOKIE         { TR::tr("Cookie").toStdString()     };
const std::string FIELDS__CLIENT         { TR::tr("Client").toStdString()     };
const std::string FIELDS__USER_AGENT     { TR::tr("User-agent").toStdString() };

const std::string MONTHS__JANUARY   { TR::tr("January").toStdString()   };
const std::string MONTHS__FEBRUARY  { TR::tr("February").toStdString()  };
const std::string MONTHS__MARCH     { TR::tr("March").toStdString()     };
const std::string MONTHS__APRIL     { TR::tr("April").toStdString()     };
const std::string MONTHS__MAY       { TR::tr("May").toStdString()       };
const std::string MONTHS__JUNE      { TR::tr("June").toStdString()      };
const std::string MONTHS__JULY      { TR::tr("July").toStdString()      };
const std::string MONTHS__AUGUST    { TR::tr("August").toStdString()    };
const std::string MONTHS__SEPTEMBER { TR::tr("September").toStdString() };
const std::string MONTHS__OCTOBER   { TR::tr("October").toStdString()   };
const std::string MONTHS__NOVEMBER  { TR::tr("November").toStdString()  };
const std::string MONTHS__DECEMBER  { TR::tr("December").toStdString()  };

const std::string DAYS__SUNDAY    { TR::tr("Sunday").toStdString()    };
const std::string DAYS__MONDAY    { TR::tr("Monday").toStdString()    };
const std::string DAYS__TUESDAY   { TR::tr("Tuesday").toStdString()   };
const std::string DAYS__WEDNESDAY { TR::tr("Wednesday").toStdString() };
const std::string DAYS__THURSDAY  { TR::tr("Thursday").toStdString()  };
const std::string DAYS__FRIDAY    { TR::tr("Friday").toStdString()    };
const std::string DAYS__SATURDAY  { TR::tr("Saturday").toStdString()  };


#endif // LOGDOCTOR__SHARED_H
