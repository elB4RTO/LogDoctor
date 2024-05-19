#ifndef LOGDOCTOR__CRAPCONF__LIB_H
#define LOGDOCTOR__CRAPCONF__LIB_H


enum ConfigsPage : unsigned char {
    General_Window            =  0,
    General_Dialogs           =  1,
    General_Charts_Appearance =  2,
    General_Charts_Speed      =  3,
    General_Charts_Count      =  4,
    General_Charts_Relational =  5,
    General_TextBrowser       =  6,
    General_Databases         =  7,
    General_Security          =  8,
    Logs_Defaults             =  9,
    Logs_Control              = 10,
    Logs_Apache2_Path         = 11,
    Logs_Apache2_Format       = 12,
    Logs_Apache2_Warnlists    = 13,
    Logs_Apache2_Blacklists   = 14,
    Logs_Nginx_Path           = 15,
    Logs_Nginx_Format         = 16,
    Logs_Nginx_Warnlists      = 17,
    Logs_Nginx_Blacklists     = 18,
    Logs_IIS_Path             = 19,
    Logs_IIS_Format           = 20,
    Logs_IIS_Warnlists        = 21,
    Logs_IIS_Blacklists       = 22,
};


#endif // LOGDOCTOR__CRAPCONF__LIB_H
