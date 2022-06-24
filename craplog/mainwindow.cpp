#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);

    // initialize the colors map
    this->COLORS["black"] = QColor(0,0,0,255);
    this->COLORS["grey"]  = QColor(127,127,127,255);
    this->COLORS["white"] = QColor(255,255,255,255);

    // define text sizes
    this->font_size = 13;
    this->font_size_big = 16;
    this->font_size_small = 10;
    // load the main font
    QString main_font_family = QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Metropolis")).at(0);
    // load the script font
    QString script_font_family = QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/3270")).at(0);
    // initialize the fonts map
    this->FONTS["main"] = QFont( main_font_family, this->font_size );
    this->FONTS["main_italic"] = QFont( main_font_family, this->font_size, -1, true );
    this->FONTS["script"] = QFont( script_font_family, this->font_size );

    // initialize the TextBrowser's color scheme
    this->TB_color_scheme = 1;

    // get a fresh list of log files
    this->ui->listLogFiles->header()->resizeSection(0,200);
    this->ui->listLogFiles->header()->resizeSection(1,100);
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
    // iterate over elements of list
    std::cout << "OOOOOOOOOOOOOOOOOK" << std::endl;
    for ( const Craplog::LogFile& log_file : this->craplog.getLogsList(true) ) {
        std::cout << "XXXXXXXXXXXXXXXXXXXXXXXK" << std::endl;
        // new entry for the tree widget
        QTreeWidgetItem * item = new QTreeWidgetItem();
        // set unchecked
        item->setCheckState(0, Qt::CheckState::Unchecked );
        // set the name of the file
        item->setText( 0, log_file.name );
        item->setFont( 0, this->FONTS["main"] );
        // prepare the size of the file
        float size = (float)log_file.size / 1024;
        std::string sfx = " KiB";
        if (size > 1024) {
            size /= 1024;
            sfx = " MiB";
        }
        // cut decimals depending on how big the floor is
        std::string size_str = std::to_string( size );
        int cut_index = size_str.find('.')+1;
        if ( cut_index == 0 ) {
                cut_index = size_str.find(',')+1;
        }
        int n_decimals = 3;
        if ( size >= 100 ) {
            n_decimals = 2;
            if ( size >= 1000 ) {
                n_decimals = 1;
                if ( size >= 10000 ) {
                    n_decimals = 0;
                    cut_index --;
                }
            }
        }
        if ( cut_index >= 1 ) {
            cut_index += n_decimals;
            if ( cut_index > size_str.size()-1 ) {
                cut_index = size_str.size()-1;
            }
        }
        // apply text and color to the size text
        item->setText( 1, QString::fromStdString( size_str.substr(0,cut_index) + sfx ) );
        item->setForeground( 1, this->COLORS["grey"] );
        item->setFont( 1, this->FONTS["main_italic"] );
        // append the item (on top, forced)
        this->ui->listLogFiles->addTopLevelItem( item );
    }
    // sort the list alphabetically
    this->ui->listLogFiles->sortByColumn(0, Qt::SortOrder::AscendingOrder );
}


void MainWindow::on_checkAllLogFiles_stateChanged(int arg1)
{
    Qt::CheckState new_state;
    if ( this->ui->checkAllLogFiles->checkState() == Qt::CheckState::Checked ) {
        // check all
        new_state = Qt::CheckState::Checked;
    } else if ( this->ui->checkAllLogFiles->checkState() == Qt::CheckState::Unchecked ) {
        // un-check all
        new_state = Qt::CheckState::Unchecked;
    } else {
        // do nothing
        return;
    }
    QTreeWidgetItemIterator i(this->ui->listLogFiles);
    while ( *i ) {
        (*i)->setCheckState( 0, new_state );
        ++i;
    }
}


void MainWindow::on_buttonViewFile_clicked()
{
    /*QString file_name = this->ui->listLogFiles->selectedItems().takeFirst()->text(0);
    std::string file_path = this->craplog.getLogFilePath( file_name );
    std::string content = IOutils::readFile( file_path );
    this->ui->textLogFiles->setText( RichText::enrichLogs( content, this->TB_color_scheme ) );*/
    Craplog::LogFile item = this->craplog.getLogFileItem(
        this->ui->listLogFiles->selectedItems().takeFirst()->text(0) );
    Craplog::LogsFormat format;
    if ( item.type == Craplog::LogType::Access ) {
        format = this->craplog.getAccessLogsFormat();
    } else if ( item.type == Craplog::LogType::Error ) {
        format = this->craplog.getErrorLogsFormat();
    } else {
        // this shouldn't be
            // !!! PUT A DIALOG ERROR MESSAGE HERE !!!
    }
    this->ui->textLogFiles->setText(
        RichText::enrichLogs(
            IOutils::readFile( item.path ),
            format,
            this->TB_color_scheme ));
    this->ui->textLogFiles->setFont( this->FONTS["main"] );
}


void MainWindow::on_listLogFiles_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    this->on_buttonViewFile_clicked();
}


void MainWindow::on_listLogFiles_itemChanged(QTreeWidgetItem *item, int column)
{
    // control checked
    int n_checked = 0;
    QTreeWidgetItemIterator i(this->ui->listLogFiles);
    while ( *i ) {
        if ( (*i)->checkState(0) == Qt::CheckState::Checked ) {
            n_checked++;
        }
        ++i;
    }
    if ( n_checked == 0 ) {
        this->ui->checkAllLogFiles->setCheckState(Qt::CheckState::Unchecked);
    } else if ( n_checked == this->craplog.getLogsListSize() ) {
        this->ui->checkAllLogFiles->setCheckState(Qt::CheckState::Checked);
    } else {
        this->ui->checkAllLogFiles->setCheckState(Qt::CheckState::PartiallyChecked);
    }
}


bool MainWindow::runCraplog()
{
    // feed craplog with the checked values
    bool proceed = true;
    QTreeWidgetItemIterator i(this->ui->listLogFiles);
    while ( *i ) {
        if ( (*i)->checkState(0) == Qt::CheckState::Checked ) {
            // tell Craplog to set this file as selected
            if ( this->craplog.setLogFileSelected( (*i)->text(0) ) != 0 ) {
                // this shouldn't be, but...
                int response = QMessageBox::warning(this,
                    QString("File selection failed"),
                    QString("No file in the list matching this name:\n%1\n\nContinue?")
                        .arg( (*i)->text(0) ),
                    QMessageBox::Abort | QMessageBox::Ignore );
                if ( response == QMessageBox::Abort ) {
                    proceed = false;
                    break;
                }
            }
        }
        ++i;
    }
    if ( proceed == false ) {
        return proceed;
    }



    return proceed;
}

