
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
    // load the alternative font
    this->alternative_font_family = QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Hack")).at(0);
    // load the script font
    this->script_font_family = QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/3270")).at(0);
    // initialize the fonts map
    this->FONTS.emplace( "main", QFont(
        this->main_font_family,
        this->font_size ) );
    this->FONTS.emplace( "main_italic", QFont(
        this->main_font_family,
        this->font_size,
        -1, true ) );
    this->FONTS.emplace( "main_bold", QFont(
        this->main_font_family,
        this->font_size,
        1 ) );
    this->FONTS.emplace( "main_big", QFont(
        this->main_font_family,
        this->font_size_big ) );
    this->FONTS.emplace( "main_small", QFont(
        this->main_font_family,
        this->font_size_small ) );
    this->FONTS.emplace( "alternative", QFont(
        this->alternative_font_family,
        this->font_size ) );
    this->FONTS.emplace( "script", QFont(
        this->script_font_family,
        this->font_size ) );

    // parent font for every tab
    this->ui->CrapTabs->setFont( this->FONTS.at( "main_big" ) );

    // WebServers buttons for the LogFiles
    this->ui->button_LogFiles_Apache->setFont( this->FONTS.at( "main" ) );
    this->ui->button_LogFiles_Nginx->setFont( this->FONTS.at(  "main" ) );
    this->ui->button_LogFiles_Iis->setFont( this->FONTS.at(    "main" ) );
    // TreeView for the LogFiles
    this->ui->checkBox_LogFiles_CheckAll->setFont( this->FONTS.at( "main_small" ) );
    this->ui->listLogFiles->setFont( this->FONTS.at( "main" ) );
    // TextBrowser for the LogFiles
    this->TB.setColorScheme( 1, this->TB_COLOR_SCHEMES.at( 1 ) );
    this->TB.setFontFamily( this->main_font_family );
    this->TB.setFont( this->FONTS.at( "main" ) );
    this->ui->textLogFiles->setFont( this->TB.getFont() );
    // MakeStats labels
    this->ui->label_MakeStats_Size->setFont( this->FONTS.at(  "main"  ) );
    this->ui->label_MakeStats_Lines->setFont( this->FONTS.at( "main" ) );
    this->ui->label_MakeStats_Time->setFont( this->FONTS.at(  "main" ) );
    this->ui->label_MakeStats_Speed->setFont( this->FONTS.at( "main" ) );

    // StatsSpeed table
    this->ui->table_StatsSpeed->setFont( this->FONTS.at("main") );

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
    this->allowed_web_servers.emplace( this->APACHE_ID, true );
    this->allowed_web_servers.emplace( this->NGINX_ID,  true );
    this->allowed_web_servers.emplace( this->IIS_ID,    true );


    /////////////////
    //// CONFIGS ////
    this->craplog.setDialogLevel( 1 ); // !!! REPLACE WITH CONFIGURATION VALUE !!!

    // set user-definet tabs indexes => this->ui->StatsTabs->widget( 0 );


    ///////////////////
    //// POLISHING ////
    // disable the unallowed WebServers
    int ws = 14;
    for ( int id=13; id>10; id-- ) {
        if ( this->allowed_web_servers.at( id ) == true ) {
            ws = ( id < ws ) ? id : ws; // to be used later on
        } else {
            switch (id) {
                case 11:
                    this->ui->button_LogFiles_Apache->setEnabled( false );
                    this->ui->box_StatsWarn_WebServer->removeItem(  0 );
                    this->ui->box_StatsCount_WebServer->removeItem( 0 );
                    this->ui->box_StatsSpeed_WebServer->removeItem( 0 );
                    this->ui->box_StatsDay_WebServer->removeItem(   0 );
                    this->ui->box_StatsRelat_WebServer->removeItem( 0 );
                    this->ui->button_StatsGlob_Apache->setEnabled( false );
                    break;
                case 12:
                    this->ui->button_LogFiles_Nginx->setEnabled( false );
                    this->ui->box_StatsWarn_WebServer->removeItem(  1 );
                    this->ui->box_StatsCount_WebServer->removeItem( 1 );
                    this->ui->box_StatsSpeed_WebServer->removeItem( 1 );
                    this->ui->box_StatsDay_WebServer->removeItem(   1 );
                    this->ui->box_StatsRelat_WebServer->removeItem( 1 );
                    this->ui->button_StatsGlob_Nginx->setEnabled( false );
                    break;
                case 13:
                    this->ui->button_LogFiles_Iis->setEnabled( false );
                    this->ui->box_StatsWarn_WebServer->removeItem(  2 );
                    this->ui->box_StatsCount_WebServer->removeItem( 2 );
                    this->ui->box_StatsSpeed_WebServer->removeItem( 2 );
                    this->ui->box_StatsDay_WebServer->removeItem(   2 );
                    this->ui->box_StatsRelat_WebServer->removeItem( 2 );
                    this->ui->button_StatsGlob_Iis->setEnabled( false );
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
        this->ui->box_StatsWarn_WebServer->addItem(  "Apache2" );
        this->ui->box_StatsCount_WebServer->addItem( "Apache2" );
        this->ui->box_StatsSpeed_WebServer->addItem( "Apache2" );
        this->ui->box_StatsDay_WebServer->addItem(   "Apache2" );
        this->ui->box_StatsRelat_WebServer->addItem( "Apache2" );
        this->ui->button_StatsGlob_Apache->setEnabled( true );
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

    // set the current WS for the ViewStats and make them initialize
    switch ( ws ) {
        case 11:
            // already set to index 0 by default
            this->ui->box_StatsWarn_WebServer->setCurrentIndex(  0 );
            this->ui->box_StatsCount_WebServer->setCurrentIndex( 0 );
            this->ui->box_StatsSpeed_WebServer->setCurrentIndex( 0 );
            this->ui->box_StatsDay_WebServer->setCurrentIndex(   0 );
            this->ui->box_StatsRelat_WebServer->setCurrentIndex( 0 );
            break;
        case 12:
            for ( int i=0; i<this->ui->box_StatsCount_WebServer->count(); i++ ) {
                if ( this->ui->box_StatsCount_WebServer->itemText( i ) == "Nginx" ) {
                    this->ui->box_StatsWarn_WebServer->setCurrentIndex(  i );
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
                    this->ui->box_StatsWarn_WebServer->setCurrentIndex(  i );
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


    // make the Configs initialize
    // window
    this->ui->checkBox_ConfWindow_Geometry->setChecked( this->remember_window );
    this->ui->box_ConfWindow_Theme->setCurrentIndex( this->window_theme_id );
    // dialogs
    this->ui->slider_ConfDialogs_General->setValue( this->dialogs_Level );
    this->ui->slider_ConfDialogs_Logs->setValue( this->craplog.getDialogsLevel() );
    this->ui->slider_ConfDialogs_Stats->setValue( this->crapview.getDialogsLevel() );
    // text browser
    this->ui->box_ConfTextBrowser_Font->setCurrentText( this->TB.getFontFamily() );
    this->ui->checkBox_ConfTextBrowser_WideLines->setChecked( this->TB.getWideLinesUsage() );
    this->ui->box_ConfTextBrowser_ColorScheme->setCurrentIndex( this->TB.getColorSchemeID() );
    this->refreshTextBrowserPreview();
    // charts
    this->ui->box_ConfCharts_Theme->setCurrentIndex( this->CHARTS_THEMES.at( this->charts_theme_id ) );
    this->refreshChartsPreview();
    // logs control
    this->ui->checkBox_ConfControl_Usage->setChecked( ! this->display_used_files );
    this->ui->spinBox_ConfControl_Size->setValue( this->craplog.getWarningSize() / 1'048'576 );
    if ( this->craplog.getWarningSize() > 0 ) {
        this->ui->checkBox_ConfControl_Size->setChecked( true );
    } else {
        this->ui->checkBox_ConfControl_Size->setChecked( false );
    }
    // apache paths
    this->ui->inLine_ConfApache_Paths_AccPath->setText( QString::fromStdString(this->craplog.getLogsPath( this->APACHE_ID, this->ACCESS_LOGS )) );
    if ( this->craplog.getLogsPath( this->APACHE_ID, this->ACCESS_LOGS ) != this->craplog.getLogsPath( this->APACHE_ID, this->ERROR_LOGS ) ) {
        this->ui->checkBox_ConfApache_Paths_Different->setChecked( true );
        this->ui->inLine_ConfApache_Paths_ErrPath->setText( QString::fromStdString(this->craplog.getLogsPath( this->APACHE_ID, this->ERROR_LOGS )) );
    } else {
        this->ui->icon_ConfApache_Paths_ErrWrong->setVisible( false );
    }
    // apache formats
    this->ui->inLine_ConfApache_Formats_AccString->setText( QString::fromStdString( this->craplog.getAccessLogsFormatString( this->APACHE_ID ) ) );
    this->ui->button_ConfApache_Formats_AccSave->setEnabled( false );
    this->ui->inLine_ConfApache_Formats_ErrString->setText( QString::fromStdString( this->craplog.getErrorLogsFormatString( this->APACHE_ID ) ) );
    this->ui->button_ConfApache_Formats_ErrSave->setEnabled( false );
    // apache warnlists
    this->on_box_ConfApache_Warnlist_Acc_currentTextChanged( this->ui->box_ConfApache_Warnlist_Acc->currentText() );
    this->on_box_ConfApache_Warnlist_Err_currentTextChanged( this->ui->box_ConfApache_Warnlist_Err->currentText() );
    // apache blacklists
    this->on_box_ConfApache_Blacklist_Acc_currentTextChanged( this->ui->box_ConfApache_Blacklist_Acc->currentText() );
    this->on_box_ConfApache_Blacklist_Err_currentTextChanged( this->ui->box_ConfApache_Blacklist_Err->currentText() );
    // nginx paths
    this->ui->inLine_ConfNginx_Paths_AccPath->setText( QString::fromStdString(this->craplog.getLogsPath( this->NGINX_ID, this->ACCESS_LOGS )) );
    if ( this->craplog.getLogsPath( this->NGINX_ID, this->ACCESS_LOGS ) != this->craplog.getLogsPath( this->NGINX_ID, this->ERROR_LOGS ) ) {
        this->ui->checkBox_ConfNginx_Paths_Different->setChecked( true );
        this->ui->inLine_ConfNginx_Paths_ErrPath->setText( QString::fromStdString(this->craplog.getLogsPath( this->NGINX_ID, this->ERROR_LOGS )) );
    } else {
        this->ui->icon_ConfNginx_Paths_ErrWrong->setVisible( false );
    }
    // nginx formats
    this->ui->inLine_ConfNginx_Formats_AccString->setText( QString::fromStdString( this->craplog.getAccessLogsFormatString( this->NGINX_ID ) ) );
    this->ui->button_ConfNginx_Formats_AccSave->setEnabled( false );
    this->ui->inLine_ConfNginx_Formats_ErrString->setText( QString::fromStdString( this->craplog.getErrorLogsFormatString( this->NGINX_ID ) ) );
    this->ui->button_ConfNginx_Formats_ErrSave->setEnabled( false );
    // nginx warnlists
    this->on_box_ConfNginx_Warnlist_Acc_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Acc->currentText() );
    this->on_box_ConfNginx_Warnlist_Err_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Err->currentText() );
    // nginx blacklists
    this->on_box_ConfNginx_Blacklist_Acc_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Acc->currentText() );
    this->on_box_ConfNginx_Blacklist_Err_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Err->currentText() );
    // iis paths
    this->ui->inLine_ConfIis_Paths_AccPath->setText( QString::fromStdString(this->craplog.getLogsPath( this->IIS_ID, this->ACCESS_LOGS )) );
    if ( this->craplog.getLogsPath( this->IIS_ID, this->ACCESS_LOGS ) != this->craplog.getLogsPath( this->IIS_ID, this->ERROR_LOGS ) ) {
        this->ui->checkBox_ConfIis_Paths_Different->setChecked( true );
        this->ui->inLine_ConfIis_Paths_ErrPath->setText( QString::fromStdString(this->craplog.getLogsPath( this->IIS_ID, this->ERROR_LOGS )) );
    } else {
        this->ui->icon_ConfIis_Paths_ErrWrong->setVisible( false );
    }
    // iis formats
    this->ui->inLine_ConfIis_Formats_AccString->setText( QString::fromStdString( this->craplog.getAccessLogsFormatString( this->IIS_ID ) ) );
    this->ui->button_ConfIis_Formats_AccSave->setEnabled( false );
    this->ui->inLine_ConfIis_Formats_ErrString->setText( QString::fromStdString( this->craplog.getErrorLogsFormatString( this->IIS_ID ) ) );
    this->ui->button_ConfIis_Formats_ErrSave->setEnabled( false );
    // iis warnlists
    this->on_box_ConfIis_Warnlist_Acc_currentTextChanged( this->ui->box_ConfIis_Warnlist_Acc->currentText() );
    this->on_box_ConfIis_Warnlist_Err_currentTextChanged( this->ui->box_ConfIis_Warnlist_Err->currentText() );
    // iis blacklists
    this->on_box_ConfIis_Blacklist_Acc_currentTextChanged( this->ui->box_ConfIis_Blacklist_Acc->currentText() );
    this->on_box_ConfIis_Blacklist_Err_currentTextChanged( this->ui->box_ConfIis_Blacklist_Err->currentText() );



    // get a fresh list of LogFiles
    this->craplog_timer = new QTimer(this);
    connect(this->craplog_timer, SIGNAL(timeout()), this, SLOT(wait_ActiveWindow()));
    this->craplog_timer->start(250);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    // save actual configurations

    // save tabs positions => this->ui->CrapTabs->tabText( 0 );
    //                     => this->ui->CrapTabs->tabText( 0 );
    this->ui->StatsTabs->move( 0,1 );
    // save splitters sizes => this->ui->splitter_StatsCount->sizes();
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
        QString rich_text;
        RichText::richLogsDefault( rich_text );
        this->ui->textLogFiles->setText( rich_text );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        rich_text.clear();
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
        QString rich_text;
        RichText::richLogsDefault( rich_text );
        this->ui->textLogFiles->setText( rich_text );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        rich_text.clear();
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
        QString rich_text;
        RichText::richLogsDefault( rich_text );
        this->ui->textLogFiles->setText( rich_text );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        rich_text.clear();
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
        QTreeWidgetItem *item = new QTreeWidgetItem();

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
                QString rich_content;
                RichText::enrichLogs(
                    rich_content, content,
                    format, this->TB );
                this->ui->textLogFiles->setText( rich_content );
                this->ui->textLogFiles->setFont( this->TB.getFont() );
                rich_content.clear();
            }
            content.clear();
        }
        if ( proceed == false ) {
            // failed to read
            QString rich_text;
            RichText::richLogsFailure( rich_text );
            this->ui->textLogFiles->setText( rich_text );
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
    bool proceed = true;
    // check that the format has been set
    const FormatOps::LogsFormat& lf = this->craplog.getAccessLogsFormat( this->craplog.getCurrentWSID() );
    if ( lf.string.size() == 0 ) {
        // format string not set
        proceed = false;
        warn
    } else if ( lf.fields.size() == 0 ) {
        // no field, useless to parse
        proceed = false;
    } else if ( lf.separators.size() == 0 && lf.fields.size() > 1 ) {
        // no separator, but more than one field
        proceed = false;
    }

    if ( proceed == true ) {
        // take actions on Craplog's start
        this->craplogStarted();

        // feed craplog with the checked files
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
    this->craplog.makeCharts(
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->ui->chart_MakeStats_Access, this->ui->chart_MakeStats_Error, this->ui->chart_MakeStats_Traffic );
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
    this->on_box_StatsWarn_WebServer_currentIndexChanged(0);
    this->on_box_StatsSpeed_WebServer_currentIndexChanged(0);
    this->on_box_StatsCount_WebServer_currentIndexChanged(0);
    this->on_box_StatsDay_WebServer_currentIndexChanged(0);
    this->on_box_StatsRelat_WebServer_currentIndexChanged(0);
}


//////////////
//// WARN ////
void MainWindow::checkStatsWarnDrawable()
{
    if ( this->ui->box_StatsWarn_Year->currentIndex() >= 0
      && this->ui->box_StatsWarn_Month->currentIndex() >= 0
      && this->ui->box_StatsWarn_Day->currentIndex() >= 0 ) {
        // enable the draw buttpn
        this->ui->button_StatsWarn_Draw->setEnabled( true );
    } else {
        // disable the draw button
        this->ui->button_StatsWarn_Draw->setEnabled( false );
    }
}

void MainWindow::on_box_StatsWarn_WebServer_currentIndexChanged(int index)
{
    if ( this->ui->box_StatsWarn_LogsType->currentIndex() != 0 ) {
        this->ui->box_StatsWarn_LogsType->setCurrentIndex( 0 );
    } else {
        this->on_box_StatsWarn_LogsType_currentIndexChanged( 0 );
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_box_StatsWarn_LogsType_currentIndexChanged(int index)
{
    this->ui->box_StatsWarn_Year->clear();
    if ( index != -1 ) {
        this->ui->box_StatsWarn_Year->addItems(
            this->crapview.getYears(
                this->ui->box_StatsWarn_WebServer->currentText(),
                this->ui->box_StatsWarn_LogsType->currentText() ));
        this->ui->box_StatsWarn_Year->setCurrentIndex( 0 );
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_box_StatsWarn_Year_currentIndexChanged(int index)
{
    this->ui->box_StatsWarn_Month->clear();
    if ( index != -1 ) {
        this->ui->box_StatsWarn_Month->addItems(
            this->crapview.getMonths(
                this->ui->box_StatsWarn_WebServer->currentText(),
                this->ui->box_StatsWarn_LogsType->currentText(),
                this->ui->box_StatsWarn_Year->currentText() ) );
        this->ui->box_StatsWarn_Month->setCurrentIndex( 0 );
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_box_StatsWarn_Month_currentIndexChanged(int index)
{
    this->ui->box_StatsWarn_Day->clear();
    if ( index != -1 ) {
        this->ui->box_StatsWarn_Day->addItems(
            this->crapview.getDays(
                this->ui->box_StatsWarn_WebServer->currentText(),
                this->ui->box_StatsWarn_LogsType->currentText(),
                this->ui->box_StatsWarn_Year->currentText(),
                this->ui->box_StatsWarn_Month->currentText() ) );
        this->ui->box_StatsWarn_Day->setCurrentIndex( 0 );
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_box_StatsWarn_Day_currentIndexChanged(int index)
{
    if ( this->ui->checkBox_StatsWarn_Hour->isChecked() == true ) {
        this->ui->box_StatsWarn_Hour->clear();
        if ( index != -1 ) {
            this->ui->box_StatsWarn_Hour->addItems( this->crapview.getHours() );
            this->ui->box_StatsWarn_Hour->setCurrentIndex( 0 );
        }
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_checkBox_StatsWarn_Hour_stateChanged(int state)
{
    if ( state == Qt::CheckState::Checked ) {
        this->ui->box_StatsWarn_Hour->setEnabled( true );
        // add available dates
        this->on_box_StatsWarn_Day_currentIndexChanged( 0 );
    } else {
        this->ui->box_StatsWarn_Hour->clear();
        this->ui->box_StatsWarn_Hour->setEnabled( false );
    }
}

void MainWindow::on_box_StatsWarn_Hour_currentIndexChanged(int index)
{
    this->checkStatsWarnDrawable();
}

void MainWindow::on_button_StatsWarn_Draw_clicked()
{
    this->ui->table_StatsWarn->setRowCount(0);
    this->crapview.drawWarn(
        this->ui->table_StatsWarn, this->ui->chart_StatsWarn,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->ui->box_StatsWarn_WebServer->currentText(),
        this->ui->box_StatsWarn_LogsType->currentText(),
        this->ui->box_StatsWarn_Year->currentText(),
        this->ui->box_StatsWarn_Month->currentText(),
        this->ui->box_StatsWarn_Day->currentText(),
        (this->ui->checkBox_StatsWarn_Hour->isChecked()) ? this->ui->box_StatsWarn_Hour->currentText() : "" );
    this->ui->button_StatsWarn_Update->setEnabled( true );
}


void MainWindow::on_button_StatsWarn_Update_clicked()
{
    this->crapview.updateWarn(
        this->ui->table_StatsWarn,
        this->ui->box_StatsWarn_WebServer->currentText(),
        this->ui->box_StatsWarn_LogsType->currentText() );
    this->on_button_StatsWarn_Draw_clicked();
}


///////////////
//// SPEED ////
void MainWindow::checkStatsSpeedDrawable()
{
    if ( this->ui->box_StatsSpeed_Year->currentIndex() >= 0
      && this->ui->box_StatsSpeed_Month->currentIndex() >= 0
      && this->ui->box_StatsSpeed_Day->currentIndex() >= 0 ) {
        // enable the draw buttpn
        this->ui->button_StatsSpeed_Draw->setEnabled( true );
    } else {
        // disable the draw button
        this->ui->button_StatsSpeed_Draw->setEnabled( false );
    }
}

void MainWindow::on_box_StatsSpeed_WebServer_currentIndexChanged(int index)
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
    this->checkStatsSpeedDrawable();
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
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_box_StatsSpeed_Day_currentIndexChanged(int index)
{
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_button_StatsSpeed_Draw_clicked()
{
    //this->ui->table_StatsSpeed->clear();
    this->ui->table_StatsSpeed->setRowCount(0);
    this->crapview.drawSpeed(
        this->ui->table_StatsSpeed,
        this->ui->chart_SatsSpeed,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->ui->box_StatsSpeed_WebServer->currentText(),
        this->ui->box_StatsSpeed_Year->currentText(),
        this->ui->box_StatsSpeed_Month->currentText(),
        this->ui->box_StatsSpeed_Day->currentText(),
        this->crapview.parseTextualFilter( this->ui->inLine_StatsSpeed_Protocol->text() ),
        this->crapview.parseTextualFilter( this->ui->inLine_StatsSpeed_Method->text() ),
        this->crapview.parseTextualFilter( this->ui->inLine_StatsSpeed_Uri->text() ),
        this->crapview.parseTextualFilter( this->ui->inLine_StatsSpeed_Query->text() ),
        this->crapview.parseNumericFilter( this->ui->inLine_StatsSpeed_Response->text() ) );
}


///////////////
//// COUNT ////
void MainWindow::checkStatsCountDrawable()
{
    if ( this->ui->box_StatsCount_Year->currentIndex() >= 0
      && this->ui->box_StatsCount_Month->currentIndex() >= 0
      && this->ui->box_StatsCount_Day->currentIndex() >= 0 ) {
        // enable the draw buttpn
        this->ui->scrollArea_StatsCount_Access->setEnabled( true );
        this->ui->scrollArea_StatsCount_Error->setEnabled( true );
    } else {
        // disable the draw button
        this->ui->scrollArea_StatsCount_Access->setEnabled( false );
        this->ui->scrollArea_StatsCount_Error->setEnabled( false );
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
    this->checkStatsCountDrawable();
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
    this->checkStatsCountDrawable();
}

void MainWindow::on_box_StatsCount_Day_currentIndexChanged(int index)
{
    this->checkStatsCountDrawable();
}

void MainWindow::resetStatsCountAccButtons()
{
    if ( this->ui->button_StatsCount_Protocol->isFlat() == false ) {
        this->ui->button_StatsCount_Protocol->setFlat( true );
    }
    if ( this->ui->button_StatsCount_Method->isFlat() == false ) {
        this->ui->button_StatsCount_Method->setFlat( true );
    }
    if ( this->ui->button_StatsCount_Request->isFlat() == false ) {
        this->ui->button_StatsCount_Request->setFlat( true );
    }
    if ( this->ui->button_StatsCount_Query->isFlat() == false ) {
        this->ui->button_StatsCount_Query->setFlat( true );
    }
    if ( this->ui->button_StatsCount_Response->isFlat() == false ) {
        this->ui->button_StatsCount_Response->setFlat( true );
    }
    if ( this->ui->button_StatsCount_Referrer->isFlat() == false ) {
        this->ui->button_StatsCount_Referrer->setFlat( true );
    }
    if ( this->ui->button_StatsCount_Cookie->isFlat() == false ) {
        this->ui->button_StatsCount_Cookie->setFlat( true );
    }
    if ( this->ui->button_StatsCount_UserAgent->isFlat() == false ) {
        this->ui->button_StatsCount_UserAgent->setFlat( true );
    }
    if ( this->ui->button_StatsCount_AccClient->isFlat() == false ) {
        this->ui->button_StatsCount_AccClient->setFlat( true );
    }
}

void MainWindow::resetStatsCountErrButtons()
{
    if ( this->ui->button_StatsCount_Level->isFlat() == false ) {
        this->ui->button_StatsCount_Level->setFlat( true );
    }
    if ( this->ui->button_StatsCount_Message->isFlat() == false ) {
        this->ui->button_StatsCount_Message->setFlat( true );
    }
    if ( this->ui->button_StatsCount_Source->isFlat() == false ) {
        this->ui->button_StatsCount_Source->setFlat( true );
    }
    if ( this->ui->button_StatsCount_Port->isFlat() == false ) {
        this->ui->button_StatsCount_Port->setFlat( true );
    }
    if ( this->ui->button_StatsCount_ErrClient->isFlat() == false ) {
        this->ui->button_StatsCount_ErrClient->setFlat( true );
    }
}

void MainWindow::on_button_StatsCount_Protocol_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Protocol->text() );
    this->ui->button_StatsCount_Protocol->setFlat( false );
}

void MainWindow::on_button_StatsCount_Method_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Method->text() );
    this->ui->button_StatsCount_Method->setFlat( false );
}

void MainWindow::on_button_StatsCount_Request_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Request->text() );
    this->ui->button_StatsCount_Request->setFlat( false );
}

void MainWindow::on_button_StatsCount_Query_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Query->text() );
    this->ui->button_StatsCount_Query->setFlat( false );
}

void MainWindow::on_button_StatsCount_Response_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Response->text() );
    this->ui->button_StatsCount_Response->setFlat( false );
}

void MainWindow::on_button_StatsCount_Referrer_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Referrer->text() );
    this->ui->button_StatsCount_Referrer->setFlat( false );
}

void MainWindow::on_button_StatsCount_Cookie_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Cookie->text() );
    this->ui->button_StatsCount_Cookie->setFlat( false );
}

void MainWindow::on_button_StatsCount_UserAgent_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_UserAgent->text() );
    this->ui->button_StatsCount_UserAgent->setFlat( false );
}

void MainWindow::on_button_StatsCount_AccClient_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_AccClient->text() );
    this->ui->button_StatsCount_AccClient->setFlat( false );
}


void MainWindow::on_button_StatsCount_Level_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Level->text() );
    this->ui->button_StatsCount_Level->setFlat( false );
}

void MainWindow::on_button_StatsCount_Message_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Message->text() );
    this->ui->button_StatsCount_Message->setFlat( false );
}

void MainWindow::on_button_StatsCount_Source_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Source->text() );
    this->ui->button_StatsCount_Source->setFlat( false );
}

void MainWindow::on_button_StatsCount_Port_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_Port->text() );
    this->ui->button_StatsCount_Port->setFlat( false );
}

void MainWindow::on_button_StatsCount_ErrClient_clicked()
{
    this->drawStatsCount( this->ui->button_StatsCount_ErrClient->text() );
    this->ui->button_StatsCount_ErrClient->setFlat( false );
}

void MainWindow::drawStatsCount( const QString& field )
{
    this->ui->table_StatsCount->setRowCount(0);
    this->crapview.drawCount(
        this->ui->table_StatsCount, this->ui->chart_StatsCount,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->ui->box_StatsCount_WebServer->currentText(), this->ui->tabs_StatsCount_AccErr->tabText( this->ui->tabs_StatsCount_AccErr->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(), this->ui->box_StatsCount_Month->currentText(), this->ui->box_StatsCount_Day->currentText(),
        field );
    this->resetStatsCountAccButtons();
    this->resetStatsCountErrButtons();
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
    if ( this->ui->box_StatsDay_FromYear->currentIndex() < 0
      && this->ui->box_StatsDay_FromMonth->currentIndex() < 0
      && this->ui->box_StatsDay_FromDay->currentIndex() < 0 ) {
        aux = false;
    }
    // check log field validity
    if ( this->ui->box_StatsDay_LogsField->currentIndex() < 0 ) {
        aux = false;
    }
    this->ui->button_StatsDay_Draw->setEnabled( aux);
}

void MainWindow::on_box_StatsDay_WebServer_currentIndexChanged(int index)
{
    if ( this->ui->box_StatsDay_LogsType->currentIndex() != 0 ) {
        this->ui->box_StatsDay_LogsType->setCurrentIndex( 0 );
    } else {
        this->on_box_StatsDay_LogsType_currentIndexChanged( 0 );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_LogsType_currentIndexChanged(int index)
{
    this->ui->box_StatsDay_LogsField->clear();
    if ( index != -1 ) {
        // refresh fields
        this->ui->box_StatsDay_LogsField->addItems(
            this->crapview.getFields(
                "Daytime",
                this->ui->box_StatsDay_LogsType->currentText() ));
        this->ui->box_StatsDay_LogsField->setCurrentIndex( 0 );
        // refresh dates
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
    }
    this->checkStatsDayDrawable();
}


void MainWindow::on_box_StatsDay_LogsField_currentIndexChanged(int index)
{
    this->ui->inLine_StatsDay_Filter->clear();
    this->checkStatsDayDrawable();
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
    QString filter;
    if ( this->ui->box_StatsDay_LogsField->currentIndex() == 0 ) {
        filter = this->crapview.parseBooleanFilter( this->ui->inLine_StatsDay_Filter->text() );
    } else if ( this->ui->box_StatsDay_LogsType->currentIndex() == 0
      && this->ui->box_StatsDay_LogsField->currentIndex() == 5 ) {
        filter = this->crapview.parseNumericFilter( this->ui->inLine_StatsDay_Filter->text() );
    } else {
        filter = this->crapview.parseTextualFilter( this->ui->inLine_StatsDay_Filter->text() );
    }
    this->crapview.drawDay(
        this->ui->chart_StatsDay,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->ui->box_StatsDay_WebServer->currentText(),
        this->ui->box_StatsDay_LogsType->currentText(),
        this->ui->box_StatsDay_FromYear->currentText(),
        this->ui->box_StatsDay_FromMonth->currentText(),
        this->ui->box_StatsDay_FromDay->currentText(),
        ( this->ui->checkBox_StatsDay_Period->isChecked() ) ? this->ui->box_StatsDay_ToYear->currentText() : "",
        ( this->ui->checkBox_StatsDay_Period->isChecked() ) ? this->ui->box_StatsDay_ToMonth->currentText() : "",
        ( this->ui->checkBox_StatsDay_Period->isChecked() ) ? this->ui->box_StatsDay_ToDay->currentText() : "",
        this->ui->box_StatsDay_LogsField->currentText(),
        filter );
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
    // check log field validity
    if ( this->ui->box_StatsRelat_LogsField_1->currentIndex() < 0
      || this->ui->box_StatsRelat_LogsField_2->currentIndex() < 0 ) {
        aux = false;
    }
    this->ui->button_StatsRelat_Draw->setEnabled( aux );
}

void MainWindow::on_box_StatsRelat_WebServer_currentIndexChanged(int index)
{
    if ( this->ui->box_StatsRelat_LogsType->currentIndex() != 0 ) {
        this->ui->box_StatsRelat_LogsType->setCurrentIndex( 0 );
    } else {
        this->on_box_StatsRelat_LogsType_currentIndexChanged( 0 );
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_LogsType_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_LogsField_1->clear();
    this->ui->box_StatsRelat_LogsField_2->clear();
    if ( index != -1 ) {
        // refresh fields
        QStringList fields = this->crapview.getFields(
            "Relational",
            this->ui->box_StatsRelat_LogsType->currentText() );
        this->ui->box_StatsRelat_LogsField_1->addItems( fields );
        this->ui->box_StatsRelat_LogsField_2->addItems( fields );
        this->ui->box_StatsRelat_LogsField_1->setCurrentIndex( 0 );
        this->ui->box_StatsRelat_LogsField_2->setCurrentIndex( 0 );
        // refresh dates
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
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_LogsField_1_currentIndexChanged(int index)
{
    this->ui->inLine_StatsRelat_Filter_1->clear();
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_LogsField_2_currentIndexChanged(int index)
{
    this->ui->inLine_StatsRelat_Filter_2->clear();
    this->checkStatsRelatDrawable();
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
    this->checkStatsRelatDrawable();
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
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_FromDay_currentIndexChanged(int index)
{
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_ToYear_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_ToMonth->clear();
    if ( index != -1 ) {
        this->ui->box_StatsRelat_ToMonth->addItems(
            this->crapview.getMonths(
                this->ui->box_StatsRelat_WebServer->currentText(),
                this->ui->box_StatsRelat_LogsType->currentText(),
                this->ui->box_StatsRelat_ToYear->currentText() ) );
        this->ui->box_StatsRelat_ToMonth->setCurrentIndex( 0 );
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_ToMonth_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_ToDay->clear();
    if ( index != -1 ) {
        this->ui->box_StatsRelat_ToDay->addItems(
            this->crapview.getDays(
                this->ui->box_StatsRelat_WebServer->currentText(),
                this->ui->box_StatsRelat_LogsType->currentText(),
                this->ui->box_StatsRelat_ToYear->currentText(),
                this->ui->box_StatsRelat_ToMonth->currentText() ) );
        this->ui->box_StatsRelat_ToDay->setCurrentIndex( 0 );
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_ToDay_currentIndexChanged(int index)
{
    this->checkStatsRelatDrawable();
}


void MainWindow::on_button_StatsRelat_Draw_clicked()
{
    QString filter1, filter2;
    if ( this->ui->box_StatsRelat_LogsType->currentIndex() == 0 ) {
        int aux;
        aux = this->ui->box_StatsRelat_LogsField_1->currentIndex();
        if ( aux == 0 ) {
            filter1 = this->crapview.parseBooleanFilter( this->ui->inLine_StatsRelat_Filter_1->text() );
        } else if ( aux >= 5 && aux <= 8 ) {
            filter1 = this->crapview.parseNumericFilter( this->ui->inLine_StatsRelat_Filter_1->text() );
        } else {
            filter1 = this->ui->inLine_StatsRelat_Filter_1->text();
        }
        aux = this->ui->box_StatsRelat_LogsField_2->currentIndex();
        if ( aux == 0 ) {
            filter2 = this->crapview.parseBooleanFilter( this->ui->inLine_StatsRelat_Filter_2->text() );
        } else if ( aux >= 5 && aux <= 8 ) {
            filter2 = this->crapview.parseNumericFilter( this->ui->inLine_StatsRelat_Filter_2->text() );
        } else {
            filter2 = this->crapview.parseTextualFilter( this->ui->inLine_StatsRelat_Filter_2->text() );
        }
    } else {
        if ( this->ui->box_StatsRelat_LogsField_1->currentIndex() == 0 ) {
            filter1 = this->crapview.parseBooleanFilter( this->ui->inLine_StatsRelat_Filter_1->text() );
        } else {
            filter1 = this->ui->inLine_StatsRelat_Filter_1->text();
        }
        if ( this->ui->box_StatsRelat_LogsField_2->currentIndex() == 0 ) {
            filter2 = this->crapview.parseBooleanFilter( this->ui->inLine_StatsRelat_Filter_2->text() );
        } else {
            filter2 = this->crapview.parseTextualFilter( this->ui->inLine_StatsRelat_Filter_2->text() );
        }
    }
    this->crapview.drawRelat(
        this->ui->chart_StatsRelat,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->ui->box_StatsRelat_WebServer->currentText(),
        this->ui->box_StatsRelat_LogsType->currentText(),
        this->ui->box_StatsRelat_FromYear->currentText(),
        this->ui->box_StatsRelat_FromMonth->currentText(),
        this->ui->box_StatsRelat_FromDay->currentText(),
        this->ui->box_StatsRelat_ToYear->currentText(),
        this->ui->box_StatsRelat_ToMonth->currentText(),
        this->ui->box_StatsRelat_ToDay->currentText(),
        this->ui->box_StatsRelat_LogsField_1->currentText(), filter1,
        this->ui->box_StatsRelat_LogsField_2->currentText(), filter2 );
}



////////////////
//// GLOBAL ////
void MainWindow::on_button_StatsGlob_Apache_clicked()
{
    this->crapview.calcGlobals(
        this->FONTS,
        this->ui->button_StatsGlob_Apache->text(),
        "Access" );
    if ( this->ui->button_StatsGlob_Apache->isFlat() == true ) {
        // un-flat
        this->ui->button_StatsGlob_Apache->setFlat( false );
        this->ui->button_StatsGlob_Nginx->setFlat( true );
        this->ui->button_StatsGlob_Iis->setFlat( true );
    }
}


void MainWindow::on_button_StatsGlob_Nginx_clicked()
{

}


void MainWindow::on_button_StatsGlob_Iis_clicked()
{

}



/////////////////////////
//////// CONFIGS ////////
/////////////////////////

/////////////////
//// GENERAL ////
/////////////////

////////////////
//// WINDOW ////
void MainWindow::on_checkBox_ConfWindow_Geometry_clicked(bool checked)
{
    this->remember_window = checked;
}

void MainWindow::on_box_ConfWindow_Theme_currentIndexChanged(int index)
{

}


/////////////////
//// DIALOGS ////
void MainWindow::on_slider_ConfDialogs_General_sliderReleased()
{
    this->dialogs_Level = this->ui->slider_ConfDialogs_General->value();
}
void MainWindow::on_slider_ConfDialogs_Logs_sliderReleased()
{
    this->craplog.setDialogLevel( this->ui->slider_ConfDialogs_Logs->value() );
}
void MainWindow::on_slider_ConfDialogs_Stats_sliderReleased()
{
    this->crapview.setDialogLevel( this->ui->slider_ConfDialogs_Stats->value() );
}


//////////////////////
//// TEXT BROWSER ////
void MainWindow::on_box_ConfTextBrowser_Font_currentIndexChanged(int index)
{
    QFont font;
    switch ( index ) {
        case 0:
            font = this->FONTS.at( "main" );
            break;
        case 1:
            font = this->FONTS.at( "alternative" );
            break;
        case 2:
            font = this->FONTS.at( "script" );
            break;
        default:
            throw ("Unexpected Font index: "[index]);
    }
    this->TB.setFont( font );
    this->TB.setFontFamily( this->ui->box_ConfTextBrowser_Font->currentText() );
    this->ui->textBrowser_ConfTextBrowser_Preview->setFont( font );
}
void MainWindow::on_checkBox_ConfTextBrowser_WideLines_clicked(bool checked)
{
    this->TB.setWideLinesUsage( checked );
    this->refreshTextBrowserPreview();
}
void MainWindow::on_box_ConfTextBrowser_ColorScheme_currentIndexChanged(int index)
{
    this->TB.setColorScheme( index, this->TB_COLOR_SCHEMES.at( index ) );
    this->refreshTextBrowserPreview();
}
void MainWindow::refreshTextBrowserPreview()
{
    QString content = "";
    this->TB.makePreview( content );
    this->ui->textBrowser_ConfTextBrowser_Preview->setText( content );
    this->ui->textBrowser_ConfTextBrowser_Preview->setFont( this->TB.getFont() );
}


////////////////
//// CHARTS ////
void MainWindow::on_box_ConfCharts_Theme_currentIndexChanged(int index)
{
    this->charts_theme_id = index;
    this->refreshChartsPreview();
}
void MainWindow::refreshChartsPreview()
{
    QColor col;
    QBarSet *bars_1 = new QBarSet( "Bars 1" );
    col = Qt::GlobalColor::darkCyan;
    bars_1->setColor( col.lighter( 130 ) );
    QBarSet *bars_2 = new QBarSet( "Bars 2" );
    col = Qt::GlobalColor::darkRed;
    bars_2->setColor( col.lighter( 130 ) );

    int aux, max=0;
    for ( int i=0; i<24; i++ ) {
        aux = rand() %100; *bars_1 << aux;
        if ( aux > max ) { max = aux; }
        aux = rand() %100; *bars_2 << aux;
        if ( aux > max ) { max = aux; }
    }

    QBarSeries *bars = new QBarSeries();
    bars->append( bars_1 ); bars->append( bars_2 );

    QChart *t_chart = new QChart();
    // apply the theme
    t_chart->setTheme( this->CHARTS_THEMES.at( this->charts_theme_id ) );

    t_chart->addSeries( bars );
    t_chart->setTitle( "Sample preview" );
    t_chart->setTitleFont( this->FONTS.at("main") );
    t_chart->legend()->setFont( this->FONTS.at("main_small") );
    t_chart->setAnimationOptions( QChart::SeriesAnimations );

    QStringList categories;
    categories << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
               << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append( categories );
    axisX->setLabelsFont( this->FONTS.at( "main_small" ) );
    t_chart->addAxis( axisX, Qt::AlignBottom );
    bars->attachAxis( axisX );

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat( "%d" );
    axisY->setRange( 0, max );
    axisY->setLabelsFont( this->FONTS.at( "main_small" ) );
    t_chart->addAxis( axisY, Qt::AlignLeft );
    bars->attachAxis( axisY) ;

    t_chart->legend()->setVisible( true );
    t_chart->legend()->setAlignment( Qt::AlignBottom );

    this->ui->chart_ConfCharts_Preview->setChart( t_chart );
    this->ui->chart_ConfCharts_Preview->setRenderHint( QPainter::Antialiasing );
}


//////////////
//// LOGS ////
//////////////

/////////////////
//// CONTROL ////
void MainWindow::on_checkBox_ConfControl_Usage_clicked(bool checked)
{
    this->display_used_files = ! checked;
}
void MainWindow::on_checkBox_ConfControl_Size_clicked(bool checked)
{
    if ( checked == false ) {
        // disable size warning
        this->ui->spinBox_ConfControl_Size->setEnabled( false );
        this->craplog.setWarningSize( 0 );
    } else {
        // enable warning
        this->ui->spinBox_ConfControl_Size->setEnabled( true );
        this->craplog.setWarningSize( (this->ui->spinBox_ConfControl_Size->value() * 1'048'576) +1 );
    }
}
void MainWindow::on_spinBox_ConfControl_Size_editingFinished()
{
    this->craplog.setWarningSize( (this->ui->spinBox_ConfControl_Size->value() * 1'048'576) +1 );
}


////////////////
//// APACHE ////
// paths
void MainWindow::on_checkBox_ConfApache_Paths_Different_clicked(bool checked)
{
    if ( checked == true ) {
        // enable the error logs path line
        this->ui->label_ConfApache_Paths_Access->setEnabled( true );
        this->ui->label_ConfApache_Paths_Error->setEnabled( true );
        this->ui->inLine_ConfApache_Paths_ErrPath->setEnabled( true );
        this->ui->inLine_ConfApache_Paths_ErrPath->setText( QString::fromStdString(
            this->craplog.getLogsPath( this->APACHE_ID, this->ERROR_LOGS ) ) );
        this->on_inLine_ConfApache_Paths_ErrPath_textChanged(
            this->ui->inLine_ConfApache_Paths_ErrPath->text() );
    } else {
        // disable the error logs path line
        this->ui->label_ConfApache_Paths_Access->setEnabled( false );
        this->ui->label_ConfApache_Paths_Error->setEnabled( false );
        this->ui->inLine_ConfApache_Paths_ErrPath->setEnabled( false );
        this->ui->icon_ConfApache_Paths_ErrWrong->setVisible( false );
        // set the error logs path equals to the access logs path
        this->craplog.setLogsPath( this->APACHE_ID, this->ERROR_LOGS,
            this->craplog.getLogsPath( this->APACHE_ID, this->ACCESS_LOGS ) );
    }
}

void MainWindow::on_inLine_ConfApache_Paths_AccPath_textChanged(const QString &arg1)
{
    std::string path = StringOps::strip( arg1.toStdString() );
    if ( IOutils::checkDir( path ) == true ) {
        this->ui->icon_ConfApache_Paths_AccWrong->setVisible( false );
        if ( this->ui->icon_ConfApache_Paths_ErrWrong->isVisible() == false ) {
            this->ui->button_ConfApache_Paths_SavePaths->setEnabled( true );
        } else {
            this->ui->button_ConfApache_Paths_SavePaths->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfApache_Paths_AccWrong->setVisible( true );
        this->ui->button_ConfApache_Paths_SavePaths->setEnabled( false );
    }
    this->ui->inLine_ConfApache_Paths_AccPath->setText( QString::fromStdString( path ) );
}

void MainWindow::on_inLine_ConfApache_Paths_ErrPath_textChanged(const QString &arg1)
{
    std::string path = StringOps::strip( arg1.toStdString() );
    if ( IOutils::checkDir( path ) == true ) {
        this->ui->icon_ConfApache_Paths_ErrWrong->setVisible( false );
        if ( this->ui->icon_ConfApache_Paths_AccWrong->isVisible() == false ) {
            this->ui->button_ConfApache_Paths_SavePaths->setEnabled( true );
        } else {
            this->ui->button_ConfApache_Paths_SavePaths->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfApache_Paths_ErrWrong->setVisible( true );
        this->ui->button_ConfApache_Paths_SavePaths->setEnabled( false );
    }
    this->ui->inLine_ConfApache_Paths_ErrPath->setText( QString::fromStdString( path ) );
}

void MainWindow::on_button_ConfApache_Paths_SavePaths_clicked()
{
    if ( this->ui->icon_ConfApache_Paths_ErrWrong->isVisible() == false
      && this->ui->icon_ConfApache_Paths_AccWrong->isVisible() == false ) {
        // set the paths
        std::string path = StringOps::strip( this->ui->inLine_ConfApache_Paths_AccPath->text().toStdString() );
        if ( StringOps::endsWith( path, "/" ) ) {
            path = StringOps::rstrip( path, "/" );
        }
        this->craplog.setLogsPath( this->APACHE_ID, this->ACCESS_LOGS, path );
        // check if the logs path is different or not
        if ( this->ui->checkBox_ConfApache_Paths_Different->isChecked() == true ) {
            // handle the error logs path too
            path = StringOps::strip( this->ui->inLine_ConfApache_Paths_ErrPath->text().toStdString() );
            if ( StringOps::endsWith( path, "/" ) ) {
                path = StringOps::rstrip( path, "/" );
            }
        }
        this->craplog.setLogsPath( this->APACHE_ID, this->ERROR_LOGS, path );
    }
    this->ui->button_ConfApache_Paths_SavePaths->setEnabled( false );
}

// formats
void MainWindow::on_inLine_ConfApache_Formats_AccString_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfApache_Formats_AccSave->setEnabled( true );
    } else {
        this->ui->button_ConfApache_Formats_AccSave->setEnabled( false );
    }
}
void MainWindow::on_button_ConfApache_Formats_AccSave_clicked()
{
    this->craplog.setApacheALF( this->ui->inLine_ConfApache_Formats_AccString->text().toStdString() );
    this->ui->button_ConfApache_Formats_AccSave->setEnabled( false );
}
void MainWindow::on_button_ConfApache_Formats_AccSample_clicked()
{
    this->ui->preview_ConfApache_Formats_AccSample->setText(
        this->craplog.getLogsFormatSample( this->APACHE_ID, this->ACCESS_LOGS ) );
}
void MainWindow::on_button_ConfApache_Formats_AccHelp_clicked()
{
    // !!! 2 COMPLETE !!!
}

void MainWindow::on_inLine_ConfApache_Formats_ErrString_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfApache_Formats_ErrSave->setEnabled( true );
    } else {
        this->ui->button_ConfApache_Formats_ErrSave->setEnabled( false );
    }
}
void MainWindow::on_button_ConfApache_Formats_ErrSave_clicked()
{
    this->craplog.setApacheELF( this->ui->inLine_ConfApache_Formats_ErrString->text().toStdString() );
    this->ui->button_ConfApache_Formats_ErrSave->setEnabled( false );
}
void MainWindow::on_button_ConfApache_Formats_ErrSample_clicked()
{
    this->ui->preview_ConfApache_Formats_ErrSample->setText(
        this->craplog.getLogsFormatSample( this->APACHE_ID, this->ERROR_LOGS ) );
}
void MainWindow::on_button_ConfApache_Formats_ErrHelp_clicked()
{
    // !!! 2 COMPLETE !!!
}

// warnlists
void MainWindow::on_box_ConfApache_Warnlist_Acc_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfApache_Warnlist_Acc->clear();
    this->ui->list_ConfApache_Warnlist_Acc->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getWarnlist(
        this->APACHE_ID, this->ACCESS_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfApache_Warnlist_Acc->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isWarnlistUsed(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Acc->currentText() ) );
    this->ui->checkBox_ConfApache_Warnlist_AccUsed->setChecked( used );
    this->on_checkBox_ConfApache_Warnlist_AccUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfApache_Warnlist_AccUsed_clicked(bool checked)
{
    this->craplog.setWarnlistUsed(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Acc->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfApache_Warnlist_Acc->setEnabled( true );
        this->ui->list_ConfApache_Warnlist_Acc->setEnabled( true );
    } else {
        this->ui->inLine_ConfApache_Warnlist_Acc->clear();
        this->ui->inLine_ConfApache_Warnlist_Acc->setEnabled( false );
        this->ui->list_ConfApache_Warnlist_Acc->clearSelection();
        this->ui->list_ConfApache_Warnlist_Acc->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfApache_Warnlist_Acc_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfApache_Warnlist_AccAdd->setEnabled( true );
    } else {
        this->ui->button_ConfApache_Warnlist_AccAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfApache_Warnlist_Acc_returnPressed()
{
    this->on_button_ConfApache_Warnlist_AccAdd_clicked();
}
void MainWindow::on_button_ConfApache_Warnlist_AccAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfApache_Warnlist_Acc->text();
    if ( this->ui->list_ConfApache_Warnlist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        this->ui->list_ConfApache_Warnlist_Acc->addItem( item );
        this->craplog.warnlistAdd(
            this->APACHE_ID, this->ACCESS_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Acc->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfApache_Warnlist_Acc->clearSelection();
    this->ui->list_ConfApache_Warnlist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfApache_Warnlist_Acc->clear();
}

void MainWindow::on_list_ConfApache_Warnlist_Acc_itemSelectionChanged()
{
    if ( this->ui->list_ConfApache_Warnlist_Acc->selectedItems().size() == 1 ) {
        this->ui->button_ConfApache_Warnlist_AccRemove->setEnabled( true );
        this->ui->button_ConfApache_Warnlist_AccUp->setEnabled( true );
        this->ui->button_ConfApache_Warnlist_AccDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfApache_Warnlist_Acc->selectedItems().at(0);
        const int max = this->ui->list_ConfApache_Warnlist_Acc->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfApache_Warnlist_AccUp->setEnabled( false );
            this->ui->button_ConfApache_Warnlist_AccDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfApache_Warnlist_Acc->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfApache_Warnlist_AccUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfApache_Warnlist_AccDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfApache_Warnlist_AccRemove->setEnabled( false );
        this->ui->button_ConfApache_Warnlist_AccUp->setEnabled( false );
        this->ui->button_ConfApache_Warnlist_AccDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfApache_Warnlist_AccRemove_clicked()
{
    const auto& item = this->ui->list_ConfApache_Warnlist_Acc->selectedItems().at(0);
    this->craplog.warnlistRemove(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Acc_currentTextChanged( this->ui->box_ConfApache_Warnlist_Acc->currentText() );
}
void MainWindow::on_button_ConfApache_Warnlist_AccUp_clicked()
{
    const auto& item = this->ui->list_ConfApache_Warnlist_Acc->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveUp(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Acc_currentTextChanged( this->ui->box_ConfApache_Warnlist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Warnlist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfApache_Warnlist_Acc->setFocus();
}
void MainWindow::on_button_ConfApache_Warnlist_AccDown_clicked()
{
    const auto& item = this->ui->list_ConfApache_Warnlist_Acc->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveDown(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Acc_currentTextChanged( this->ui->box_ConfApache_Warnlist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Warnlist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfApache_Warnlist_Acc->setFocus();
}


void MainWindow::on_box_ConfApache_Warnlist_Err_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfApache_Warnlist_Err->clear();
    this->ui->list_ConfApache_Warnlist_Err->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getWarnlist(
        this->APACHE_ID, this->ERROR_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfApache_Warnlist_Err->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isWarnlistUsed(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Err->currentText() ) );
    this->ui->checkBox_ConfApache_Warnlist_ErrUsed->setChecked( used );
    this->on_checkBox_ConfApache_Warnlist_ErrUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfApache_Warnlist_ErrUsed_clicked(bool checked)
{
    this->craplog.setWarnlistUsed(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Err->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfApache_Warnlist_Err->setEnabled( true );
        this->ui->list_ConfApache_Warnlist_Err->setEnabled( true );
    } else {
        this->ui->inLine_ConfApache_Warnlist_Err->clear();
        this->ui->inLine_ConfApache_Warnlist_Err->setEnabled( false );
        this->ui->list_ConfApache_Warnlist_Err->clearSelection();
        this->ui->list_ConfApache_Warnlist_Err->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfApache_Warnlist_Err_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfApache_Warnlist_ErrAdd->setEnabled( true );
    } else {
        this->ui->button_ConfApache_Warnlist_ErrAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfApache_Warnlist_Err_returnPressed()
{
    this->on_button_ConfApache_Warnlist_ErrAdd_clicked();
}
void MainWindow::on_button_ConfApache_Warnlist_ErrAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfApache_Warnlist_Err->text();
    if ( this->ui->list_ConfApache_Warnlist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        this->ui->list_ConfApache_Warnlist_Err->addItem( item );
        this->craplog.warnlistAdd(
            this->APACHE_ID, this->ERROR_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Err->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfApache_Warnlist_Err->clearSelection();
    this->ui->list_ConfApache_Warnlist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfApache_Warnlist_Err->clear();
}

void MainWindow::on_list_ConfApache_Warnlist_Err_itemSelectionChanged()
{
    if ( this->ui->list_ConfApache_Warnlist_Err->selectedItems().size() > 0 ) {
        this->ui->button_ConfApache_Warnlist_ErrRemove->setEnabled( true );
        this->ui->button_ConfApache_Warnlist_ErrUp->setEnabled( true );
        this->ui->button_ConfApache_Warnlist_ErrDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfApache_Warnlist_Err->selectedItems().at(0);
        const int max = this->ui->list_ConfApache_Warnlist_Err->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfApache_Warnlist_ErrUp->setEnabled( false );
            this->ui->button_ConfApache_Warnlist_ErrDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfApache_Warnlist_Err->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfApache_Warnlist_ErrUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfApache_Warnlist_ErrDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfApache_Warnlist_ErrRemove->setEnabled( false );
        this->ui->button_ConfApache_Warnlist_ErrUp->setEnabled( false );
        this->ui->button_ConfApache_Warnlist_ErrDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfApache_Warnlist_ErrRemove_clicked()
{
    const auto& item = this->ui->list_ConfApache_Warnlist_Err->selectedItems().at(0);
    this->craplog.warnlistRemove(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Err_currentTextChanged( this->ui->box_ConfApache_Warnlist_Err->currentText() );
}
void MainWindow::on_button_ConfApache_Warnlist_ErrUp_clicked()
{
    const auto& item = this->ui->list_ConfApache_Warnlist_Err->selectedItems().at(0);
    int i = this->craplog.warnlistMoveUp(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Err_currentTextChanged( this->ui->box_ConfApache_Warnlist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Warnlist_Err->item( i )->setSelected( true );
    this->ui->list_ConfApache_Warnlist_Err->setFocus();
}
void MainWindow::on_button_ConfApache_Warnlist_ErrDown_clicked()
{
    const auto& item = this->ui->list_ConfApache_Warnlist_Err->selectedItems().at(0);
    int i = this->craplog.warnlistMoveDown(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Err_currentTextChanged( this->ui->box_ConfApache_Warnlist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Warnlist_Err->item( i )->setSelected( true );
    this->ui->list_ConfApache_Warnlist_Err->setFocus();
}


// blacklist
void MainWindow::on_box_ConfApache_Blacklist_Acc_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfApache_Blacklist_Acc->clear();
    this->ui->list_ConfApache_Blacklist_Acc->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getBlacklist(
        this->APACHE_ID, this->ACCESS_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfApache_Blacklist_Acc->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isBlacklistUsed(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Acc->currentText() ) );
    this->ui->checkBox_ConfApache_Blacklist_AccUsed->setChecked( used );
    this->on_checkBox_ConfApache_Blacklist_AccUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfApache_Blacklist_AccUsed_clicked(bool checked)
{
    this->craplog.setBlacklistUsed(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Acc->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfApache_Blacklist_Acc->setEnabled( true );
        this->ui->list_ConfApache_Blacklist_Acc->setEnabled( true );
    } else {
        this->ui->inLine_ConfApache_Blacklist_Acc->clear();
        this->ui->inLine_ConfApache_Blacklist_Acc->setEnabled( false );
        this->ui->list_ConfApache_Blacklist_Acc->clearSelection();
        this->ui->list_ConfApache_Blacklist_Acc->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfApache_Blacklist_Acc_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfApache_Blacklist_AccAdd->setEnabled( true );
    } else {
        this->ui->button_ConfApache_Blacklist_AccAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfApache_Blacklist_Acc_returnPressed()
{
    this->on_button_ConfApache_Blacklist_AccAdd_clicked();
}
void MainWindow::on_button_ConfApache_Blacklist_AccAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfApache_Blacklist_Acc->text();
    if ( this->ui->list_ConfApache_Blacklist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        this->ui->list_ConfApache_Blacklist_Acc->addItem( item );
        this->craplog.blacklistAdd(
            this->APACHE_ID, this->ACCESS_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Acc->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfApache_Blacklist_Acc->clearSelection();
    this->ui->list_ConfApache_Blacklist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfApache_Blacklist_Acc->clear();
}

void MainWindow::on_list_ConfApache_Blacklist_Acc_itemSelectionChanged()
{
    if ( this->ui->list_ConfApache_Blacklist_Acc->selectedItems().size() == 1 ) {
        this->ui->button_ConfApache_Blacklist_AccRemove->setEnabled( true );
        this->ui->button_ConfApache_Blacklist_AccUp->setEnabled( true );
        this->ui->button_ConfApache_Blacklist_AccDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfApache_Blacklist_Acc->selectedItems().at(0);
        const int max = this->ui->list_ConfApache_Blacklist_Acc->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfApache_Blacklist_AccUp->setEnabled( false );
            this->ui->button_ConfApache_Blacklist_AccDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfApache_Blacklist_Acc->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfApache_Blacklist_AccUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfApache_Blacklist_AccDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfApache_Blacklist_AccRemove->setEnabled( false );
        this->ui->button_ConfApache_Blacklist_AccUp->setEnabled( false );
        this->ui->button_ConfApache_Blacklist_AccDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfApache_Blacklist_AccRemove_clicked()
{
    const auto& item = this->ui->list_ConfApache_Blacklist_Acc->selectedItems().at(0);
    this->craplog.blacklistRemove(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Acc_currentTextChanged( this->ui->box_ConfApache_Blacklist_Acc->currentText() );
}
void MainWindow::on_button_ConfApache_Blacklist_AccUp_clicked()
{
    const auto& item = this->ui->list_ConfApache_Blacklist_Acc->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveUp(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Acc_currentTextChanged( this->ui->box_ConfApache_Blacklist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Blacklist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfApache_Blacklist_Acc->setFocus();
}
void MainWindow::on_button_ConfApache_Blacklist_AccDown_clicked()
{
    const auto& item = this->ui->list_ConfApache_Blacklist_Acc->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveDown(
        this->APACHE_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Acc_currentTextChanged( this->ui->box_ConfApache_Blacklist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Blacklist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfApache_Blacklist_Acc->setFocus();
}


void MainWindow::on_box_ConfApache_Blacklist_Err_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfApache_Blacklist_Err->clear();
    this->ui->list_ConfApache_Blacklist_Err->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getBlacklist(
        this->APACHE_ID, this->ERROR_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfApache_Blacklist_Err->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isBlacklistUsed(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Err->currentText() ) );
    this->ui->checkBox_ConfApache_Blacklist_ErrUsed->setChecked( used );
    this->on_checkBox_ConfApache_Blacklist_ErrUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfApache_Blacklist_ErrUsed_clicked(bool checked)
{
    this->craplog.setBlacklistUsed(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Err->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfApache_Blacklist_Err->setEnabled( true );
        this->ui->list_ConfApache_Blacklist_Err->setEnabled( true );
    } else {
        this->ui->inLine_ConfApache_Blacklist_Err->clear();
        this->ui->inLine_ConfApache_Blacklist_Err->setEnabled( false );
        this->ui->list_ConfApache_Blacklist_Err->clearSelection();
        this->ui->list_ConfApache_Blacklist_Err->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfApache_Blacklist_Err_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfApache_Blacklist_ErrAdd->setEnabled( true );
    } else {
        this->ui->button_ConfApache_Blacklist_ErrAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfApache_Blacklist_Err_returnPressed()
{
    this->on_button_ConfApache_Blacklist_ErrAdd_clicked();
}
void MainWindow::on_button_ConfApache_Blacklist_ErrAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfApache_Blacklist_Err->text();
    if ( this->ui->list_ConfApache_Blacklist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        this->ui->list_ConfApache_Blacklist_Err->addItem( item );
        this->craplog.blacklistAdd(
            this->APACHE_ID, this->ERROR_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Err->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfApache_Blacklist_Err->clearSelection();
    this->ui->list_ConfApache_Blacklist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfApache_Blacklist_Err->clear();
}

void MainWindow::on_list_ConfApache_Blacklist_Err_itemSelectionChanged()
{
    if ( this->ui->list_ConfApache_Blacklist_Err->selectedItems().size() > 0 ) {
        this->ui->button_ConfApache_Blacklist_ErrRemove->setEnabled( true );
        this->ui->button_ConfApache_Blacklist_ErrUp->setEnabled( true );
        this->ui->button_ConfApache_Blacklist_ErrDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfApache_Blacklist_Err->selectedItems().at(0);
        const int max = this->ui->list_ConfApache_Blacklist_Err->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfApache_Blacklist_ErrUp->setEnabled( false );
            this->ui->button_ConfApache_Blacklist_ErrDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfApache_Blacklist_Err->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfApache_Blacklist_ErrUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfApache_Blacklist_ErrDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfApache_Blacklist_ErrRemove->setEnabled( false );
        this->ui->button_ConfApache_Blacklist_ErrUp->setEnabled( false );
        this->ui->button_ConfApache_Blacklist_ErrDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfApache_Blacklist_ErrRemove_clicked()
{
    const auto& item = this->ui->list_ConfApache_Blacklist_Err->selectedItems().at(0);
    this->craplog.blacklistRemove(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Err_currentTextChanged( this->ui->box_ConfApache_Blacklist_Err->currentText() );
}
void MainWindow::on_button_ConfApache_Blacklist_ErrUp_clicked()
{
    const auto& item = this->ui->list_ConfApache_Blacklist_Err->selectedItems().at(0);
    int i = this->craplog.blacklistMoveUp(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Err_currentTextChanged( this->ui->box_ConfApache_Blacklist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Blacklist_Err->item( i )->setSelected( true );
    this->ui->list_ConfApache_Blacklist_Err->setFocus();
}
void MainWindow::on_button_ConfApache_Blacklist_ErrDown_clicked()
{
    const auto& item = this->ui->list_ConfApache_Blacklist_Err->selectedItems().at(0);
    int i = this->craplog.blacklistMoveDown(
        this->APACHE_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Err_currentTextChanged( this->ui->box_ConfApache_Blacklist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Blacklist_Err->item( i )->setSelected( true );
    this->ui->list_ConfApache_Blacklist_Err->setFocus();
}


////////////////
//// NGINX ////
// paths
void MainWindow::on_checkBox_ConfNginx_Paths_Different_clicked(bool checked)
{
    if ( checked == true ) {
        // enable the error logs path line
        this->ui->label_ConfNginx_Paths_Access->setEnabled( true );
        this->ui->label_ConfNginx_Paths_Error->setEnabled( true );
        this->ui->inLine_ConfNginx_Paths_ErrPath->setEnabled( true );
        this->ui->inLine_ConfNginx_Paths_ErrPath->setText( QString::fromStdString(
            this->craplog.getLogsPath( this->NGINX_ID, this->ERROR_LOGS ) ) );
        this->on_inLine_ConfNginx_Paths_ErrPath_textChanged(
            this->ui->inLine_ConfNginx_Paths_ErrPath->text() );
    } else {
        // disable the error logs path line
        this->ui->label_ConfNginx_Paths_Access->setEnabled( false );
        this->ui->label_ConfNginx_Paths_Error->setEnabled( false );
        this->ui->inLine_ConfNginx_Paths_ErrPath->setEnabled( false );
        this->ui->icon_ConfNginx_Paths_ErrWrong->setVisible( false );
        // set the error logs path equals to the access logs path
        this->craplog.setLogsPath( this->NGINX_ID, this->ERROR_LOGS,
            this->craplog.getLogsPath( this->NGINX_ID, this->ACCESS_LOGS ) );
    }
}

void MainWindow::on_inLine_ConfNginx_Paths_AccPath_textChanged(const QString &arg1)
{
    std::string path = StringOps::strip( arg1.toStdString() );
    if ( IOutils::checkDir( path ) == true ) {
        this->ui->icon_ConfNginx_Paths_AccWrong->setVisible( false );
        if ( this->ui->icon_ConfNginx_Paths_ErrWrong->isVisible() == false ) {
            this->ui->button_ConfNginx_Paths_SavePaths->setEnabled( true );
        } else {
            this->ui->button_ConfNginx_Paths_SavePaths->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfNginx_Paths_AccWrong->setVisible( true );
        this->ui->button_ConfNginx_Paths_SavePaths->setEnabled( false );
    }
    this->ui->inLine_ConfNginx_Paths_AccPath->setText( QString::fromStdString( path ) );
}

void MainWindow::on_inLine_ConfNginx_Paths_ErrPath_textChanged(const QString &arg1)
{
    std::string path = StringOps::strip( arg1.toStdString() );
    if ( IOutils::checkDir( path ) == true ) {
        this->ui->icon_ConfNginx_Paths_ErrWrong->setVisible( false );
        if ( this->ui->icon_ConfNginx_Paths_AccWrong->isVisible() == false ) {
            this->ui->button_ConfNginx_Paths_SavePaths->setEnabled( true );
        } else {
            this->ui->button_ConfNginx_Paths_SavePaths->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfNginx_Paths_ErrWrong->setVisible( true );
        this->ui->button_ConfNginx_Paths_SavePaths->setEnabled( false );
    }
    this->ui->inLine_ConfNginx_Paths_ErrPath->setText( QString::fromStdString( path ) );
}

void MainWindow::on_button_ConfNginx_Paths_SavePaths_clicked()
{
    if ( this->ui->icon_ConfNginx_Paths_ErrWrong->isVisible() == false
      && this->ui->icon_ConfNginx_Paths_AccWrong->isVisible() == false ) {
        // set the paths
        std::string path = StringOps::strip( this->ui->inLine_ConfNginx_Paths_AccPath->text().toStdString() );
        if ( StringOps::endsWith( path, "/" ) ) {
            path = StringOps::rstrip( path, "/" );
        }
        this->craplog.setLogsPath( this->NGINX_ID, this->ACCESS_LOGS, path );
        // check if the logs path is different or not
        if ( this->ui->checkBox_ConfNginx_Paths_Different->isChecked() == true ) {
            // handle the error logs path too
            path = StringOps::strip( this->ui->inLine_ConfNginx_Paths_ErrPath->text().toStdString() );
            if ( StringOps::endsWith( path, "/" ) ) {
                path = StringOps::rstrip( path, "/" );
            }
        }
        this->craplog.setLogsPath( this->NGINX_ID, this->ERROR_LOGS, path );
    }
    this->ui->button_ConfNginx_Paths_SavePaths->setEnabled( false );
}

// formats
void MainWindow::on_inLine_ConfNginx_Formats_AccString_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfNginx_Formats_AccSave->setEnabled( true );
    } else {
        this->ui->button_ConfNginx_Formats_AccSave->setEnabled( false );
    }
}
void MainWindow::on_button_ConfNginx_Formats_AccSave_clicked()
{
    this->craplog.setNginxALF( this->ui->inLine_ConfNginx_Formats_AccString->text().toStdString() );
    this->ui->button_ConfNginx_Formats_AccSave->setEnabled( false );
}
void MainWindow::on_button_ConfNginx_Formats_AccSample_clicked()
{
    this->ui->preview_ConfNginx_Formats_AccSample->setText(
        this->craplog.getLogsFormatSample( this->NGINX_ID, this->ACCESS_LOGS ) );
}
void MainWindow::on_button_ConfNginx_Formats_AccHelp_clicked()
{
    // !!! 2 COMPLETE !!!
}

void MainWindow::on_inLine_ConfNginx_Formats_ErrString_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfNginx_Formats_ErrSave->setEnabled( true );
    } else {
        this->ui->button_ConfNginx_Formats_ErrSave->setEnabled( false );
    }
}
void MainWindow::on_button_ConfNginx_Formats_ErrSave_clicked()
{
    this->craplog.setNginxELF( this->ui->inLine_ConfNginx_Formats_ErrString->text().toStdString() );
    this->ui->button_ConfNginx_Formats_ErrSave->setEnabled( false );
}
void MainWindow::on_button_ConfNginx_Formats_ErrSample_clicked()
{
    this->ui->preview_ConfNginx_Formats_ErrSample->setText(
        this->craplog.getLogsFormatSample( this->NGINX_ID, this->ERROR_LOGS ) );
}
void MainWindow::on_button_ConfNginx_Formats_ErrHelp_clicked()
{
    // !!! 2 COMPLETE !!!
}

// warnlists
void MainWindow::on_box_ConfNginx_Warnlist_Acc_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfNginx_Warnlist_Acc->clear();
    this->ui->list_ConfNginx_Warnlist_Acc->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getWarnlist(
        this->NGINX_ID, this->ACCESS_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfNginx_Warnlist_Acc->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isWarnlistUsed(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Acc->currentText() ) );
    this->ui->checkBox_ConfNginx_Warnlist_AccUsed->setChecked( used );
    this->on_checkBox_ConfNginx_Warnlist_AccUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfNginx_Warnlist_AccUsed_clicked(bool checked)
{
    this->craplog.setWarnlistUsed(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Acc->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfNginx_Warnlist_Acc->setEnabled( true );
        this->ui->list_ConfNginx_Warnlist_Acc->setEnabled( true );
    } else {
        this->ui->inLine_ConfNginx_Warnlist_Acc->clear();
        this->ui->inLine_ConfNginx_Warnlist_Acc->setEnabled( false );
        this->ui->list_ConfNginx_Warnlist_Acc->clearSelection();
        this->ui->list_ConfNginx_Warnlist_Acc->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfNginx_Warnlist_Acc_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfNginx_Warnlist_AccAdd->setEnabled( true );
    } else {
        this->ui->button_ConfNginx_Warnlist_AccAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfNginx_Warnlist_Acc_returnPressed()
{
    this->on_button_ConfNginx_Warnlist_AccAdd_clicked();
}
void MainWindow::on_button_ConfNginx_Warnlist_AccAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfNginx_Warnlist_Acc->text();
    if ( this->ui->list_ConfNginx_Warnlist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        this->ui->list_ConfNginx_Warnlist_Acc->addItem( item );
        this->craplog.warnlistAdd(
            this->NGINX_ID, this->ACCESS_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Acc->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfNginx_Warnlist_Acc->clearSelection();
    this->ui->list_ConfNginx_Warnlist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfNginx_Warnlist_Acc->clear();
}

void MainWindow::on_list_ConfNginx_Warnlist_Acc_itemSelectionChanged()
{
    if ( this->ui->list_ConfNginx_Warnlist_Acc->selectedItems().size() == 1 ) {
        this->ui->button_ConfNginx_Warnlist_AccRemove->setEnabled( true );
        this->ui->button_ConfNginx_Warnlist_AccUp->setEnabled( true );
        this->ui->button_ConfNginx_Warnlist_AccDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfNginx_Warnlist_Acc->selectedItems().at(0);
        const int max = this->ui->list_ConfNginx_Warnlist_Acc->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfNginx_Warnlist_AccUp->setEnabled( false );
            this->ui->button_ConfNginx_Warnlist_AccDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfNginx_Warnlist_Acc->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfNginx_Warnlist_AccUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfNginx_Warnlist_AccDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfNginx_Warnlist_AccRemove->setEnabled( false );
        this->ui->button_ConfNginx_Warnlist_AccUp->setEnabled( false );
        this->ui->button_ConfNginx_Warnlist_AccDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfNginx_Warnlist_AccRemove_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Warnlist_Acc->selectedItems().at(0);
    this->craplog.warnlistRemove(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Acc_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Acc->currentText() );
}
void MainWindow::on_button_ConfNginx_Warnlist_AccUp_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Warnlist_Acc->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveUp(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Acc_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Warnlist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Warnlist_Acc->setFocus();
}
void MainWindow::on_button_ConfNginx_Warnlist_AccDown_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Warnlist_Acc->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveDown(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Acc_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Warnlist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Warnlist_Acc->setFocus();
}


void MainWindow::on_box_ConfNginx_Warnlist_Err_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfNginx_Warnlist_Err->clear();
    this->ui->list_ConfNginx_Warnlist_Err->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getWarnlist(
        this->NGINX_ID, this->ERROR_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfNginx_Warnlist_Err->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isWarnlistUsed(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Err->currentText() ) );
    this->ui->checkBox_ConfNginx_Warnlist_ErrUsed->setChecked( used );
    this->on_checkBox_ConfNginx_Warnlist_ErrUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfNginx_Warnlist_ErrUsed_clicked(bool checked)
{
    this->craplog.setWarnlistUsed(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Err->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfNginx_Warnlist_Err->setEnabled( true );
        this->ui->list_ConfNginx_Warnlist_Err->setEnabled( true );
    } else {
        this->ui->inLine_ConfNginx_Warnlist_Err->clear();
        this->ui->inLine_ConfNginx_Warnlist_Err->setEnabled( false );
        this->ui->list_ConfNginx_Warnlist_Err->clearSelection();
        this->ui->list_ConfNginx_Warnlist_Err->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfNginx_Warnlist_Err_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfNginx_Warnlist_ErrAdd->setEnabled( true );
    } else {
        this->ui->button_ConfNginx_Warnlist_ErrAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfNginx_Warnlist_Err_returnPressed()
{
    this->on_button_ConfNginx_Warnlist_ErrAdd_clicked();
}
void MainWindow::on_button_ConfNginx_Warnlist_ErrAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfNginx_Warnlist_Err->text();
    if ( this->ui->list_ConfNginx_Warnlist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        this->ui->list_ConfNginx_Warnlist_Err->addItem( item );
        this->craplog.warnlistAdd(
            this->NGINX_ID, this->ERROR_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Err->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfNginx_Warnlist_Err->clearSelection();
    this->ui->list_ConfNginx_Warnlist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfNginx_Warnlist_Err->clear();
}

void MainWindow::on_list_ConfNginx_Warnlist_Err_itemSelectionChanged()
{
    if ( this->ui->list_ConfNginx_Warnlist_Err->selectedItems().size() > 0 ) {
        this->ui->button_ConfNginx_Warnlist_ErrRemove->setEnabled( true );
        this->ui->button_ConfNginx_Warnlist_ErrUp->setEnabled( true );
        this->ui->button_ConfNginx_Warnlist_ErrDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfNginx_Warnlist_Err->selectedItems().at(0);
        const int max = this->ui->list_ConfNginx_Warnlist_Err->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfNginx_Warnlist_ErrUp->setEnabled( false );
            this->ui->button_ConfNginx_Warnlist_ErrDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfNginx_Warnlist_Err->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfNginx_Warnlist_ErrUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfNginx_Warnlist_ErrDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfNginx_Warnlist_ErrRemove->setEnabled( false );
        this->ui->button_ConfNginx_Warnlist_ErrUp->setEnabled( false );
        this->ui->button_ConfNginx_Warnlist_ErrDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfNginx_Warnlist_ErrRemove_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Warnlist_Err->selectedItems().at(0);
    this->craplog.warnlistRemove(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Err_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Err->currentText() );
}
void MainWindow::on_button_ConfNginx_Warnlist_ErrUp_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Warnlist_Err->selectedItems().at(0);
    int i = this->craplog.warnlistMoveUp(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Err_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Warnlist_Err->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Warnlist_Err->setFocus();
}
void MainWindow::on_button_ConfNginx_Warnlist_ErrDown_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Warnlist_Err->selectedItems().at(0);
    int i = this->craplog.warnlistMoveDown(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Err_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Warnlist_Err->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Warnlist_Err->setFocus();
}


// blacklist
void MainWindow::on_box_ConfNginx_Blacklist_Acc_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfNginx_Blacklist_Acc->clear();
    this->ui->list_ConfNginx_Blacklist_Acc->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getBlacklist(
        this->NGINX_ID, this->ACCESS_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfNginx_Blacklist_Acc->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isBlacklistUsed(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Acc->currentText() ) );
    this->ui->checkBox_ConfNginx_Blacklist_AccUsed->setChecked( used );
    this->on_checkBox_ConfNginx_Blacklist_AccUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfNginx_Blacklist_AccUsed_clicked(bool checked)
{
    this->craplog.setBlacklistUsed(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Acc->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfNginx_Blacklist_Acc->setEnabled( true );
        this->ui->list_ConfNginx_Blacklist_Acc->setEnabled( true );
    } else {
        this->ui->inLine_ConfNginx_Blacklist_Acc->clear();
        this->ui->inLine_ConfNginx_Blacklist_Acc->setEnabled( false );
        this->ui->list_ConfNginx_Blacklist_Acc->clearSelection();
        this->ui->list_ConfNginx_Blacklist_Acc->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfNginx_Blacklist_Acc_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfNginx_Blacklist_AccAdd->setEnabled( true );
    } else {
        this->ui->button_ConfNginx_Blacklist_AccAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfNginx_Blacklist_Acc_returnPressed()
{
    this->on_button_ConfNginx_Blacklist_AccAdd_clicked();
}
void MainWindow::on_button_ConfNginx_Blacklist_AccAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfNginx_Blacklist_Acc->text();
    if ( this->ui->list_ConfNginx_Blacklist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        this->ui->list_ConfNginx_Blacklist_Acc->addItem( item );
        this->craplog.blacklistAdd(
            this->NGINX_ID, this->ACCESS_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Acc->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfNginx_Blacklist_Acc->clearSelection();
    this->ui->list_ConfNginx_Blacklist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfNginx_Blacklist_Acc->clear();
}

void MainWindow::on_list_ConfNginx_Blacklist_Acc_itemSelectionChanged()
{
    if ( this->ui->list_ConfNginx_Blacklist_Acc->selectedItems().size() == 1 ) {
        this->ui->button_ConfNginx_Blacklist_AccRemove->setEnabled( true );
        this->ui->button_ConfNginx_Blacklist_AccUp->setEnabled( true );
        this->ui->button_ConfNginx_Blacklist_AccDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfNginx_Blacklist_Acc->selectedItems().at(0);
        const int max = this->ui->list_ConfNginx_Blacklist_Acc->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfNginx_Blacklist_AccUp->setEnabled( false );
            this->ui->button_ConfNginx_Blacklist_AccDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfNginx_Blacklist_Acc->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfNginx_Blacklist_AccUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfNginx_Blacklist_AccDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfNginx_Blacklist_AccRemove->setEnabled( false );
        this->ui->button_ConfNginx_Blacklist_AccUp->setEnabled( false );
        this->ui->button_ConfNginx_Blacklist_AccDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfNginx_Blacklist_AccRemove_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Blacklist_Acc->selectedItems().at(0);
    this->craplog.blacklistRemove(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Acc_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Acc->currentText() );
}
void MainWindow::on_button_ConfNginx_Blacklist_AccUp_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Blacklist_Acc->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveUp(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Acc_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Blacklist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Blacklist_Acc->setFocus();
}
void MainWindow::on_button_ConfNginx_Blacklist_AccDown_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Blacklist_Acc->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveDown(
        this->NGINX_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Acc_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Blacklist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Blacklist_Acc->setFocus();
}


void MainWindow::on_box_ConfNginx_Blacklist_Err_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfNginx_Blacklist_Err->clear();
    this->ui->list_ConfNginx_Blacklist_Err->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getBlacklist(
        this->NGINX_ID, this->ERROR_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfNginx_Blacklist_Err->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isBlacklistUsed(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Err->currentText() ) );
    this->ui->checkBox_ConfNginx_Blacklist_ErrUsed->setChecked( used );
    this->on_checkBox_ConfNginx_Blacklist_ErrUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfNginx_Blacklist_ErrUsed_clicked(bool checked)
{
    this->craplog.setBlacklistUsed(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Err->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfNginx_Blacklist_Err->setEnabled( true );
        this->ui->list_ConfNginx_Blacklist_Err->setEnabled( true );
    } else {
        this->ui->inLine_ConfNginx_Blacklist_Err->clear();
        this->ui->inLine_ConfNginx_Blacklist_Err->setEnabled( false );
        this->ui->list_ConfNginx_Blacklist_Err->clearSelection();
        this->ui->list_ConfNginx_Blacklist_Err->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfNginx_Blacklist_Err_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfNginx_Blacklist_ErrAdd->setEnabled( true );
    } else {
        this->ui->button_ConfNginx_Blacklist_ErrAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfNginx_Blacklist_Err_returnPressed()
{
    this->on_button_ConfNginx_Blacklist_ErrAdd_clicked();
}
void MainWindow::on_button_ConfNginx_Blacklist_ErrAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfNginx_Blacklist_Err->text();
    if ( this->ui->list_ConfNginx_Blacklist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        this->ui->list_ConfNginx_Blacklist_Err->addItem( item );
        this->craplog.blacklistAdd(
            this->NGINX_ID, this->ERROR_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Err->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfNginx_Blacklist_Err->clearSelection();
    this->ui->list_ConfNginx_Blacklist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfNginx_Blacklist_Err->clear();
}

void MainWindow::on_list_ConfNginx_Blacklist_Err_itemSelectionChanged()
{
    if ( this->ui->list_ConfNginx_Blacklist_Err->selectedItems().size() > 0 ) {
        this->ui->button_ConfNginx_Blacklist_ErrRemove->setEnabled( true );
        this->ui->button_ConfNginx_Blacklist_ErrUp->setEnabled( true );
        this->ui->button_ConfNginx_Blacklist_ErrDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfNginx_Blacklist_Err->selectedItems().at(0);
        const int max = this->ui->list_ConfNginx_Blacklist_Err->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfNginx_Blacklist_ErrUp->setEnabled( false );
            this->ui->button_ConfNginx_Blacklist_ErrDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfNginx_Blacklist_Err->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfNginx_Blacklist_ErrUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfNginx_Blacklist_ErrDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfNginx_Blacklist_ErrRemove->setEnabled( false );
        this->ui->button_ConfNginx_Blacklist_ErrUp->setEnabled( false );
        this->ui->button_ConfNginx_Blacklist_ErrDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfNginx_Blacklist_ErrRemove_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Blacklist_Err->selectedItems().at(0);
    this->craplog.blacklistRemove(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Err_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Err->currentText() );
}
void MainWindow::on_button_ConfNginx_Blacklist_ErrUp_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Blacklist_Err->selectedItems().at(0);
    int i = this->craplog.blacklistMoveUp(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Err_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Blacklist_Err->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Blacklist_Err->setFocus();
}
void MainWindow::on_button_ConfNginx_Blacklist_ErrDown_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Blacklist_Err->selectedItems().at(0);
    int i = this->craplog.blacklistMoveDown(
        this->NGINX_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Err_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Blacklist_Err->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Blacklist_Err->setFocus();
}


////////////////
//// IIS ////
// paths
void MainWindow::on_checkBox_ConfIis_Paths_Different_clicked(bool checked)
{
    if ( checked == true ) {
        // enable the error logs path line
        this->ui->label_ConfIis_Paths_Access->setEnabled( true );
        this->ui->label_ConfIis_Paths_Error->setEnabled( true );
        this->ui->inLine_ConfIis_Paths_ErrPath->setEnabled( true );
        this->ui->inLine_ConfIis_Paths_ErrPath->setText( QString::fromStdString(
            this->craplog.getLogsPath( this->IIS_ID, this->ERROR_LOGS ) ) );
        this->on_inLine_ConfIis_Paths_ErrPath_textChanged(
            this->ui->inLine_ConfIis_Paths_ErrPath->text() );
    } else {
        // disable the error logs path line
        this->ui->label_ConfIis_Paths_Access->setEnabled( false );
        this->ui->label_ConfIis_Paths_Error->setEnabled( false );
        this->ui->inLine_ConfIis_Paths_ErrPath->setEnabled( false );
        this->ui->icon_ConfIis_Paths_ErrWrong->setVisible( false );
        // set the error logs path equals to the access logs path
        this->craplog.setLogsPath( this->IIS_ID, this->ERROR_LOGS,
            this->craplog.getLogsPath( this->IIS_ID, this->ACCESS_LOGS ) );
    }
}

void MainWindow::on_inLine_ConfIis_Paths_AccPath_textChanged(const QString &arg1)
{
    std::string path = StringOps::strip( arg1.toStdString() );
    if ( IOutils::checkDir( path ) == true ) {
        this->ui->icon_ConfIis_Paths_AccWrong->setVisible( false );
        if ( this->ui->icon_ConfIis_Paths_ErrWrong->isVisible() == false ) {
            this->ui->button_ConfIis_Paths_SavePaths->setEnabled( true );
        } else {
            this->ui->button_ConfIis_Paths_SavePaths->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfIis_Paths_AccWrong->setVisible( true );
        this->ui->button_ConfIis_Paths_SavePaths->setEnabled( false );
    }
    this->ui->inLine_ConfIis_Paths_AccPath->setText( QString::fromStdString( path ) );
}

void MainWindow::on_inLine_ConfIis_Paths_ErrPath_textChanged(const QString &arg1)
{
    std::string path = StringOps::strip( arg1.toStdString() );
    if ( IOutils::checkDir( path ) == true ) {
        this->ui->icon_ConfIis_Paths_ErrWrong->setVisible( false );
        if ( this->ui->icon_ConfIis_Paths_AccWrong->isVisible() == false ) {
            this->ui->button_ConfIis_Paths_SavePaths->setEnabled( true );
        } else {
            this->ui->button_ConfIis_Paths_SavePaths->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfIis_Paths_ErrWrong->setVisible( true );
        this->ui->button_ConfIis_Paths_SavePaths->setEnabled( false );
    }
    this->ui->inLine_ConfIis_Paths_ErrPath->setText( QString::fromStdString( path ) );
}

void MainWindow::on_button_ConfIis_Paths_SavePaths_clicked()
{
    if ( this->ui->icon_ConfIis_Paths_ErrWrong->isVisible() == false
      && this->ui->icon_ConfIis_Paths_AccWrong->isVisible() == false ) {
        // set the paths
        std::string path = StringOps::strip( this->ui->inLine_ConfIis_Paths_AccPath->text().toStdString() );
        if ( StringOps::endsWith( path, "/" ) ) {
            path = StringOps::rstrip( path, "/" );
        }
        this->craplog.setLogsPath( this->IIS_ID, this->ACCESS_LOGS, path );
        // check if the logs path is different or not
        if ( this->ui->checkBox_ConfIis_Paths_Different->isChecked() == true ) {
            // handle the error logs path too
            path = StringOps::strip( this->ui->inLine_ConfIis_Paths_ErrPath->text().toStdString() );
            if ( StringOps::endsWith( path, "/" ) ) {
                path = StringOps::rstrip( path, "/" );
            }
        }
        this->craplog.setLogsPath( this->IIS_ID, this->ERROR_LOGS, path );
    }
    this->ui->button_ConfIis_Paths_SavePaths->setEnabled( false );
}

// formats
const int MainWindow::getIisLogsModule()
{
    int module = 0;
    if ( this->ui->radio_ConfIis_Formats_NCSA->isChecked() == true ) {
        module = 1;
    } else if ( this->ui->radio_ConfIis_Formats_IIS->isChecked() == true ) {
        module = 2;
    }
    return module;
}

void MainWindow::on_radio_ConfIis_Formats_W3C_toggled(bool checked)
{
    if ( checked == true ) {
        this->craplog.setIisALF( "", 0 );
        this->ui->inLine_ConfIis_Formats_AccString->clear();
        this->ui->inLine_ConfIis_Formats_AccString->setEnabled( true );
        this->ui->inLine_ConfIis_Formats_AccString->setFocus();
    }
}
void MainWindow::on_radio_ConfIis_Formats_NCSA_toggled(bool checked)
{
    if ( checked == true ) {
        this->craplog.setIisALF( "c-ip s-sitename s-computername [date:time] sc-status sc-bytes", 1 );
        this->ui->inLine_ConfIis_Formats_AccString->clear();
        this->ui->inLine_ConfIis_Formats_AccString->setText( QString::fromStdString( this->craplog.getAccessLogsFormatString( this->IIS_ID ) ) );
        this->ui->inLine_ConfIis_Formats_AccString->setEnabled( false );
        this->ui->button_ConfIis_Formats_AccSave->setEnabled( false );
    }
}
void MainWindow::on_radio_ConfIis_Formats_IIS_toggled(bool checked)
{
    if ( checked == true ) {
        this->craplog.setIisALF( "c-ip, cs-username, date, time, s-sitename, s-computername, s-ip, time-taken, cs-bytes, sc-bytes, sc-status, sc-win32-status, cs-method, cs-uri-stem, cs-uri-query,", 2 );
        this->ui->inLine_ConfIis_Formats_AccString->clear();
        this->ui->inLine_ConfIis_Formats_AccString->setText( QString::fromStdString( this->craplog.getAccessLogsFormatString( this->IIS_ID ) ) );
        this->ui->inLine_ConfIis_Formats_AccString->setEnabled( false );
        this->ui->button_ConfIis_Formats_AccSave->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfIis_Formats_AccString_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfIis_Formats_AccSave->setEnabled( true );
    } else {
        this->ui->button_ConfIis_Formats_AccSave->setEnabled( false );
    }
}
void MainWindow::on_button_ConfIis_Formats_AccSave_clicked()
{
    this->craplog.setIisALF( StringOps::strip( this->ui->inLine_ConfIis_Formats_AccString->text().toStdString() ), this->getIisLogsModule() );
    this->ui->button_ConfIis_Formats_AccSave->setEnabled( false );
}
void MainWindow::on_button_ConfIis_Formats_AccSample_clicked()
{
    this->ui->preview_ConfIis_Formats_AccSample->setText(
        this->craplog.getLogsFormatSample( this->IIS_ID, this->ACCESS_LOGS ) );
}
void MainWindow::on_button_ConfIis_Formats_AccHelp_clicked()
{
    // !!! 2 COMPLETE !!!
}

void MainWindow::on_inLine_ConfIis_Formats_ErrString_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfIis_Formats_ErrSave->setEnabled( true );
    } else {
        this->ui->button_ConfIis_Formats_ErrSave->setEnabled( false );
    }
}
void MainWindow::on_button_ConfIis_Formats_ErrSave_clicked()
{
    /*this->craplog.setIisELF( this->ui->inLine_ConfIis_Formats_ErrString->text().toStdString() );
    this->ui->button_ConfIis_Formats_ErrSave->setEnabled( false );*/
}
void MainWindow::on_button_ConfIis_Formats_ErrSample_clicked()
{
    this->ui->preview_ConfIis_Formats_ErrSample->setText(
        this->craplog.getLogsFormatSample( this->IIS_ID, this->ERROR_LOGS ) );
}
void MainWindow::on_button_ConfIis_Formats_ErrHelp_clicked()
{
    // !!! 2 COMPLETE !!!
}

// warnlists
void MainWindow::on_box_ConfIis_Warnlist_Acc_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfIis_Warnlist_Acc->clear();
    this->ui->list_ConfIis_Warnlist_Acc->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getWarnlist(
        this->IIS_ID, this->ACCESS_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfIis_Warnlist_Acc->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isWarnlistUsed(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Acc->currentText() ) );
    this->ui->checkBox_ConfIis_Warnlist_AccUsed->setChecked( used );
    this->on_checkBox_ConfIis_Warnlist_AccUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfIis_Warnlist_AccUsed_clicked(bool checked)
{
    this->craplog.setWarnlistUsed(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Acc->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfIis_Warnlist_Acc->setEnabled( true );
        this->ui->list_ConfIis_Warnlist_Acc->setEnabled( true );
    } else {
        this->ui->inLine_ConfIis_Warnlist_Acc->clear();
        this->ui->inLine_ConfIis_Warnlist_Acc->setEnabled( false );
        this->ui->list_ConfIis_Warnlist_Acc->clearSelection();
        this->ui->list_ConfIis_Warnlist_Acc->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfIis_Warnlist_Acc_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfIis_Warnlist_AccAdd->setEnabled( true );
    } else {
        this->ui->button_ConfIis_Warnlist_AccAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfIis_Warnlist_Acc_returnPressed()
{
    this->on_button_ConfIis_Warnlist_AccAdd_clicked();
}
void MainWindow::on_button_ConfIis_Warnlist_AccAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfIis_Warnlist_Acc->text();
    if ( this->ui->list_ConfIis_Warnlist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        this->ui->list_ConfIis_Warnlist_Acc->addItem( item );
        this->craplog.warnlistAdd(
            this->IIS_ID, this->ACCESS_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Acc->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfIis_Warnlist_Acc->clearSelection();
    this->ui->list_ConfIis_Warnlist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfIis_Warnlist_Acc->clear();
}

void MainWindow::on_list_ConfIis_Warnlist_Acc_itemSelectionChanged()
{
    if ( this->ui->list_ConfIis_Warnlist_Acc->selectedItems().size() == 1 ) {
        this->ui->button_ConfIis_Warnlist_AccRemove->setEnabled( true );
        this->ui->button_ConfIis_Warnlist_AccUp->setEnabled( true );
        this->ui->button_ConfIis_Warnlist_AccDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfIis_Warnlist_Acc->selectedItems().at(0);
        const int max = this->ui->list_ConfIis_Warnlist_Acc->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfIis_Warnlist_AccUp->setEnabled( false );
            this->ui->button_ConfIis_Warnlist_AccDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfIis_Warnlist_Acc->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfIis_Warnlist_AccUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfIis_Warnlist_AccDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfIis_Warnlist_AccRemove->setEnabled( false );
        this->ui->button_ConfIis_Warnlist_AccUp->setEnabled( false );
        this->ui->button_ConfIis_Warnlist_AccDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfIis_Warnlist_AccRemove_clicked()
{
    const auto& item = this->ui->list_ConfIis_Warnlist_Acc->selectedItems().at(0);
    this->craplog.warnlistRemove(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Acc_currentTextChanged( this->ui->box_ConfIis_Warnlist_Acc->currentText() );
}
void MainWindow::on_button_ConfIis_Warnlist_AccUp_clicked()
{
    const auto& item = this->ui->list_ConfIis_Warnlist_Acc->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveUp(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Acc_currentTextChanged( this->ui->box_ConfIis_Warnlist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Warnlist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfIis_Warnlist_Acc->setFocus();
}
void MainWindow::on_button_ConfIis_Warnlist_AccDown_clicked()
{
    const auto& item = this->ui->list_ConfIis_Warnlist_Acc->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveDown(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Acc_currentTextChanged( this->ui->box_ConfIis_Warnlist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Warnlist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfIis_Warnlist_Acc->setFocus();
}


void MainWindow::on_box_ConfIis_Warnlist_Err_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfIis_Warnlist_Err->clear();
    this->ui->list_ConfIis_Warnlist_Err->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getWarnlist(
        this->IIS_ID, this->ERROR_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfIis_Warnlist_Err->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isWarnlistUsed(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Err->currentText() ) );
    this->ui->checkBox_ConfIis_Warnlist_ErrUsed->setChecked( used );
    this->on_checkBox_ConfIis_Warnlist_ErrUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfIis_Warnlist_ErrUsed_clicked(bool checked)
{
    this->craplog.setWarnlistUsed(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Err->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfIis_Warnlist_Err->setEnabled( true );
        this->ui->list_ConfIis_Warnlist_Err->setEnabled( true );
    } else {
        this->ui->inLine_ConfIis_Warnlist_Err->clear();
        this->ui->inLine_ConfIis_Warnlist_Err->setEnabled( false );
        this->ui->list_ConfIis_Warnlist_Err->clearSelection();
        this->ui->list_ConfIis_Warnlist_Err->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfIis_Warnlist_Err_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfIis_Warnlist_ErrAdd->setEnabled( true );
    } else {
        this->ui->button_ConfIis_Warnlist_ErrAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfIis_Warnlist_Err_returnPressed()
{
    this->on_button_ConfIis_Warnlist_ErrAdd_clicked();
}
void MainWindow::on_button_ConfIis_Warnlist_ErrAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfIis_Warnlist_Err->text();
    if ( this->ui->list_ConfIis_Warnlist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        this->ui->list_ConfIis_Warnlist_Err->addItem( item );
        this->craplog.warnlistAdd(
            this->IIS_ID, this->ERROR_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Err->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfIis_Warnlist_Err->clearSelection();
    this->ui->list_ConfIis_Warnlist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfIis_Warnlist_Err->clear();
}

void MainWindow::on_list_ConfIis_Warnlist_Err_itemSelectionChanged()
{
    if ( this->ui->list_ConfIis_Warnlist_Err->selectedItems().size() > 0 ) {
        this->ui->button_ConfIis_Warnlist_ErrRemove->setEnabled( true );
        this->ui->button_ConfIis_Warnlist_ErrUp->setEnabled( true );
        this->ui->button_ConfIis_Warnlist_ErrDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfIis_Warnlist_Err->selectedItems().at(0);
        const int max = this->ui->list_ConfIis_Warnlist_Err->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfIis_Warnlist_ErrUp->setEnabled( false );
            this->ui->button_ConfIis_Warnlist_ErrDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfIis_Warnlist_Err->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfIis_Warnlist_ErrUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfIis_Warnlist_ErrDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfIis_Warnlist_ErrRemove->setEnabled( false );
        this->ui->button_ConfIis_Warnlist_ErrUp->setEnabled( false );
        this->ui->button_ConfIis_Warnlist_ErrDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfIis_Warnlist_ErrRemove_clicked()
{
    const auto& item = this->ui->list_ConfIis_Warnlist_Err->selectedItems().at(0);
    this->craplog.warnlistRemove(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Err_currentTextChanged( this->ui->box_ConfIis_Warnlist_Err->currentText() );
}
void MainWindow::on_button_ConfIis_Warnlist_ErrUp_clicked()
{
    const auto& item = this->ui->list_ConfIis_Warnlist_Err->selectedItems().at(0);
    int i = this->craplog.warnlistMoveUp(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Err_currentTextChanged( this->ui->box_ConfIis_Warnlist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Warnlist_Err->item( i )->setSelected( true );
    this->ui->list_ConfIis_Warnlist_Err->setFocus();
}
void MainWindow::on_button_ConfIis_Warnlist_ErrDown_clicked()
{
    const auto& item = this->ui->list_ConfIis_Warnlist_Err->selectedItems().at(0);
    int i = this->craplog.warnlistMoveDown(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Err_currentTextChanged( this->ui->box_ConfIis_Warnlist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Warnlist_Err->item( i )->setSelected( true );
    this->ui->list_ConfIis_Warnlist_Err->setFocus();
}


// blacklist
void MainWindow::on_box_ConfIis_Blacklist_Acc_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfIis_Blacklist_Acc->clear();
    this->ui->list_ConfIis_Blacklist_Acc->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getBlacklist(
        this->IIS_ID, this->ACCESS_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfIis_Blacklist_Acc->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isBlacklistUsed(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Acc->currentText() ) );
    this->ui->checkBox_ConfIis_Blacklist_AccUsed->setChecked( used );
    this->on_checkBox_ConfIis_Blacklist_AccUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfIis_Blacklist_AccUsed_clicked(bool checked)
{
    this->craplog.setBlacklistUsed(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Acc->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfIis_Blacklist_Acc->setEnabled( true );
        this->ui->list_ConfIis_Blacklist_Acc->setEnabled( true );
    } else {
        this->ui->inLine_ConfIis_Blacklist_Acc->clear();
        this->ui->inLine_ConfIis_Blacklist_Acc->setEnabled( false );
        this->ui->list_ConfIis_Blacklist_Acc->clearSelection();
        this->ui->list_ConfIis_Blacklist_Acc->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfIis_Blacklist_Acc_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfIis_Blacklist_AccAdd->setEnabled( true );
    } else {
        this->ui->button_ConfIis_Blacklist_AccAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfIis_Blacklist_Acc_returnPressed()
{
    this->on_button_ConfIis_Blacklist_AccAdd_clicked();
}
void MainWindow::on_button_ConfIis_Blacklist_AccAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfIis_Blacklist_Acc->text();
    if ( this->ui->list_ConfIis_Blacklist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        this->ui->list_ConfIis_Blacklist_Acc->addItem( item );
        this->craplog.blacklistAdd(
            this->IIS_ID, this->ACCESS_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Acc->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfIis_Blacklist_Acc->clearSelection();
    this->ui->list_ConfIis_Blacklist_Acc->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfIis_Blacklist_Acc->clear();
}

void MainWindow::on_list_ConfIis_Blacklist_Acc_itemSelectionChanged()
{
    if ( this->ui->list_ConfIis_Blacklist_Acc->selectedItems().size() == 1 ) {
        this->ui->button_ConfIis_Blacklist_AccRemove->setEnabled( true );
        this->ui->button_ConfIis_Blacklist_AccUp->setEnabled( true );
        this->ui->button_ConfIis_Blacklist_AccDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfIis_Blacklist_Acc->selectedItems().at(0);
        const int max = this->ui->list_ConfIis_Blacklist_Acc->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfIis_Blacklist_AccUp->setEnabled( false );
            this->ui->button_ConfIis_Blacklist_AccDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfIis_Blacklist_Acc->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfIis_Blacklist_AccUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfIis_Blacklist_AccDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfIis_Blacklist_AccRemove->setEnabled( false );
        this->ui->button_ConfIis_Blacklist_AccUp->setEnabled( false );
        this->ui->button_ConfIis_Blacklist_AccDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfIis_Blacklist_AccRemove_clicked()
{
    const auto& item = this->ui->list_ConfIis_Blacklist_Acc->selectedItems().at(0);
    this->craplog.blacklistRemove(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Acc_currentTextChanged( this->ui->box_ConfIis_Blacklist_Acc->currentText() );
}
void MainWindow::on_button_ConfIis_Blacklist_AccUp_clicked()
{
    const auto& item = this->ui->list_ConfIis_Blacklist_Acc->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveUp(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Acc_currentTextChanged( this->ui->box_ConfIis_Blacklist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Blacklist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfIis_Blacklist_Acc->setFocus();
}
void MainWindow::on_button_ConfIis_Blacklist_AccDown_clicked()
{
    const auto& item = this->ui->list_ConfIis_Blacklist_Acc->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveDown(
        this->IIS_ID, this->ACCESS_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Acc->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Acc_currentTextChanged( this->ui->box_ConfIis_Blacklist_Acc->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Blacklist_Acc->item( i )->setSelected( true );
    this->ui->list_ConfIis_Blacklist_Acc->setFocus();
}


void MainWindow::on_box_ConfIis_Blacklist_Err_currentTextChanged(const QString &arg1)
{
    this->ui->inLine_ConfIis_Blacklist_Err->clear();
    this->ui->list_ConfIis_Blacklist_Err->clear();
    // update the list
    const std::vector<std::string>& list = this->craplog.getBlacklist(
        this->IIS_ID, this->ERROR_LOGS, this->crapview.getLogFieldID( arg1 ) );
    for ( const std::string& item : list ) {
        this->ui->list_ConfIis_Blacklist_Err->addItem( QString::fromStdString( item ) );
    }
    // check/uncheck the usage option
    bool used = this->craplog.isBlacklistUsed(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Err->currentText() ) );
    this->ui->checkBox_ConfIis_Blacklist_ErrUsed->setChecked( used );
    this->on_checkBox_ConfIis_Blacklist_ErrUsed_clicked( used );
}
void MainWindow::on_checkBox_ConfIis_Blacklist_ErrUsed_clicked(bool checked)
{
    this->craplog.setBlacklistUsed(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Err->currentText() ),
        checked );
    if ( checked == true ) {
        this->ui->inLine_ConfIis_Blacklist_Err->setEnabled( true );
        this->ui->list_ConfIis_Blacklist_Err->setEnabled( true );
    } else {
        this->ui->inLine_ConfIis_Blacklist_Err->clear();
        this->ui->inLine_ConfIis_Blacklist_Err->setEnabled( false );
        this->ui->list_ConfIis_Blacklist_Err->clearSelection();
        this->ui->list_ConfIis_Blacklist_Err->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfIis_Blacklist_Err_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfIis_Blacklist_ErrAdd->setEnabled( true );
    } else {
        this->ui->button_ConfIis_Blacklist_ErrAdd->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfIis_Blacklist_Err_returnPressed()
{
    this->on_button_ConfIis_Blacklist_ErrAdd_clicked();
}
void MainWindow::on_button_ConfIis_Blacklist_ErrAdd_clicked()
{
    const QString& item = this->ui->inLine_ConfIis_Blacklist_Err->text();
    if ( this->ui->list_ConfIis_Blacklist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        this->ui->list_ConfIis_Blacklist_Err->addItem( item );
        this->craplog.blacklistAdd(
            this->IIS_ID, this->ERROR_LOGS,
            this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Err->currentText() ),
            item.toStdString() );
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfIis_Blacklist_Err->clearSelection();
    this->ui->list_ConfIis_Blacklist_Err->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfIis_Blacklist_Err->clear();
}

void MainWindow::on_list_ConfIis_Blacklist_Err_itemSelectionChanged()
{
    if ( this->ui->list_ConfIis_Blacklist_Err->selectedItems().size() > 0 ) {
        this->ui->button_ConfIis_Blacklist_ErrRemove->setEnabled( true );
        this->ui->button_ConfIis_Blacklist_ErrUp->setEnabled( true );
        this->ui->button_ConfIis_Blacklist_ErrDown->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfIis_Blacklist_Err->selectedItems().at(0);
        const int max = this->ui->list_ConfIis_Blacklist_Err->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfIis_Blacklist_ErrUp->setEnabled( false );
            this->ui->button_ConfIis_Blacklist_ErrDown->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfIis_Blacklist_Err->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfIis_Blacklist_ErrUp->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfIis_Blacklist_ErrDown->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfIis_Blacklist_ErrRemove->setEnabled( false );
        this->ui->button_ConfIis_Blacklist_ErrUp->setEnabled( false );
        this->ui->button_ConfIis_Blacklist_ErrDown->setEnabled( false );
    }
}
void MainWindow::on_button_ConfIis_Blacklist_ErrRemove_clicked()
{
    const auto& item = this->ui->list_ConfIis_Blacklist_Err->selectedItems().at(0);
    this->craplog.blacklistRemove(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Err_currentTextChanged( this->ui->box_ConfIis_Blacklist_Err->currentText() );
}
void MainWindow::on_button_ConfIis_Blacklist_ErrUp_clicked()
{
    const auto& item = this->ui->list_ConfIis_Blacklist_Err->selectedItems().at(0);
    int i = this->craplog.blacklistMoveUp(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Err_currentTextChanged( this->ui->box_ConfIis_Blacklist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Blacklist_Err->item( i )->setSelected( true );
    this->ui->list_ConfIis_Blacklist_Err->setFocus();
}
void MainWindow::on_button_ConfIis_Blacklist_ErrDown_clicked()
{
    const auto& item = this->ui->list_ConfIis_Blacklist_Err->selectedItems().at(0);
    int i = this->craplog.blacklistMoveDown(
        this->IIS_ID, this->ERROR_LOGS,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Err->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Err_currentTextChanged( this->ui->box_ConfIis_Blacklist_Err->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Blacklist_Err->item( i )->setSelected( true );
    this->ui->list_ConfIis_Blacklist_Err->setFocus();
}



