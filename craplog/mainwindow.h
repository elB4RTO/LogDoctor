#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDatabase>
#include <QMessageBox>

#include "./window/craplog.h"
#include "qtreewidget.h"

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
    std::unordered_map<std::string, QFont> FONTS;
    QString font_name;
    int font_size, font_size_big, font_size_small;
    QFont main_font;
};
#endif // MAINWINDOW_H
