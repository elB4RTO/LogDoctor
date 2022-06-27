#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmainwindow.h"
#include "qfontdatabase.h"
#include "qmessagebox.h"

#include "qtreewidget.h"

#include "utilities.h"
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

private slots:
    void on_buttonViewFile_clicked();

    void on_checkBox_LogFiles_CheckAll_stateChanged(int arg1);

    void on_buttonRefreshList_clicked();

    void on_listLogFiles_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_listLogFiles_itemChanged(QTreeWidgetItem *item, int column);

    void on_button_LogFiles_Apache_clicked();

    void on_button_LogFiles_Nginx_clicked();

    void on_button_LogFiles_Iis_clicked();

private:
    Ui::MainWindow *ui;

    Craplog craplog;
    bool runCraplog();

    // quantoty of informational dialogs to display
    int dialogs_Level = 1; // 0: essential, 1: usefull, 2: explanatory

    std::unordered_map<std::string, QColor> COLORS;
    std::unordered_map<std::string, QFont>  FONTS;
    int font_size       = 13,
        font_size_big   = 16,
        font_size_small = 10;

    // base font families, to build fonts from
    QString main_font_family;
    QString script_font_family;

    //////////////
    //// LOGS ////
    // web servers related
    bool loading_LogsList = false;
    std::unordered_map<int, bool> allowed_web_servers;
    void disableAllButtons_LogFiles_WS();
    void enableAllButtons_LogFiles_WS();
    // logs list related
    bool display_used_files = true;
    bool display_warnsize_files = true;

    // text browser related
    struct LogFilesTB {
        bool wide_lines;
        int  color_scheme;
        int  font_size;
        QString font_family;
        QFont   font;
    };
    LogFilesTB TB;
    void setTB_FontSize( int size );
    void setTB_FontFamily( QString font_family );
    void updateTB_Font();
    int getTB_FontSize();
    QString getTB_FontFamily();
};
#endif // MAINWINDOW_H
