#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    
private:
    Ui::MainWindow *ui;
    Craplog craplog;
};
#endif // MAINWINDOW_H
