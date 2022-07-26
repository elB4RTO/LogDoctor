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

private:
    Ui::MainWindow *ui;

    // quantoty of informational dialogs to display
    int dialogs_Level = 1; // 0: essential, 1: usefull, 2: explanatory

    void makeInitialChecks();

    //////////////////
    //// GRAPHICS ////
    //////////////////
    std::unordered_map<int, std::unordered_map<std::string, QString>> TB_COLOR_SCHEMES;
    std::unordered_map<std::string, QColor> COLORS;
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
