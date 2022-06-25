#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmainwindow.h"
#include "qfontdatabase.h"
#include "qmessagebox.h"

#include "qtreewidget.h"

#include "utilities.h"
#include "tools/craplog.h"

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

    void on_checkAllLogFiles_stateChanged(int arg1);

    void on_buttonRefreshList_clicked();

    void on_listLogFiles_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_listLogFiles_itemChanged(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    Craplog craplog;

    bool runCraplog();

    std::unordered_map<std::string, QColor> COLORS;
    std::unordered_map<std::string, QFont>  FONTS;
    int font_size       = 13,
        font_size_big   = 16,
        font_size_small = 10;

    // base font families, to build fonts from
    QString main_font_family;
    QString script_font_family;

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
