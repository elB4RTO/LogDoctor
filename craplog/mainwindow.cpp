
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "qtimer.h"

#include <chrono>

#include <iostream> // !!! REMOVE !!!


MainWindow::MainWindow( QWidget *parent )
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
    this->ui->CrapTabs->setFont( this->FONTS.at("main_big") );

    // WebServers buttons for the LogFiles
    this->ui->button_LogFiles_Apache->setFont( this->FONTS.at("main") );
    this->ui->button_LogFiles_Nginx->setFont( this->FONTS.at("main") );
    this->ui->button_LogFiles_Iis->setFont( this->FONTS.at("main") );
    // TreeView for the LogFiles
    this->ui->checkBox_LogFiles_CheckAll->setFont( this->FONTS.at("main_small") );
    this->ui->listLogFiles->setFont( this->FONTS.at("main") );
    // TextBrowser for the LogFiles
    this->TB.setColorScheme( 1, this->TB_COLOR_SCHEMES.at(1) );
    this->TB.setFontFamily( this->main_font_family );
    this->TB.setFont( this->FONTS.at("main") );
    this->ui->textLogFiles->setFont( this->TB.getFont() );
    // MakeStats labels
    this->ui->label_MakeStats_Size->setFont( this->FONTS.at("main") );
    this->ui->label_MakeStats_Lines->setFont( this->FONTS.at("main") );
    this->ui->label_MakeStats_Time->setFont( this->FONTS.at("main") );
    this->ui->label_MakeStats_Speed->setFont( this->FONTS.at("main") );

    // StatsSpeed table
    this->ui->table_StatsSpeed_Fields->setFont( this->FONTS.at("main") );

    // adjust LogsList headers width
    this->ui->listLogFiles->header()->resizeSection(0,200);
    this->ui->listLogFiles->header()->resizeSection(1,100);


    ////////////////////////
    //// INITIALIZATION ////
    // sqlite databases paths
    this->db_stats_path  = "collection.db";//"~/.craplog/collection.db"; !!! RESTORE
    this->db_hashes_path = "hashes.db";//"~/.craplog/hashes.db"; !!! RESTORE
    this->crapview.setDbPath( this->db_stats_path );
    // WebServers for the LogsList
    this->allowed_web_servers[11] = true; // apache2
    this->allowed_web_servers[12] = true; // nginx
    this->allowed_web_servers[13] = true; // iis


    /////////////////
    //// CONFIGS ////
    this->craplog.setDialogLevel( 1 ); // !!! REPLACE WITH CONFIGURATION VALUE !!!


    ///////////////////
    //// POLISHING ////
    // disable the unallowed WebServers
    int ws = 14;
    for ( int id=13; id>10; id-- ) {
        if ( this->allowed_web_servers.at( id ) == true ) {
            ws = ( id < ws ) ? id : ws;
        } else {
            switch (id) {
                case 11:
                    this->ui->button_LogFiles_Apache->setEnabled( false );
                    this->ui->box_StatsCount_WebServer->removeItem( 0 );
                    this->ui->box_StatsSpeed_WebServer->removeItem( 0 );
                    this->ui->box_StatsDay_WebServer->removeItem(   0 );
                    this->ui->box_StatsRelat_WebServer->removeItem( 0 );
                    break;
                case 12:
                    this->ui->button_LogFiles_Nginx->setEnabled( false );
                    this->ui->box_StatsCount_WebServer->removeItem( 1 );
                    this->ui->box_StatsSpeed_WebServer->removeItem( 1 );
                    this->ui->box_StatsDay_WebServer->removeItem(   1 );
                    this->ui->box_StatsRelat_WebServer->removeItem( 1 );
                    break;
                case 13:
                    this->ui->button_LogFiles_Iis->setEnabled( false );
                    this->ui->box_StatsCount_WebServer->removeItem( 2 );
                    this->ui->box_StatsSpeed_WebServer->removeItem( 2 );
                    this->ui->box_StatsDay_WebServer->removeItem(   2 );
                    this->ui->box_StatsRelat_WebServer->removeItem( 2 );
                    break;
            }
        }
    }
    if ( ws == 14 ) {
        // no WS is allowed (???), fallback to the default one
        ws = 11;
        this->craplog.setCurrentWSID( ws );
        this->allowed_web_servers.at( ws ) = true;
        this->on_button_LogFiles_Apache_clicked();
        this->ui->button_LogFiles_Apache->setEnabled( true );
        this->ui->box_StatsCount_WebServer->addItem( "Apache2" );
        this->ui->box_StatsSpeed_WebServer->addItem( "Apache2" );
        this->ui->box_StatsDay_WebServer->addItem(   "Apache2" );
        this->ui->box_StatsRelat_WebServer->addItem( "Apache2" );
    }
    // set the LogList to the current WebServer
    if ( this->allowed_web_servers.at( this->craplog.getCurrentWSID() ) == false ) {
        // the current craplog's WS is not allowed, fallback to the default one
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
        default:
            // shouldn't be here
            throw( &"Unexpected WebServer ID for Craplog: "[this->craplog.getCurrentWSID()] );
    }
    // set the current WS for the ViewStats
    switch ( ws ) {
        case 11:
            // already set to index 0 by default
            break;
        case 12:
            for ( int i=0; i<this->ui->box_StatsCount_WebServer->count(); i++ ) {
                if ( this->ui->box_StatsCount_WebServer->itemText( i ) == "Nginx" ) {
                    this->ui->box_StatsCount_WebServer->setCurrentIndex( i );
                    this->ui->box_StatsSpeed_WebServer->setCurrentIndex( i );
                    this->ui->box_StatsDay_WebServer->setCurrentIndex(   i );
                    this->ui->box_StatsRelat_WebServer->setCurrentIndex( i );
                    break;
                }
            }
            break;
        case 13:
            for ( int i=0; i<this->ui->box_StatsCount_WebServer->count(); i++ ) {
                if ( this->ui->box_StatsCount_WebServer->itemText( i ) == "IIS" ) {
                    this->ui->box_StatsCount_WebServer->setCurrentIndex( i );
                    this->ui->box_StatsSpeed_WebServer->setCurrentIndex( i );
                    this->ui->box_StatsDay_WebServer->setCurrentIndex(   i );
                    this->ui->box_StatsRelat_WebServer->setCurrentIndex( i );
                    break;
                }
            }
            break;
        default:
            // shouldn't be here
            throw( &"Unexpected WebServer ID: "[ws] );
    }


    // get a fresh list of LogFiles
    this->craplog_timer = new QTimer(this);
    connect(this->craplog_timer, SIGNAL(timeout()), this, SLOT(wait_ActiveWindow()));
    this->craplog_timer->start(250);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//////////////////////////
//// INTEGRITY CHECKS ////
void MainWindow::wait_ActiveWindow()
{
    if ( this->isActiveWindow() == false ) {
        std::this_thread::sleep_for( std::chrono::milliseconds(250) );
    } else {
        this->craplog_timer->stop();
        this->makeInitialChecks();
    }
}
void MainWindow::makeInitialChecks()
{
    bool ok = true;
    // check that the sqlite plugin is available
    if ( QSqlDatabase::drivers().contains("QSQLITE") == false ) {
        // checks failed, abort
        DialogSec::errSqlDriverNotFound( nullptr, "QSQLITE" );
        ok = false;
    }

    if ( ok == true ) {
        // statistics' database
        if ( CheckSec::checkStatsDatabase( this->db_stats_path ) == false ) {
            // checks failed, abort
            ok = false;
        } else {
            this->craplog.setStatsDatabasePath( this->db_stats_path );
            // used-files' hashes' database
            if ( CheckSec::checkHashesDatabase( this->db_hashes_path ) == false ) {
                // checks failed, abort
                ok = false;
            } else {
                this->craplog.setHashesDatabasePath( this->db_hashes_path );
                if ( this->craplog.hashOps.loadUsedHashesLists( this->db_hashes_path ) == false ) {
                    // failed to load the list, abort
                    ok = false;
                } else {
                    // craplog variables
                    if ( CheckSec::checkCraplog( this->craplog ) == false ) {
                        // checks failed, abort
                        ok = false;
                    }
                }
            }
        }
    }
    if ( ok == false ) {
        this->close();
        //QCoreApplication::exit(0);
        //this->destroy();
    } else {
        // get available stats dates
        this->refreshStatsDates();
        // initialize logs fields boxes
        this->on_box_StatsDay_LogsType_currentIndexChanged(0);
        this->on_box_StatsRelat_LogsType_currentIndexChanged(0);
        // get a fresh list of log files
        this->on_button_LogFiles_RefreshList_clicked();
    }
}


/////////////////////
//// GENERAL USE ////
/////////////////////
// printable size with suffix and limited decimals
QString MainWindow::printableSize( const int bytes )
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
QString MainWindow::printableSpeed( const int bytes, const int secs_ )
{
    std::string speed_str, speed_sfx=" B/s";
    int secs = secs_;
    if ( secs == 0 ) {
        secs = 1;
    }
    float speed = (float)bytes / (float)secs;
    if (speed > 1024) {
        speed /= 1024;
        speed_sfx = " KiB/s";
        if (speed > 1024) {
            speed /= 1024;
            speed_sfx = " MiB/s";
        }
    }
    // cut decimals depending on how big the floor is
    speed_str = std::to_string( speed );
    int cut_index = speed_str.find('.')+1;
    if ( cut_index == 0 ) {
            cut_index = speed_str.find(',')+1;
    }
    int n_decimals = 3;
    if ( speed >= 100 ) {
        n_decimals = 2;
        if ( speed >= 1000 ) {
            n_decimals = 1;
            if ( speed >= 10000 ) {
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

QString MainWindow::printableTime( const int secs_ )
{
    int secs = secs_;
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
      && this->allowed_web_servers.at( 11 ) == true ) {
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
      && this->allowed_web_servers.at( 12 ) == true) {
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
      && this->allowed_web_servers.at( 13 ) == true ) {
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
            item->setForeground( 0, this->COLORS.at( "red" ) );
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
        item->setForeground( 1, this->COLORS.at( col ) );

        // set the name
        item->setText( 0, log_file.name );
        // set the size
        item->setText( 1, this->printableSize( log_file.size ) );
        item->setFont( 1, this->FONTS.at("main_italic") );
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
            DialogSec::errUndefinedLogType( nullptr, item.name );
        }

        if ( proceed == true ) {
            std::string content;
            try {
                content = IOutils::readFile( item.path );
            } catch (const std::ios_base::failure& err) {
                // failed reading
                proceed = false;
                // >> err.what() << //
                DialogSec::errFailedReadFile( nullptr, item.name );
            } catch (...) {
                // failed somehow
                proceed = false;
                DialogSec::errFailedReadFile( nullptr, item.name );
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
        // check files to be used before to start
        proceed = this->craplog.checkStuff();
    } else {
        this->craplogFinished();
    }

    if ( proceed == true ) {
        // periodically update perfs
        this->craplog_timer = new QTimer(this);
        connect(this->craplog_timer, SIGNAL(timeout()), this, SLOT(update_Craplog_PerfData()));
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
    // update values
    int size, secs;
    // size and lines
    if ( this->craplog.isParsing() == true ) {
        this->craplog.collectPerfData();
    }
    size = this->craplog.getParsedSize();
    this->ui->label_MakeStats_Size->setText( this->printableSize( size ) );
    this->ui->label_MakeStats_Lines->setText( QString::fromStdString(std::to_string(this->craplog.getParsedLines())) );
    // time and speed
    this->craplog_timer_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        this->craplog_timer_start - std::chrono::system_clock::now()
    );
    size = this->craplog.getPerfSize();
    secs = this->craplog_timer_elapsed.count() / -1000000000;
    this->ui->label_MakeStats_Time->setText( this->printableTime( secs ));
    this->ui->label_MakeStats_Speed->setText( this->printableSpeed( size, secs ));
}

void MainWindow::update_Craplog_PerfData()
{
    // craplog is running as thread, update the values meanwhile
    this->update_MakeStats_labels();
    // check if Craplog has finished working
    if ( this->craplog.isWorking() == false ) {
        this->craplog_timer->stop();
        this->craplog_thread.join();
        this->craplogFinished();
    }
}

void MainWindow::craplogStarted()
{
    // reset perfs
    this->reset_MakeStats_labels();
    this->craplog.logOps.resetPerfData();
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
    // disable the stats/settings tab
    this->ui->View->setEnabled(false);
    this->ui->Set->setEnabled(false);
}

void MainWindow::craplogFinished()
{
    // update the perf data one last time, just in case
    this->update_MakeStats_labels();
    this->craplog.makeGraphs( this->FONTS, this->ui->chart_MakeStats_Access, this->ui->chart_MakeStats_Error, this->ui->chart_MakeStats_Traffic );
    // clean up temp vars
    this->craplog.clearDataCollection();
    this->craplog.logOps.resetPerfData();

    // refresh the logs list
    this->on_button_LogFiles_RefreshList_clicked();
    // enable the LogFiles section
    this->ui->LogBoxFiles->setEnabled(true);
    // enable all labels (needed only the first time each session)
    this->ui->icon_MakeStats_Size->setEnabled(true);
    this->ui->icon_MakeStats_Lines->setEnabled(true);
    this->ui->icon_MakeStats_Time->setEnabled(true);
    this->ui->icon_MakeStats_Speed->setEnabled(true);
    // enable back the stats/settings tab
    this->ui->View->setEnabled(true);
    this->ui->Set->setEnabled(true);
    // get a fresh collection of available stats dates
    this->refreshStatsDates();
}



///////////////
//// STATS ////
///////////////
// refresh all the dates boxes
void MainWindow::refreshStatsDates()
{
    this->crapview.refreshDates();
    this->on_box_StatsSpeed_WebServer_currentIndexChanged(0);
    this->on_box_StatsCount_WebServer_currentIndexChanged(0);
    this->on_box_StatsDay_WebServer_currentIndexChanged(0);
    this->on_box_StatsRelat_WebServer_currentIndexChanged(0);
}

///////////////
//// SPEED ////
void MainWindow::checkStatsSpeedDrawable()
{
    if ( this->ui->box_StatsSpeed_Year->currentIndex() >= 0
      && this->ui->box_StatsSpeed_Month->currentIndex() >= 0
      && this->ui->box_StatsSpeed_Day->currentIndex() >= 0 ) {
        // enable the draw buttpn
        this->ui->button_StatsSpeed_Draw->setEnabled(true);
    } else {
        // disable the draw button
        this->ui->button_StatsSpeed_Draw->setEnabled(false);
    }
}

void MainWindow::on_box_StatsSpeed_WebServer_currentIndexChanged( int index )
{
    this->ui->box_StatsSpeed_Year->clear();
    this->ui->box_StatsSpeed_Year->addItems(
        this->crapview.getYears(
            this->ui->box_StatsSpeed_WebServer->currentText(),
            "Access" ) );
    this->ui->box_StatsSpeed_Year->setCurrentIndex( 0 );
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_box_StatsSpeed_Year_currentIndexChanged(int index)
{
    this->ui->box_StatsSpeed_Month->clear();
    if ( index != -1 ) {
        this->ui->box_StatsSpeed_Month->addItems(
            this->crapview.getMonths(
                this->ui->box_StatsSpeed_WebServer->currentText(),
                "Access",
                this->ui->box_StatsSpeed_Year->currentText() ) );
        this->ui->box_StatsSpeed_Month->setCurrentIndex( 0 );
    }
}

void MainWindow::on_box_StatsSpeed_Month_currentIndexChanged(int index)
{
    this->ui->box_StatsSpeed_Day->clear();
    if ( index != -1 ) {
        this->ui->box_StatsSpeed_Day->addItems(
            this->crapview.getDays(
                this->ui->box_StatsSpeed_WebServer->currentText(),
                "Access",
                this->ui->box_StatsSpeed_Year->currentText(),
                this->ui->box_StatsSpeed_Month->currentText() ) );
        this->ui->box_StatsSpeed_Day->setCurrentIndex( 0 );
    }
}

void MainWindow::on_button_StatsSpeed_Draw_clicked()
{
    this->crapview.drawSpeed(
        this->ui->table_StatsSpeed_Fields,
        this->ui->chart_SatsSpeed,
        this->FONTS,
        this->ui->box_StatsSpeed_WebServer->currentText(),
        this->ui->box_StatsSpeed_Year->currentText(),
        this->ui->box_StatsSpeed_Month->currentText(),
        this->ui->box_StatsSpeed_Day->currentText(),
        this->ui->inLine_StatsSpeed_Protocol->text(),
        this->ui->inLine_StatsSpeed_Method->text(),
        this->ui->inLine_StatsSpeed_Uri->text(),
        this->ui->inLine_StatsSpeed_Query->text(),
        this->ui->inLine_StatsSpeed_Response->text() );
}


///////////////
//// COUNT ////
void MainWindow::checkStatsCountDrawable()
{
    if ( this->ui->box_StatsCount_Year->currentIndex() >= 0
      && this->ui->box_StatsCount_Month->currentIndex() >= 0
      && this->ui->box_StatsCount_Day->currentIndex() >= 0 ) {
        // enable the draw buttpn
        this->ui->scrollArea_StatsCount_Access->setEnabled(true);
        this->ui->scrollArea_StatsCount_Error->setEnabled(true);
    } else {
        // disable the draw button
        this->ui->scrollArea_StatsCount_Access->setEnabled(false);
        this->ui->scrollArea_StatsCount_Error->setEnabled(false);
    }
}

void MainWindow::on_box_StatsCount_WebServer_currentIndexChanged(int index)
{
    this->ui->box_StatsCount_Year->clear();
    this->ui->box_StatsCount_Year->addItems(
        this->crapview.getYears(
            this->ui->box_StatsCount_WebServer->currentText(),
            this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() )));
    this->ui->box_StatsCount_Year->setCurrentIndex( 0 );
    this->resetStatsCountAccButtons();
    this->resetStatsCountErrButtons();
    this->checkStatsCountDrawable();
}

void MainWindow::on_tabs_StatsCount_AccErr_currentChanged(int index)
{
    this->ui->box_StatsCount_Year->clear();
    this->ui->box_StatsCount_Year->addItems(
        this->crapview.getYears(
            this->ui->box_StatsCount_WebServer->currentText(),
            this->ui->tabs_StatsCount_AccErr->tabText( index ) ));
    this->ui->box_StatsCount_Year->setCurrentIndex( 0 );
    this->checkStatsCountDrawable();
}

void MainWindow::on_box_StatsCount_Year_currentIndexChanged(int index)
{
    this->ui->box_StatsCount_Month->clear();
    if ( index != -1 ) {
        this->ui->box_StatsCount_Month->addItems(
            this->crapview.getMonths(
                this->ui->box_StatsCount_WebServer->currentText(),
                this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
                this->ui->box_StatsCount_Year->currentText() ) );
        this->ui->box_StatsCount_Month->setCurrentIndex( 0 );
    }
}

void MainWindow::on_box_StatsCount_Month_currentIndexChanged(int index)
{
    this->ui->box_StatsCount_Day->clear();
    if ( index != -1 ) {
        this->ui->box_StatsCount_Day->addItems(
            this->crapview.getDays(
                this->ui->box_StatsCount_WebServer->currentText(),
                this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
                this->ui->box_StatsCount_Year->currentText(),
                this->ui->box_StatsCount_Month->currentText() ) );
        this->ui->box_StatsCount_Day->setCurrentIndex( 0 );
    }
}

void MainWindow::resetStatsCountAccButtons()
{
    this->ui->button_StatsCount_Protocol->setFlat( true );
    this->ui->button_StatsCount_Method->setFlat( true );
    this->ui->button_StatsCount_Request->setFlat( true );
    this->ui->button_StatsCount_Query->setFlat( true );
    this->ui->button_StatsCount_Response->setFlat( true );
    this->ui->button_StatsCount_Referrer->setFlat( true );
    this->ui->button_StatsCount_Cookie->setFlat( true );
    this->ui->button_StatsCount_UserAgent->setFlat( true );
    this->ui->button_StatsCount_AccClient->setFlat( true );
}

void MainWindow::resetStatsCountErrButtons()
{
    this->ui->button_StatsCount_Level->setFlat( true );
    this->ui->button_StatsCount_Message->setFlat( true );
    this->ui->button_StatsCount_Source->setFlat( true );
    this->ui->button_StatsCount_Port->setFlat( true );
    this->ui->button_StatsCount_ErrClient->setFlat( true );
}

void MainWindow::on_button_StatsCount_Protocol_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Protocol->text() );
    this->resetStatsCountErrButtons();
    this->resetStatsCountAccButtons();
    this->ui->button_StatsCount_Protocol->setFlat( false );
}

void MainWindow::on_button_StatsCount_Method_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Method->text() );
    this->resetStatsCountErrButtons();
    this->resetStatsCountAccButtons();
    this->ui->button_StatsCount_Method->setFlat( false );
}

void MainWindow::on_button_StatsCount_Request_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Request->text() );
    this->resetStatsCountErrButtons();
    this->resetStatsCountAccButtons();
    this->ui->button_StatsCount_Request->setFlat( false );
}

void MainWindow::on_button_StatsCount_Query_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Query->text() );
    this->resetStatsCountErrButtons();
    this->resetStatsCountAccButtons();
    this->ui->button_StatsCount_Query->setFlat( false );
}

void MainWindow::on_button_StatsCount_Response_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Response->text() );
    this->resetStatsCountErrButtons();
    this->resetStatsCountAccButtons();
    this->ui->button_StatsCount_Response->setFlat( false );
}

void MainWindow::on_button_StatsCount_Referrer_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Referrer->text() );
    this->resetStatsCountErrButtons();
    this->resetStatsCountAccButtons();
    this->ui->button_StatsCount_Referrer->setFlat( false );
}

void MainWindow::on_button_StatsCount_Cookie_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Cookie->text() );
    this->resetStatsCountErrButtons();
    this->resetStatsCountAccButtons();
    this->ui->button_StatsCount_Cookie->setFlat( false );
}

void MainWindow::on_button_StatsCount_UserAgent_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_UserAgent->text() );
    this->resetStatsCountErrButtons();
    this->resetStatsCountAccButtons();
    this->ui->button_StatsCount_UserAgent->setFlat( false );
}

void MainWindow::on_button_StatsCount_AccClient_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_AccClient->text() );
    this->resetStatsCountErrButtons();
    this->resetStatsCountAccButtons();
    this->ui->button_StatsCount_AccClient->setFlat( false );
}


void MainWindow::on_button_StatsCount_Level_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Level->text() );
    this->resetStatsCountAccButtons();
    this->resetStatsCountErrButtons();
    this->ui->button_StatsCount_Level->setFlat( false );
}

void MainWindow::on_button_StatsCount_Message_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Message->text() );
    this->resetStatsCountAccButtons();
    this->resetStatsCountErrButtons();
    this->ui->button_StatsCount_Message->setFlat( false );
}

void MainWindow::on_button_StatsCount_Source_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Source->text() );
    this->resetStatsCountAccButtons();
    this->resetStatsCountErrButtons();
    this->ui->button_StatsCount_Source->setFlat( false );
}

void MainWindow::on_button_StatsCount_Port_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_Port->text() );
    this->resetStatsCountAccButtons();
    this->resetStatsCountErrButtons();
    this->ui->button_StatsCount_Port->setFlat( false );
}

void MainWindow::on_button_StatsCount_ErrClient_clicked()
{
    this->crapview.drawCount(
        this->ui->chart_StatsCount, this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        this->ui->button_StatsCount_ErrClient->text() );
    this->resetStatsCountAccButtons();
    this->resetStatsCountErrButtons();
    this->ui->button_StatsCount_ErrClient->setFlat( false );
}


/////////////
//// DAY ////
void MainWindow::checkStatsDayDrawable()
{
    bool aux = true;
    // secondary date (period)
    if ( this->ui->checkBox_StatsDay_Period->isChecked() == true ) {
        if ( this->ui->box_StatsDay_ToYear->currentIndex() < 0
          && this->ui->box_StatsDay_ToMonth->currentIndex() < 0
          && this->ui->box_StatsDay_ToDay->currentIndex() < 0 ) {
           aux = false;
        } else {
            int a,b;
            a = this->ui->box_StatsDay_ToYear->currentText().toInt();
            b = this->ui->box_StatsDay_FromYear->currentText().toInt();
            if ( a < b ) {
                // year 'to' is less than 'from'
                aux = false;
            } else if ( a == b ) {
                a = this->crapview.getMonthNumber( this->ui->box_StatsDay_ToMonth->currentText() );
                b = this->crapview.getMonthNumber( this->ui->box_StatsDay_FromMonth->currentText() );
                if ( a < b ) {
                    // month 'to' is less than 'from'
                    aux = false;
                } else if ( a == b ) {
                    a = this->ui->box_StatsDay_ToDay->currentText().toInt();
                    b = this->ui->box_StatsDay_FromDay->currentText().toInt();
                    if ( a < b ) {
                        // day 'to' is less than 'from'
                        aux = false;
                    }
                }
            }
        }
    }
    // primary date
    if ( this->ui->box_StatsDay_FromYear->currentIndex() >= 0
      && this->ui->box_StatsDay_FromMonth->currentIndex() >= 0
      && this->ui->box_StatsDay_FromDay->currentIndex() >= 0
      && aux == true ) {
        // enable the draw buttpn
        this->ui->button_StatsDay_Draw->setEnabled(true);
    } else {
        // disable the draw button
        this->ui->button_StatsDay_Draw->setEnabled(false);
    }
}

void MainWindow::on_box_StatsDay_WebServer_currentIndexChanged(int index)
{
    QStringList years = this->crapview.getYears(
        this->ui->box_StatsDay_WebServer->currentText(),
        this->ui->box_StatsDay_LogsType->currentText() );

    this->ui->box_StatsDay_FromYear->clear();
    this->ui->box_StatsDay_FromYear->addItems( years );
    this->ui->box_StatsDay_FromYear->setCurrentIndex( 0 );
    if ( this->ui->checkBox_StatsDay_Period->isChecked() == true ) {
        this->ui->box_StatsDay_ToYear->clear();
        this->ui->box_StatsDay_ToYear->addItems( years );
        this->ui->box_StatsDay_ToYear->setCurrentIndex( 0 );
    }
    years.clear();
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_LogsType_currentIndexChanged(int index)
{
    this->on_box_StatsDay_WebServer_currentIndexChanged(0);
    this->ui->box_StatsDay_LogsField->clear();
    if ( index != -1 ) {
        this->ui->box_StatsDay_LogsField->addItems(
            this->crapview.getFields(
                "Daytime",
                this->ui->box_StatsDay_LogsType->currentText() ));
        this->ui->box_StatsDay_LogsField->setCurrentIndex( 0 );
    }
}


void MainWindow::on_box_StatsDay_LogsField_currentIndexChanged(int index)
{
    this->ui->inLine_StatsDay_Filter->clear();
}

void MainWindow::on_box_StatsDay_FromYear_currentIndexChanged(int index)
{
    this->ui->box_StatsDay_FromMonth->clear();
    if ( index != -1 ) {
        this->ui->box_StatsDay_FromMonth->addItems(
            this->crapview.getMonths(
                this->ui->box_StatsDay_WebServer->currentText(),
                this->ui->box_StatsDay_LogsType->currentText(),
                this->ui->box_StatsDay_FromYear->currentText() ) );
        this->ui->box_StatsDay_FromMonth->setCurrentIndex( 0 );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_FromMonth_currentIndexChanged(int index)
{
    this->ui->box_StatsDay_FromDay->clear();
    if ( index != -1 ) {
        this->ui->box_StatsDay_FromDay->addItems(
            this->crapview.getDays(
                this->ui->box_StatsDay_WebServer->currentText(),
                this->ui->box_StatsDay_LogsType->currentText(),
                this->ui->box_StatsDay_FromYear->currentText(),
                this->ui->box_StatsDay_FromMonth->currentText() ) );
        this->ui->box_StatsDay_FromDay->setCurrentIndex( 0 );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_FromDay_currentIndexChanged(int index)
{
    this->checkStatsDayDrawable();
}

void MainWindow::on_checkBox_StatsDay_Period_stateChanged(int state)
{
    if ( state == Qt::CheckState::Checked ) {
        this->ui->box_StatsDay_ToYear->setEnabled( true );
        this->ui->box_StatsDay_ToMonth->setEnabled( true );
        this->ui->box_StatsDay_ToDay->setEnabled( true );
        // add available dates
        this->ui->box_StatsDay_ToYear->addItems( this->crapview.getYears(
            this->ui->box_StatsDay_WebServer->currentText(),
            this->ui->box_StatsDay_LogsType->currentText() ) );
        this->ui->box_StatsDay_ToYear->setCurrentIndex( 0 );
    } else {
        this->ui->box_StatsDay_ToYear->clear();
        this->ui->box_StatsDay_ToYear->setEnabled( false );
        this->ui->box_StatsDay_ToMonth->clear();
        this->ui->box_StatsDay_ToMonth->setEnabled( false );
        this->ui->box_StatsDay_ToDay->clear();
        this->ui->box_StatsDay_ToDay->setEnabled( false );
    }
}

void MainWindow::on_box_StatsDay_ToYear_currentIndexChanged(int index)
{
    this->ui->box_StatsDay_ToMonth->clear();
    if ( index != -1 ) {
        this->ui->box_StatsDay_ToMonth->addItems(
            this->crapview.getMonths(
                this->ui->box_StatsDay_WebServer->currentText(),
                this->ui->box_StatsDay_LogsType->currentText(),
                this->ui->box_StatsDay_ToYear->currentText() ) );
        this->ui->box_StatsDay_ToMonth->setCurrentIndex( 0 );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_ToMonth_currentIndexChanged(int index)
{
    this->ui->box_StatsDay_ToDay->clear();
    if ( index != -1 ) {
        this->ui->box_StatsDay_ToDay->addItems(
            this->crapview.getDays(
                this->ui->box_StatsDay_WebServer->currentText(),
                this->ui->box_StatsDay_LogsType->currentText(),
                this->ui->box_StatsDay_ToYear->currentText(),
                this->ui->box_StatsDay_ToMonth->currentText() ) );
        this->ui->box_StatsDay_ToDay->setCurrentIndex( 0 );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_ToDay_currentIndexChanged(int index)
{
    this->checkStatsDayDrawable();
}


void MainWindow::on_button_StatsDay_Draw_clicked()
{
    std::cout << this->ui->inLine_StatsDay_Filter->text().toStdString() << std::endl;
    this->crapview.drawDay(
        this->ui->chart_StatsDay,
        this->FONTS,
        this->ui->box_StatsDay_WebServer->currentText(),
        this->ui->box_StatsDay_LogsType->currentText(),
        this->ui->box_StatsDay_FromYear->currentText(),
        this->ui->box_StatsDay_FromMonth->currentText(),
        this->ui->box_StatsDay_FromDay->currentText(),
        ( this->ui->checkBox_StatsDay_Period->isChecked() ) ? this->ui->box_StatsDay_ToYear->currentText() : "",
        ( this->ui->checkBox_StatsDay_Period->isChecked() ) ? this->ui->box_StatsDay_ToMonth->currentText() : "",
        ( this->ui->checkBox_StatsDay_Period->isChecked() ) ? this->ui->box_StatsDay_ToDay->currentText() : "",
        this->ui->box_StatsDay_LogsField->currentText(),
        this->ui->inLine_StatsDay_Filter->text() );
}



////////////////////
//// RELATIONAL ////
void MainWindow::checkStatsRelatDrawable()
{
    bool aux = true;
    if ( this->ui->box_StatsRelat_FromYear->currentIndex() >= 0
      && this->ui->box_StatsRelat_FromMonth->currentIndex() >= 0
      && this->ui->box_StatsRelat_FromDay->currentIndex() >= 0
      && this->ui->box_StatsRelat_ToYear->currentIndex() >= 0
      && this->ui->box_StatsRelat_ToMonth->currentIndex() >= 0
      && this->ui->box_StatsRelat_ToDay->currentIndex() >= 0 ) {
        // check period validity
        int a,b;
        a = this->ui->box_StatsRelat_ToYear->currentText().toInt();
        b = this->ui->box_StatsRelat_FromYear->currentText().toInt();
        if ( a < b ) {
            // year 'to' is less than 'from'
            aux = false;
        } else if ( a == b ) {
            a = this->crapview.getMonthNumber( this->ui->box_StatsRelat_ToMonth->currentText() );
            b = this->crapview.getMonthNumber( this->ui->box_StatsRelat_FromMonth->currentText() );
            if ( a < b ) {
                // month 'to' is less than 'from'
                aux = false;
            } else if ( a == b ) {
                a = this->ui->box_StatsRelat_ToDay->currentText().toInt();
                b = this->ui->box_StatsRelat_FromDay->currentText().toInt();
                if ( a < b ) {
                    // day 'to' is less than 'from'
                    aux = false;
                }
            }
        }
    } else {
        // disable the draw button
        aux = false;
    }
    this->ui->button_StatsRelat_Draw->setEnabled( aux );
}

void MainWindow::on_box_StatsRelat_WebServer_currentIndexChanged(int index)
{
    QStringList years = this->crapview.getYears(
        this->ui->box_StatsRelat_WebServer->currentText(),
        this->ui->box_StatsRelat_LogsType->currentText() );
    // from
    this->ui->box_StatsRelat_FromYear->clear();
    this->ui->box_StatsRelat_FromYear->addItems( years );
    this->ui->box_StatsRelat_FromYear->setCurrentIndex( 0 );
    // to
    this->ui->box_StatsRelat_ToYear->clear();
    this->ui->box_StatsRelat_ToYear->addItems( years );
    this->ui->box_StatsRelat_ToYear->setCurrentIndex( 0 );
    years.clear();
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_LogsType_currentIndexChanged(int index)
{
    this->on_box_StatsRelat_WebServer_currentIndexChanged(0);
    this->ui->box_StatsRelat_LogsField_1->clear();
    this->ui->box_StatsRelat_LogsField_2->clear();
    if ( index != -1 ) {
        QStringList fields = this->crapview.getFields(
            "Relational",
            this->ui->box_StatsRelat_LogsType->currentText() );
        this->ui->box_StatsRelat_LogsField_1->addItems( fields );
        this->ui->box_StatsRelat_LogsField_2->addItems( fields );
        this->ui->box_StatsRelat_LogsField_1->setCurrentIndex( 0 );
        this->ui->box_StatsRelat_LogsField_2->setCurrentIndex( 0 );
    }
}

void MainWindow::on_box_StatsRelat_LogsField_1_currentIndexChanged(int index)
{
    this->ui->inLine_StatsRelat_Filter_1->clear();
}

void MainWindow::on_box_StatsRelat_LogsField_2_currentIndexChanged(int index)
{
    this->ui->inLine_StatsRelat_Filter_2->clear();
}

void MainWindow::on_box_StatsRelat_FromYear_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_FromMonth->clear();
    if ( index != -1 ) {
        this->ui->box_StatsRelat_FromMonth->addItems(
            this->crapview.getMonths(
                this->ui->box_StatsRelat_WebServer->currentText(),
                this->ui->box_StatsRelat_LogsType->currentText(),
                this->ui->box_StatsRelat_FromYear->currentText() ) );
        this->ui->box_StatsRelat_FromMonth->setCurrentIndex( 0 );
    }
}

void MainWindow::on_box_StatsRelat_FromMonth_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_FromDay->clear();
    if ( index != -1 ) {
        this->ui->box_StatsRelat_FromDay->addItems(
            this->crapview.getDays(
                this->ui->box_StatsRelat_WebServer->currentText(),
                this->ui->box_StatsRelat_LogsType->currentText(),
                this->ui->box_StatsRelat_FromYear->currentText(),
                this->ui->box_StatsRelat_FromMonth->currentText() ) );
        this->ui->box_StatsRelat_FromDay->setCurrentIndex( 0 );
    }
}

void MainWindow::on_box_StatsRelat_FromDay_currentIndexChanged(int index)
{

}

void MainWindow::on_box_StatsRelat_ToYear_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_ToMonth->clear();
    if ( index != -1 ) {
        this->ui->box_StatsRelat_ToMonth->addItems(
            this->crapview.getMonths(
                this->ui->box_StatsRelat_WebServer->currentText(),
                this->ui->box_StatsRelat_LogsType->currentText(),
                this->ui->box_StatsRelat_FromYear->currentText() ) );
        this->ui->box_StatsRelat_ToMonth->setCurrentIndex( 0 );
    }
}

void MainWindow::on_box_StatsRelat_ToMonth_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_ToDay->clear();
    if ( index != -1 ) {
        this->ui->box_StatsRelat_ToDay->addItems(
            this->crapview.getDays(
                this->ui->box_StatsRelat_WebServer->currentText(),
                this->ui->box_StatsRelat_LogsType->currentText(),
                this->ui->box_StatsRelat_FromYear->currentText(),
                this->ui->box_StatsRelat_FromMonth->currentText() ) );
        this->ui->box_StatsRelat_ToDay->setCurrentIndex( 0 );
    }
}

void MainWindow::on_box_StatsRelat_ToDay_currentIndexChanged(int index)
{

}


void MainWindow::on_button_StatsRelat_Draw_clicked()
{
    this->crapview.drawRelat(
        this->ui->chart_StatsRelat,
        this->FONTS,
        this->ui->box_StatsRelat_WebServer->currentText(),
        this->ui->box_StatsRelat_LogsType->currentText(),
        this->ui->box_StatsRelat_FromYear->currentText(),
        this->ui->box_StatsRelat_FromMonth->currentText(),
        this->ui->box_StatsRelat_FromDay->currentText(),
        this->ui->box_StatsRelat_ToYear->currentText(),
        this->ui->box_StatsRelat_ToMonth->currentText(),
        this->ui->box_StatsRelat_ToDay->currentText(),
        this->ui->box_StatsRelat_LogsField_1->currentText(),
        this->ui->inLine_StatsRelat_Filter_1->text(),
        this->ui->box_StatsRelat_LogsField_2->currentText(),
        this->ui->inLine_StatsRelat_Filter_2->text() );
}



