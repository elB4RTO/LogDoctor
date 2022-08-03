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
    void on_button_LogFiles_ViewFile_clicked();

    void on_checkBox_LogFiles_CheckAll_stateChanged(int arg1);

    void on_button_LogFiles_RefreshList_clicked();

    void on_listLogFiles_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_listLogFiles_itemChanged(QTreeWidgetItem *item, int column);

    void on_button_LogFiles_Apache_clicked();

    void on_button_LogFiles_Nginx_clicked();

    void on_button_LogFiles_Iis_clicked();

    void on_button_MakeStats_Start_clicked();

    void on_box_StatsWarn_WebServer_currentIndexChanged(int index);

    void on_box_StatsWarn_LogsType_currentIndexChanged(int index);

    void on_box_StatsWarn_Year_currentIndexChanged(int index);

    void on_box_StatsWarn_Month_currentIndexChanged(int index);

    void on_box_StatsWarn_Day_currentIndexChanged(int index);

    void on_checkBox_StatsWarn_Hour_stateChanged(int state);

    void on_box_StatsWarn_Hour_currentIndexChanged(int index);

    void on_button_StatsWarn_Draw_clicked();

    void on_button_StatsWarn_Update_clicked();

    void on_box_StatsSpeed_WebServer_currentIndexChanged(int index);

    void on_box_StatsSpeed_Year_currentIndexChanged(int index);

    void on_box_StatsSpeed_Month_currentIndexChanged(int index);

    void on_box_StatsSpeed_Day_currentIndexChanged(int index);

    void on_button_StatsSpeed_Draw_clicked();

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

    void on_checkBox_ConfGeneral_Window_clicked(bool checked);

    void on_slider_ConfGeneral_General_sliderReleased();

    void on_slider_ConfGeneral_Logs_sliderReleased();

    void on_slider_ConfGeneral_Stats_sliderReleased();

    void on_box_ConfGeneral_Theme_Window_currentIndexChanged(int index);

    void on_box_ConfGeneral_Theme_TextBrowser_currentIndexChanged(int index);

    void on_box_ConfGeneral_Theme_Charts_currentIndexChanged(int index);

    void on_checkBox_ConfControl_Usage_clicked(bool checked);

    void on_checkBox_ConfControl_Size_clicked(bool checked);

    void on_spinBox_ConfControl_Size_editingFinished();

    void on_checkBox_ConfApache_Paths_Different_clicked(bool checked);

    void on_inLine_ConfApache_Paths_AccPath_textChanged(const QString &arg1);

    void on_inLine_ConfApache_Paths_ErrPath_textChanged(const QString &arg1);

    void on_button_ConfApache_Paths_SavePaths_clicked();

    void on_inLine_ConfApache_Formats_AccString_cursorPositionChanged(int arg1, int arg2);

    void on_button_ConfApache_Format_AccSave_clicked();

    void on_button_ConfApache_Formats_AccSample_clicked();

    void on_button_ConfApache_Formats_AccHelp_clicked();

    void on_inLine_ConfApache_Formats_ErrString_cursorPositionChanged(int arg1, int arg2);

    void on_button_ConfApache_Format_ErrSave_clicked();

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
    int window_theme = 0,
        charts_theme = 0;
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

};
#endif // MAINWINDOW_H
