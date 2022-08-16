#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QCloseEvent>

#include <QFontDatabase>
#include <QMessageBox>
#include <QTreeWidget>
#include <QChartView>

#include <thread>

#include "utilities.h"
#include "modules.h"
#include "tools/craplog/craplog.h"
#include "tools/crapview/crapview.h"
#include "tools/craphelp/craphelp.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent=nullptr );
    ~MainWindow();
    void closeEvent( QCloseEvent *event );

    //void operator()( int a );

private slots:
    // custom
    void wait_ActiveWindow();

    void update_Craplog_PerfData();

    // Qt's
    /////////////////
    //// CRAPLOG ////

    void on_button_LogFiles_ViewFile_clicked();

    void on_checkBox_LogFiles_CheckAll_stateChanged(int arg1);

    void on_button_LogFiles_RefreshList_clicked();

    void on_listLogFiles_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_listLogFiles_itemChanged(QTreeWidgetItem *item, int column);

    void on_button_LogFiles_Apache_clicked();

    void on_button_LogFiles_Nginx_clicked();

    void on_button_LogFiles_Iis_clicked();

    void on_button_MakeStats_Start_clicked();

    //////////////////
    //// CRAPVIEW ////

    //// WARNINGS ////

    void on_box_StatsWarn_WebServer_currentIndexChanged(int index);

    void on_box_StatsWarn_Year_currentIndexChanged(int index);

    void on_box_StatsWarn_Month_currentIndexChanged(int index);

    void on_box_StatsWarn_Day_currentIndexChanged(int index);

    void on_checkBox_StatsWarn_Hour_stateChanged(int state);

    void on_box_StatsWarn_Hour_currentIndexChanged(int index);

    void on_button_StatsWarn_Draw_clicked();

    void on_button_StatsWarn_Update_clicked();

    //// SPEED ////

    void on_box_StatsSpeed_WebServer_currentIndexChanged(int index);

    void on_box_StatsSpeed_Year_currentIndexChanged(int index);

    void on_box_StatsSpeed_Month_currentIndexChanged(int index);

    void on_box_StatsSpeed_Day_currentIndexChanged(int index);

    void on_button_StatsSpeed_Draw_clicked();

    //// COUNTS ////

    void on_box_StatsCount_WebServer_currentIndexChanged(int index);

    void on_box_StatsCount_Year_currentIndexChanged(int index);

    void on_box_StatsCount_Month_currentIndexChanged(int index);

    void on_box_StatsCount_Day_currentIndexChanged(int index);

    void on_button_StatsCount_Protocol_clicked();

    void on_button_StatsCount_Method_clicked();

    void on_button_StatsCount_Request_clicked();

    void on_button_StatsCount_Query_clicked();

    void on_button_StatsCount_Response_clicked();

    void on_button_StatsCount_Referrer_clicked();

    void on_button_StatsCount_Cookie_clicked();

    void on_button_StatsCount_UserAgent_clicked();

    void on_button_StatsCount_Client_clicked();

    //// DAY-TIME ////

    void on_box_StatsDay_WebServer_currentIndexChanged(int index);

    void on_box_StatsDay_LogsField_currentIndexChanged(int index);

    void on_box_StatsDay_FromYear_currentIndexChanged(int index);

    void on_box_StatsDay_FromMonth_currentIndexChanged(int index);

    void on_box_StatsDay_FromDay_currentIndexChanged(int index);

    void on_checkBox_StatsDay_Period_stateChanged(int state);

    void on_box_StatsDay_ToYear_currentIndexChanged(int index);

    void on_box_StatsDay_ToMonth_currentIndexChanged(int index);

    void on_box_StatsDay_ToDay_currentIndexChanged(int index);

    void on_button_StatsDay_Draw_clicked();

    //// RELATIONAL ////

    void on_box_StatsRelat_WebServer_currentIndexChanged(int index);

    void on_box_StatsRelat_LogsField_1_currentIndexChanged(int index);

    void on_box_StatsRelat_LogsField_2_currentIndexChanged(int index);

    void on_box_StatsRelat_FromYear_currentIndexChanged(int index);

    void on_box_StatsRelat_FromMonth_currentIndexChanged(int index);

    void on_box_StatsRelat_FromDay_currentIndexChanged(int index);

    void on_box_StatsRelat_ToYear_currentIndexChanged(int index);

    void on_box_StatsRelat_ToMonth_currentIndexChanged(int index);

    void on_box_StatsRelat_ToDay_currentIndexChanged(int index);

    void on_button_StatsRelat_Draw_clicked();

    void on_button_StatsGlob_Apache_clicked();

    void on_button_StatsGlob_Nginx_clicked();

    void on_button_StatsGlob_Iis_clicked();

    /////////////////
    //// CRAPSET ////

    //// WINDOW ////

    void on_checkBox_ConfWindow_Geometry_clicked(bool checked);

    void on_box_ConfWindow_Theme_currentIndexChanged(int index);

    //// DIALOGS ////

    void on_slider_ConfDialogs_General_sliderReleased();

    void on_slider_ConfDialogs_Logs_sliderReleased();

    void on_slider_ConfDialogs_Stats_sliderReleased();

    //// TEXT BROWSER ////

    void on_box_ConfTextBrowser_Font_currentIndexChanged(int index);

    void on_checkBox_ConfTextBrowser_WideLines_clicked(bool checked);

    void on_box_ConfTextBrowser_ColorScheme_currentIndexChanged(int index);

    //// CHARTS ////

    void on_box_ConfCharts_Theme_currentIndexChanged(int index);

    //// LOGS DEFAULTS ////

    void on_radio_ConfDefaults_Apache_toggled(bool checked);

    void on_radio_ConfDefaults_Nginx_toggled(bool checked);

    void on_radio_ConfDefaults_Iis_toggled(bool checked);

    //// LOGS CONTROL ////

    void on_checkBox_ConfControl_Usage_clicked(bool checked);

    void on_checkBox_ConfControl_Size_clicked(bool checked);

    void on_spinBox_ConfControl_Size_editingFinished();

    //// APACHE ////

    void on_inLine_ConfApache_Path_String_textChanged(const QString &arg1);

    void on_inLine_ConfApache_Path_String_returnPressed();

    void on_button_ConfApache_Path_Save_clicked();

    void on_inLine_ConfApache_Format_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Format_String_returnPressed();

    void on_button_ConfApache_Format_Save_clicked();

    void on_button_ConfApache_Format_Sample_clicked();

    void on_button_ConfApache_Format_Help_clicked();

    void on_box_ConfApache_Warnlist_Field_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfApache_Warnlist_Used_clicked(bool checked);

    void on_inLine_ConfApache_Warnlist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Warnlist_String_returnPressed();

    void on_button_ConfApache_Warnlist_Add_clicked();

    void on_list_ConfApache_Warnlist_List_itemSelectionChanged();

    void on_button_ConfApache_Warnlist_Remove_clicked();

    void on_button_ConfApache_Warnlist_Up_clicked();

    void on_button_ConfApache_Warnlist_Down_clicked();

    void on_box_ConfApache_Blacklist_Field_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfApache_Blacklist_Used_clicked(bool checked);

    void on_inLine_ConfApache_Blacklist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Blacklist_String_returnPressed();

    void on_button_ConfApache_Blacklist_Add_clicked();

    void on_list_ConfApache_Blacklist_List_itemSelectionChanged();

    void on_button_ConfApache_Blacklist_Remove_clicked();

    void on_button_ConfApache_Blacklist_Up_clicked();

    void on_button_ConfApache_Blacklist_Down_clicked();

    //// NGINX ////

    void on_inLine_ConfNginx_Path_String_textChanged(const QString &arg1);

    void on_inLine_ConfNginx_Path_String_returnPressed();

    void on_button_ConfNginx_Path_Save_clicked();

    void on_inLine_ConfNginx_Format_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Format_String_returnPressed();

    void on_button_ConfNginx_Format_Save_clicked();

    void on_button_ConfNginx_Format_Sample_clicked();

    void on_button_ConfNginx_Format_Help_clicked();

    void on_box_ConfNginx_Warnlist_Field_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfNginx_Warnlist_Used_clicked(bool checked);

    void on_inLine_ConfNginx_Warnlist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Warnlist_String_returnPressed();

    void on_button_ConfNginx_Warnlist_Add_clicked();

    void on_list_ConfNginx_Warnlist_List_itemSelectionChanged();

    void on_button_ConfNginx_Warnlist_Remove_clicked();

    void on_button_ConfNginx_Warnlist_Up_clicked();

    void on_button_ConfNginx_Warnlist_Down_clicked();

    void on_box_ConfNginx_Blacklist_Field_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfNginx_Blacklist_Used_clicked(bool checked);

    void on_inLine_ConfNginx_Blacklist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Blacklist_String_returnPressed();

    void on_button_ConfNginx_Blacklist_Add_clicked();

    void on_list_ConfNginx_Blacklist_List_itemSelectionChanged();

    void on_button_ConfNginx_Blacklist_Remove_clicked();

    void on_button_ConfNginx_Blacklist_Up_clicked();

    void on_button_ConfNginx_Blacklist_Down_clicked();

    //// IIS ////

    void on_inLine_ConfIis_Path_String_textChanged(const QString &arg1);

    void on_inLine_ConfIis_Path_String_returnPressed();

    void on_button_ConfIis_Path_Save_clicked();

    void on_radio_ConfIis_Format_W3C_toggled(bool checked);

    void on_radio_ConfIis_Format_NCSA_toggled(bool checked);

    void on_radio_ConfIis_Format_IIS_toggled(bool checked);

    void on_inLine_ConfIis_Format_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfIis_Format_String_returnPressed();

    void on_button_ConfIis_Format_Save_clicked();

    void on_button_ConfIis_Format_Sample_clicked();

    void on_button_ConfIis_Format_Help_clicked();

    void on_box_ConfIis_Warnlist_Field_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfIis_Warnlist_Used_clicked(bool checked);

    void on_inLine_ConfIis_Warnlist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfIis_Warnlist_String_returnPressed();

    void on_button_ConfIis_Warnlist_Add_clicked();

    void on_list_ConfIis_Warnlist_List_itemSelectionChanged();

    void on_button_ConfIis_Warnlist_Remove_clicked();

    void on_button_ConfIis_Warnlist_Up_clicked();

    void on_button_ConfIis_Warnlist_Down_clicked();

    void on_box_ConfIis_Blacklist_Field_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfIis_Blacklist_Used_clicked(bool checked);

    void on_inLine_ConfIis_Blacklist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfIis_Blacklist_String_returnPressed();

    void on_button_ConfIis_Blacklist_Add_clicked();

    void on_list_ConfIis_Blacklist_List_itemSelectionChanged();

    void on_button_ConfIis_Blacklist_Remove_clicked();

    void on_button_ConfIis_Blacklist_Up_clicked();

    void on_button_ConfIis_Blacklist_Down_clicked();

    void on_inLine_ConfDatabases_Data_Path_textChanged(const QString &arg1);

    void on_inLine_ConfDatabases_Data_Path_returnPressed();

    void on_button_ConfDatabases_Data_Save_clicked();

    void on_inLine_ConfDatabases_Hashes_Path_textChanged(const QString &arg1);

    void on_inLine_ConfDatabases_Hashes_Path_returnPressed();

    void on_button_ConfDatabases_Hashes_Save_clicked();

private:
    Ui::MainWindow *ui;

    // web servers ID constants
    const unsigned int APACHE_ID=11, NGINX_ID=12, IIS_ID=13;

    // operating system
    // 1: linux, 2:windows, 3:mac
    #if defined( Q_OS_UNIX )
        // Unix-like systems: Linux, BSD and SysV
        const unsigned int OS = 1;
    #elif defined( Q_OS_WIN )
        // Microsoft Windows systems
        const unsigned int this->OS = 2;
    #elif defined( Q_OS_DARWIN )
        // Darwin-based systems: macOS, macOS, iOS, watchOS and tvOS.
        const unsigned int this->OS = 3;
    #else
        #error "System not supported"
    #endif

    void defineOSspec();
    std::string logdoc_path;


    ////////////////////////
    //// CONFIGURATIONS ////
    ////////////////////////
    std::string configs_path;
    void readConfigs();
    void writeConfigs();
    // string to bool and vice versa
    const std::unordered_map<std::string, bool> s2b = { {"true",true}, {"false",false} };
    const std::unordered_map<bool, std::string> b2s = { {true,"true"}, {false,"false"} };
    // language
    std::string language = "en";
    void updateUiLanguage();
    // window geometry
    const std::string geometryToString();
    void geometryFromString( const std::string& geometry );
    // quantoty of informational dialogs to display
    int dialogs_level = 2; // 0: essential, 1: usefull, 2: explanatory
    // default web server
    int default_ws = 11;
    // list to string and vice versa
    const std::string list2string( const std::vector<std::string>& list, const bool& user_agent=false );
    const std::vector<std::string> string2list( const std::string& string, const bool& user_agent=false );

    ////////////////
    //// CHECKS ////
    void makeInitialChecks();

    //////////////////
    //// GRAPHICS ////
    //////////////////
    // remember window position and sizes
    bool remember_window = true;
    // themes
    int window_theme_id = 0,
        charts_theme_id = 0;
    void updateUiTheme();
    const std::vector<QChart::ChartTheme> CHARTS_THEMES = {
        QChart::ChartTheme::ChartThemeLight,
        QChart::ChartTheme::ChartThemeDark,
        QChart::ChartTheme::ChartThemeBrownSand,
        QChart::ChartTheme::ChartThemeBlueCerulean
    };
    // color schemes
    std::unordered_map<int, std::unordered_map<std::string, QString>> TB_COLOR_SCHEMES;
    std::unordered_map<std::string, QColor> COLORS;
    // fonts
    std::unordered_map<std::string, QFont>  FONTS;
    int font_size       = 13,
        font_size_big   = 16,
        font_size_small = 10;
    // base font families, to build fonts from
    QString main_font_family,
            alternative_font_family,
            script_font_family;


    /////////////////////
    //// GENERAL USE ////
    /////////////////////
    // get a printable size, from Bytes to the best fit
    const QString printableSize(  const int& bytes ),
                  printableSpeed( const int& bytes, const int& secs ),
                  printableTime(  const int& seconds );
    // resolve a path
    const std::string resolvePath( const std::string& path );
    const std::string basePath( const std::string& path );

    ///////////////////
    //// DATABASES ////
    ///////////////////
    std::string db_data_path,
                db_hashes_path;


    /////////////////
    //// CRAPLOG ////
    /////////////////
    Craplog craplog;
    std::thread craplog_thread;
    QTimer *craplog_timer;
    std::chrono::system_clock::time_point craplog_timer_start;
    std::chrono::system_clock::duration craplog_timer_elapsed;
    void craplogStarted();
    void craplogFinished();

    //////////////
    //// LOGS ////
    // web servers related
    bool loading_LogsList = false;
    void disableAllButtons_LogFiles_WS(),
         enableAllButtons_LogFiles_WS();
    // logs list related
    bool hide_used_files = false;
    // text browser related
    TextBrowser TB;

    //////////////////////////
    //// LOGS PERFORMANCE ////
    void update_MakeStats_labels(),
         update_MakeStats_graphs(),
         reset_MakeStats_labels(),
         reset_MakeStats_graphs();


    //////////////////
    //// CRAPVIEW ////
    //////////////////
    Crapview crapview;
    // refresh dates: query a new collection from the db and apply to the tabs
    void refreshStatsDates();
    // check if drawing conditions are met
    void checkStatsWarnDrawable();
    void checkStatsSpeedDrawable();
    void checkStatsCountDrawable();
    void checkStatsDayDrawable();
    void checkStatsRelatDrawable();
    // polishing
    void resetStatsCountButtons();
    // draw
    void drawStatsCount( const QString& field );
    // globals
    void makeStatsGlobals( const QString& web_server );


    /////////////////
    //// CRAPSET ////
    /////////////////
    void refreshTextBrowserPreview();
    void refreshChartsPreview();
    const int getIisLogsModule();


    //////////////////
    //// CRAPHELP ////
    //////////////////
    Craphelp* craphelp = new Craphelp();
    void showHelp( const std::string& file_name );

};
#endif // MAINWINDOW_H
