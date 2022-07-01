
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "modules/dialogs.h"

#include "qtimer.h"

#include <iostream>
#include <chrono>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //////////////////
    //// GRAPHICS ////
    this->ui->setupUi(this);

    // initialize the color-schemes map
    this->TB_COLOR_SCHEMES = ColorSec::getColorSchemes();
    // initialize the colors map
    this->COLORS = ColorSec::getColors();

    // load the main font
    this->main_font_family = QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Metropolis")).at(0);
    // load the script font
    this->script_font_family = QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/3270")).at(0);
    // initialize the fonts map
    this->FONTS["main"] = QFont(
        this->main_font_family,
        this->font_size );
    this->FONTS["main_italic"] = QFont(
        this->main_font_family,
        this->font_size,
        -1, true );
    this->FONTS["main_bold"] = QFont(
        this->main_font_family,
        this->font_size,
        1 );
    this->FONTS["main_big"] = QFont(
        this->main_font_family,
        this->font_size_big );
    this->FONTS["main_small"] = QFont(
        this->main_font_family,
        this->font_size_small );
    this->FONTS["script"] = QFont(
        this->script_font_family,
        this->font_size );

    // parent font for every tab
    this->ui->CrapTabs->setFont( this->FONTS["main_big"] );

    // TreeView for the LogFiles
    this->ui->checkBox_LogFiles_CheckAll->setFont( this->FONTS["main_small"] );
    this->ui->listLogFiles->setFont( this->FONTS["main"] );
    // TextBrowser for the LogFiles
    this->TB.setColorScheme( 1, this->TB_COLOR_SCHEMES[1] );
    this->TB.setFontFamily( this->main_font_family );
    this->TB.setFont( QFont(
        this->main_font_family,
        this->font_size ));
    this->ui->textLogFiles->setFont( this->TB.getFont() );


    ////////////////////////
    //// INITIALIZATION ////
    // WebServers for the LogsList
    this->allowed_web_servers[11] = true; // apache2
    this->allowed_web_servers[12] = true; // nginx
    this->allowed_web_servers[13] = true; // iis


    /////////////////
    //// CONFIGS ////


    ///////////////////
    //// POLISHING ////
    // disable the unallowed WebServers
    int ws = 14;
    for ( int id=11; id<14; id++ ) {
        if ( this->allowed_web_servers[ id ] == true ) {
            ws = ( id < ws ) ? id : ws;
        } else {
            switch (id) {
                case 11: this->ui->button_LogFiles_Apache->setEnabled( false ); break;
                case 12: this->ui->button_LogFiles_Nginx->setEnabled( false ); break;
                case 13: this->ui->button_LogFiles_Iis->setEnabled( false ); break;
            }
        }
    }
    if ( ws == 14 ) {
        // no WS is allowed (???), fallback to the default one
        ws = 11;
        this->craplog.setCurrentWSID( 11 );
        this->allowed_web_servers[ 11 ] = true;
        this->on_button_LogFiles_Apache_clicked();
        this->ui->button_LogFiles_Apache->setEnabled( true );
    }
    // set the LogList to the current WebServer
    if ( this->allowed_web_servers[ this->craplog.getCurrentWSID() ] == false ) {
        // the current WS is not allowed, fallback to the default one
        this->craplog.setCurrentWSID( ws );
    }
    // set the current WS for the LogList
    switch ( this->craplog.getCurrentWSID() ) {
        case 11:
            this->ui->button_LogFiles_Apache->setFlat( false );
            break;
        case 12:
            this->ui->button_LogFiles_Nginx->setFlat( false );
            break;
        case 13:
            this->ui->button_LogFiles_Iis->setFlat( false );
            break;
    }
    // get a fresh list of LogFiles
    this->ui->listLogFiles->header()->resizeSection(0,200);
    this->ui->listLogFiles->header()->resizeSection(1,100);
    this->on_button_LogFiles_RefreshList_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}


/////////////////////
//// GENERAL USE ////
/////////////////////
// printable size with suffix and limited decimals
QString MainWindow::printableSize( int bytes )
{
    std::string size_str, size_sfx=" B";
    float size = (float)bytes;
    if (size > 1024) {
        size /= 1024;
        size_sfx = " KiB";
        if (size > 1024) {
            size /= 1024;
            size_sfx = " MiB";
        }
    }
    // cut decimals depending on how big the floor is
    size_str = std::to_string( size );
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
    return QString::fromStdString( size_str.substr(0, cut_index ) + size_sfx );
}

// printable speed with suffix and limited decimals
QString MainWindow::printableSpeed( int bytes, int secs )
{
    std::string speed_str, speed_sfx=" B/s";
    float size = (float)bytes;
    if (size > 1024) {
        size /= 1024;
        speed_sfx = " KiB/s";
        if (size > 1024) {
            size /= 1024;
            speed_sfx = " MiB/s";
        }
    }
    // cut decimals depending on how big the floor is
    speed_str = std::to_string( size / secs );
    int cut_index = speed_str.find('.')+1;
    if ( cut_index == 0 ) {
            cut_index = speed_str.find(',')+1;
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
        if ( cut_index > speed_str.size()-1 ) {
            cut_index = speed_str.size()-1;
        }
    }
    return QString::fromStdString( speed_str.substr(0, cut_index ) + speed_sfx );
}

QString MainWindow::printableTime( int secs )
{
    int mins = secs / 60;
    secs = secs - (mins*60);
    std::string mins_str = (mins<10) ? "0"+std::to_string(mins) : std::to_string(mins);
    std::string secs_str = (secs<10) ? "0"+std::to_string(secs) : std::to_string(secs);
    return QString::fromStdString( mins_str +":"+ secs_str );
}


//////////////
//// LOGS ////
//////////////
// switch to apache web server
void MainWindow::on_button_LogFiles_Apache_clicked()
{
    if ( this->craplog.getCurrentWSID() != 11
      && this->allowed_web_servers[11] == true ) {
        // enable the enables
        this->ui->button_LogFiles_Apache->setFlat( false );
        this->ui->button_LogFiles_Nginx->setFlat( true );
        this->ui->button_LogFiles_Iis->setFlat( true );
        // load the list
        this->craplog.setCurrentWSID( 11 );
        this->on_button_LogFiles_RefreshList_clicked();
        this->ui->textLogFiles->setText( RichText::richLogsDefault() );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
    }
}
// switch to nginx web server
void MainWindow::on_button_LogFiles_Nginx_clicked()
{
    if ( this->craplog.getCurrentWSID() != 12
      && this->allowed_web_servers[12] == true) {
        // enable the enables
        this->ui->button_LogFiles_Nginx->setFlat( false );
        this->ui->button_LogFiles_Apache->setFlat( true );
        this->ui->button_LogFiles_Iis->setFlat( true );
        // load the list
        this->craplog.setCurrentWSID( 12 );
        this->on_button_LogFiles_RefreshList_clicked();
        this->ui->textLogFiles->setText( RichText::richLogsDefault() );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
    }
}
// switch to iis web server
void MainWindow::on_button_LogFiles_Iis_clicked()
{
    if ( this->craplog.getCurrentWSID() != 13
      && this->allowed_web_servers[13] == true ) {
        // load the list
        this->ui->button_LogFiles_Iis->setFlat( false );
        this->ui->button_LogFiles_Apache->setFlat( true );
        this->ui->button_LogFiles_Nginx->setFlat( true );
        // load the list
        this->craplog.setCurrentWSID( 13 );
        this->on_button_LogFiles_RefreshList_clicked();
        this->ui->textLogFiles->setText( RichText::richLogsDefault() );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
    }
}


// refresh the log files list
void MainWindow::on_button_LogFiles_RefreshList_clicked()
{
    std::string col;
    // clear the current tree
    this->ui->listLogFiles->clear();
    this->ui->checkBox_LogFiles_CheckAll->setCheckState( Qt::CheckState::Unchecked );
    // iterate over elements of list
    for ( const Craplog::LogFile& log_file : this->craplog.getLogsList(true) ) {
        // new entry for the tree widget
        QTreeWidgetItem * item = new QTreeWidgetItem();

        // preliminary check for file-type display
        if ( this->display_access_logs == false && log_file.type == LogOps::LogType::Access ) {
            // do not display
            delete item; // possible memory leak, says cppcheck
            continue;
        } else if ( this->display_error_logs == false && log_file.type == LogOps::LogType::Error ) {
            // do not display
            delete item; // possible memory leak, says cppcheck
            continue;
        }

        // preliminary check for file usage display
        if ( log_file.used_already == true ) {
            if ( this->display_used_files == false ) {
                // do not display
                delete item; // possible memory leak, says cppcheck
                continue;
            }
            // display with red foreground
            item->setForeground( 0, this->COLORS["red"] );
        }

        // preliminary check on file size
        col = "grey";
        if ( log_file.size > this->craplog.getWarningSize() ) {
            if ( this->display_warnsize_files == false ) {
                // do not display
                delete item; // possible memory leak, says cppcheck
                continue;
            }
            col = "orange";
        }
        item->setForeground( 1, this->COLORS[ col ] );

        // set the name
        item->setText( 0, log_file.name );
        // set the size
        item->setText( 1, this->printableSize( log_file.size ) );
        item->setFont( 1, this->FONTS["main_italic"] );
        // append the item (on top, forced)
        item->setCheckState(0, Qt::CheckState::Unchecked );
        this->ui->listLogFiles->addTopLevelItem( item );
    }
    if ( this->craplog.getLogsListSize() > 0 ) {
        // sort the list alphabetically
        this->ui->listLogFiles->sortByColumn(0, Qt::SortOrder::AscendingOrder );
        this->ui->checkBox_LogFiles_CheckAll->setEnabled( true );
    } else {
        this->ui->checkBox_LogFiles_CheckAll->setCheckState( Qt::CheckState::Unchecked );
        this->ui->checkBox_LogFiles_CheckAll->setEnabled( false );
    }
}


void MainWindow::on_checkBox_LogFiles_CheckAll_stateChanged(int arg1)
{
    Qt::CheckState new_state;
    if ( this->ui->checkBox_LogFiles_CheckAll->checkState() == Qt::CheckState::Checked ) {
        // check all
        new_state = Qt::CheckState::Checked;
        this->ui->button_MakeStats_Start->setEnabled(true);
    } else if ( this->ui->checkBox_LogFiles_CheckAll->checkState() == Qt::CheckState::Unchecked ) {
        // un-check all
        new_state = Qt::CheckState::Unchecked;
        this->ui->button_MakeStats_Start->setEnabled(false);
    } else {
        // do nothing
        this->ui->button_MakeStats_Start->setEnabled(true);
        return;
    }
    QTreeWidgetItemIterator i(this->ui->listLogFiles);
    while ( *i ) {
        (*i)->setCheckState( 0, new_state );
        ++i;
    }
}


void MainWindow::on_button_LogFiles_ViewFile_clicked()
{
    if ( this->ui->listLogFiles->selectedItems().size() > 0 ) {
        bool proceed = true;
        // display the selected item
        Craplog::LogFile item = this->craplog.getLogFileItem(
            this->ui->listLogFiles->selectedItems().takeFirst()->text(0) );
        FormatOps::LogsFormat format;
        if ( item.type == LogOps::LogType::Access ) {
            format = this->craplog.getCurrentALF();
        } else if ( item.type == LogOps::LogType::Error ) {
            format = this->craplog.getCurrentELF();
        } else {
            // this shouldn't be reached, but...
            proceed = false;
            DialogSec::msgUndefinedLogType( nullptr, item.name );
        }

        if ( proceed == true ) {
            std::string content;
            try {
                content = IOutils::readFile( item.path );
            } catch (const std::ios_base::failure& err) {
                // failed reading
                proceed = false;
                // >> err.what() << //
                DialogSec::msgFailedReadFile( nullptr, item.name );
            } catch (...) {
                // failed somehow
                proceed = false;
                QString err_msg = QMessageBox::tr("An error occured while reading");
                DialogSec::msgGenericError( nullptr, err_msg +":\n"+ item.name );
            }

            if ( proceed == true ) {
                // succesfully read, now enriched and display
                this->ui->textLogFiles->setText(
                    RichText::enrichLogs(
                        content,
                        format,
                        this->TB ));
            }
        }
        if ( proceed == false ) {
            // failed to read
            this->ui->textLogFiles->setText( RichText::richLogsFailure() );
            this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        }
    }
}


void MainWindow::on_listLogFiles_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    this->on_button_LogFiles_ViewFile_clicked();
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
        this->ui->checkBox_LogFiles_CheckAll->setCheckState(Qt::CheckState::Unchecked);
    } else if ( n_checked == this->craplog.getLogsListSize() ) {
        this->ui->checkBox_LogFiles_CheckAll->setCheckState(Qt::CheckState::Checked);
    } else {
        this->ui->checkBox_LogFiles_CheckAll->setCheckState(Qt::CheckState::PartiallyChecked);
    }
}


void MainWindow::on_button_MakeStats_Start_clicked()
{
    // take actions on Craplog's start
    this->craplogStarted();

    // feed craplog with the checked files
    bool proceed = true;
    QTreeWidgetItemIterator i(this->ui->listLogFiles);
    while ( *i ) {
        if ( (*i)->checkState(0) == Qt::CheckState::Checked ) {
            // tell Craplog to set this file as selected
            if ( this->craplog.setLogFileSelected( (*i)->text(0) ) == false ) {
                // this shouldn't be, but...
                if ( DialogSec::choiceSelectedFileNotFound( nullptr, (*i)->text(0) ) == false ) {
                    proceed = false;
                    break;
                }
            }
        }
        ++i;
    }

    if ( proceed == true ) {
        // reset perfs
        this->reset_MakeStats_labels();
        // periodically update perfs
        this->craplog_timer = new QTimer(this);
        connect(this->craplog_timer, SIGNAL(timeout()), this, SLOT(update_MakeStats_labels()));
        this->craplog_timer->start(250);
        // run craplog as thread
        this->craplog_timer_start = std::chrono::system_clock::now();
        this->craplog_thread = std::thread( &Craplog::run, &this->craplog );
    } else {
        this->craplogFinished();
    }
}

void MainWindow::reset_MakeStats_labels()
{
    // reset to default
    this->ui->label_MakeStats_Size->setText( "0 B" );
    this->ui->label_MakeStats_Lines->setText( "0" );
    // time and speed
    this->ui->label_MakeStats_Time->setText( "00:00" );
    this->ui->label_MakeStats_Speed->setText( "0 B/s" );
}

void MainWindow::update_MakeStats_labels()
{
    // craplog is running as thread, update the values meanwhile
    int size, secs;
    // update values
    // size and lines
    size = this->craplog.getParsedSize();
    this->ui->label_MakeStats_Size->setText( this->printableSize( size ) );
    this->ui->label_MakeStats_Lines->setText( QString::fromStdString(std::to_string(this->craplog.getParsedLines())) );
    // time and speed
    this->craplog_timer_lapse = std::chrono::system_clock::now();
    this->craplog_timer_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        this->craplog_timer_start - this->craplog_timer_lapse
    );
    secs = this->craplog_timer_elapsed.count() / -1000000000;
    this->ui->label_MakeStats_Time->setText( this->printableTime( secs ));
    this->ui->label_MakeStats_Speed->setText( this->printableSpeed( size, secs ));
    if ( this->craplog.isWorking() == false ) {
        this->craplog_timer->stop();
        this->craplog_thread.join();
        this->craplogFinished();
    }
}

void MainWindow::craplogStarted()
{
    // disable the LogFiles section
    this->ui->LogBoxFiles->setEnabled(false);
    // disable the start button
    this->ui->button_MakeStats_Start->setEnabled(false);
    // enable all labels (needed only the first time)
    this->ui->icon_MakeStats_Size->setEnabled(false);
    this->ui->label_MakeStats_Size->setEnabled(true);
    this->ui->icon_MakeStats_Lines->setEnabled(false);
    this->ui->label_MakeStats_Lines->setEnabled(true);
    this->ui->icon_MakeStats_Time->setEnabled(false);
    this->ui->label_MakeStats_Time->setEnabled(true);
    this->ui->icon_MakeStats_Speed->setEnabled(false);
    this->ui->label_MakeStats_Speed->setEnabled(true);
}

void MainWindow::craplogFinished()
{
    // refresh the logs list
    this->on_button_LogFiles_RefreshList_clicked();
    // enable the LogFiles section
    this->ui->LogBoxFiles->setEnabled(true);
    // enable all labels (needed only the first time)
    this->ui->icon_MakeStats_Size->setEnabled(true);
    this->ui->icon_MakeStats_Lines->setEnabled(true);
    this->ui->icon_MakeStats_Time->setEnabled(true);
    this->ui->icon_MakeStats_Speed->setEnabled(true);
}

