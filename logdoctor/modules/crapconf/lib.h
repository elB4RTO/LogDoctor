#ifndef LOGDOCTOR__CRAPCONF__LIB_H
#define LOGDOCTOR__CRAPCONF__LIB_H


enum ConfigsPage : unsigned char {
    General_Window          =  0,
    General_Dialogs         =  1,
    General_Charts          =  2,
    General_TextBrowser     =  3,
    General_Databases       =  4,
    Logs_Defaults           =  5,
    Logs_Control            =  6,
    Logs_Apache2_Path       =  7,
    Logs_Apache2_Format     =  8,
    Logs_Apache2_Warnlists  =  9,
    Logs_Apache2_Blacklists = 10,
    Logs_Nginx_Path         = 11,
    Logs_Nginx_Format       = 12,
    Logs_Nginx_Warnlists    = 13,
    Logs_Nginx_Blacklists   = 14,
    Logs_IIS_Path           = 15,
    Logs_IIS_Format         = 16,
    Logs_IIS_Warnlists      = 17,
    Logs_IIS_Blacklists     = 18,
};


#endif // LOGDOCTOR__CRAPCONF__LIB_H
