#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmainwindow.h"
#include "qfontdatabase.h"
#include "qmessagebox.h"

#include "qtreewidget.h"
#include "qchartview.h"

#include <thread>

#include "utilities.h"
#include "modules.h"
#include "tools/craplog/craplog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //void operator()( int a );

private slots:
    void on_button_LogFiles_ViewFile_clicked();

    void on_checkBox_LogFiles_CheckAll_stateChanged(int arg1);

    void on_button_LogFiles_RefreshList_clicked();

    void on_listLogFiles_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_listLogFiles_itemChanged(QTreeWidgetItem *item, int column);

    void on_button_LogFiles_Apache_clicked();

    void on_button_LogFiles_Nginx_clicked();

    void on_button_LogFiles_Iis_clicked();

    void on_button_MakeStats_Start_clicked();

    // custom
    void wait_ActiveWindow();

    void update_Craplog_PerfData();

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

};
#endif // MAINWINDOW_H
