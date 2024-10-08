#ifndef LOGDOCTOR__MAINWINDOW_H
#define LOGDOCTOR__MAINWINDOW_H


#include <QMainWindow>

#include "utilities/strings.h"

#include "modules/tb.h"

#include "modules/security/path.h"

#include "modules/database/database.h"

#include "modules/blacklists/blacklists.h"
#include "modules/warnlists/warnlists.h"

#include "modules/craplog/craplog.h"
#include "modules/crapview/crapview.h"


class Crappath;
class Craphelp;
class Crapup;
class Crapinfo;
class Crapnote;

class Changelog;

class CrissCross;
class SnakeGame;

class QCloseEvent;
class QTranslator;
class QTreeWidgetItem;

enum class LogField;
enum class ColorsScheme : unsigned char;

enum ConfigsPage : unsigned char;


namespace Ui {
    class MainWindow;
}


//! MainWindow
/*!
    The parent window
*/
class MainWindow final : public QMainWindow
{
    Q_OBJECT

    // current version of LogDoctor
    const float version{ 4.02f };

public:

    explicit MainWindow( QWidget* parent=nullptr );
    Q_DISABLE_COPY_MOVE(MainWindow)

signals:

    void refreshLogs();

    void runCraplog( const Blacklists& blacklists );


private slots:

    ///////////////////
    //// OVERRIDES ////

    void closeEvent( QCloseEvent* event ) override;


    //////////////
    //// INIT ////

    void waitActiveWindow(); // CUSTOM


    //////////////
    //// TABS ////

    void on_button_Tab_Log_clicked();

    void on_button_Tab_View_clicked();

    void on_button_Tab_Conf_clicked();

    //// STATS ////

    void on_button_Tab_StatsWarn_clicked();

    void on_button_Tab_StatsSpeed_clicked();

    void on_button_Tab_StatsCount_clicked();

    void on_button_Tab_StatsDay_clicked();

    void on_button_Tab_StatsRelat_clicked();

    void on_button_Tab_StatsGlob_clicked();


    /////////////////
    //// CRAPLOG ////

    void appendToLogsList( const LogFile& log_file ); // CUSTOM

    void refreshFinished(); // CUSTOM

    void updatePerfsLabels(); // CUSTOM

    void craplogFinished(); // CUSTOM

    void afterCraplogFinished(); // CUSTOM

    void on_button_Logs_Down_clicked();

    void on_button_Logs_Up_clicked();

    void on_button_LogFiles_ViewFile_clicked();

    void on_checkBox_LogFiles_CheckAll_stateChanged(int arg1);

    void on_button_LogFiles_RefreshList_clicked();

    void on_listLogFiles_itemDoubleClicked(QTreeWidgetItem* item, int column);

    void on_listLogFiles_itemChanged(QTreeWidgetItem* item, int column);

    void on_button_LogFiles_Apache_clicked();

    void on_button_LogFiles_Nginx_clicked();

    void on_button_LogFiles_Iis_clicked();

    void on_button_MakeStats_Start_clicked();


    //////////////////
    //// CRAPVIEW ////

    void drawStatsWarn(); // CUSTOM

    void drawStatsSpeed(); // CUSTOM

    void drawStatsCount(); // CUSTOM

    void drawStatsDay(); // CUSTOM

    void drawStatsRelat(); // CUSTOM

    void drawStatsGlobals(); // CUSTOM

    //// WARNINGS ////

    void on_box_StatsWarn_WebServer_currentIndexChanged(int index);

    void on_box_StatsWarn_Year_currentIndexChanged(int index);

    void on_box_StatsWarn_Month_currentIndexChanged(int index);

    void on_box_StatsWarn_Day_currentIndexChanged(int index);

    void on_checkBox_StatsWarn_Hour_stateChanged(int state);

    void on_box_StatsWarn_Hour_currentIndexChanged(int index);

    void on_button_StatsWarn_Draw_clicked();

    //// SPEED ////

    void on_box_StatsSpeed_WebServer_currentIndexChanged(int index);

    void on_box_StatsSpeed_Year_currentIndexChanged(int index);

    void on_box_StatsSpeed_Month_currentIndexChanged(int index);

    void on_box_StatsSpeed_Day_currentIndexChanged(int index);

    void on_inLine_StatsSpeed_Protocol_textChanged(const QString& arg1);

    void on_inLine_StatsSpeed_Method_textChanged(const QString& arg1);

    void on_inLine_StatsSpeed_Uri_textChanged(const QString& arg1);

    void on_inLine_StatsSpeed_Query_textChanged(const QString& arg1);

    void on_inLine_StatsSpeed_Response_textChanged(const QString& arg1);

    void on_button_StatsSpeed_Draw_clicked();

    //// COUNTS ////

    void on_box_StatsCount_WebServer_currentIndexChanged(int index);

    void on_box_StatsCount_Year_currentIndexChanged(int index);

    void on_box_StatsCount_Month_currentIndexChanged(int index);

    void on_box_StatsCount_Day_currentIndexChanged(int index);

    void on_button_StatsCount_Protocol_clicked();

    void on_button_StatsCount_Method_clicked();

    void on_button_StatsCount_Uri_clicked();

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

    void on_inLine_StatsDay_Filter_textChanged(const QString& arg1);

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

    void on_inLine_StatsRelat_Filter_1_textChanged(const QString& arg1);

    void on_inLine_StatsRelat_Filter_2_textChanged(const QString& arg1);

    void on_button_StatsRelat_Draw_clicked();

    //// GLOBALS ////

    void on_button_StatsGlob_Apache_clicked();

    void on_button_StatsGlob_Nginx_clicked();

    void on_button_StatsGlob_Iis_clicked();


    //////////////////
    //// CRAPCONF ////

    void on_tree_ConfSections_itemClicked(QTreeWidgetItem *item, int column);

    void on_tree_ConfSections_itemDoubleClicked(QTreeWidgetItem *item, int column);

    //// WINDOW ////

    void on_checkBox_ConfWindow_Geometry_clicked(bool checked);

    void on_box_ConfWindow_Theme_currentIndexChanged(int index);

    void on_box_ConfWindow_Icons_currentIndexChanged(int index);

    void on_box_ConfWindow_Language_currentIndexChanged(int index);

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

    void on_box_ConfCharts_Speed_TimeInterval_currentTextChanged(const QString& arg1);

    void on_box_ConfCharts_Speed_TimeFormat_currentTextChanged(const QString& arg1);

    void on_doubleSpinBox_ConfCharts_Count_PieSize_valueChanged(double arg1);

    void on_spinBox_ConfCharts_Count_MaxSlices_valueChanged(int arg1);

    void on_box_ConfCharts_Relat_TimeFormat_currentTextChanged(const QString& arg1);

    //// DATABASES ////

    void on_tool_ConfDatabases_Data_Dialog_clicked();

    void on_inLine_ConfDatabases_Data_Path_textChanged(const QString& arg1);

    void on_inLine_ConfDatabases_Data_Path_returnPressed();

    void on_button_ConfDatabases_Data_Save_clicked();

    void on_tool_ConfDatabases_Hashes_Dialog_clicked();

    void on_inLine_ConfDatabases_Hashes_Path_textChanged(const QString& arg1);

    void on_inLine_ConfDatabases_Hashes_Path_returnPressed();

    void on_button_ConfDatabases_Hashes_Save_clicked();

    void on_checkBox_ConfDatabases_DoBackup_clicked(bool checked);

    void on_spinBox_ConfDatabases_NumBackups_valueChanged(int arg1);

    //// SECURITY ////

    void on_checkBox_ConfSecurity_Paths_Symlinks_toggled(bool checked);

    //// LOGS DEFAULTS ////

    void on_radio_ConfDefaults_Apache_toggled(bool checked);

    void on_radio_ConfDefaults_Nginx_toggled(bool checked);

    void on_radio_ConfDefaults_Iis_toggled(bool checked);

    //// LOGS CONTROL ////

    void on_checkBox_ConfControl_Usage_clicked(bool checked);

    void on_checkBox_ConfControl_Size_clicked(bool checked);

    void on_spinBox_ConfControl_Size_editingFinished();

    //// APACHE ////

    void on_tool_ConfApache_Path_Dialog_clicked();

    void on_inLine_ConfApache_Path_String_textChanged(const QString& arg1);

    void on_inLine_ConfApache_Path_String_returnPressed();

    void on_button_ConfApache_Path_Save_clicked();

    void on_inLine_ConfApache_Format_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Format_String_returnPressed();

    void on_button_ConfApache_Format_Save_clicked();

    void on_button_ConfApache_Format_Sample_clicked();

    void on_button_ConfApache_Format_Help_clicked();

    void on_box_ConfApache_Warnlist_Field_currentTextChanged(const QString& arg1);

    void on_checkBox_ConfApache_Warnlist_Used_clicked(bool checked);

    void on_inLine_ConfApache_Warnlist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Warnlist_String_returnPressed();

    void on_button_ConfApache_Warnlist_Add_clicked();

    void on_list_ConfApache_Warnlist_List_itemSelectionChanged();

    void on_button_ConfApache_Warnlist_Remove_clicked();

    void on_button_ConfApache_Warnlist_Up_clicked();

    void on_button_ConfApache_Warnlist_Down_clicked();

    void on_box_ConfApache_Blacklist_Field_currentTextChanged(const QString& arg1);

    void on_checkBox_ConfApache_Blacklist_Used_clicked(bool checked);

    void on_inLine_ConfApache_Blacklist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Blacklist_String_returnPressed();

    void on_button_ConfApache_Blacklist_Add_clicked();

    void on_list_ConfApache_Blacklist_List_itemSelectionChanged();

    void on_button_ConfApache_Blacklist_Remove_clicked();

    void on_button_ConfApache_Blacklist_Up_clicked();

    void on_button_ConfApache_Blacklist_Down_clicked();

    //// NGINX ////

    void on_tool_ConfNginx_Path_Dialog_clicked();

    void on_inLine_ConfNginx_Path_String_textChanged(const QString& arg1);

    void on_inLine_ConfNginx_Path_String_returnPressed();

    void on_button_ConfNginx_Path_Save_clicked();

    void on_inLine_ConfNginx_Format_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Format_String_returnPressed();

    void on_button_ConfNginx_Format_Save_clicked();

    void on_button_ConfNginx_Format_Sample_clicked();

    void on_button_ConfNginx_Format_Help_clicked();

    void on_box_ConfNginx_Warnlist_Field_currentTextChanged(const QString& arg1);

    void on_checkBox_ConfNginx_Warnlist_Used_clicked(bool checked);

    void on_inLine_ConfNginx_Warnlist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Warnlist_String_returnPressed();

    void on_button_ConfNginx_Warnlist_Add_clicked();

    void on_list_ConfNginx_Warnlist_List_itemSelectionChanged();

    void on_button_ConfNginx_Warnlist_Remove_clicked();

    void on_button_ConfNginx_Warnlist_Up_clicked();

    void on_button_ConfNginx_Warnlist_Down_clicked();

    void on_box_ConfNginx_Blacklist_Field_currentTextChanged(const QString& arg1);

    void on_checkBox_ConfNginx_Blacklist_Used_clicked(bool checked);

    void on_inLine_ConfNginx_Blacklist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Blacklist_String_returnPressed();

    void on_button_ConfNginx_Blacklist_Add_clicked();

    void on_list_ConfNginx_Blacklist_List_itemSelectionChanged();

    void on_button_ConfNginx_Blacklist_Remove_clicked();

    void on_button_ConfNginx_Blacklist_Up_clicked();

    void on_button_ConfNginx_Blacklist_Down_clicked();

    //// IIS ////

    void on_tool_ConfIis_Path_Dialog_clicked();

    void on_inLine_ConfIis_Path_String_textChanged(const QString& arg1);

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

    void on_box_ConfIis_Warnlist_Field_currentTextChanged(const QString& arg1);

    void on_checkBox_ConfIis_Warnlist_Used_clicked(bool checked);

    void on_inLine_ConfIis_Warnlist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfIis_Warnlist_String_returnPressed();

    void on_button_ConfIis_Warnlist_Add_clicked();

    void on_list_ConfIis_Warnlist_List_itemSelectionChanged();

    void on_button_ConfIis_Warnlist_Remove_clicked();

    void on_button_ConfIis_Warnlist_Up_clicked();

    void on_button_ConfIis_Warnlist_Down_clicked();

    void on_box_ConfIis_Blacklist_Field_currentTextChanged(const QString& arg1);

    void on_checkBox_ConfIis_Blacklist_Used_clicked(bool checked);

    void on_inLine_ConfIis_Blacklist_String_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfIis_Blacklist_String_returnPressed();

    void on_button_ConfIis_Blacklist_Add_clicked();

    void on_list_ConfIis_Blacklist_List_itemSelectionChanged();

    void on_button_ConfIis_Blacklist_Remove_clicked();

    void on_button_ConfIis_Blacklist_Up_clicked();

    void on_button_ConfIis_Blacklist_Down_clicked();


    //////////////
    //// MENU ////

    //// TOOLS ////

    void menu_actionBlockNote_triggered();

    //// UTILITIES ////

    void menu_actionInfos_triggered();

    void menu_actionChangelog_triggered();

    void menu_actionCheckUpdates_triggered();

    //// GAMES ////

    void menu_actionCrissCross_triggered();

    void menu_actionSnake_triggered();


private:
    QSharedPointer<Ui::MainWindow> ui;

    QString wsFromIndex( const int index ) const;
    WebServer wsEnumFromIndex( const int index ) const;


    //////////////////////////
    //// OPERATING SYSTEM ////

    const std::string home_path{ StringOps::rstrip( QStandardPaths::locate( QStandardPaths::HomeLocation, "", QStandardPaths::LocateDirectory ).toStdString(), '/' ) };

    #if defined( Q_OS_MACOS )
        const PathHandler configs_path { this->home_path + "/Lybrary/Preferences/LogDoctor/logdoctor.conf" };
        const PathHandler logdoc_path  { this->home_path + "/Lybrary/Application Support/LogDoctor" };
    #elif defined( Q_OS_WINDOWS )
        const PathHandler configs_path { this->home_path + "/AppData/Local/LogDoctor/logdoctor.conf" };
        const PathHandler logdoc_path  { this->home_path + "/AppData/Local/LogDoctor" };
    #elif defined( Q_OS_LINUX ) || defined( Q_OS_BSD4 )
        const PathHandler configs_path { this->home_path + "/.config/LogDoctor/logdoctor.conf" };
        const PathHandler logdoc_path  { std::string("/usr/share/LogDoctor") };
    #else
        #error "System not supported"
    #endif


    //! Defines OS specific stuff
    void defineOSspec();


    ////////////////////////
    //// CONFIGURATIONS ////
    ////////////////////////

    //! Reads the configurations file and apply the resulting configuration
    void readConfigs();

    //! Writes the current configuration on file
    void writeConfigs();

    //! Converts a list of items to a string
    /*!
        \param list The list to stringify
        \param user_agents Whether to apply the special rule to parse user-agents lists or not
        \return The resulting string
        \see writeConfigs()
    */
    std::string list2string( const std::vector<std::string>& list, const bool user_agent=false ) const;

    //! Retrieves a list of items from the given string
    /*!
        \param list The list to stringify
        \param user_agents Whether to apply the special rule to parse user-agents lists or not
        \return The resulting list
        \see readConfigs()
    */
    std::vector<std::string> string2list( const std::string& string, const bool user_agent=false ) const;

    // string to bool and vice versa
    const std::unordered_map<std::string, bool> s2b{ {"true",true}, {"false",false} };
    const std::unordered_map<bool, std::string> b2s{ {true,"true"}, {false,"false"} };


    //////////////////
    //// LANGUAGE ////

    QTranslator translator;

    std::string language{ "en_GB" };

    const std::vector<std::string> available_languages{
        "ar_AR", "az_AZ", "bg_BG", "bn_BN", "ca_SP", "cs_CZ", "da_DA", "de_GE", "el_GR",
        "en_GB", "eo_EO", "es_SP", "et_ES", "fa_PE", "fi_FI", "fr_FR", "ga_IR", "he_HE",
        "hi_IN", "hu_HU", "id_ID", "it_IT", "ja_JP", "ko_KO", "lt_LI", "lv_LA", "ms_MA",
        "nb_NB", "nl_NL", "pl_PL", "pt_BR", "pt_PT", "ro_RO", "ru_RU", "sk_SK", "sl_SL",
        "sq_AL", "sv_SV", "th_TH", "tl_PH", "tr_TR", "uk_UK", "ur_PA", "zh_CN", "zt_CN"
    };

    //! Translates the UI to the selected language
    void updateUiLanguage();

    //! Unchecks all the languages in the menu
    void uncheckAllLanguageMenuEntries();


    /////////////////////////
    //// WINDOW GEOMETRY ////

    //! Converts the window's geometry to string
    /*!
        \see writeConfigs()
    */
    std::string geometryToString() const;

    //! Retrieves the window geometry from the given string
    /*!
        \throw std::exception
        \see readConfigs()
    */
    void setGeometryFromString( const std::string& geometry );


    /////////////////
    //// DIALOGS ////

    // quantoty of informational dialogs to display
    DialogsLevel dialogs_level{ DL_NORMAL };

    //! Retrieves the dialogs level from the given string
    /*!
        \throw std::exception
        \see readConfigs()
    */
    void setDialogsLevelFromString( const std::string& dialogs_level );

    //! Returns the dialogs level corresponding to the given number
    /*!
        \throw DoNotCatchException
    */
    DialogsLevel dialogsLevelFromInt( const int dialogs_level );


    ////////////////////
    //// WEB SERVER ////

    // default web server
    WebServer default_web_server{ WS_APACHE };

    //! Retrieves the Web Server from the given string
    /*!
        \throw DoNotCatchException
        \see readConfigs()
    */
    void setWebServerFromString( const std::string& web_server );


    //////////////////
    //// GRAPHICS ////
    //////////////////

    // remember window position and sizes
    bool remember_window{ true };

    //! Auto-detects the icon-set to use depending on the current window theme
    /*!
        \throw DoNotCatchException
    */
    void detectIconsTheme();

    //! Updates the icons on the window
    /*!
        \throw DoNotCatchException
    */
    void updateUiIcons();

    //! Updates the window theme
    /*!
        \throw DoNotCatchException
    */
    void updateUiTheme();

    //! Updates the fonts on the window
    /*!
        \throw std::exception
    */
    void updateUiFonts();

    const std::vector<QChart::ChartTheme> CHARTS_THEMES{
        QChart::ChartTheme::ChartThemeLight,
        QChart::ChartTheme::ChartThemeDark
    };

    // color schemes
    const std::unordered_map<ColorsScheme, std::unordered_map<std::string, QString>> tb_colors_schemes;

    // colors
    const std::unordered_map<std::string, QColor> colors;

    // fonts
    std::unordered_map<std::string, QFont>  fonts;

    int font_size       { 13 };
    int font_size_big   { 16 };
    int font_size_small { 10 };

    // base font families, to build fonts from
    QString main_font_family;
    QString alternative_font_family;
    QString script_font_family;

    // fallback stylesheets
    QString stylesheet_lineedit;

    // errors stylesheets
    const QString stylesheet_lineedit_err{ "QLineEdit{ background-color: rgb(223, 31, 31) }" };


    /////////////////////
    //// GENERAL USE ////
    /////////////////////

    QScopedPointer<QTimer> waiter_timer;
    std::chrono::system_clock::time_point waiter_timer_start;
    /*std::chrono::system_clock::duration   waiter_timer_elapsed;*/


    ////////////////
    //// CHECKS ////
    ////////////////

    bool initiating{ true };

    //! Makes the initial integrity checks
    void makeInitialChecks();


    ///////////////////
    //// DATABASES ////
    ///////////////////

    DatabaseHandler dbHandler;

    // true if a process edited the database and so a backup is required
    bool db_edited{ false };

    // user-defined configuration: whether to make backups or not
    bool db_do_backup{ true };

    // user-defined configuration: maximum number of backups to keep
    unsigned db_backups_number{ 3 };

    //! Backs-up the logs data collection database
    void backupDatabase() const;

    PathHandler db_data_path;
    PathHandler db_hashes_path;

    // true when a process is working on a db
    bool db_working{ false };

    //! Called when a member begins/ends performing operations on the database
    void setDbWorkingState( const bool working );

    //! Weak check on the logs data collection database
    /*!
        Returns true if the database is free (no other process is running
        that may access it for read/write operations) and it exists
        \see checkDataDB
    */
    bool dbUsable();

    // false if the logs data collection database is invalid
    bool db_ok{ true };

    //! Checks the integrity of the logs data collection database
    bool checkDataDB();


    //////////////////////////////
    //// BLACKLIST / WARNLIST ////
    //////////////////////////////

    Blacklists blacklists;
    Warnlists warnlists;

    BlacklistField blacklistFieldFromString( const QString& str );
    WarnlistField warnlistFieldFromString( const QString& str );


    //////////////////
    //// CRAPTABS ////
    //////////////////

    void switchMainTab( const int new_index );


    /////////////////
    //// CRAPLOG ////
    /////////////////

    Craplog craplog;

    void craplogStarted();

    void checkMakeStats_Makable();


    //////////////
    //// LOGS ////

    bool loading_LogsList{ false };

    void disableAllButtons_LogFiles_WS();
    void enableAllButtons_LogFiles_WS();

    bool hide_used_files{ false };
    bool refreshing_list{ false };


    //////////////////////
    //// TEXT BROWSER ////

    TextBrowser TB;


    //////////////////////////
    //// LOGS PERFORMANCE ////

    void resetPerfsLabels();

    bool force_updating_labels{ false };


    //////////////////
    //// CRAPVIEW ////
    //////////////////

    Crapview crapview;

    QScopedPointer<QTimer> crapview_timer;

    // change tab
    void switchStatsTab( const int new_index );

    //! Queries the available dates from the db and apply to the tabs
    /*!
        \see Crapview::refreshDates()
    */
    void refreshStatsDates();

    // check if drawing conditions are met
    void checkStatsWarnDrawable();
    void checkStatsSpeedDrawable();
    void checkStatsCountDrawable();
    void checkStatsDayDrawable();
    void checkStatsRelatDrawable();

    // count
    QString count_fld;
    void makeStatsCount();
    void resetStatsCountButtons();

    // day-time
    LogField getStatsDayLogField() const;
    std::optional<QString> getStatsDayParsedFilter() const;

    // relational
    LogField getStatsRelatLogField( const int filter_num ) const;
    std::optional<QString> getStatsRelatParsedFilter( const int filter_num ) const;

    // globals
    QString glob_ws;
    void makeStatsGlob();
    void resetStatsGlob();


    //////////////////
    //// CRAPCONF ////
    //////////////////

    void setConfigsPage( const ConfigsPage page );

    void refreshTextBrowserPreview();

    void refreshChartsPreview();

    IISLogsModule getIisLogsModule() const;


    //////////////////
    //// CRAPPATH ////
    //////////////////

    QSharedPointer<Crappath> crappath;


    //////////////////
    //// CRAPHELP ////
    //////////////////

    QSharedPointer<Craphelp> craphelp;

    void showHelp( const std::string& file_name );


    ////////////////
    //// CRAPUP ////
    ////////////////

    QSharedPointer<Crapup> crapup;


    //////////////////
    //// CRAPNOTE ////
    //////////////////

    QSharedPointer<Crapnote> crapnote;


    //////////////////
    //// CRAPINFO ////
    //////////////////

    QSharedPointer<Crapinfo> crapinfo;


    ///////////////////
    //// CRANGELOG ////
    ///////////////////

    QSharedPointer<Changelog> changelog;


    ///////////////////
    //// CRAPGAMES ////
    ///////////////////

    QSharedPointer<CrissCross> crisscross;

    QSharedPointer<SnakeGame> snake;

};


#endif // LOGDOCTOR__MAINWINDOW_H
