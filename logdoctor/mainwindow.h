#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    void on_box_StatsWarn_LogsType_currentIndexChanged(int index);

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

    void on_tabs_StatsCount_AccErr_currentChanged(int index);

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

    void on_button_StatsCount_AccClient_clicked();

    void on_button_StatsCount_Level_clicked();

    void on_button_StatsCount_Message_clicked();

    void on_button_StatsCount_Source_clicked();

    void on_button_StatsCount_Port_clicked();

    void on_button_StatsCount_ErrClient_clicked();

    //// DAY-TIME ////

    void on_box_StatsDay_WebServer_currentIndexChanged(int index);

    void on_box_StatsDay_LogsType_currentIndexChanged(int index);

    void on_box_StatsDay_LogsField_currentIndexChanged(int index);

    void on_box_StatsDay_FromYear_currentIndexChanged(int index);

    void on_box_StatsDay_FromMonth_currentIndexChanged(int index);

    void on_box_StatsDay_FromDay_currentIndexChanged(int index);

    void on_checkBox_StatsDay_Period_stateChanged(int state);

    void on_box_StatsDay_ToYear_currentIndexChanged(int index);

    void on_box_StatsDay_ToMonth_currentIndexChanged(int index);

    void on_box_StatsDay_ToDay_currentIndexChanged(int index);

    void on_button_StatsDay_Draw_clicked();

    //// RELATIONSL ////

    void on_box_StatsRelat_WebServer_currentIndexChanged(int index);

    void on_box_StatsRelat_LogsType_currentIndexChanged(int index);

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

    //// LOGS CONTROL ////

    void on_checkBox_ConfControl_Usage_clicked(bool checked);

    void on_checkBox_ConfControl_Size_clicked(bool checked);

    void on_spinBox_ConfControl_Size_editingFinished();

    //// APACHE ////

    void on_checkBox_ConfApache_Paths_Different_clicked(bool checked);

    void on_inLine_ConfApache_Paths_AccPath_textChanged(const QString &arg1);

    void on_inLine_ConfApache_Paths_ErrPath_textChanged(const QString &arg1);

    void on_button_ConfApache_Paths_SavePaths_clicked();

    void on_inLine_ConfApache_Formats_AccString_cursorPositionChanged(int arg1, int arg2);

    void on_button_ConfApache_Formats_AccSave_clicked();

    void on_button_ConfApache_Formats_AccSample_clicked();

    void on_button_ConfApache_Formats_AccHelp_clicked();

    void on_inLine_ConfApache_Formats_ErrString_cursorPositionChanged(int arg1, int arg2);

    void on_button_ConfApache_Formats_ErrSave_clicked();

    void on_button_ConfApache_Formats_ErrSample_clicked();

    void on_button_ConfApache_Formats_ErrHelp_clicked();

    void on_box_ConfApache_Warnlist_Acc_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfApache_Warnlist_AccUsed_clicked(bool checked);

    void on_inLine_ConfApache_Warnlist_Acc_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Warnlist_Acc_returnPressed();

    void on_button_ConfApache_Warnlist_AccAdd_clicked();

    void on_list_ConfApache_Warnlist_Acc_itemSelectionChanged();

    void on_button_ConfApache_Warnlist_AccRemove_clicked();

    void on_button_ConfApache_Warnlist_AccUp_clicked();

    void on_button_ConfApache_Warnlist_AccDown_clicked();

    void on_box_ConfApache_Warnlist_Err_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfApache_Warnlist_ErrUsed_clicked(bool checked);

    void on_inLine_ConfApache_Warnlist_Err_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Warnlist_Err_returnPressed();

    void on_button_ConfApache_Warnlist_ErrAdd_clicked();

    void on_list_ConfApache_Warnlist_Err_itemSelectionChanged();

    void on_button_ConfApache_Warnlist_ErrRemove_clicked();

    void on_button_ConfApache_Warnlist_ErrUp_clicked();

    void on_button_ConfApache_Warnlist_ErrDown_clicked();

    void on_box_ConfApache_Blacklist_Acc_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfApache_Blacklist_AccUsed_clicked(bool checked);

    void on_inLine_ConfApache_Blacklist_Acc_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Blacklist_Acc_returnPressed();

    void on_button_ConfApache_Blacklist_AccAdd_clicked();

    void on_list_ConfApache_Blacklist_Acc_itemSelectionChanged();

    void on_button_ConfApache_Blacklist_AccRemove_clicked();

    void on_button_ConfApache_Blacklist_AccUp_clicked();

    void on_button_ConfApache_Blacklist_AccDown_clicked();

    void on_box_ConfApache_Blacklist_Err_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfApache_Blacklist_ErrUsed_clicked(bool checked);

    void on_inLine_ConfApache_Blacklist_Err_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfApache_Blacklist_Err_returnPressed();

    void on_button_ConfApache_Blacklist_ErrAdd_clicked();

    void on_list_ConfApache_Blacklist_Err_itemSelectionChanged();

    void on_button_ConfApache_Blacklist_ErrRemove_clicked();

    void on_button_ConfApache_Blacklist_ErrUp_clicked();

    void on_button_ConfApache_Blacklist_ErrDown_clicked();

    //// NGINX ////

    void on_checkBox_ConfNginx_Paths_Different_clicked(bool checked);

    void on_inLine_ConfNginx_Paths_AccPath_textChanged(const QString &arg1);

    void on_inLine_ConfNginx_Paths_ErrPath_textChanged(const QString &arg1);

    void on_button_ConfNginx_Paths_SavePaths_clicked();

    void on_inLine_ConfNginx_Formats_AccString_cursorPositionChanged(int arg1, int arg2);

    void on_button_ConfNginx_Formats_AccSave_clicked();

    void on_button_ConfNginx_Formats_AccSample_clicked();

    void on_button_ConfNginx_Formats_AccHelp_clicked();

    void on_inLine_ConfNginx_Formats_ErrString_cursorPositionChanged(int arg1, int arg2);

    void on_button_ConfNginx_Formats_ErrSave_clicked();

    void on_button_ConfNginx_Formats_ErrSample_clicked();

    void on_button_ConfNginx_Formats_ErrHelp_clicked();

    void on_box_ConfNginx_Warnlist_Acc_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfNginx_Warnlist_AccUsed_clicked(bool checked);

    void on_inLine_ConfNginx_Warnlist_Acc_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Warnlist_Acc_returnPressed();

    void on_button_ConfNginx_Warnlist_AccAdd_clicked();

    void on_list_ConfNginx_Warnlist_Acc_itemSelectionChanged();

    void on_button_ConfNginx_Warnlist_AccRemove_clicked();

    void on_button_ConfNginx_Warnlist_AccUp_clicked();

    void on_button_ConfNginx_Warnlist_AccDown_clicked();

    void on_box_ConfNginx_Warnlist_Err_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfNginx_Warnlist_ErrUsed_clicked(bool checked);

    void on_inLine_ConfNginx_Warnlist_Err_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Warnlist_Err_returnPressed();

    void on_button_ConfNginx_Warnlist_ErrAdd_clicked();

    void on_list_ConfNginx_Warnlist_Err_itemSelectionChanged();

    void on_button_ConfNginx_Warnlist_ErrRemove_clicked();

    void on_button_ConfNginx_Warnlist_ErrUp_clicked();

    void on_button_ConfNginx_Warnlist_ErrDown_clicked();

    void on_box_ConfNginx_Blacklist_Acc_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfNginx_Blacklist_AccUsed_clicked(bool checked);

    void on_inLine_ConfNginx_Blacklist_Acc_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Blacklist_Acc_returnPressed();

    void on_button_ConfNginx_Blacklist_AccAdd_clicked();

    void on_list_ConfNginx_Blacklist_Acc_itemSelectionChanged();

    void on_button_ConfNginx_Blacklist_AccRemove_clicked();

    void on_button_ConfNginx_Blacklist_AccUp_clicked();

    void on_button_ConfNginx_Blacklist_AccDown_clicked();

    void on_box_ConfNginx_Blacklist_Err_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfNginx_Blacklist_ErrUsed_clicked(bool checked);

    void on_inLine_ConfNginx_Blacklist_Err_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfNginx_Blacklist_Err_returnPressed();

    void on_button_ConfNginx_Blacklist_ErrAdd_clicked();

    void on_list_ConfNginx_Blacklist_Err_itemSelectionChanged();

    void on_button_ConfNginx_Blacklist_ErrRemove_clicked();

    void on_button_ConfNginx_Blacklist_ErrUp_clicked();

    void on_button_ConfNginx_Blacklist_ErrDown_clicked();

    //// IIS ////

    void on_checkBox_ConfIis_Paths_Different_clicked(bool checked);

    void on_inLine_ConfIis_Paths_AccPath_textChanged(const QString &arg1);

    void on_inLine_ConfIis_Paths_ErrPath_textChanged(const QString &arg1);

    void on_button_ConfIis_Paths_SavePaths_clicked();

    void on_radio_ConfIis_Formats_W3C_toggled(bool checked);

    void on_radio_ConfIis_Formats_NCSA_toggled(bool checked);

    void on_radio_ConfIis_Formats_IIS_toggled(bool checked);

    void on_inLine_ConfIis_Formats_AccString_cursorPositionChanged(int arg1, int arg2);

    void on_button_ConfIis_Formats_AccSave_clicked();

    void on_button_ConfIis_Formats_AccSample_clicked();

    void on_button_ConfIis_Formats_AccHelp_clicked();

    void on_inLine_ConfIis_Formats_ErrString_cursorPositionChanged(int arg1, int arg2);

    void on_button_ConfIis_Formats_ErrSave_clicked();

    void on_button_ConfIis_Formats_ErrSample_clicked();

    void on_button_ConfIis_Formats_ErrHelp_clicked();

    void on_box_ConfIis_Warnlist_Acc_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfIis_Warnlist_AccUsed_clicked(bool checked);

    void on_inLine_ConfIis_Warnlist_Acc_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfIis_Warnlist_Acc_returnPressed();

    void on_button_ConfIis_Warnlist_AccAdd_clicked();

    void on_list_ConfIis_Warnlist_Acc_itemSelectionChanged();

    void on_button_ConfIis_Warnlist_AccRemove_clicked();

    void on_button_ConfIis_Warnlist_AccUp_clicked();

    void on_button_ConfIis_Warnlist_AccDown_clicked();

    void on_box_ConfIis_Warnlist_Err_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfIis_Warnlist_ErrUsed_clicked(bool checked);

    void on_inLine_ConfIis_Warnlist_Err_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfIis_Warnlist_Err_returnPressed();

    void on_button_ConfIis_Warnlist_ErrAdd_clicked();

    void on_list_ConfIis_Warnlist_Err_itemSelectionChanged();

    void on_button_ConfIis_Warnlist_ErrRemove_clicked();

    void on_button_ConfIis_Warnlist_ErrUp_clicked();

    void on_button_ConfIis_Warnlist_ErrDown_clicked();

    void on_box_ConfIis_Blacklist_Acc_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfIis_Blacklist_AccUsed_clicked(bool checked);

    void on_inLine_ConfIis_Blacklist_Acc_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfIis_Blacklist_Acc_returnPressed();

    void on_button_ConfIis_Blacklist_AccAdd_clicked();

    void on_list_ConfIis_Blacklist_Acc_itemSelectionChanged();

    void on_button_ConfIis_Blacklist_AccRemove_clicked();

    void on_button_ConfIis_Blacklist_AccUp_clicked();

    void on_button_ConfIis_Blacklist_AccDown_clicked();

    void on_box_ConfIis_Blacklist_Err_currentTextChanged(const QString &arg1);

    void on_checkBox_ConfIis_Blacklist_ErrUsed_clicked(bool checked);

    void on_inLine_ConfIis_Blacklist_Err_cursorPositionChanged(int arg1, int arg2);

    void on_inLine_ConfIis_Blacklist_Err_returnPressed();

    void on_button_ConfIis_Blacklist_ErrAdd_clicked();

    void on_list_ConfIis_Blacklist_Err_itemSelectionChanged();

    void on_button_ConfIis_Blacklist_ErrRemove_clicked();

    void on_button_ConfIis_Blacklist_ErrUp_clicked();

    void on_button_ConfIis_Blacklist_ErrDown_clicked();

private:
    Ui::MainWindow *ui;

    // quantoty of informational dialogs to display
    int dialogs_Level = 1; // 0: essential, 1: usefull, 2: explanatory

    // web servers ID constants
    const unsigned int APACHE_ID=11, NGINX_ID=12, IIS_ID=13;
    // log type constants
    const unsigned int FAILED=0, ACCESS_LOGS=1, ERROR_LOGS=2;

    void makeInitialChecks();

    //////////////////
    //// GRAPHICS ////
    //////////////////
    // remember window position and sizes
    bool remember_window = true;
    // themes
    int window_theme_id = 0,
        charts_theme_id = 0;
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
    QString printableSize( const int bytes ),
            printableSpeed( const int bytes, const int secs ),
            printableTime( const int seconds );

    //////////////////
    //// DATABASE ////
    std::string db_stats_path,
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
    std::unordered_map<int, bool> allowed_web_servers;
    void disableAllButtons_LogFiles_WS(),
         enableAllButtons_LogFiles_WS();
    // logs list related
    bool display_access_logs = true,
         display_error_logs = true,
         display_used_files = true,
         display_warnsize_files = true;
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
    void resetStatsCountAccButtons();
    void resetStatsCountErrButtons();
    // draw
    void drawStatsCount( const QString& field );


    /////////////////
    //// CRAPSET ////
    /////////////////
    void refreshTextBrowserPreview();
    void refreshChartsPreview();
    const int getIisLogsModule();

};
#endif // MAINWINDOW_H
