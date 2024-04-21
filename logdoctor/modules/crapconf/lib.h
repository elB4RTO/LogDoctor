#ifndef LOGDOCTOR__CRAPCONF__LIB_H
#define LOGDOCTOR__CRAPCONF__LIB_H


enum ConfigsPage : unsigned char {
    General_Window            =  0,
    General_Dialogs           =  1,
    General_Charts_Appearence =  2,
    General_Charts_Speed      =  3,
    General_Charts_Count      =  4,
    General_TextBrowser       =  5,
    General_Databases         =  6,
    Logs_Defaults             =  7,
    Logs_Control              =  8,
    Logs_Apache2_Path         =  9,
    Logs_Apache2_Format       = 10,
    Logs_Apache2_Warnlists    = 11,
    Logs_Apache2_Blacklists   = 12,
    Logs_Nginx_Path           = 13,
    Logs_Nginx_Format         = 14,
    Logs_Nginx_Warnlists      = 15,
    Logs_Nginx_Blacklists     = 16,
    Logs_IIS_Path             = 17,
    Logs_IIS_Format           = 18,
    Logs_IIS_Warnlists        = 19,
    Logs_IIS_Blacklists       = 20,
};


#endif // LOGDOCTOR__CRAPCONF__LIB_H
