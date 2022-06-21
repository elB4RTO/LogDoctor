#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //craplog.scanLogsDir();
    ui->listLogFiles->header()->resizeSection(0,160);
    ui->listLogFiles->header()->resizeSection(1,75);
    this->on_buttonRefreshList_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//////////////
//// LOGS ////
//////////////
// refresh the log files list
void MainWindow::on_buttonRefreshList_clicked()
{
    // clear the current tree
    this->ui->listLogFiles->clear();
    // grey color to use for the size column
    QColor grey = QColor(127,127,127,255);
    // iterate over elements of list
    for ( Craplog::LogFile& log_file : this->craplog.getLogsList(true) ) {
        // new entry for the tree widget
        QTreeWidgetItem * item = new QTreeWidgetItem();
        // set unchecked
        item->setCheckState(0, Qt::CheckState::Unchecked );
        // name to be showed
        item-> setText( 0, log_file.name );
        // size to be showed
        float size = (float)log_file.size / 1024;
        int n_decimals = 3;
        std::string sfx = " KiB";
        if (size > 1024) {
            size /= 1024;
            sfx = " MiB";
        }
        // cut decimals depending on how big the floor is
        std::string size_str = std::to_string( size );
        int point_i = size_str.find('.')+1;
        if ( point_i == 0 ) {
                point_i = size_str.find(',')+1;
        }
        if ( size >= 100 ) {
            n_decimals = 2;
            if ( size >= 1000 ) {
                n_decimals = 1;
                if ( size >= 10000 ) {
                    n_decimals = 0;
                    point_i --;
                }
            }
        }
        if ( point_i >= 1 ) {
            for ( int i=0; i<n_decimals; i++ ) {
                if ( i >= size_str.size() ) {
                    break;
                }
                point_i ++;
            }
        }
        // apply text and color
        item-> setText( 1, QString::fromStdString( size_str.substr(0,point_i) + sfx ) );
        item->setForeground(1, grey);
        // append the item (on top, forced)
        this->ui->listLogFiles->addTopLevelItem( item );
    }
    // sort the list alphabetically
    this->ui->listLogFiles->sortByColumn(0, Qt::SortOrder::AscendingOrder );
}


void MainWindow::on_checkAllLogFiles_stateChanged(int arg1)
{
    QTreeWidgetItemIterator i(this->ui->listLogFiles);
    while ( *i ) {
        (*i)->setCheckState( 0, Qt::CheckState::Checked );
    }
}


void MainWindow::on_buttonViewFile_clicked()
{
    
}


void MainWindow::on_listLogFiles_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    
}

