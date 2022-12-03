
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "utilities/checks.h"
#include "utilities/colors.h"
#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/rtf.h"
#include "utilities/stylesheets.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"
#include "modules/shared.h"

#include <QTimer>

#include <chrono>
#include <filesystem>


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

    // parent fonts
    this->ui->mainwidget->setFont( this->FONTS.at( "main_big" ) );

    // TextBrowser for the LogFiles
    this->TB.setColorScheme( 1, this->TB_COLOR_SCHEMES.at( 1 ) );
    this->TB.setFont( this->FONTS.at( "main" ) );
    this->ui->textLogFiles->setFont( this->TB.getFont() );

    // adjust LogsList headers width
    this->ui->listLogFiles->header()->resizeSection(0,200);
    this->ui->listLogFiles->header()->resizeSection(1,100);

    // blocknote
    this->crapnote->setFont( this->FONTS.at( "main" ) );


    //////////////
    //// MENU ////
    // languages
    connect( this->ui->actionEnglish,  &QAction::triggered, this, &MainWindow::menu_actionEnglish_triggered  );
    connect( this->ui->actionEspanol,  &QAction::triggered, this, &MainWindow::menu_actionEspanol_triggered  );
    connect( this->ui->actionFrancais, &QAction::triggered, this, &MainWindow::menu_actionFrancais_triggered );
    connect( this->ui->actionItaliano, &QAction::triggered, this, &MainWindow::menu_actionItaliano_triggered );
    // tools
    connect( this->ui->actionBlockNote, &QAction::triggered, this, &MainWindow::menu_actionBlockNote_triggered );
    // utilities
    connect( this->ui->actionInfos, &QAction::triggered, this, &MainWindow::menu_actionInfos_triggered );
    connect( this->ui->actionCheckUpdates, &QAction::triggered, this, &MainWindow::menu_actionCheckUpdates_triggered );
    // games
    connect( this->ui->actionCrissCross, &QAction::triggered, this, &MainWindow::menu_actionCrissCross_triggered );
    connect( this->ui->actionSnake, &QAction::triggered, this, &MainWindow::menu_actionSnake_triggered );


    /////////////////
    //// CONFIGS ////
    this->defineOSspec();
    this->readConfigs();


    ///////////////////
    //// POLISHING ////
    // default tabs
    this->switchMainTab( 0 );
    this->switchStatsTab( 0 );

    // language menu
    if ( this->language != "en" ) {
        this->ui->actionEnglish->setChecked( false );
        if ( language == "es" ) {
            this->ui->actionEspanol->setChecked( true );
        } else if ( language == "fr" ) {
            this->ui->actionFrancais->setChecked( true );
        } else if ( language == "it" ) {
            this->ui->actionItaliano->setChecked( true );
        }
    }

    // set the default WS as the current one
    switch ( this->default_ws ) {
        case 11:
            this->ui->button_LogFiles_Apache->setFlat( false );
            this->ui->radio_ConfDefaults_Apache->setChecked( true );
            break;
        case 12:
            this->ui->button_LogFiles_Nginx->setFlat( false );
            this->ui->radio_ConfDefaults_Nginx->setChecked( true );
            break;
        case 13:
            this->ui->button_LogFiles_Iis->setFlat( false );
            this->ui->radio_ConfDefaults_Iis->setChecked( true );
            break;
        default:
            // shouldn't be here
            throw WebServerException( "Unexpected WebServer ID: "+std::to_string( this->default_ws ) );
    }
    this->craplog.setCurrentWSID( this->default_ws );


    // make the Configs initialize
    // window
    this->ui->checkBox_ConfWindow_Geometry->setChecked( this->remember_window );
    this->ui->box_ConfWindow_Theme->setCurrentIndex( this->window_theme_id );
    this->ui->box_ConfWindow_Icons->setCurrentIndex( this->icons_theme_id );
    // dialogs
    this->ui->slider_ConfDialogs_General->setValue( this->dialogs_level );
    this->ui->slider_ConfDialogs_Logs->setValue( this->craplog.getDialogsLevel() );
    this->ui->slider_ConfDialogs_Stats->setValue( this->crapview.getDialogsLevel() );
    // text browser
    this->ui->box_ConfTextBrowser_Font->setCurrentText( this->TB.getFontFamily() );
    this->ui->checkBox_ConfTextBrowser_WideLines->setChecked( this->TB.getWideLinesUsage() );
    this->ui->box_ConfTextBrowser_ColorScheme->setCurrentIndex( this->TB.getColorSchemeID() );
    this->refreshTextBrowserPreview();
    // charts
    this->ui->box_ConfCharts_Theme->setCurrentIndex( this->charts_theme_id );
    this->refreshChartsPreview();
    // databases
    this->ui->inLine_ConfDatabases_Data_Path->setText( QString::fromStdString( this->db_data_path ) );
    this->ui->button_ConfDatabases_Data_Save->setEnabled( false );
    this->ui->inLine_ConfDatabases_Hashes_Path->setText( QString::fromStdString( this->db_hashes_path ) );
    this->ui->button_ConfDatabases_Hashes_Save->setEnabled( false );
    this->ui->spinBox_ConfDatabases_NumBackups->setValue( this->db_backups_number );
    this->ui->checkBox_ConfDatabases_DoBackup->setChecked( this->db_do_backup );
    // logs control
    this->ui->checkBox_ConfControl_Usage->setChecked( this->hide_used_files );
    this->ui->spinBox_ConfControl_Size->setValue( this->craplog.getWarningSize() / 1'048'576 );
    if ( this->craplog.getWarningSize() > 0 ) {
        this->ui->checkBox_ConfControl_Size->setChecked( true );
    } else {
        this->ui->checkBox_ConfControl_Size->setChecked( false );
    }
    // apache paths
    this->ui->inLine_ConfApache_Path_String->setText( QString::fromStdString(this->craplog.getLogsPath( this->APACHE_ID )) );
    this->ui->button_ConfApache_Path_Save->setEnabled( false );
    // apache formats
    this->ui->inLine_ConfApache_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( this->APACHE_ID ) ) );
    this->ui->button_ConfApache_Format_Save->setEnabled( false );
    // apache warnlists
    this->on_box_ConfApache_Warnlist_Field_currentTextChanged( this->ui->box_ConfApache_Warnlist_Field->currentText() );
    // apache blacklists
    this->on_box_ConfApache_Blacklist_Field_currentTextChanged( this->ui->box_ConfApache_Blacklist_Field->currentText() );
    // nginx paths
    this->ui->inLine_ConfNginx_Path_String->setText( QString::fromStdString(this->craplog.getLogsPath( this->NGINX_ID )) );
    this->ui->button_ConfNginx_Path_Save->setEnabled( false );
    // nginx formats
    this->ui->inLine_ConfNginx_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( this->NGINX_ID ) ) );
    this->ui->button_ConfNginx_Format_Save->setEnabled( false );
    // nginx warnlists
    this->on_box_ConfNginx_Warnlist_Field_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Field->currentText() );
    // nginx blacklists
    this->on_box_ConfNginx_Blacklist_Field_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Field->currentText() );
    // iis paths
    this->ui->inLine_ConfIis_Path_String->setText( QString::fromStdString(this->craplog.getLogsPath( this->IIS_ID )) );
    this->ui->button_ConfIis_Path_Save->setEnabled( false );
    // iis formats
    this->ui->inLine_ConfIis_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( this->IIS_ID ) ) );
    this->ui->button_ConfIis_Format_Save->setEnabled( false );
    // iis warnlists
    this->on_box_ConfIis_Warnlist_Field_currentTextChanged( this->ui->box_ConfIis_Warnlist_Field->currentText() );
    // iis blacklists
    this->on_box_ConfIis_Blacklist_Field_currentTextChanged( this->ui->box_ConfIis_Blacklist_Field->currentText() );


    // blocknote's font and colors
    this->crapnote->setTextFont( this->TB.getFont() );
    this->crapnote->setColorScheme( this->TB.getColorSchemeID() );

    // text browser's default message
    {
        QString rich_text;
        RichText::richLogsDefault( rich_text );
        this->ui->textLogFiles->setText( rich_text );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        rich_text.clear();
    }


    ///////////////////
    //// INTERFACE ////
    this->updateUiLanguage();
    this->updateUiTheme();


    ///////////////
    //// START ////
    // get a fresh list of LogFiles
    this->waiter_timer = new QTimer(this);
    connect(this->waiter_timer, SIGNAL(timeout()), this, SLOT(wait_ActiveWindow()));
    this->waiter_timer->start(250);
}

MainWindow::~MainWindow()
{
    delete this->ui;
    delete this->waiter_timer;
    delete this->craplog_timer;
    delete this->crapview_timer;
    delete this->craphelp;
    delete this->crapnote;
    delete this->crapinfo;
    delete this->crapup;
    delete this->crisscross;
    delete this->snake;
}

void MainWindow::closeEvent( QCloseEvent *event )
{
    // save actual configurations
    this->writeConfigs();
    // backup the database
    if ( this->db_do_backup && this->db_edited ) {
        this->backupDatabase();
    }
    // save splitters sizes => this->ui->splitter_StatsCount->sizes();
}



////////////////////////
//// CONFIGURATIONS ////
////////////////////////
// os definition
void MainWindow::defineOSspec()
{
    /*const std::string home_path = StringOps::rstrip( QStandardPaths::locate( QStandardPaths::HomeLocation, "", QStandardPaths::LocateDirectory ).toStdString(), "/" );*/
    switch ( this->OS ) {
        case 1:
            // unix-like
            /*this->configs_path   = home_path + "/.config/LogDoctor/logdoctor.conf";
            this->logdoc_path    = home_path + "/.local/share/LogDoctor";*/
            this->db_data_path   = this->logdoc_path;
            this->db_hashes_path = this->logdoc_path;
            break;

        case 2:
            // windows
            /*this->configs_path   = home_path + "/AppData/Local/LogDoctor/logdoctor.conf";
            this->logdoc_path    = home_path + "/AppData/Local/LogDoctor";*/
            this->db_data_path   = this->logdoc_path;
            this->db_hashes_path = this->logdoc_path;
            break;

        case 3:
            // darwin-based
            /*this->configs_path   = home_path + "/Lybrary/Preferences/LogDoctor/logdoctor.conf";
            this->logdoc_path    = home_path + "/Lybrary/Application Support/LogDoctor";*/
            this->db_data_path   = this->logdoc_path;
            this->db_hashes_path = this->logdoc_path;
            break;

        default:
            // shouldn't be here
            throw GenericException( "Unexpected OS ID: "+std::to_string( this->OS ), true );
    }
}

void MainWindow::readConfigs()
{
    bool proceed = true;
    std::error_code err;
    QString err_msg = "";
    // check the file
    if ( IOutils::exists( this->configs_path ) ) {
        if ( IOutils::checkFile( this->configs_path ) ) {
            if ( ! IOutils::checkFile( this->configs_path, true ) ) {
                // file not readable, try to assign permissions
                std::filesystem::permissions( this->configs_path,
                                              std::filesystem::perms::owner_read,
                                              std::filesystem::perm_options::add,
                                              err );
                if ( err.value() ) {
                    proceed = false;
                    QString file = "";
                    if ( this->dialogs_level > 0 ) {
                        file = QString::fromStdString( this->configs_path );
                        err_msg = QString::fromStdString( err.message() );
                    }
                    DialogSec::errConfFileNotReadable( file, err_msg );
                }
            }
        } else {
            // the given path doesn't point to a file
            proceed = DialogSec::choiceFileNotFile( QString::fromStdString( this->configs_path ) );
            if ( proceed ) {
                proceed = IOutils::renameAsCopy( this->configs_path, err );
                if ( ! proceed ) {
                    QString path = "";
                    if ( this->dialogs_level > 0 ) {
                        path = QString::fromStdString( this->configs_path );
                        if ( err.value() ) {
                            err_msg = QString::fromStdString( err.message() );
                        }
                    }
                    DialogSec::errRenaming( QString::fromStdString( this->configs_path ), err_msg );
                }
            }
        }
    } else {
        // configuration file not found
        proceed = false;
        QString file = "";
        if ( this->dialogs_level == 2 ) {
            file = QString::fromStdString( this->configs_path );
        }
        DialogSec::warnConfFileNotFound( file );
    }

    if ( proceed ) {
        QString err_msg="", aux_err_msg;
        std::vector<std::string> aux, configs;
        try {
            // reset the lists when a config file is found
            for ( int w=this->APACHE_ID; w<=this->IIS_ID; w++ ) {
                for ( const int& f : std::vector<int>({11,12,20,21}) ) {
                    this->craplog.setWarnlist( w, f, {} );
                }
                this->craplog.setBlacklist( w, 20, {} );
            }
            std::string content;
            IOutils::readFile( this->configs_path, content );
            StringOps::splitrip( configs, content );
            for ( const std::string& line : configs ) {
                if ( StringOps::startsWith( line, "[") ) {
                    // section descriptor
                    continue;
                }
                aux.clear();
                StringOps::splitrip( aux, line, "=" );
                if ( aux.size() < 2 ) {
                    // nothing to do
                    continue;
                }
                // if here, a value is present
                const std::string& var = aux.at( 0 ),
                                   val = aux.at( 1 );

                if ( val.size() == 0 ) {
                    // nothing to do, no value stored
                    continue;
                }

                if ( var == "Language" ) {
                    if ( val.size() > 2 ) {
                        // not a valid locale, keep the default
                        DialogSec::errLangLocaleInvalid( QString::fromStdString( val ) );
                    } else {
                        if ( val == "en" || val == "es" || val == "fr" || val == "it" ) {
                            this->language = val;
                        } else {
                            DialogSec::errLangNotAccepted( QString::fromStdString( val ) );
                        }
                    }

                } else if ( var == "RememberGeometry" ) {
                    this->remember_window = this->s2b.at( val );

                } else if ( var == "Geometry" ) {
                    this->geometryFromString( val );

                } else if ( var == "WindowTheme" ) {
                    this->window_theme_id = std::stoi( val );

                } else if ( var == "ChartsTheme" ) {
                    this->charts_theme_id = std::stoi( val );

                } else if ( var == "IconsTheme" ) {
                    this->icons_theme_id = std::stoi( val );

                } else if ( var == "MainDialogLevel" ) {
                    this->dialogs_level = std::stoi( val );

                } else if ( var == "DefaultWebServer" ) {
                    this->default_ws = std::stoi( val );

                } else if ( var == "DatabaseDataPath" ) {
                    this->db_data_path = this->resolvePath( val );

                } else if ( var == "DatabaseHashesPath" ) {
                    this->db_hashes_path = this->resolvePath( val );

                } else if ( var == "DatabaseDoBackup" ) {
                    this->db_do_backup = this->s2b.at( val );

                } else if ( var == "DatabaseBackupsNumber" ) {
                    this->db_backups_number = std::stoi( val );

                } else if ( var == "Font" ) {
                    this->on_box_ConfTextBrowser_Font_currentIndexChanged( std::stoi( val ) );

                } else if ( var == "WideLines" ) {
                    this->TB.setWideLinesUsage( this->s2b.at( val ) );

                } else if ( var == "ColorScheme" ) {
                    this->on_box_ConfTextBrowser_ColorScheme_currentIndexChanged( std::stoi( val ) );

                } else if ( var == "CraplogDialogLevel" ) {
                    this->craplog.setDialogsLevel( std::stoi( val ) );

                } else if ( var == "HideUsedFiles" ) {
                    hide_used_files = this->s2b.at( val );

                } else if ( var == "WarningSize" ) {
                    this->craplog.setWarningSize( std::stoi( val ) );

                } else if ( var == "ApacheLogsPath" ) {
                    this->craplog.setLogsPath( this->APACHE_ID, this->resolvePath( val ) );

                } else if ( var == "ApacheLogsFormat" ) {
                    if ( ! this->craplog.setApacheLogFormat( val ) ) {
                        throw("");
                    }

                } else if ( var == "ApacheWarnlistMethod" ) {
                    aux_err_msg = QString("Apache -> %1 (%2)")
                        .arg( TR::tr(FIELDS__METHOD.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->APACHE_ID, 11, this->string2list( val ) );

                } else if ( var == "ApacheWarnlistMethodUsed" ) {
                    this->craplog.setWarnlistUsed( this->APACHE_ID, 11, this->s2b.at( val ) );

                } else if ( var == "ApacheWarnlistURI" ) {
                    aux_err_msg = QString("Apache -> %1 (%2)")
                        .arg( TR::tr(FIELDS__URI.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->APACHE_ID, 12, this->string2list( val ) );

                } else if ( var == "ApacheWarnlistURIUsed" ) {
                    this->craplog.setWarnlistUsed( this->APACHE_ID, 12, this->s2b.at( val ) );

                } else if ( var == "ApacheWarnlistClient" ) {
                    aux_err_msg = QString("Apache -> %1 (%2)")
                        .arg( TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->APACHE_ID, 20, this->string2list( val ) );

                } else if ( var == "ApacheWarnlistClientUsed" ) {
                    this->craplog.setWarnlistUsed( this->APACHE_ID, 20, this->s2b.at( val ) );

                } else if ( var == "ApacheWarnlistUserAgent" ) {
                    aux_err_msg = QString("Apache -> %1 (%2)")
                        .arg( TR::tr(FIELDS__USER_AGENT.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->APACHE_ID, 21, this->string2list( val, true ) );

                } else if ( var == "ApacheWarnlistUserAgentUsed" ) {
                    this->craplog.setWarnlistUsed( this->APACHE_ID, 21, this->s2b.at( val ) );

                } else if ( var == "ApacheBlacklistClient" ) {
                    aux_err_msg = QString("Apache -> %1 (%2)")
                        .arg( TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("blacklist") );
                    this->craplog.setBlacklist( this->APACHE_ID, 20, this->string2list( val ) );

                } else if ( var == "ApacheBlacklistClientUsed" ) {
                    this->craplog.setBlacklistUsed( this->APACHE_ID, 20, this->s2b.at( val ) );

                } else if ( var == "NginxLogsPath" ) {
                    this->craplog.setLogsPath( this->NGINX_ID, this->resolvePath( val ) );

                } else if ( var == "NginxLogsFormat" ) {
                    if ( ! this->craplog.setNginxLogFormat( val ) ) {
                        throw("");
                    }

                } else if ( var == "NginxWarnlistMethod" ) {
                    aux_err_msg = QString("Nginx -> %1 (%2)")
                        .arg( TR::tr(FIELDS__METHOD.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->NGINX_ID, 11, this->string2list( val ) );

                } else if ( var == "NginxWarnlistMethodUsed" ) {
                    this->craplog.setWarnlistUsed( this->NGINX_ID, 11, this->s2b.at( val ) );

                } else if ( var == "NginxWarnlistURI" ) {
                    aux_err_msg = QString("Nginx -> %1 (%2)")
                        .arg( TR::tr(FIELDS__URI.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->NGINX_ID, 12, this->string2list( val ) );

                } else if ( var == "NginxWarnlistURIUsed" ) {
                    this->craplog.setWarnlistUsed( this->NGINX_ID, 12, this->s2b.at( val ) );

                } else if ( var == "NginxWarnlistClient" ) {
                    aux_err_msg = QString("Nginx -> %1 (%2)")
                        .arg( TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->NGINX_ID, 20, this->string2list( val ) );

                } else if ( var == "NginxWarnlistClientUsed" ) {
                    this->craplog.setWarnlistUsed( this->NGINX_ID, 20, this->s2b.at( val ) );

                } else if ( var == "NginxWarnlistUserAgent" ) {
                    aux_err_msg = QString("Nginx -> %1 (%2)")
                        .arg( TR::tr(FIELDS__USER_AGENT.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->NGINX_ID, 21, this->string2list( val, true ) );

                } else if ( var == "NginxWarnlistUserAgentUsed" ) {
                    this->craplog.setWarnlistUsed( this->NGINX_ID, 21, this->s2b.at( val ) );

                } else if ( var == "NginxBlacklistClient" ) {
                    aux_err_msg = QString("Nginx -> %1 (%2)")
                        .arg( TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("blacklist") );
                    this->craplog.setBlacklist( this->NGINX_ID, 20, this->string2list( val ) );

                } else if ( var == "NginxBlacklistClientUsed" ) {
                    this->craplog.setBlacklistUsed( this->NGINX_ID, 20, this->s2b.at( val ) );

                } else if ( var == "IisLogsPath" ) {
                    this->craplog.setLogsPath( this->IIS_ID, this->resolvePath( val ) );

                } else if ( var == "IisLogsModule" ) {
                    if ( val == "1" ) {
                        this->ui->radio_ConfIis_Format_NCSA->setChecked( true );
                    } else if ( val == "2" ) {
                        this->ui->radio_ConfIis_Format_IIS->setChecked( true );
                    }

                } else if ( var == "IisLogsFormat" ) {
                    int module = 0;
                    if ( this->ui->radio_ConfIis_Format_NCSA->isChecked() ) {
                        module = 1;
                    } else if ( this->ui->radio_ConfIis_Format_IIS->isChecked() ) {
                        module = 2;
                    }
                    if ( ! this->craplog.setIisLogFormat( val, module ) ) {
                        throw("");
                    }

                } else if ( var == "IisWarnlistMethod" ) {
                    aux_err_msg = QString("IIS -> %1 (%2)")
                        .arg( TR::tr(FIELDS__METHOD.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->IIS_ID, 11, this->string2list( val ) );

                } else if ( var == "IisWarnlistMethodUsed" ) {
                    this->craplog.setWarnlistUsed( this->IIS_ID, 11, this->s2b.at( val ) );

                } else if ( var == "IisWarnlistURI" ) {
                    aux_err_msg = QString("IIS -> %1 (%2)")
                        .arg( TR::tr(FIELDS__URI.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->IIS_ID, 12, this->string2list( val ) );

                } else if ( var == "IisWarnlistURIUsed" ) {
                    this->craplog.setWarnlistUsed( this->IIS_ID, 12, this->s2b.at( val ) );

                } else if ( var == "IisWarnlistClient" ) {
                    aux_err_msg = QString("IIS -> %1 (%2)")
                        .arg( TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->IIS_ID, 20, this->string2list( val ) );

                } else if ( var == "IisWarnlistClientUsed" ) {
                    this->craplog.setWarnlistUsed( this->IIS_ID, 20, this->s2b.at( val ) );

                } else if ( var == "IisWarnlistUserAgent" ) {
                    aux_err_msg = QString("IIS -> %1 (%2)")
                        .arg( TR::tr(FIELDS__USER_AGENT.c_str()), MainWindow::tr("warnlist") );
                    this->craplog.setWarnlist( this->IIS_ID, 21, this->string2list( val, true ) );

                } else if ( var == "IisWarnlistUserAgentUsed" ) {
                    this->craplog.setWarnlistUsed( this->IIS_ID, 21, this->s2b.at( val ) );

                } else if ( var == "IisBlacklistClient" ) {
                    aux_err_msg = QString("IIS -> %1 (%2)")
                        .arg( TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("blacklist") );
                    this->craplog.setBlacklist( this->IIS_ID, 20, this->string2list( val ) );

                } else if ( var == "IisBlacklistClientUsed" ) {
                    this->craplog.setBlacklistUsed( this->IIS_ID, 20, this->s2b.at( val ) );

                } else if ( var == "CrapviewDialogLevel" ) {
                    this->crapview.setDialogsLevel( std::stoi( val ) );

                }/* else {
                    // not valid
                }*/
            }

        } catch ( const std::ios_base::failure& ) {
            // failed reading
            proceed = false;
            err_msg = DialogSec::tr("An error occured while reading the configuration file");
        } catch ( const LogFormatException& ) {
            proceed = false; // message already shown
        } catch ( const BWlistException& ) {
            proceed = false;
            err_msg = QString("%1:\n%2").arg(
                DialogSec::tr("One of the lists has an invalid item"),
                aux_err_msg );
        } catch (...) {
            // something failed
            proceed = false;
            err_msg = DialogSec::tr("An error occured while parsing configuration file's data");
        }
        if ( ! proceed ) {
            DialogSec::errFailedApplyingConfigs( err_msg );
            this->closeEvent( new QCloseEvent() );
        }
    }
}

void MainWindow::writeConfigs()
{
    std::error_code err;
    bool proceed=true, msg_shown=false;
    QString msg="", err_msg="";
    // check the file first
    if ( IOutils::exists( this->configs_path ) ) {
        if ( IOutils::checkFile( this->configs_path ) ) {
            if ( ! IOutils::checkFile( this->configs_path, false, true ) ) {
                // file not writable, try to assign permissions
                std::filesystem::permissions( this->configs_path,
                                              std::filesystem::perms::owner_write,
                                              std::filesystem::perm_options::add,
                                              err );
                if ( err.value() ) {
                    proceed = false;
                    QString file = "";
                    if ( this->dialogs_level > 0 ) {
                        file = QString::fromStdString( this->configs_path );
                        err_msg = QString::fromStdString( err.message() );
                    }
                    DialogSec::errConfFileNotWritable( file, err_msg );
                    msg_shown = true;
                }
            }
        } else {
            // the given path doesn't point to a file
            proceed = DialogSec::choiceFileNotFile( QString::fromStdString( this->configs_path ) );
            if ( proceed ) {
                proceed = IOutils::renameAsCopy( this->configs_path, err );
                if ( ! proceed ) {
                    QString path = "";
                    if ( this->dialogs_level > 0 ) {
                        path = QString::fromStdString( this->configs_path );
                        if ( err.value() ) {
                            err_msg = QString::fromStdString( err.message() );
                        }
                    }
                    DialogSec::errRenaming( path, err_msg );
                    msg_shown = true;
                }
            }
        }
    } else {
        // file does not exists, check if at least the folder exists
        const std::string base_path = this->basePath( this->configs_path );
        if ( IOutils::exists( base_path ) ) {
            if ( IOutils::isDir( base_path ) ) {
                if ( ! IOutils::checkDir( base_path, false, true ) ) {
                    // directory not writable, try to assign permissions
                    std::filesystem::permissions( base_path,
                                                  std::filesystem::perms::owner_write,
                                                  std::filesystem::perm_options::add,
                                                  err );
                    if ( err.value() ) {
                        proceed = false;
                        QString file = "";
                        if ( this->dialogs_level > 0 ) {
                            file = QString::fromStdString( base_path );
                            err_msg = QString::fromStdString( err.message() );
                        }
                        DialogSec::errConfDirNotWritable( file, err_msg );
                        msg_shown = true;
                    }
                }
            } else {
                // not a directory
                proceed = DialogSec::choiceDirNotDir( QString::fromStdString( base_path ) );
                if ( proceed ) {
                    proceed = IOutils::renameAsCopy( base_path, err );
                    if ( ! proceed ) {
                        QString path = "";
                        if ( this->dialogs_level > 0 ) {
                            path = QString::fromStdString( base_path );
                            err_msg = QString::fromStdString( err.message() );
                        }
                        DialogSec::errRenaming( path, err_msg );
                        msg_shown = true;
                    } else {
                        // make the new folder
                        proceed = IOutils::makeDir( base_path, err );
                        if ( ! proceed ) {
                            msg = DialogSec::tr("Failed to create the configuration file's directory");
                            if ( this->dialogs_level > 0 ) {
                                msg += ":\n"+QString::fromStdString( base_path );
                                err_msg = QString::fromStdString( err.message() );
                            }
                        }
                    }
                }
            }
        } else {
            // the folder does not exist too
            proceed = IOutils::makeDir( base_path, err );
            if ( ! proceed ) {
                msg = DialogSec::tr("Failed to create the configuration file's directory");
                if ( this->dialogs_level > 0 ) {
                    msg += ":\n"+QString::fromStdString( base_path );
                    err_msg = QString::fromStdString( err.message() );
                }
            }
        }
    }
    if ( !proceed && !msg_shown ) {
        DialogSec::errConfFailedWriting( msg, err_msg );
    }

    if ( proceed ) {
        //// USER INTERFACE ////
        std::string configs = "";
        configs += "\n\n[UI]";
        configs += "\nLanguage=" + this->language;
        configs += "\nRememberGeometry=" + this->b2s.at( this->remember_window );
        configs += "\nGeometry=" + this->geometryToString();
        configs += "\nWindowTheme=" + std::to_string( this->window_theme_id );
        configs += "\nChartsTheme=" + std::to_string( this->charts_theme_id );
        configs += "\nIconsTheme=" + std::to_string( this->icons_theme_id );
        configs += "\nMainDialogLevel=" + std::to_string( this->dialogs_level );
        configs += "\nDefaultWebServer=" + std::to_string( this->default_ws );
        configs += "\nDatabaseDataPath=" + this->db_data_path;
        configs += "\nDatabaseHashesPath=" + this->db_hashes_path;
        configs += "\nDatabaseDoBackup=" + this->b2s.at( this->db_do_backup );
        configs += "\nDatabaseBackupsNumber=" + std::to_string( this->db_backups_number );
        //// TEXT BROWSER ////
        configs += "\n\n[TextBrowser]";
        configs += "\nFont=" + std::to_string( this->ui->box_ConfTextBrowser_Font->currentIndex() );
        configs += "\nWideLines=" + this->b2s.at( this->TB.getWideLinesUsage() );
        configs += "\nColorScheme=" + std::to_string( this->TB.getColorSchemeID() );
        //// CRAPLOG ////
        configs += "\n\n[Craplog]";
        configs += "\nCraplogDialogLevel=" + std::to_string( this->craplog.getDialogsLevel() );
        configs += "\nHideUsedFiles=" + this->b2s.at( this->hide_used_files );
        configs += "\nWarningSize=" + std::to_string( this->craplog.getWarningSize() );
        //// APACHE2 ////
        configs += "\n\n[Apache2]";
        configs += "\nApacheLogsPath=" + this->craplog.getLogsPath( this->APACHE_ID );
        configs += "\nApacheLogsFormat=" + this->craplog.getLogsFormatString( this->APACHE_ID );
        configs += "\nApacheWarnlistMethod=" + this->list2string( this->craplog.getWarnlist( this->APACHE_ID, 11 ) );
        configs += "\nApacheWarnlistMethodUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->APACHE_ID, 11 ) );
        configs += "\nApacheWarnlistURI=" + this->list2string( this->craplog.getWarnlist( this->APACHE_ID, 12 ) );
        configs += "\nApacheWarnlistURIUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->APACHE_ID, 12 ) );
        configs += "\nApacheWarnlistClient=" + this->list2string( this->craplog.getWarnlist( this->APACHE_ID, 20 ) );
        configs += "\nApacheWarnlistClientUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->APACHE_ID, 20 ) );
        configs += "\nApacheWarnlistUserAgent=" + this->list2string( this->craplog.getWarnlist( this->APACHE_ID, 21 ), true );
        configs += "\nApacheWarnlistUserAgentUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->APACHE_ID, 21 ) );
        configs += "\nApacheBlacklistClient=" + this->list2string( this->craplog.getBlacklist( this->APACHE_ID, 20 ) );
        configs += "\nApacheBlacklistClientUsed=" + this->b2s.at( this->craplog.isBlacklistUsed( this->APACHE_ID, 20 ) );
        //// NGINX ////
        configs += "\n\n[Nginx]";
        configs += "\nNginxLogsPath=" + this->craplog.getLogsPath( this->NGINX_ID );
        configs += "\nNginxLogsFormat=" + this->craplog.getLogsFormatString( this->NGINX_ID );
        configs += "\nNginxWarnlistMethod=" + this->list2string( this->craplog.getWarnlist( this->NGINX_ID, 11 ) );
        configs += "\nNginxWarnlistMethodUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->NGINX_ID, 11 ) );
        configs += "\nNginxWarnlistURI=" + this->list2string( this->craplog.getWarnlist( this->NGINX_ID, 12 ) );
        configs += "\nNginxWarnlistURIUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->NGINX_ID, 12 ) );
        configs += "\nNginxWarnlistClient=" + this->list2string( this->craplog.getWarnlist( this->NGINX_ID, 20 ) );
        configs += "\nNginxWarnlistClientUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->NGINX_ID, 20 ) );
        configs += "\nNginxWarnlistUserAgent=" + this->list2string( this->craplog.getWarnlist( this->NGINX_ID, 21 ), true );
        configs += "\nNginxWarnlistUserAgentUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->NGINX_ID, 21 ) );
        configs += "\nNginxBlacklistClient=" + this->list2string( this->craplog.getBlacklist( this->NGINX_ID, 20 ) );
        configs += "\nNginxBlacklistClientUsed=" + this->b2s.at( this->craplog.isBlacklistUsed( this->NGINX_ID, 20 ) );
        //// IIS ////
        configs += "\n\n[IIS]";
        configs += "\nIisLogsPath=" + this->craplog.getLogsPath( this->IIS_ID );
        std::string module = "0";
        if ( this->ui->radio_ConfIis_Format_NCSA->isChecked() ) {
            module = "1";
        } else if ( this->ui->radio_ConfIis_Format_IIS->isChecked() ) {
            module = "2";
        }
        configs += "\nIisLogsModule=" + module;
        configs += "\nIisLogsFormat=" + this->craplog.getLogsFormatString( this->IIS_ID );
        configs += "\nIisWarnlistMethod=" + this->list2string( this->craplog.getWarnlist( this->IIS_ID, 11 ) );
        configs += "\nIisWarnlistMethodUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->IIS_ID, 11 ) );
        configs += "\nIisWarnlistURI=" + this->list2string( this->craplog.getWarnlist( this->IIS_ID, 12 ) );
        configs += "\nIisWarnlistURIUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->IIS_ID, 12 ) );
        configs += "\nIisWarnlistClient=" + this->list2string( this->craplog.getWarnlist( this->IIS_ID, 20 ) );
        configs += "\nIisWarnlistClientUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->IIS_ID, 20 ) );
        configs += "\nIisWarnlistUserAgent=" + this->list2string( this->craplog.getWarnlist( this->IIS_ID, 21 ), true );
        configs += "\nIisWarnlistUserAgentUsed=" + this->b2s.at( this->craplog.isWarnlistUsed( this->IIS_ID, 21 ) );
        configs += "\nIisBlacklistClient=" + this->list2string( this->craplog.getBlacklist( this->IIS_ID, 20 ) );
        configs += "\nIisBlacklistClientUsed=" + this->b2s.at( this->craplog.isBlacklistUsed( this->IIS_ID, 20 ) );
        //// CRAPVIEW ////
        configs += "\n\n[Crapview]";
        configs += "\nCrapviewDialogLevel=" + std::to_string( this->crapview.getDialogsLevel() );

        // write on file
        try {
            IOutils::writeOnFile( this->configs_path, configs );

        } catch ( const std::ios_base::failure&/* err*/ ) {
            // failed writing
            DialogSec::errGeneric( DialogSec::tr("An error occured while writing the configuration file") );
        } catch (...) {
            // something failed
            DialogSec::errGeneric( DialogSec::tr("An error occured while preparing the configuration file's data") );
        }
    }
}


void MainWindow::backupDatabase()
{
    bool proceed = true;
    std::error_code err;
    QString err_msg = "";
    if ( IOutils::checkFile( this->db_data_path+"/collection.db" ) ) {
        // db exists and is a file
        const std::string path = this->db_data_path+"/backups";
        if ( std::filesystem::exists( path ) ) {
            if ( !std::filesystem::is_directory( path, err ) ) {
                // exists but it's not a directory, rename as copy and make a new one
                proceed = DialogSec::choiceDirNotDir( QString::fromStdString( path ) );
                if ( proceed ) {
                    proceed = IOutils::renameAsCopy( path, err );
                    if ( ! proceed ) {
                        QString p = "";
                        if ( this->dialogs_level > 0 ) {
                            p = QString::fromStdString( path );
                            if ( err.value() ) {
                                err_msg = QString::fromStdString( err.message() );
                            }
                        }
                        DialogSec::errRenaming( p, err_msg );
                    } else {
                        // sucesfully renamed, make the new one
                        proceed = IOutils::makeDir( path, err );
                        if ( ! proceed ) {
                            QString msg = DialogSec::tr("Failed to create the database backups' directory");
                            if ( this->dialogs_level > 0 ) {
                                msg += ":\n"+QString::fromStdString( path );
                                if ( err.value() ) {
                                    err_msg = QString::fromStdString( err.message() );
                                }
                            }
                            DialogSec::errFailedMakeDir( msg, err_msg );
                        }
                    }
                }
            }
        } else {
            // backups directory doesn't exists, make it
            proceed = IOutils::makeDir( path, err );
            if ( ! proceed ) {
                QString msg = DialogSec::tr("Failed to create the database backups' directory");
                if ( this->dialogs_level > 0 ) {
                    msg += ":\n"+QString::fromStdString( path );
                    if ( err.value() ) {
                        err_msg = QString::fromStdString( err.message() );
                    }
                }
                DialogSec::errFailedMakeDir( msg, err_msg );
            }
        }

    }/* else {
        // db doesn't exists or is not a file
    }*/

    if ( proceed ) {
        // copy the database to a new file
        proceed = std::filesystem::copy_file(
            this->db_data_path+"/collection.db",
            this->db_data_path+"/backups/collection.db.0",
            std::filesystem::copy_options::update_existing,
            err );
        if ( ! proceed ) {
            // failed to copy
            if ( err.value() ) {
                err_msg = QString::fromStdString( err.message() );
            }
            DialogSec::errDatabaseFailedBackup( DialogSec::tr( "Failed to copy the database file" ), err_msg );
        } else {
            // succesfully copied, now rename the already existing copies (up to the choosen number of copies)
            std::string path, new_path;
            path = this->db_data_path+"/backups/collection.db."+std::to_string(this->db_backups_number);
            if ( std::filesystem::exists( path ) ) {
                std::ignore = std::filesystem::remove_all( path, err );
                if ( err.value() ) {
                    err_msg = QString::fromStdString( err.message() );
                    proceed = false;
                } else {
                    proceed = ! std::filesystem::exists( path );
                }
                if ( ! proceed ) {
                    DialogSec::errDatabaseFailedBackup( DialogSec::tr( "Failed to update the backups" ), err_msg );
                }
            }
            if ( proceed ) {
                // cascade rename
                for ( int n=this->db_backups_number-1; n>=0; n-- ) {
                    path = this->db_data_path+"/backups/collection.db."+std::to_string( n );
                    if ( std::filesystem::exists( path ) ) {
                        new_path = this->db_data_path+"/backups/collection.db."+std::to_string( n+1 );
                        std::filesystem::rename( path, new_path, err );
                        if ( err.value() ) {
                            err_msg = QString::fromStdString( err.message() );
                            proceed = false;
                        } else {
                            proceed = ! std::filesystem::exists( path );
                        }
                    }
                    if ( ! proceed ) {
                        // seems it failed to rename
                        DialogSec::errDatabaseFailedBackup( DialogSec::tr( "Failed to update the backups" ), err_msg );
                        break;
                    }
                }
            }
        }
    }
}


const std::string MainWindow::geometryToString()
{
    QRect geometry = this->geometry();
    std::string string = "";
    string += std::to_string( geometry.x() );
    string += ",";
    string += std::to_string( geometry.y() );
    string += ",";
    string += std::to_string( geometry.width() );
    string += ",";
    string += std::to_string( geometry.height() );
    string += ",";
    string += this->b2s.at( this->isMaximized() );
    return string;
}
void MainWindow::geometryFromString( const std::string& geometry )
{
    std::vector<std::string> aux;
    StringOps::splitrip( aux, geometry, "," );
    QRect new_geometry;
    new_geometry.setRect( std::stoi(aux.at(0)), std::stoi(aux.at(1)), std::stoi(aux.at(2)), std::stoi(aux.at(3)) );
    this->setGeometry( new_geometry );
    if ( aux.at(4) == "true" ) {
        this->showMaximized();
    }
}


const std::string MainWindow::list2string( const std::vector<std::string>& list, const bool& user_agent )
{
    std::string string;
    if ( user_agent ) {
        for ( const std::string& str : list ) {
            string += StringOps::replace( str, " ", "%@#" ) + " ";
        }
    } else {
        for ( const std::string& str : list ) {
            string += str + " ";
        }
    }
    return string;
}
const std::vector<std::string> MainWindow::string2list( const std::string& string, const bool& user_agent )
{
    std::vector<std::string> list, aux;
    StringOps::splitrip( aux, string, " " );
    if ( user_agent ) {
        for ( const std::string& str : aux ) {
            list.push_back( StringOps::replace( str, "%@#", " " ) );
        }
    } else {
        for ( const std::string& str : aux ) {
            list.push_back( str );
        }
    }
    return list;
}


//////////////////
//// GRAPHICS ////
//////////////////
void MainWindow::detectIconsTheme()
{
    switch ( this->window_theme_id ) {
        case 0:
            // system default, use window color to determine the theme
            if ( this->palette().window().color().black() > 127 ) {
                this->icons_theme = "light";
            } else {
                this->icons_theme = "dark";
            }
            break;
        case 1:
        case 3:
            // ash / herb
            this->icons_theme = "light";
            break;
        case 2:
        case 4:
            // candy / powder
            this->icons_theme = "dark";
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(this->window_theme_id), true );
            break;
    }
}


void MainWindow::updateUiTheme()
{
    // window and fonts
    switch ( this->window_theme_id ) {
        case 0:
            // window first
            this->setStyleSheet("");
            // icons last
            this->updateUiIcons();
            break;
        case 1: case 2: case 3: case 4:
            {
            // icons first
            this->updateUiIcons();
            // window last
            QString ss;
            StyleSec::getStyleSheet( ss, this->icons_theme, this->window_theme_id );
            this->setStyleSheet( ss );
            break;
            }
        default:
            // wrong
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(this->window_theme_id), true );
            break;
    }
    // fonts
    this->updateUiFonts();
}

void MainWindow::updateUiIcons()
{
    const QString old_icons_theme = this->icons_theme;
    switch ( this->icons_theme_id ) {
        case 0:
            this->detectIconsTheme();
            break;
        case 1:
            this->icons_theme = "light";
            break;
        case 2:
            this->icons_theme = "dark";
            break;
        default:
            throw GenericException( "Unexpected IconSet index: "+std::to_string(this->icons_theme_id), true );
            break;
    }

    if ( this->icons_theme != old_icons_theme ) {
        // main tabs
        const int m_index = this->ui->stacked_Tabs_Pages->currentIndex();
        this->ui->button_Tab_Log->setIcon(
            QIcon(QString(":/icons/icons/%1/log_%2.png").arg(
                this->icons_theme,
                (m_index==0) ? "on" : "off" )) );
        this->ui->button_Tab_View->setIcon(
            QIcon(QString(":/icons/icons/%1/view_%2.png").arg(
                this->icons_theme,
                (m_index==1) ? "on" : "off" )) );
        this->ui->button_Tab_Conf->setIcon(
            QIcon(QString(":/icons/icons/%1/conf_%2.png").arg(
                this->icons_theme,
                (m_index==2) ? "on" : "off" )) );
        // view logs
        this->ui->button_LogFiles_ViewFile->setIcon(
            QIcon(QString(":/icons/icons/%1/show_file.png").arg(this->icons_theme)) );
        this->ui->button_LogFiles_RefreshList->setIcon(
            QIcon(QString(":/icons/icons/%1/refresh.png").arg(this->icons_theme)) );
        // parse logs
        this->ui->icon_MakeStats_Size->setPixmap(
            QPixmap(QString(":/icons/icons/%1/mk_size.png").arg(this->icons_theme)) );
        this->ui->icon_MakeStats_Lines->setPixmap(
            QPixmap(QString(":/icons/icons/%1/mk_lines.png").arg(this->icons_theme)) );
        this->ui->icon_MakeStats_Time->setPixmap(
            QPixmap(QString(":/icons/icons/%1/mk_time.png").arg(this->icons_theme)) );
        this->ui->icon_MakeStats_Speed->setPixmap(
            QPixmap(QString(":/icons/icons/%1/mk_speed.png").arg(this->icons_theme)) );
        // stats
        const int s_index = this->ui->stacked_Stats_Pages->currentIndex();
        // stats warn
        this->ui->button_Tab_StatsWarn->setIcon(
            QIcon(QString(":/icons/icons/%1/warn_%2.png").arg(
                this->icons_theme,
                (s_index==0) ? "on" : "off" )) );
        this->ui->button_StatsWarn_Draw->setIcon(
            QIcon(QString(":/icons/icons/%1/draw.png").arg(this->icons_theme)) );
        this->ui->button_StatsWarn_Update->setIcon(
            QIcon(QString(":/icons/icons/%1/save.png").arg(this->icons_theme)) );
        // stats speed
        this->ui->button_Tab_StatsSpeed->setIcon(
            QIcon(QString(":/icons/icons/%1/speed_%2.png").arg(
                this->icons_theme,
                (s_index==1) ? "on" : "off" )) );
        this->ui->button_StatsSpeed_Draw->setIcon(
            QIcon(QString(":/icons/icons/%1/draw.png").arg(this->icons_theme)) );
        // stats count
        this->ui->button_Tab_StatsCount->setIcon(
            QIcon(QString(":/icons/icons/%1/count_%2.png").arg(
                this->icons_theme,
                (s_index==2) ? "on" : "off" )) );
        // stats daytime
        this->ui->button_Tab_StatsDay->setIcon(
            QIcon(QString(":/icons/icons/%1/daytime_%2.png").arg(
                this->icons_theme,
                (s_index==3) ? "on" : "off" )) );
        this->ui->button_StatsDay_Draw->setIcon(
            QIcon(QString(":/icons/icons/%1/draw.png").arg(this->icons_theme)) );
        // stats relational
        this->ui->button_Tab_StatsRelat->setIcon(
            QIcon(QString(":/icons/icons/%1/relational_%2.png").arg(
                this->icons_theme,
                (s_index==4) ? "on" : "off" )) );
        this->ui->button_StatsRelat_Draw->setIcon(
            QIcon(QString(":/icons/icons/%1/draw.png").arg(this->icons_theme)) );
        // stats globals
        this->ui->button_Tab_StatsGlob->setIcon(
            QIcon(QString(":/icons/icons/%1/global_%2.png").arg(
                this->icons_theme,
                (s_index==5) ? "on" : "off" )) );
        // configs
        this->ui->button_ConfDatabases_Data_Save->setIcon(
            QIcon(QString(":/icons/icons/%1/save.png").arg(this->icons_theme)) );
        this->ui->button_ConfDatabases_Hashes_Save->setIcon(
            QIcon(QString(":/icons/icons/%1/save.png").arg(this->icons_theme)) );
        this->ui->button_ConfApache_Path_Save->setIcon(
            QIcon(QString(":/icons/icons/%1/save.png").arg(this->icons_theme)) );
        this->ui->button_ConfApache_Format_Save->setIcon(
            QIcon(QString(":/icons/icons/%1/save.png").arg(this->icons_theme)) );
        this->ui->button_ConfApache_Format_Help->setIcon(
            QIcon(QString(":/icons/icons/%1/help.png").arg(this->icons_theme)) );
        this->ui->button_ConfNginx_Path_Save->setIcon(
            QIcon(QString(":/icons/icons/%1/save.png").arg(this->icons_theme)) );
        this->ui->button_ConfNginx_Format_Save->setIcon(
            QIcon(QString(":/icons/icons/%1/save.png").arg(this->icons_theme)) );
        this->ui->button_ConfNginx_Format_Help->setIcon(
            QIcon(QString(":/icons/icons/%1/help.png").arg(this->icons_theme)) );
        this->ui->button_ConfIis_Path_Save->setIcon(
            QIcon(QString(":/icons/icons/%1/save.png").arg(this->icons_theme)) );
        this->ui->button_ConfIis_Format_Save->setIcon(
            QIcon(QString(":/icons/icons/%1/save.png").arg(this->icons_theme)) );
        this->ui->button_ConfIis_Format_Help->setIcon(
            QIcon(QString(":/icons/icons/%1/help.png").arg(this->icons_theme)) );
    }
}

void MainWindow::updateUiFonts()
{
    const QFont &small_font = this->FONTS.at( "main_small" );
    const QFont &font = this->FONTS.at( "main" );
    const QFont &big_font = this->FONTS.at( "main_big" );
    QFont menu_font = this->FONTS.at( "main_small" );
    menu_font.setPointSizeF( this->font_size_small+1.5 );
    QFont header_font = this->FONTS.at( "main_small" );
    header_font.setPointSizeF( this->font_size_small+2 );
    // menu
    this->ui->menuLanguage->setFont( menu_font );
    this->ui->actionEnglish->setFont( menu_font );
    this->ui->actionEspanol->setFont( menu_font );
    this->ui->actionFrancais->setFont( menu_font );
    this->ui->actionItaliano->setFont( menu_font );
    this->ui->menuTools->setFont( menu_font );
    this->ui->actionBlockNote->setFont( menu_font );
    this->ui->menuUtilities->setFont( menu_font );
    this->ui->actionInfos->setFont( menu_font );
    this->ui->actionCheckUpdates->setFont( menu_font );
    this->ui->menuGames->setFont( menu_font );
    this->ui->actionCrissCross->setFont( menu_font );
    this->ui->actionSnake->setFont( menu_font );
    // log files view
    this->ui->button_LogFiles_Apache->setFont( font );
    this->ui->button_LogFiles_Nginx->setFont( font );
    this->ui->button_LogFiles_Iis->setFont( font );
    this->ui->checkBox_LogFiles_CheckAll->setFont( small_font );
    this->ui->listLogFiles->setFont( font );
    this->ui->listLogFiles->headerItem()->setFont( 0, header_font );
    this->ui->listLogFiles->headerItem()->setFont( 1, header_font );
    this->ui->textLogFiles->setFont( this->TB.getFont() );
    // log files parse
    this->ui->label_MakeStats_Size->setFont( font );
    this->ui->label_MakeStats_Lines->setFont( font );
    this->ui->label_MakeStats_Time->setFont( font );
    this->ui->label_MakeStats_Speed->setFont( font );
    this->ui->button_MakeStats_Start->setFont( big_font );
    // stats warn
    this->ui->label_StatsWarn_WebServer->setFont( font );
    this->ui->box_StatsWarn_WebServer->setFont( font );
    this->ui->label_StatsWarn_Year->setFont( font );
    this->ui->box_StatsWarn_Year->setFont( font );
    this->ui->label_StatsWarn_Month->setFont( font );
    this->ui->box_StatsWarn_Month->setFont( font );
    this->ui->label_StatsWarn_Day->setFont( font );
    this->ui->box_StatsWarn_Day->setFont( font );
    this->ui->checkBox_StatsWarn_Hour->setFont( font );
    this->ui->box_StatsWarn_Hour->setFont( font );
    this->ui->table_StatsWarn->setFont( font );
    this->ui->table_StatsWarn->horizontalHeader()->setFont( header_font );
    // stats speed
    this->ui->box_StatsSpeed_WebServer->setFont( font );
    this->ui->label_StatsSpeed_Year->setFont( font );
    this->ui->box_StatsSpeed_Year->setFont( font );
    this->ui->label_StatsSpeed_Month->setFont( font );
    this->ui->box_StatsSpeed_Month->setFont( font );
    this->ui->label_StatsSpeed_Day->setFont( font );
    this->ui->box_StatsSpeed_Day->setFont( font );
    this->ui->label_StatsSpeed_Filters->setFont( big_font );
    this->ui->label_StatsSpeed_Protocol->setFont( font );
    this->ui->inLine_StatsSpeed_Protocol->setFont( font );
    this->ui->label_StatsSpeed_Method->setFont( font );
    this->ui->inLine_StatsSpeed_Method->setFont( font );
    this->ui->label_StatsSpeed_Uri->setFont( font );
    this->ui->inLine_StatsSpeed_Uri->setFont( font );
    this->ui->label_StatsSpeed_Query->setFont( font );
    this->ui->inLine_StatsSpeed_Query->setFont( font );
    this->ui->label_StatsSpeed_Response->setFont( font );
    this->ui->inLine_StatsSpeed_Response->setFont( font );
    this->ui->table_StatsSpeed->setFont( font );
    this->ui->table_StatsSpeed->horizontalHeader()->setFont( header_font );
    // stats count
    this->ui->box_StatsCount_WebServer->setFont( font );
    this->ui->label_StatsCount_Year->setFont( font );
    this->ui->box_StatsCount_Year->setFont( font );
    this->ui->label_StatsCount_Month->setFont( font );
    this->ui->box_StatsCount_Month->setFont( font );
    this->ui->label_StatsCount_Day->setFont( font );
    this->ui->box_StatsCount_Day->setFont( font );
    this->ui->button_StatsCount_Protocol->setFont( font );
    this->ui->button_StatsCount_Method->setFont( font );
    this->ui->button_StatsCount_Uri->setFont( font );
    this->ui->button_StatsCount_Query->setFont( font );
    this->ui->button_StatsCount_Response->setFont( font );
    this->ui->button_StatsCount_Referrer->setFont( font );
    this->ui->button_StatsCount_Cookie->setFont( font );
    this->ui->button_StatsCount_UserAgent->setFont( font );
    this->ui->button_StatsCount_Client->setFont( font );
    this->ui->table_StatsCount->setFont( font );
    this->ui->table_StatsCount->horizontalHeader()->setFont( header_font );
    // stats day
    this->ui->box_StatsDay_WebServer->setFont( font );
    this->ui->label_StatsDay_From->setFont( font );
    this->ui->checkBox_StatsDay_Period->setFont( font );
    this->ui->label_StatsDay_Year->setFont( font );
    this->ui->box_StatsDay_FromYear->setFont( font );
    this->ui->box_StatsDay_ToYear->setFont( font );
    this->ui->label_StatsDay_Month->setFont( font );
    this->ui->box_StatsDay_FromMonth->setFont( font );
    this->ui->box_StatsDay_ToMonth->setFont( font );
    this->ui->label_StatsDay_Day->setFont( font );
    this->ui->box_StatsDay_FromDay->setFont( font );
    this->ui->box_StatsDay_ToDay->setFont( font );
    this->ui->label_StatsDay_LogsField->setFont( font );
    this->ui->box_StatsDay_LogsField->setFont( font );
    this->ui->label_StatsDay_Filter->setFont( font );
    this->ui->inLine_StatsDay_Filter->setFont( font );
    // stats relat
    this->ui->box_StatsRelat_WebServer->setFont( font );
    this->ui->label_StatsRelat_From->setFont( font );
    this->ui->label_StatsRelat_To->setFont( font );
    this->ui->box_StatsRelat_FromYear->setFont( font );
    this->ui->box_StatsRelat_ToYear->setFont( font );
    this->ui->box_StatsRelat_FromMonth->setFont( font );
    this->ui->box_StatsRelat_ToMonth->setFont( font );
    this->ui->box_StatsRelat_FromDay->setFont( font );
    this->ui->box_StatsRelat_ToDay->setFont( font );
    this->ui->label_StatsRelat_LogsField_1->setFont( font );
    this->ui->box_StatsRelat_LogsField_1->setFont( font );
    this->ui->label_StatsRelat_Filter_1->setFont( font );
    this->ui->inLine_StatsRelat_Filter_1->setFont( font );
    this->ui->label_StatsRelat_LogsField_2->setFont( font );
    this->ui->box_StatsRelat_LogsField_2->setFont( font );
    this->ui->label_StatsRelat_Filter_2->setFont( font );
    this->ui->inLine_StatsRelat_Filter_2->setFont( font );
    // stats glob
    this->ui->button_StatsGlob_Apache->setFont( font );
    this->ui->button_StatsGlob_Nginx->setFont( font );
    this->ui->button_StatsGlob_Iis->setFont( font );
    this->ui->label_StatsGlob_Recur->setFont( big_font );
    this->ui->label_StatsGlob_Recur_Protocol->setFont( font );
    this->ui->label_StatsGlob_Recur_Protocol_String->setFont( font );
    this->ui->label_StatsGlob_Recur_Protocol_Count->setFont( font );
    this->ui->label_StatsGlob_Recur_Method->setFont( font );
    this->ui->label_StatsGlob_Recur_Method_String->setFont( font );
    this->ui->label_StatsGlob_Recur_Method_Count->setFont( font );
    this->ui->label_StatsGlob_Recur_URI->setFont( font );
    this->ui->label_StatsGlob_Recur_URI_String->setFont( font );
    this->ui->label_StatsGlob_Recur_URI_Count->setFont( font );
    this->ui->label_StatsGlob_Recur_UserAgent->setFont( font );
    this->ui->label_StatsGlob_Recur_UserAgent_String->setFont( font );
    this->ui->label_StatsGlob_Recur_UserAgent_Count->setFont( font );
    this->ui->label_StatsGlob_Perf->setFont( big_font );
    this->ui->label_StatsGlob_Perf_Time->setFont( font );
    this->ui->label_StatsGlob_Perf_Time_Mean->setFont( font );
    this->ui->label_StatsGlob_Perf_Time_Max->setFont( font );
    this->ui->label_StatsGlob_Perf_Sent->setFont( font );
    this->ui->label_StatsGlob_Perf_Sent_Mean->setFont( font );
    this->ui->label_StatsGlob_Perf_Sent_Max->setFont( font );
    this->ui->label_StatsGlob_Perf_Received->setFont( font );
    this->ui->label_StatsGlob_Perf_Received_Mean->setFont( font );
    this->ui->label_StatsGlob_Perf_Received_Max->setFont( font );
    this->ui->label_StatsGlob_Traffic->setFont( big_font );
    this->ui->label_StatsGlob_Traffic_Date->setFont( font );
    this->ui->label_StatsGlob_Traffic_Date_String->setFont( font );
    this->ui->label_StatsGlob_Traffic_Date_Count->setFont( font );
    this->ui->label_StatsGlob_Traffic_Day->setFont( font );
    this->ui->label_StatsGlob_Traffic_Day_String->setFont( font );
    this->ui->label_StatsGlob_Traffic_Day_Count->setFont( font );
    this->ui->label_StatsGlob_Traffic_Hour->setFont( font );
    this->ui->label_StatsGlob_Traffic_Hour_String->setFont( font );
    this->ui->label_StatsGlob_Traffic_Hour_Count->setFont( font );
    this->ui->label_StatsGlob_Work->setFont( big_font );
    this->ui->label_StatsGlob_Work_Req->setFont( font );
    this->ui->label_StatsGlob_Work_Req_Count->setFont( font );
    this->ui->label_StatsGlob_Work_Time->setFont( font );
    this->ui->label_StatsGlob_Work_Time_Count->setFont( font );
    this->ui->label_StatsGlob_Work_Sent->setFont( font );
    this->ui->label_StatsGlob_Work_Sent_Count->setFont( font );
    // configs tab
    this->ui->ConfTabs->tabBar()->setFont( big_font );
    this->ui->tabs_ConfGeneral->tabBar()->setFont( font );
    this->ui->tabs_ConfLogs->tabBar()->setFont( font );
    this->ui->tabs_ConfApache->tabBar()->setFont( font );
    this->ui->tabs_ConfNginx->tabBar()->setFont( font );
    this->ui->tabs_ConfIis->tabBar()->setFont( font );
    // conf window
    this->ui->label_ConfWindow_Geometry->setFont( big_font );
    this->ui->checkBox_ConfWindow_Geometry->setFont( font );
    this->ui->label_ConfWindow_Theme->setFont( big_font );
    this->ui->box_ConfWindow_Theme->setFont( font );
    this->ui->label_ConfWindow_Icons->setFont( big_font );
    this->ui->box_ConfWindow_Icons->setFont( font );
    // conf dialogs
    this->ui->label_ConfDialogs_Level->setFont( big_font );
    this->ui->label_ConfDialogs_General->setFont( font );
    this->ui->label_ConfDialogs_Logs->setFont( font );
    this->ui->label_ConfDialogs_Stats->setFont( font );
    this->ui->label_ConfDialogs_Essential->setFont( small_font );
    this->ui->label_ConfDialogs_Normal->setFont( small_font );
    this->ui->label_ConfDialogs_Explanatory->setFont( small_font );
    // conf text-browser
    this->ui->label_ConfTextBrowser_Font->setFont( big_font );
    this->ui->box_ConfTextBrowser_Font->setFont( font );
    this->ui->label_ConfTextBrowser_Lines->setFont( big_font );
    this->ui->checkBox_ConfTextBrowser_WideLines->setFont( font );
    this->ui->label_ConfTextBrowser_ColorScheme->setFont( big_font );
    this->ui->box_ConfTextBrowser_ColorScheme->setFont( font );
    this->ui->label_ConfTextBrowser_Preview->setFont( big_font );
    this->ui->textBrowser_ConfTextBrowser_Preview->setFont( this->TB.getFont() );
    // conf charts
    this->ui->label_ConfCharts_Theme->setFont( big_font );
    this->ui->box_ConfCharts_Theme->setFont( font );
    this->ui->label_ConfCharts_Preview->setFont( big_font );
    // conf databases
    this->ui->label_ConfDatabases_Paths->setFont( big_font );
    this->ui->label_ConfDatabases_Data->setFont( font );
    this->ui->inLine_ConfDatabases_Data_Path->setFont( font );
    this->ui->label_ConfDatabases_Hashes->setFont( font );
    this->ui->inLine_ConfDatabases_Hashes_Path->setFont( font );
    this->ui->label_ConfDatabases_Backups->setFont( big_font );
    this->ui->checkBox_ConfDatabases_DoBackup->setFont( font );
    this->ui->spinBox_ConfDatabases_NumBackups->setFont( font );
    // conf logs default
    this->ui->label_ConfDefaults_WebServer->setFont( big_font );
    this->ui->radio_ConfDefaults_Apache->setFont( font );
    this->ui->radio_ConfDefaults_Nginx->setFont( font );
    this->ui->radio_ConfDefaults_Iis->setFont( font );
    // conf logs control
    this->ui->label_ConfControl_Usage->setFont( big_font );
    this->ui->checkBox_ConfControl_Usage->setFont( font );
    this->ui->label_ConfControl_Size->setFont( big_font );
    this->ui->checkBox_ConfControl_Size->setFont( font );
    this->ui->spinBox_ConfControl_Size->setFont( font );
    // conf apache
    this->ui->label_ConfApache_Path_Path->setFont( font );
    this->ui->inLine_ConfApache_Path_String->setFont( font );
    this->ui->label_ConfApache_Format_String->setFont( font );
    this->ui->inLine_ConfApache_Format_String->setFont( font );
    this->ui->button_ConfApache_Format_Sample->setFont( font );
    this->ui->preview_ConfApache_Format_Sample->setFont( this->TB.getFont() );
    this->ui->box_ConfApache_Warnlist_Field->setFont( font );
    this->ui->checkBox_ConfApache_Warnlist_Used->setFont( font );
    this->ui->inLine_ConfApache_Warnlist_String->setFont( font );
    this->ui->list_ConfApache_Warnlist_List->setFont( font );
    this->ui->box_ConfApache_Blacklist_Field->setFont( font );
    this->ui->checkBox_ConfApache_Blacklist_Used->setFont( font );
    this->ui->inLine_ConfApache_Blacklist_String->setFont( font );
    this->ui->list_ConfApache_Blacklist_List->setFont( font );
    // conf nginx
    this->ui->label_ConfNginx_Path_Path->setFont( font );
    this->ui->inLine_ConfNginx_Path_String->setFont( font );
    this->ui->label_ConfNginx_Format_String->setFont( font );
    this->ui->inLine_ConfNginx_Format_String->setFont( font );
    this->ui->button_ConfNginx_Format_Sample->setFont( font );
    this->ui->preview_ConfNginx_Format_Sample->setFont( this->TB.getFont() );
    this->ui->box_ConfNginx_Warnlist_Field->setFont( font );
    this->ui->checkBox_ConfNginx_Warnlist_Used->setFont( font );
    this->ui->inLine_ConfNginx_Warnlist_String->setFont( font );
    this->ui->list_ConfNginx_Warnlist_List->setFont( font );
    this->ui->box_ConfNginx_Blacklist_Field->setFont( font );
    this->ui->checkBox_ConfNginx_Blacklist_Used->setFont( font );
    this->ui->inLine_ConfNginx_Blacklist_String->setFont( font );
    this->ui->list_ConfNginx_Blacklist_List->setFont( font );
    // conf iis
    this->ui->label_ConfIis_Path_Path->setFont( font );
    this->ui->inLine_ConfIis_Path_String->setFont( font );
    this->ui->label_ConfIis_Format_String->setFont( font );
    this->ui->inLine_ConfIis_Format_String->setFont( font );
    this->ui->button_ConfIis_Format_Sample->setFont( font );
    this->ui->preview_ConfIis_Format_Sample->setFont( this->TB.getFont() );
    this->ui->box_ConfIis_Warnlist_Field->setFont( font );
    this->ui->checkBox_ConfIis_Warnlist_Used->setFont( font );
    this->ui->inLine_ConfIis_Warnlist_String->setFont( font );
    this->ui->list_ConfIis_Warnlist_List->setFont( font );
    this->ui->box_ConfIis_Blacklist_Field->setFont( font );
    this->ui->checkBox_ConfIis_Blacklist_Used->setFont( font );
    this->ui->inLine_ConfIis_Blacklist_String->setFont( font );
    this->ui->list_ConfIis_Blacklist_List->setFont( font );
}




//////////////////
//// LANGUAGE ////
//////////////////
void MainWindow::updateUiLanguage()
{
    // remove the old translator
    QCoreApplication::removeTranslator( &this->translator );
    if ( this->translator.load( QString(":/translations/%1").arg(QString::fromStdString( this->language )) ) ) {
        // apply the new translator
        QCoreApplication::installTranslator( &this->translator );
        this->ui->retranslateUi( this );
        // stats warn table header
        {
            const QStringList h = {
                this->crapview.getLogFieldString(0),
                TR::tr( WORDS__DATE.c_str() ),
                TR::tr( WORDS__TIME.c_str() ),
                this->crapview.getLogFieldString(10),
                this->crapview.getLogFieldString(11),
                this->crapview.getLogFieldString(12),
                this->crapview.getLogFieldString(13),
                this->crapview.getLogFieldString(14),
                this->crapview.getLogFieldString(18),
                this->crapview.getLogFieldString(22),
                this->crapview.getLogFieldString(21),
                this->crapview.getLogFieldString(20),
                this->crapview.getLogFieldString(17),
                this->crapview.getLogFieldString(16),
                this->crapview.getLogFieldString(15),
                "rowid" };
            this->ui->table_StatsWarn->setColumnCount( h.size() );
            this->ui->table_StatsWarn->setHorizontalHeaderLabels( h );
        }
        // stats speed table header
        {
            const QStringList h = {
                this->crapview.getLogFieldString(15),
                this->crapview.getLogFieldString(12),
                this->crapview.getLogFieldString(13),
                this->crapview.getLogFieldString(11),
                this->crapview.getLogFieldString(10),
                this->crapview.getLogFieldString(14),
                TR::tr( WORDS__TIME.c_str() ) };
            this->ui->table_StatsSpeed->setColumnCount( h.size() );
            this->ui->table_StatsSpeed->setHorizontalHeaderLabels( h );
        }
        // stats count buttons
        this->ui->button_StatsCount_Protocol->setText(  this->crapview.getLogFieldString( 10 ) );
        this->ui->button_StatsCount_Method->setText(    this->crapview.getLogFieldString( 11 ) );
        this->ui->button_StatsCount_Uri->setText(       this->crapview.getLogFieldString( 12 ) );
        this->ui->button_StatsCount_Query->setText(     this->crapview.getLogFieldString( 13 ) );
        this->ui->button_StatsCount_Response->setText(  this->crapview.getLogFieldString( 14 ) );
        this->ui->button_StatsCount_Referrer->setText(  this->crapview.getLogFieldString( 18 ) );
        this->ui->button_StatsCount_Cookie->setText(    this->crapview.getLogFieldString( 22 ) );
        this->ui->button_StatsCount_UserAgent->setText( this->crapview.getLogFieldString( 21 ) );
        this->ui->button_StatsCount_Client->setText(    this->crapview.getLogFieldString( 20 ) );
        // configs warn/black-lists
        {
            const QStringList wl = {
                this->crapview.getLogFieldString( 11 ),
                this->crapview.getLogFieldString( 12 ),
                this->crapview.getLogFieldString( 21 ),
                this->crapview.getLogFieldString( 20 ) };
            const QStringList bl = {
                this->crapview.getLogFieldString( 20 ) };
            // set
            this->ui->box_ConfApache_Warnlist_Field->clear();
            this->ui->box_ConfApache_Warnlist_Field->addItems( wl );
            this->ui->box_ConfNginx_Warnlist_Field->clear();
            this->ui->box_ConfNginx_Warnlist_Field->addItems( wl );
            this->ui->box_ConfIis_Warnlist_Field->clear();
            this->ui->box_ConfIis_Warnlist_Field->addItems( wl );
            this->ui->box_ConfApache_Blacklist_Field->clear();
            this->ui->box_ConfApache_Blacklist_Field->addItems( bl );
            this->ui->box_ConfNginx_Blacklist_Field->clear();
            this->ui->box_ConfNginx_Blacklist_Field->addItems( bl );
            this->ui->box_ConfIis_Blacklist_Field->clear();
            this->ui->box_ConfIis_Blacklist_Field->addItems( bl );
        }
        // renew the dates
        this->on_box_StatsWarn_WebServer_currentIndexChanged(  this->ui->box_StatsWarn_WebServer->currentIndex()  );
        this->on_box_StatsSpeed_WebServer_currentIndexChanged( this->ui->box_StatsSpeed_WebServer->currentIndex() );
        this->on_box_StatsCount_WebServer_currentIndexChanged( this->ui->box_StatsCount_WebServer->currentIndex() );
        this->on_box_StatsDay_WebServer_currentIndexChanged(   this->ui->box_StatsDay_WebServer->currentIndex()   );
        this->on_box_StatsRelat_WebServer_currentIndexChanged( this->ui->box_StatsRelat_WebServer->currentIndex() );
    }
}


//////////////////////////
//// INTEGRITY CHECKS ////
//////////////////////////
void MainWindow::wait_ActiveWindow()
{
    if ( ! this->isActiveWindow() ) {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 250 );
    } else {
        this->waiter_timer->stop();
        this->makeInitialChecks();
    }
}
void MainWindow::makeInitialChecks()
{
    bool ok = true;
    std::error_code err;
    QString err_msg = "";
    // check that the sqlite plugin is available
    if ( ! QSqlDatabase::drivers().contains("QSQLITE") ) {
        // checks failed, abort
        DialogSec::errSqlDriverNotFound( "QSQLITE" );
        ok = false;
    }

    if ( ok ) {
        // check LogDoctor's folders paths
        for ( const std::string& path : std::vector<std::string>({this->basePath(this->configs_path), this->logdoc_path}) ) {
            if ( IOutils::exists( path ) ) {
                if ( IOutils::isDir( path ) ) {
                    if ( ! IOutils::checkDir( path, true ) ) {
                        // directory not readable, try to assign permissions
                        std::filesystem::permissions( path,
                                                      std::filesystem::perms::owner_read,
                                                      std::filesystem::perm_options::add,
                                                      err );
                        if ( err.value() ) {
                            ok = false;
                            err_msg = QString::fromStdString( err.message() );
                            DialogSec::errDirNotReadable( QString::fromStdString( path ), err_msg );
                        }
                    }
                    if ( ok ) {
                        if ( ! IOutils::checkDir( path, false, true ) ) {
                            // directory not writable, try to assign permissions
                            std::filesystem::permissions( path,
                                                          std::filesystem::perms::owner_write,
                                                          std::filesystem::perm_options::add,
                                                          err );
                            if ( err.value() ) {
                                ok = false;
                                err_msg = QString::fromStdString( err.message() );
                                DialogSec::errDirNotWritable( QString::fromStdString( path ), err_msg );
                            }
                        }
                    }

                } else {
                    // not a directory, rename as copy a make a new one
                    ok = DialogSec::choiceDirNotDir( QString::fromStdString( path ) );
                    if ( ok ) {
                        ok = IOutils::renameAsCopy( path, err );
                        if ( ! ok ) {
                            QString p = "";
                            if ( this->dialogs_level > 0 ) {
                                p = QString::fromStdString( path );
                                if ( err.value() ) {
                                    err_msg = QString::fromStdString( err.message() );
                                }
                            }
                            DialogSec::errRenaming( p, err_msg );
                        } else {
                            ok = IOutils::makeDir( path, err );
                            if ( ! ok ) {
                                QString msg = DialogSec::tr("Failed to create the directory");
                                if ( this->dialogs_level > 0 ) {
                                    msg += ":\n"+QString::fromStdString( path );
                                    if ( err.value() ) {
                                        err_msg = QString::fromStdString( err.message() );
                                    }
                                }
                                DialogSec::errFailedMakeDir( msg, err_msg );
                            }
                        }
                    }
                }

            } else {
                ok = IOutils::makeDir( path, err );
                if ( ! ok ) {
                    QString msg = DialogSec::tr("Failed to create the directory");
                    if ( this->dialogs_level > 0 ) {
                        msg += ":\n"+QString::fromStdString( path );
                        if ( err.value() ) {
                            err_msg = QString::fromStdString( err.message() );
                        }
                    }
                    DialogSec::errFailedMakeDir( msg, err_msg );
                }
            }
            if ( ! ok ) {
                break;
            }
        }
    }

    if ( ok ) {
        // statistics' database
        if ( ! CheckSec::checkCollectionDatabase( this->db_data_path + "/collection.db" ) ) {
            // checks failed, abort
            ok = false;
        } else {
            this->crapview.setDbPath( this->db_data_path );
            this->craplog.setStatsDatabasePath( this->db_data_path );
            // used-files' hashes' database
            if ( ! CheckSec::checkHashesDatabase( this->db_hashes_path + "/hashes.db" ) ) {
                // checks failed, abort
                ok = false;
            } else {
                this->craplog.setHashesDatabasePath( this->db_hashes_path );
                if ( ! this->craplog.hashOps.loadUsedHashesLists( this->db_hashes_path + "/hashes.db" ) ) {
                    // failed to load the list, abort
                    ok = false;
                }
            }
        }
    }
    if ( ! ok ) {
        this->close();
        //QCoreApplication::exit(0);
        //this->destroy();
    } else {
        // get available stats dates
        this->refreshStatsDates();
        // get a fresh list of log files
        this->on_button_LogFiles_RefreshList_clicked();
        // set the default WS as the current one
        switch ( this->craplog.getCurrentWSID() ) {
            case 11:
                this->ui->box_StatsWarn_WebServer->setCurrentIndex(  0 );
                this->ui->box_StatsCount_WebServer->setCurrentIndex( 0 );
                this->ui->box_StatsSpeed_WebServer->setCurrentIndex( 0 );
                this->ui->box_StatsDay_WebServer->setCurrentIndex(   0 );
                this->ui->box_StatsRelat_WebServer->setCurrentIndex( 0 );
                break;
            case 12:
                this->ui->box_StatsWarn_WebServer->setCurrentIndex(  1 );
                this->ui->box_StatsCount_WebServer->setCurrentIndex( 1 );
                this->ui->box_StatsSpeed_WebServer->setCurrentIndex( 1 );
                this->ui->box_StatsDay_WebServer->setCurrentIndex(   1 );
                this->ui->box_StatsRelat_WebServer->setCurrentIndex( 1 );
                break;
            case 13:
                this->ui->box_StatsWarn_WebServer->setCurrentIndex(  2 );
                this->ui->box_StatsCount_WebServer->setCurrentIndex( 2 );
                this->ui->box_StatsSpeed_WebServer->setCurrentIndex( 2 );
                this->ui->box_StatsDay_WebServer->setCurrentIndex(   2 );
                this->ui->box_StatsRelat_WebServer->setCurrentIndex( 2 );
                break;
            default:
                // shouldn't be here
                throw WebServerException( "Unexpected WebServer ID: "+std::to_string( this->default_ws ) );
        }
        this->initiating = false;
    }
}


const bool& MainWindow::checkDataDB()
{
    if ( ! this->initiating ) { // avoid recursions
        // check the db
        const std::string path = this->db_data_path + "/collection.db";
        bool ok = IOutils::checkFile( path, true );
        if ( ! ok ) {
            ok = CheckSec::checkCollectionDatabase( path );
            // update ui stuff
            if ( ! ok ) {
                // checks failed
                this->crapview.clearDates();
                this->ui->box_StatsWarn_Year->clear();
                this->ui->box_StatsSpeed_Year->clear();
                this->ui->box_StatsCount_Year->clear();
                this->ui->box_StatsDay_FromYear->clear();
                if ( this->ui->checkBox_StatsDay_Period->isChecked() ) {
                    this->ui->box_StatsDay_ToYear->clear();
                }
                this->ui->box_StatsRelat_FromYear->clear();
                this->ui->box_StatsRelat_ToYear->clear();
            }
        }
        if ( ok && !this->db_ok ) {
            this->db_ok = ok;
            this->initiating = true;
            this->refreshStatsDates();
            this->initiating = false;
        } else {
            this->db_ok = ok;
        }
    }
    return this->db_ok;
}


/////////////////////
//// GENERAL USE ////
/////////////////////
const QString MainWindow::wsFromIndex( const int& index )
{
    switch (index) {
    case 0:
        return QString("apache");
    case 1:
        return QString("nginx");
    case 2:
        return QString("iis");
    default:
        throw WebServerException( "Unexpected WebServer index: "+std::to_string( index ) );
    }
}

const std::string MainWindow::resolvePath( const std::string& path )
{
    std::string p;
    try {
        p = std::filesystem::canonical( StringOps::strip( path ) ).string();
    } catch (...) {
        ;
    }
    return p;
}
const std::string MainWindow::basePath( const std::string& path )
{
    const int stop = path.rfind( '/' );
    return path.substr( 0, stop );
}

// printable size with suffix and limited decimals
const QString MainWindow::printableSize( const int& bytes )
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
const QString MainWindow::printableSpeed( const int& bytes, const int& secs_ )
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

const QString MainWindow::printableTime( const int& secs_ )
{
    int secs = secs_;
    int mins = secs / 60;
    secs = secs - (mins*60);
    std::string mins_str = (mins<10) ? "0"+std::to_string(mins) : std::to_string(mins);
    std::string secs_str = (secs<10) ? "0"+std::to_string(secs) : std::to_string(secs);
    return QString::fromStdString( mins_str +":"+ secs_str );
}


//////////////
//// HELP ////
//////////////
void MainWindow::showHelp( const std::string& file_name )
{
    const std::string link = "https://github.com/elB4RTO/LogDoctor/tree/main/installation_stuff/logdocdata/help/";
    const std::string path =  this->logdoc_path+"/help/"+this->language+"/"+file_name+".html";
    if ( IOutils::exists( path ) ) {
        if ( IOutils::isFile( path ) ) {
            if ( IOutils::checkFile( path, true ) ) {
                // everything ok, delete the old help window and open a new one
                delete this->craphelp;
                this->craphelp = new Craphelp();
                this->craphelp->helpLogsFormat(
                    path,
                    this->TB.getFont(),
                    this->TB.getColorSchemeID() );
                if ( this->isMaximized() ) {
                    this->craphelp->showMaximized();
                } else {
                    this->craphelp->show();
                }
            } else {
                // resource not readable
                DialogSec::errHelpNotReadable( QString::fromStdString( link ) );
            }
        } else {
            // resource is not a file
            DialogSec::errHelpFailed( QString::fromStdString( link ), DialogSec::tr("unrecognized entry") );
        }
    } else {
        // resource not found
        DialogSec::errHelpNotFound( QString::fromStdString( link ) );
    }
}



/***************************************************************
 * MainWindow'S OPERATIONS START FROM HERE
 ***************************************************************/


//////////////
//// MENU ////
/// //////////
// switch language
void MainWindow::menu_actionEnglish_triggered()
{
    this->ui->actionEnglish->setChecked(   true );
    this->ui->actionEspanol->setChecked(  false );
    this->ui->actionFrancais->setChecked( false );
    this->ui->actionItaliano->setChecked( false );
    this->language = "en";
    this->updateUiLanguage();
}
void MainWindow::menu_actionEspanol_triggered()
{
    this->ui->actionEnglish->setChecked(  false );
    this->ui->actionEspanol->setChecked(   true );
    this->ui->actionFrancais->setChecked( false );
    this->ui->actionItaliano->setChecked( false );
    this->language = "es";
    this->updateUiLanguage();
}
void MainWindow::menu_actionFrancais_triggered()
{
    this->ui->actionEnglish->setChecked(  false );
    this->ui->actionEspanol->setChecked(  false );
    this->ui->actionFrancais->setChecked(  true );
    this->ui->actionItaliano->setChecked( false );
    this->language = "fr";
    this->updateUiLanguage();
}
void MainWindow::menu_actionItaliano_triggered()
{

    this->ui->actionEnglish->setChecked(  false );
    this->ui->actionEspanol->setChecked(  false );
    this->ui->actionFrancais->setChecked( false );
    this->ui->actionItaliano->setChecked(  true );
    this->language = "it";
    this->updateUiLanguage();
}

// use a tool
void MainWindow::menu_actionBlockNote_triggered()
{
    if ( this->crapnote->isVisible() ) {
        this->crapnote->activateWindow();

    } else {
        delete this->crapnote;
        this->crapnote = new Crapnote();
        this->crapnote->setTextFont( this->TB.getFont() );
        this->crapnote->setColorScheme( this->TB.getColorSchemeID() );
        this->crapnote->show();
    }
}

void MainWindow::menu_actionInfos_triggered()
{
    std::string version_ = std::to_string( this->version );
    size_t cut = version_.find('.');
    if ( cut == std::string::npos ) {
        cut = version_.find(',');
        if ( cut == std::string::npos ) {
            cut = version_.size()-3;
        }
    }
    version_ = version_.substr( 0, cut+3 );
    delete this->crapinfo;
    this->crapinfo = new Crapinfo(
        this->window_theme_id,
        QString::fromStdString( version_ ),
        QString::fromStdString( this->resolvePath( "./" ) ),
        QString::fromStdString( this->configs_path ),
        QString::fromStdString( this->logdoc_path ) );
    this->crapinfo->show();
}

void MainWindow::menu_actionCheckUpdates_triggered()
{
    if ( this->crapup->isVisible() ) {
        this->crapup->activateWindow();

    } else {
        delete this->crapup;
        this->crapup = new Crapup(
            this->window_theme_id,
            this->icons_theme );
        this->crapup->show();
        this->crapup->versionCheck( 1.0 );
    }
}

// play a game
void MainWindow::menu_actionCrissCross_triggered()
{
    if ( this->crisscross->isVisible() ) {
        this->crisscross->activateWindow();

    } else {
        delete this->crisscross;
        this->crisscross = new CrissCross( this->window_theme_id );
        this->crisscross->show();
    }
}

void MainWindow::menu_actionSnake_triggered()
{
    if ( this->snake->isVisible() ) {
        this->snake->activateWindow();

    } else {
        delete this->snake;
        this->snake = new SnakeGame( this->window_theme_id, this->FONTS.at("script") );
        this->snake->show();
    }
}



//////////////
//// TABS ////
//////////////
void MainWindow::switchMainTab( const int& new_index )
{
    const int old_index = this->ui->stacked_Tabs_Pages->currentIndex();
    // turn off the old icon
    switch ( old_index ) {
        case 0:
            // make
            this->ui->button_Tab_Log->setFlat( true );
            this->ui->button_Tab_Log->setIcon(
                QIcon(QString(":/icons/icons/%1/log_off.png").arg(this->icons_theme)) );
            break;
        case 1:
            // view
            this->ui->button_Tab_View->setFlat( true );
            this->ui->button_Tab_View->setIcon(
                QIcon(QString(":/icons/icons/%1/view_off.png").arg(this->icons_theme)) );
            break;
        case 2:
            // config
            this->ui->button_Tab_Conf->setFlat( true );
            this->ui->button_Tab_Conf->setIcon(
                QIcon(QString(":/icons/icons/%1/conf_off.png").arg(this->icons_theme)) );
            break;
        default:
            throw("Unexpected Tabs index: "+std::to_string(old_index));
            break;
    }
    // turn on the new one
    switch ( new_index ) {
        case 0:
            // make
            this->ui->button_Tab_Log->setFlat( false );
            this->ui->button_Tab_Log->setIcon(
                QIcon(QString(":/icons/icons/%1/log_on.png").arg(this->icons_theme)) );
            break;
        case 1:
            // view
            this->ui->button_Tab_View->setFlat( false );
            this->ui->button_Tab_View->setIcon(
                QIcon(QString(":/icons/icons/%1/view_on.png").arg(this->icons_theme)) );
            break;
        case 2:
            // config
            this->ui->button_Tab_Conf->setFlat( false );
            this->ui->button_Tab_Conf->setIcon(
                QIcon(QString(":/icons/icons/%1/conf_on.png").arg(this->icons_theme)) );
            break;
        default:
            throw("Unexpected MainTabs index: "+std::to_string(new_index));
            break;
    }
    this->ui->stacked_Tabs_Pages->setCurrentIndex( new_index );
}


void MainWindow::on_button_Tab_Log_clicked()
{
    this->switchMainTab( 0 );
}

void MainWindow::on_button_Tab_View_clicked()
{
    this->switchMainTab( 1 );
}

void MainWindow::on_button_Tab_Conf_clicked()
{
    this->switchMainTab( 2 );
}


//// STATS ////
void MainWindow::switchStatsTab( const int& new_index )
{
    const int old_index = this->ui->stacked_Stats_Pages->currentIndex();
    // turn off the old icon
    switch ( old_index ) {
        case 0:
            // warning
            this->ui->button_Tab_StatsWarn->setFlat( true );
            this->ui->button_Tab_StatsWarn->setIcon(
                QIcon(QString(":/icons/icons/%1/warn_off.png").arg(this->icons_theme)) );
            break;
        case 1:
            // speed
            this->ui->button_Tab_StatsSpeed->setFlat( true );
            this->ui->button_Tab_StatsSpeed->setIcon(
                QIcon(QString(":/icons/icons/%1/speed_off.png").arg(this->icons_theme)) );
            break;
        case 2:
            // counts
            this->ui->button_Tab_StatsCount->setFlat( true );
            this->ui->button_Tab_StatsCount->setIcon(
                QIcon(QString(":/icons/icons/%1/count_off.png").arg(this->icons_theme)) );
            break;
        case 3:
            // daytime
            this->ui->button_Tab_StatsDay->setFlat( true );
            this->ui->button_Tab_StatsDay->setIcon(
                QIcon(QString(":/icons/icons/%1/daytime_off.png").arg(this->icons_theme)) );
            break;
        case 4:
            // relational
            this->ui->button_Tab_StatsRelat->setFlat( true );
            this->ui->button_Tab_StatsRelat->setIcon(
                QIcon(QString(":/icons/icons/%1/relational_off.png").arg(this->icons_theme)) );
            break;
        case 5:
            // globals
            this->ui->button_Tab_StatsGlob->setFlat( true );
            this->ui->button_Tab_StatsGlob->setIcon(
                QIcon(QString(":/icons/icons/%1/global_off.png").arg(this->icons_theme)) );
            break;
        default:
            throw("Unexpected StatsTabs index: "+std::to_string(old_index));
            break;
    }
    // turn on the new one
    switch ( new_index ) {
        case 0:
            // warning
            this->ui->button_Tab_StatsWarn->setFlat( false );
            this->ui->button_Tab_StatsWarn->setIcon(
                QIcon(QString(":/icons/icons/%1/warn_on.png").arg(this->icons_theme)) );
            break;
        case 1:
            // speed
            this->ui->button_Tab_StatsSpeed->setFlat( false );
            this->ui->button_Tab_StatsSpeed->setIcon(
                QIcon(QString(":/icons/icons/%1/speed_on.png").arg(this->icons_theme)) );
            break;
        case 2:
            // counts
            this->ui->button_Tab_StatsCount->setFlat( false );
            this->ui->button_Tab_StatsCount->setIcon(
                QIcon(QString(":/icons/icons/%1/count_on.png").arg(this->icons_theme)) );
            break;
        case 3:
            // daytime
            this->ui->button_Tab_StatsDay->setFlat( false );
            this->ui->button_Tab_StatsDay->setIcon(
                QIcon(QString(":/icons/icons/%1/daytime_on.png").arg(this->icons_theme)) );
            break;
        case 4:
            // relational
            this->ui->button_Tab_StatsRelat->setFlat( false );
            this->ui->button_Tab_StatsRelat->setIcon(
                QIcon(QString(":/icons/icons/%1/relational_on.png").arg(this->icons_theme)) );
            break;
        case 5:
            // globals
            this->ui->button_Tab_StatsGlob->setFlat( false );
            this->ui->button_Tab_StatsGlob->setIcon(
                QIcon(QString(":/icons/icons/%1/global_on.png").arg(this->icons_theme)) );
            break;
        default:
            throw("Unexpected StatsTabs index: "+std::to_string(new_index));
            break;
    }
    this->ui->stacked_Stats_Pages->setCurrentIndex( new_index );
}


void MainWindow::on_button_Tab_StatsWarn_clicked()
{
    this->switchStatsTab( 0 );
}

void MainWindow::on_button_Tab_StatsSpeed_clicked()
{
    this->switchStatsTab( 1 );
}

void MainWindow::on_button_Tab_StatsCount_clicked()
{
    this->switchStatsTab( 2 );
}

void MainWindow::on_button_Tab_StatsDay_clicked()
{
    this->switchStatsTab( 3 );
}

void MainWindow::on_button_Tab_StatsRelat_clicked()
{
    this->switchStatsTab( 4 );
}

void MainWindow::on_button_Tab_StatsGlob_clicked()
{
    this->switchStatsTab( 5 );
}



////////////
//// DB ////
////////////
void MainWindow::setDbWorkingState( const bool& state )
{
    this->db_working = state;
    if ( ! state ) {
        this->checkMakeStats_Makable();
        if ( this->ui->table_StatsWarn->rowCount() > 0 ) {
            this->ui->button_StatsWarn_Update->setEnabled( true );
        }
        this->checkStatsWarnDrawable();
        this->checkStatsCountDrawable();
        this->checkStatsSpeedDrawable();
        this->checkStatsDayDrawable();
        this->checkStatsRelatDrawable();
        this->ui->button_StatsGlob_Apache->setEnabled( true );
        this->ui->button_StatsGlob_Nginx->setEnabled( true );
        this->ui->button_StatsGlob_Iis->setEnabled( true );
        this->ui->page_Tab_Conf->setEnabled( true );
    } else {
        this->ui->button_MakeStats_Start->setEnabled( false );
        this->ui->button_StatsWarn_Update->setEnabled( false );
        this->ui->button_StatsWarn_Draw->setEnabled( false );
        this->ui->scrollArea_StatsCount->setEnabled( false );
        this->ui->button_StatsSpeed_Draw->setEnabled( false );
        this->ui->button_StatsDay_Draw->setEnabled( false );
        this->ui->button_StatsRelat_Draw->setEnabled( false );
        this->ui->button_StatsGlob_Apache->setEnabled( false );
        this->ui->button_StatsGlob_Nginx->setEnabled( false );
        this->ui->button_StatsGlob_Iis->setEnabled( false );
        this->ui->page_Tab_Conf->setEnabled( false );
    }
}


//////////////
//// LOGS ////
//////////////
// switch pages
void MainWindow::on_button_Logs_Down_clicked()
{
    this->ui->stacked_Logs_Pages->setCurrentIndex( 1 );
}


void MainWindow::on_button_Logs_Up_clicked()
{
    this->ui->stacked_Logs_Pages->setCurrentIndex( 0 );
}


// check
void MainWindow::checkMakeStats_Makable()
{
    bool state = false;
    if ( ! this->db_working ) {
        // db is not busy
        if ( this->ui->checkBox_LogFiles_CheckAll->checkState() == Qt::CheckState::Checked ) {
            // all checked
            state = true;
        } else if ( this->ui->checkBox_LogFiles_CheckAll->checkState() == Qt::CheckState::PartiallyChecked ) {
            // at least one should be checked
            QTreeWidgetItemIterator i(this->ui->listLogFiles);
            while ( *i ) {
                if ( (*i)->checkState(0) == Qt::CheckState::Checked ) {
                    // an entry is checked
                    state = true;
                    break;
                }
                ++i;
            }
        }
    }
    this->ui->button_MakeStats_Start->setEnabled( state );
}


// switch to apache web server
void MainWindow::on_button_LogFiles_Apache_clicked()
{
    if ( this->craplog.getCurrentWSID() != 11 ) {
        // flat/unflat
        this->ui->button_LogFiles_Apache->setFlat( false );
        this->ui->button_LogFiles_Nginx->setFlat( true );
        this->ui->button_LogFiles_Iis->setFlat( true );
        // set the WebServer
        this->craplog.setCurrentWSID( 11 );
        // reset the log files viewer
        QString rich_text;
        RichText::richLogsDefault( rich_text );
        this->ui->textLogFiles->setText( rich_text );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        rich_text.clear();
        // load the list
        this->on_button_LogFiles_RefreshList_clicked();
    }
}
// switch to nginx web server
void MainWindow::on_button_LogFiles_Nginx_clicked()
{
    if ( this->craplog.getCurrentWSID() != 12 ) {
        // flat/unflat
        this->ui->button_LogFiles_Nginx->setFlat( false );
        this->ui->button_LogFiles_Apache->setFlat( true );
        this->ui->button_LogFiles_Iis->setFlat( true );
        // set the WebServer
        this->craplog.setCurrentWSID( 12 );
        // reset the log files viewer
        QString rich_text;
        RichText::richLogsDefault( rich_text );
        this->ui->textLogFiles->setText( rich_text );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        rich_text.clear();
        // load the list
        this->on_button_LogFiles_RefreshList_clicked();
    }
}
// switch to iis web server
void MainWindow::on_button_LogFiles_Iis_clicked()
{
    if ( this->craplog.getCurrentWSID() != 13 ) {
        // flat/unflat
        this->ui->button_LogFiles_Iis->setFlat( false );
        this->ui->button_LogFiles_Apache->setFlat( true );
        this->ui->button_LogFiles_Nginx->setFlat( true );
        // set the WebServer
        this->craplog.setCurrentWSID( 13 );
        // reset the log files viewer
        QString rich_text;
        RichText::richLogsDefault( rich_text );
        this->ui->textLogFiles->setText( rich_text );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        rich_text.clear();
        // load the list
        this->on_button_LogFiles_RefreshList_clicked();
    }
}

// refresh the log files list
void MainWindow::on_button_LogFiles_RefreshList_clicked()
{
    // clear the current tree
    this->ui->listLogFiles->clear();
    this->ui->checkBox_LogFiles_CheckAll->setCheckState( Qt::CheckState::Unchecked );
    // disable elements
    this->ui->button_LogFiles_RefreshList->setEnabled( false );
    this->ui->button_LogFiles_ViewFile->setEnabled( false );
    this->ui->button_LogFiles_Apache->setEnabled( false );
    this->ui->button_LogFiles_Nginx->setEnabled( false );
    this->ui->button_LogFiles_Iis->setEnabled( false );
    // start refreshing as thread
    this->refreshing_list = true;
    delete this->craplog_timer;
    this->craplog_timer = new QTimer(this);
    this->craplog_timer->setSingleShot( true );
    connect(this->craplog_timer, SIGNAL(timeout()), this, SLOT(refreshLogsList()));
    this->craplog_timer->start(250);
    // periodically check if thread finished
    delete this->waiter_timer;
    this->waiter_timer = new QTimer(this);
    connect(this->waiter_timer, SIGNAL(timeout()), this, SLOT(check_CraplogLLT_Finished()));
    this->waiter_timer->start(250);
}

void MainWindow::refreshLogsList()
{
    std::string col;
    // iterate over elements of list
    for ( const Craplog::LogFile& log_file : this->craplog.getLogsList(true) ) {
        // new entry for the tree widget
        QTreeWidgetItem *item = new QTreeWidgetItem();

        // preliminary check for file usage display
        if ( log_file.used_already ) {
            if ( this->hide_used_files ) {
                // do not display
                delete item; // possible memory leak
                continue;
            }
            // display with red foreground
            item->setForeground( 0, this->COLORS.at( "red" ) );
        }

        // preliminary check on file size
        col = "grey";
        if ( log_file.size > this->craplog.getWarningSize() ) {
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
    refreshing_list = false;
}
void MainWindow::check_CraplogLLT_Finished()
{
    if ( ! this->refreshing_list ) {
        this->waiter_timer->stop();
        // back to normal state
        this->ui->button_LogFiles_RefreshList->setEnabled( true );
        this->ui->button_LogFiles_ViewFile->setEnabled( true );
        this->ui->button_LogFiles_Apache->setEnabled( true );
        this->ui->button_LogFiles_Nginx->setEnabled( true );
        this->ui->button_LogFiles_Iis->setEnabled( true );
    }
}


void MainWindow::on_checkBox_LogFiles_CheckAll_stateChanged(int arg1)
{
    this->checkMakeStats_Makable();
    Qt::CheckState new_state;
    if ( arg1 == Qt::CheckState::Checked ) {
        // check all
        new_state = Qt::CheckState::Checked;
    } else if ( arg1 == Qt::CheckState::Unchecked ) {
        // un-check all
        new_state = Qt::CheckState::Unchecked;
    } else {
        // do nothing
        return;
    }
    QTreeWidgetItemIterator i( this->ui->listLogFiles );
    while ( *i ) {
        (*i)->setCheckState( 0, new_state );
        ++i;
    }
}


void MainWindow::on_button_LogFiles_ViewFile_clicked()
{
    // display the selected item
    if ( this->ui->listLogFiles->selectedItems().size() > 0 ) {
        bool proceed = true;
        Craplog::LogFile item;
        // retrieve the file item
        try {
            item = this->craplog.getLogFileItem(
                this->ui->listLogFiles->selectedItems().takeFirst()->text(0) );

        } catch ( const GenericException& ) {
            // failed to find file
            proceed = false;
            DialogSec::errFileNotFound( QString::fromStdString( item.path ), true );
        }

        // check the size
        if ( proceed ) {
            const long warn_size = this->craplog.getWarningSize();
            if ( warn_size >= 0 ) {
                if ( item.size > warn_size ) {
                    // exceeds the warning size
                    QString size_str, msg = item.name;
                    if ( this->dialogs_level >= 1 ) {
                        std::string size_sfx=" B";
                        float size = (float)item.size;
                        if (size > 1024) {
                            size /= 1024; size_sfx = " KiB";
                            if (size > 1024) {
                                size /= 1024; size_sfx = " MiB";
                            }
                        }
                        size_str = std::to_string(size).substr(0,std::to_string(size).size()-3).c_str();
                        msg += QString("\n\n%1:\n%2%3").arg( DialogSec::tr("Size of the file"), size_str, size_sfx.c_str() );
                        if ( this->dialogs_level == 2 ) {
                            size = (float)warn_size;
                            if (size > 1024) {
                                size /= 1024; size_sfx = " KiB";
                                if (size > 1024) {
                                    size /= 1024; size_sfx = " MiB";
                                }
                            }
                            size_str = std::to_string(size).substr(0,std::to_string(size).size()-3).c_str();
                            msg += QString("\n\n%1:\n%2%3").arg( DialogSec::tr("Warning size parameter"), size_str, size_sfx.c_str() );
                        }
                    }
                    // ask the user what to do
                    proceed = DialogSec::choiceFileSizeWarning2( msg );
                    if ( ! proceed ) {
                        return;
                    }
                }
            }
        }

        if ( proceed ) {
            // get the current log format
            FormatOps::LogsFormat format = this->craplog.getCurrentLogFormat();
            // read the content
            std::string content;
            try {
                try {
                    // try reading as gzip compressed file
                    GZutils::readFile( item.path, content );

                } catch ( const GenericException& ) {
                    // failed closing file pointer
                    throw;

                } catch (...) {
                    // failed as gzip, try as text file
                    if ( content.size() > 0 ) {
                        content.clear();
                    }
                    IOutils::readFile( item.path, content );
                }

            } catch ( const GenericException& ) {
                // failed closing gzip file pointer
                proceed = false;
                // >> e.what() << //
                DialogSec::errGeneric( QString("%1:\n%2").arg(
                    DialogSec::tr("Failed to read gzipped file"),
                    item.name) );

            /*} catch ( const std::ios_base::failure& err ) {
                // failed reading as text
                proceed = false;
                // >> err.what() << //
                DialogSec::errFailedReadFile( item.name );*/
            } catch (...) {
                // failed somehow
                proceed = false;
                DialogSec::errFailedReadFile( item.name );
            }

            if ( proceed ) {
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
        if ( ! proceed ) {
            // failed
            QString rich_text;
            RichText::richLogsFailure( rich_text );
            this->ui->textLogFiles->setText( rich_text );
            this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
            rich_text.clear();
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
    if ( ! this->db_working ) {
        bool proceed = true;
        // check that the format has been set
        const FormatOps::LogsFormat& lf = this->craplog.getLogsFormat( this->craplog.getCurrentWSID() );
        if ( lf.string.size() == 0 ) {
            // format string not set
            proceed = false;
            DialogSec::errLogFormatNotSet( nullptr );
        } else if ( lf.fields.size() == 0 ) {
            // no field, useless to parse
            proceed = false;
            DialogSec::errLogFormatNoFields( nullptr );
        } else if ( lf.separators.size() < lf.fields.size()-1 ) {
            // missing at least a separator between two (or more) fields
            proceed = false;
            DialogSec::errLogFormatNoSeparators( nullptr );
        }

        if ( proceed ) {
            // take actions on Craplog's start
            this->craplogStarted();

            // feed craplog with the checked files
            QTreeWidgetItemIterator i(this->ui->listLogFiles);
            while ( *i ) {
                if ( (*i)->checkState(0) == Qt::CheckState::Checked ) {
                    // tell Craplog to set this file as selected
                    if ( ! this->craplog.setLogFileSelected( (*i)->text(0) ) ) {
                        // failed to retrieve the file. this shouldn't be, but...
                        const int choice = DialogSec::choiceSelectedFileNotFound( (*i)->text(0) );
                        if ( choice == 0 ) {
                            // choosed to abort all
                            proceed = false;
                            break;
                        } else if ( choice == 1 ) {
                            // choosed to discard the file and continue
                            ;
                        } else {
                            // shouldn't be here
                            throw GenericException( "Unexpeced value returned: "+std::to_string(choice) );
                        }
                    }
                }
                ++i;
            }

            if ( proceed ) {
                // check files to be used before to start
                proceed = this->craplog.checkStuff();
            } else {
                this->craplogFinished();
            }

            if ( proceed ) {
                // periodically update perfs
                delete this->waiter_timer;
                this->waiter_timer = new QTimer(this);
                connect(this->waiter_timer, SIGNAL(timeout()), this, SLOT(update_Craplog_PerfData()));
                // run craplog as thread
                this->waiter_timer_start = std::chrono::system_clock::now();
                delete this->craplog_timer;
                this->craplog_timer = new QTimer(this);
                this->craplog_timer->setSingleShot( true );
                connect(this->craplog_timer, SIGNAL(timeout()), this, SLOT(runCraplog()));
                // start processing
                this->waiter_timer->start(250);
                this->craplog_timer->start(100);
            } else {
                this->craplogFinished();
            }
        }
    }
}
void MainWindow::runCraplog()
{
    this->craplog.run();
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
    unsigned size;
    long secs;
    // size and lines
    if ( this->craplog.isParsing() ) {
        this->craplog.collectPerfData();
    }
    size = this->craplog.getTotalSize();
    //size = this->craplog.getParsedSize();
    this->ui->label_MakeStats_Size->setText( this->printableSize( size ) );
    this->ui->label_MakeStats_Lines->setText( QString::fromStdString(std::to_string(this->craplog.getParsedLines())) );
    // time and speed
    this->waiter_timer_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        this->waiter_timer_start - std::chrono::system_clock::now()
    );
    size = this->craplog.getPerfSize();
    secs = this->waiter_timer_elapsed.count() / -1000000000;
    this->ui->label_MakeStats_Time->setText( this->printableTime( secs ));
    this->ui->label_MakeStats_Speed->setText( this->printableSpeed( size, secs ));
}

void MainWindow::update_Craplog_PerfData()
{
    // craplog is running as thread, update the values meanwhile
    this->update_MakeStats_labels();
    // check if Craplog has finished working
    if ( ! this->craplog.isWorking() ) {
        this->waiter_timer->stop();
        this->craplogFinished();
    }
}

void MainWindow::craplogStarted()
{
    // reset perfs
    this->reset_MakeStats_labels();
    this->craplog.logOps.resetPerfData();
    // disable the LogFiles section
    this->ui->stacked_Logs_Pages->setEnabled(false);
    // disable things which needs database access
    this->setDbWorkingState( true );
    // enable all labels (needed only the first time)
    this->ui->icon_MakeStats_Size->setEnabled(  false );
    this->ui->label_MakeStats_Size->setEnabled(  true );
    this->ui->icon_MakeStats_Lines->setEnabled( false );
    this->ui->label_MakeStats_Lines->setEnabled( true );
    this->ui->icon_MakeStats_Time->setEnabled(  false );
    this->ui->label_MakeStats_Time->setEnabled(  true );
    this->ui->icon_MakeStats_Speed->setEnabled( false );
    this->ui->label_MakeStats_Speed->setEnabled( true );
}

void MainWindow::craplogFinished()
{
    // update the perf data one last time, just in case
    this->update_MakeStats_labels();
    this->craplog.makeChart(
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->ui->chart_MakeStats_Size );
    if ( this->craplog.editedDatabase() ) {
        // craplog succeeded
        this->db_edited = true;
    }
    if ( this->craplog.getTotalSize() == 0 ) {
        // no data
        this->reset_MakeStats_labels();
    }
    // clean up temp vars
    this->craplog.clearDataCollection();
    this->craplog.logOps.resetPerfData();

    // refresh the logs list
    this->on_button_LogFiles_RefreshList_clicked();
    // enable the LogFiles section
    this->ui->stacked_Logs_Pages->setEnabled( true );
    // enable all labels (needed only the first time each session)
    this->ui->icon_MakeStats_Size->setEnabled(  true );
    this->ui->icon_MakeStats_Lines->setEnabled( true );
    this->ui->icon_MakeStats_Time->setEnabled(  true );
    this->ui->icon_MakeStats_Speed->setEnabled( true );
    // enable back
    this->setDbWorkingState( false );
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
    this->on_box_StatsWarn_WebServer_currentIndexChanged(  this->ui->box_StatsWarn_WebServer->currentIndex()  );
    this->on_box_StatsSpeed_WebServer_currentIndexChanged( this->ui->box_StatsSpeed_WebServer->currentIndex() );
    this->on_box_StatsCount_WebServer_currentIndexChanged( this->ui->box_StatsCount_WebServer->currentIndex() );
    this->on_box_StatsDay_WebServer_currentIndexChanged(   this->ui->box_StatsDay_WebServer->currentIndex()   );
    this->on_box_StatsRelat_WebServer_currentIndexChanged( this->ui->box_StatsRelat_WebServer->currentIndex() );
}


//////////////
//// WARN ////
void MainWindow::checkStatsWarnDrawable()
{
    if ( ! this->db_working ) {
        if ( this->ui->box_StatsWarn_Year->currentIndex() >= 0
          && this->ui->box_StatsWarn_Month->currentIndex() >= 0
          && this->ui->box_StatsWarn_Day->currentIndex() >= 0 ) {
            // enable the draw button
            this->ui->button_StatsWarn_Draw->setEnabled( true );
        } else {
            // disable the draw button
            this->ui->button_StatsWarn_Draw->setEnabled( false );
        }
    } else {
        this->ui->button_StatsRelat_Draw->setEnabled( false );
    }
}

void MainWindow::on_box_StatsWarn_WebServer_currentIndexChanged(int index)
{
    if ( this->checkDataDB() ) {
        this->ui->box_StatsWarn_Year->clear();
        if ( index != -1 ) {
            this->ui->box_StatsWarn_Year->addItems(
                this->crapview.getYears( this->wsFromIndex( index ) ));
            this->ui->box_StatsWarn_Year->setCurrentIndex( 0 );
        }
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_box_StatsWarn_Year_currentIndexChanged(int index)
{
    this->ui->box_StatsWarn_Month->clear();
    if ( index != -1 ) {
        this->ui->box_StatsWarn_Month->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsWarn_WebServer->currentIndex() ),
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
                this->wsFromIndex( this->ui->box_StatsWarn_WebServer->currentIndex() ),
                this->ui->box_StatsWarn_Year->currentText(),
                this->ui->box_StatsWarn_Month->currentText() ) );
        this->ui->box_StatsWarn_Day->setCurrentIndex( 0 );
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_box_StatsWarn_Day_currentIndexChanged(int index)
{
    if ( this->ui->checkBox_StatsWarn_Hour->isChecked() ) {
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
    if ( this->checkDataDB() ) {
        this->setDbWorkingState( true );
        delete this->crapview_timer;
        this->crapview_timer = new QTimer(this);
        this->crapview_timer->setSingleShot( true );
        connect(this->crapview_timer, SIGNAL(timeout()), this, SLOT(drawStatsWarn()));
        this->crapview_timer->start(250);
    }
}
void MainWindow::drawStatsWarn()
{
    this->ui->table_StatsWarn->setRowCount(0);
    this->crapview.drawWarn(
        this->ui->table_StatsWarn, this->ui->chart_StatsWarn,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->wsFromIndex( this->ui->box_StatsWarn_WebServer->currentIndex() ),
        this->ui->box_StatsWarn_Year->currentText(),
        this->ui->box_StatsWarn_Month->currentText(),
        this->ui->box_StatsWarn_Day->currentText(),
        (this->ui->checkBox_StatsWarn_Hour->isChecked()) ? this->ui->box_StatsWarn_Hour->currentText() : "" );
    this->setDbWorkingState( false );
}


void MainWindow::on_button_StatsWarn_Update_clicked()
{
    this->crapview.updateWarn(
        this->ui->table_StatsWarn,
        this->wsFromIndex( this->ui->box_StatsWarn_WebServer->currentIndex() ) );
    this->db_edited = true;
}


///////////////
//// SPEED ////
void MainWindow::checkStatsSpeedDrawable()
{
    if ( ! this->db_working ) {
        if ( this->ui->box_StatsSpeed_Year->currentIndex() >= 0
          && this->ui->box_StatsSpeed_Month->currentIndex() >= 0
          && this->ui->box_StatsSpeed_Day->currentIndex() >= 0 ) {
            // enable the draw button
            this->ui->button_StatsSpeed_Draw->setEnabled( true );
        } else {
            // disable the draw button
            this->ui->button_StatsSpeed_Draw->setEnabled( false );
        }
    } else {
        this->ui->button_StatsRelat_Draw->setEnabled( false );
    }
}

void MainWindow::on_box_StatsSpeed_WebServer_currentIndexChanged(int index)
{
    if ( this->checkDataDB() ) {
        this->ui->box_StatsSpeed_Year->clear();
        if ( index != -1 ) {
            this->ui->box_StatsSpeed_Year->addItems(
                this->crapview.getYears( this->wsFromIndex( index ) ) );
            this->ui->box_StatsSpeed_Year->setCurrentIndex( 0 );
        }
    }
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_box_StatsSpeed_Year_currentIndexChanged(int index)
{
    this->ui->box_StatsSpeed_Month->clear();
    if ( index != -1 ) {
        this->ui->box_StatsSpeed_Month->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsSpeed_WebServer->currentIndex() ),
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
                this->wsFromIndex( this->ui->box_StatsSpeed_WebServer->currentIndex() ),
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
    if ( this->checkDataDB() ) {
        this->setDbWorkingState( true );
        delete this->crapview_timer;
        this->crapview_timer = new QTimer(this);
        this->crapview_timer->setSingleShot( true );
        connect(this->crapview_timer, SIGNAL(timeout()), this, SLOT(drawStatsSpeed()));
        this->crapview_timer->start(250);
    }
}
void MainWindow::drawStatsSpeed()
{
    this->ui->table_StatsSpeed->setRowCount(0);
    this->crapview.drawSpeed(
        this->ui->table_StatsSpeed,
        this->ui->chart_SatsSpeed,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->wsFromIndex( this->ui->box_StatsSpeed_WebServer->currentIndex() ),
        this->ui->box_StatsSpeed_Year->currentText(),
        this->ui->box_StatsSpeed_Month->currentText(),
        this->ui->box_StatsSpeed_Day->currentText(),
        this->crapview.parseTextualFilter( this->ui->inLine_StatsSpeed_Protocol->text() ),
        this->crapview.parseTextualFilter( this->ui->inLine_StatsSpeed_Method->text() ),
        this->crapview.parseTextualFilter( this->ui->inLine_StatsSpeed_Uri->text() ),
        this->crapview.parseTextualFilter( this->ui->inLine_StatsSpeed_Query->text() ),
        this->crapview.parseNumericFilter( this->ui->inLine_StatsSpeed_Response->text() ) );
    this->setDbWorkingState( false );
}


///////////////
//// COUNT ////
void MainWindow::checkStatsCountDrawable()
{
    if ( ! this->db_working ) {
        if ( this->ui->box_StatsCount_Year->currentIndex() >= 0
          && this->ui->box_StatsCount_Month->currentIndex() >= 0
          && this->ui->box_StatsCount_Day->currentIndex() >= 0 ) {
            // enable the draw button
            this->ui->scrollArea_StatsCount->setEnabled( true );
        } else {
            // disable the draw button
            this->ui->scrollArea_StatsCount->setEnabled( false );
        }
    } else {
        this->ui->button_StatsRelat_Draw->setEnabled( false );
    }
}

void MainWindow::on_box_StatsCount_WebServer_currentIndexChanged(int index)
{
    if ( this->checkDataDB() ) {
        this->ui->box_StatsCount_Year->clear();
        if ( index != -1 ) {
            this->ui->box_StatsCount_Year->addItems(
                this->crapview.getYears( this->wsFromIndex( index ) ));
            this->ui->box_StatsCount_Year->setCurrentIndex( 0 );
            this->resetStatsCountButtons();
        }
    }
    this->checkStatsCountDrawable();
}

void MainWindow::on_box_StatsCount_Year_currentIndexChanged(int index)
{
    this->ui->box_StatsCount_Month->clear();
    if ( index != -1 ) {
        this->ui->box_StatsCount_Month->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsCount_WebServer->currentIndex() ),
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
                this->wsFromIndex( this->ui->box_StatsCount_WebServer->currentIndex() ),
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

void MainWindow::resetStatsCountButtons()
{
    if ( ! this->ui->button_StatsCount_Protocol->isFlat() ) {
        this->ui->button_StatsCount_Protocol->setFlat( true );
    }
    if ( ! this->ui->button_StatsCount_Method->isFlat() ) {
        this->ui->button_StatsCount_Method->setFlat( true );
    }
    if ( ! this->ui->button_StatsCount_Uri->isFlat() ) {
        this->ui->button_StatsCount_Uri->setFlat( true );
    }
    if ( ! this->ui->button_StatsCount_Query->isFlat() ) {
        this->ui->button_StatsCount_Query->setFlat( true );
    }
    if ( ! this->ui->button_StatsCount_Response->isFlat() ) {
        this->ui->button_StatsCount_Response->setFlat( true );
    }
    if ( ! this->ui->button_StatsCount_Referrer->isFlat() ) {
        this->ui->button_StatsCount_Referrer->setFlat( true );
    }
    if ( ! this->ui->button_StatsCount_Cookie->isFlat() ) {
        this->ui->button_StatsCount_Cookie->setFlat( true );
    }
    if ( ! this->ui->button_StatsCount_UserAgent->isFlat() ) {
        this->ui->button_StatsCount_UserAgent->setFlat( true );
    }
    if ( ! this->ui->button_StatsCount_Client->isFlat() ) {
        this->ui->button_StatsCount_Client->setFlat( true );
    }
}

void MainWindow::startCountDrawing()
{
    this->setDbWorkingState( true );
    delete this->crapview_timer;
    this->crapview_timer = new QTimer(this);
    this->crapview_timer->setSingleShot( true );
    connect(this->crapview_timer, SIGNAL(timeout()), this, SLOT(drawStatsCount()));
    this->crapview_timer->start(250);
}

void MainWindow::on_button_StatsCount_Protocol_clicked()
{
    if ( this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->ui->button_StatsCount_Protocol->setFlat( false );
        this->count_fld = this->ui->button_StatsCount_Protocol->text();
        startCountDrawing();
    }
}

void MainWindow::on_button_StatsCount_Method_clicked()
{
    if ( this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Method->text();
        this->ui->button_StatsCount_Method->setFlat( false );
        startCountDrawing();
    }
}

void MainWindow::on_button_StatsCount_Uri_clicked()
{
    if ( this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Uri->text();
        this->ui->button_StatsCount_Uri->setFlat( false );
        startCountDrawing();
    }
}

void MainWindow::on_button_StatsCount_Query_clicked()
{
    if ( this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Query->text();
        this->ui->button_StatsCount_Query->setFlat( false );
        startCountDrawing();
    }
}

void MainWindow::on_button_StatsCount_Response_clicked()
{
    if ( this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Response->text();
        this->ui->button_StatsCount_Response->setFlat( false );
        startCountDrawing();
    }
}

void MainWindow::on_button_StatsCount_Referrer_clicked()
{
    if ( this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Referrer->text();
        this->ui->button_StatsCount_Referrer->setFlat( false );
        startCountDrawing();
    }
}

void MainWindow::on_button_StatsCount_Cookie_clicked()
{
    if ( this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Cookie->text();
        this->ui->button_StatsCount_Cookie->setFlat( false );
        startCountDrawing();
    }
}

void MainWindow::on_button_StatsCount_UserAgent_clicked()
{
    if ( this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_UserAgent->text();
        this->ui->button_StatsCount_UserAgent->setFlat( false );
        startCountDrawing();
    }
}

void MainWindow::on_button_StatsCount_Client_clicked()
{
    if ( this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Client->text();
        this->ui->button_StatsCount_Client->setFlat( false );
        startCountDrawing();
    }
}

void MainWindow::drawStatsCount()
{
    this->ui->table_StatsCount->setRowCount(0);
    this->crapview.drawCount(
        this->ui->table_StatsCount, this->ui->chart_StatsCount,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->wsFromIndex( this->ui->box_StatsCount_WebServer->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(),
        this->ui->box_StatsCount_Month->currentText(),
        this->ui->box_StatsCount_Day->currentText(),
        this->count_fld );
    this->setDbWorkingState( false );
}


/////////////
//// DAY ////
void MainWindow::checkStatsDayDrawable()
{
    if ( ! this->db_working ) {
        bool aux = true;
        // secondary date (period)
        if ( this->ui->checkBox_StatsDay_Period->isChecked() ) {
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

    } else {
        // db busy
        this->ui->button_StatsRelat_Draw->setEnabled( false );
    }
}

void MainWindow::on_box_StatsDay_WebServer_currentIndexChanged(int index)
{
    if ( this->checkDataDB() ) {
        this->ui->box_StatsDay_LogsField->clear();
        this->ui->box_StatsDay_FromYear->clear();
        this->ui->box_StatsDay_ToYear->clear();
        if ( index != -1 ) {
            // refresh fields
            this->ui->box_StatsDay_LogsField->addItems(
                this->crapview.getFields( "Daytime" ));
            this->ui->box_StatsDay_LogsField->setCurrentIndex( 0 );
            // refresh dates
            QStringList years = this->crapview.getYears( this->wsFromIndex( index ) );
            this->ui->box_StatsDay_FromYear->addItems( years );
            this->ui->box_StatsDay_FromYear->setCurrentIndex( 0 );
            if ( this->ui->checkBox_StatsDay_Period->isChecked() ) {
                this->ui->box_StatsDay_ToYear->addItems( years );
                this->ui->box_StatsDay_ToYear->setCurrentIndex( 0 );
            }
            years.clear();
        }
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
                this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
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
                this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
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
            this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ) ) );
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
                this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
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
                this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
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
    if ( this->checkDataDB() ) {
        this->setDbWorkingState( true );
        delete this->crapview_timer;
        this->crapview_timer = new QTimer(this);
        this->crapview_timer->setSingleShot( true );
        connect(this->crapview_timer, SIGNAL(timeout()), this, SLOT(drawStatsDay()));
        this->crapview_timer->start(250);
    }
}
void MainWindow::drawStatsDay()
{
    QString filter;
    if ( this->ui->box_StatsDay_LogsField->currentIndex() == 0 ) {
        filter = this->crapview.parseBooleanFilter( this->ui->inLine_StatsDay_Filter->text() );
    } else if ( this->ui->box_StatsDay_LogsField->currentIndex() == 5 ) {
        filter = this->crapview.parseNumericFilter( this->ui->inLine_StatsDay_Filter->text() );
    } else {
        filter = this->crapview.parseTextualFilter( this->ui->inLine_StatsDay_Filter->text() );
    }
    this->crapview.drawDay(
        this->ui->chart_StatsDay,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
        this->ui->box_StatsDay_FromYear->currentText(),
        this->ui->box_StatsDay_FromMonth->currentText(),
        this->ui->box_StatsDay_FromDay->currentText(),
        ( this->ui->checkBox_StatsDay_Period->isChecked() ) ? this->ui->box_StatsDay_ToYear->currentText() : "",
        ( this->ui->checkBox_StatsDay_Period->isChecked() ) ? this->ui->box_StatsDay_ToMonth->currentText() : "",
        ( this->ui->checkBox_StatsDay_Period->isChecked() ) ? this->ui->box_StatsDay_ToDay->currentText() : "",
        this->ui->box_StatsDay_LogsField->currentText(),
        filter );
    this->setDbWorkingState( false );
}



////////////////////
//// RELATIONAL ////
void MainWindow::checkStatsRelatDrawable()
{
    if ( ! this->db_working ) {
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

    } else {
        // db busy
        this->ui->button_StatsRelat_Draw->setEnabled( false );
    }
}

void MainWindow::on_box_StatsRelat_WebServer_currentIndexChanged(int index)
{
    if ( this->checkDataDB() ) {
        this->ui->box_StatsRelat_LogsField_1->clear();
        this->ui->box_StatsRelat_LogsField_2->clear();
        if ( index != -1 ) {
            // refresh fields
            QStringList fields = this->crapview.getFields( "Relational" );
            this->ui->box_StatsRelat_LogsField_1->addItems( fields );
            this->ui->box_StatsRelat_LogsField_2->addItems( fields );
            this->ui->box_StatsRelat_LogsField_1->setCurrentIndex( 0 );
            this->ui->box_StatsRelat_LogsField_2->setCurrentIndex( 0 );
            // refresh dates
            QStringList years = this->crapview.getYears( this->wsFromIndex( index ) );
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
                this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
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
                this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
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
                this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
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
                this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
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
    if ( this->checkDataDB() ) {
        this->setDbWorkingState( true );
        delete this->crapview_timer;
        this->crapview_timer = new QTimer(this);
        this->crapview_timer->setSingleShot( true );
        connect(this->crapview_timer, SIGNAL(timeout()), this, SLOT(drawStatsRelat()));
        this->crapview_timer->start(250);
    }
}
void MainWindow::drawStatsRelat()
{
    int aux;
    QString filter1, filter2;
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
    this->crapview.drawRelat(
        this->ui->chart_StatsRelat,
        this->CHARTS_THEMES.at( this->charts_theme_id ), this->FONTS,
        this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
        this->ui->box_StatsRelat_FromYear->currentText(),
        this->ui->box_StatsRelat_FromMonth->currentText(),
        this->ui->box_StatsRelat_FromDay->currentText(),
        this->ui->box_StatsRelat_ToYear->currentText(),
        this->ui->box_StatsRelat_ToMonth->currentText(),
        this->ui->box_StatsRelat_ToDay->currentText(),
        this->ui->box_StatsRelat_LogsField_1->currentText(), filter1,
        this->ui->box_StatsRelat_LogsField_2->currentText(), filter2 );
    this->setDbWorkingState( false );
}



////////////////
//// GLOBAL ////
//
void MainWindow::makeStatsGlobals()
{
    if ( this->checkDataDB() ) {
        std::vector<std::tuple<QString,QString>> recur_list;
        std::vector<std::tuple<QString,QString>> traffic_list;
        std::vector<std::tuple<QString,QString>> perf_list;
        std::vector<QString> work_list;

        const bool result = this->crapview.calcGlobals(
            recur_list, traffic_list, perf_list, work_list,
            this->glob_ws );

        if ( result ) {
            this->ui->label_StatsGlob_Recur_Protocol_String->setText( std::get<0>( recur_list.at(0) ) );
            this->ui->label_StatsGlob_Recur_Protocol_Count->setText( std::get<1>( recur_list.at(0) ) );
            this->ui->label_StatsGlob_Recur_Method_String->setText( std::get<0>( recur_list.at(1) ) );
            this->ui->label_StatsGlob_Recur_Method_Count->setText( std::get<1>( recur_list.at(1) ) );
            this->ui->label_StatsGlob_Recur_URI_String->setText( std::get<0>( recur_list.at(2) ) );
            this->ui->label_StatsGlob_Recur_URI_Count->setText( std::get<1>( recur_list.at(2) ) );
            this->ui->label_StatsGlob_Recur_UserAgent_String->setText( std::get<0>( recur_list.at(3) ) );
            this->ui->label_StatsGlob_Recur_UserAgent_Count->setText( std::get<1>( recur_list.at(3) ) );

            this->ui->label_StatsGlob_Traffic_Date_String->setText( std::get<0>( traffic_list.at(0) ) );
            this->ui->label_StatsGlob_Traffic_Date_Count->setText( std::get<1>( traffic_list.at(0) ) );
            this->ui->label_StatsGlob_Traffic_Day_String->setText( std::get<0>( traffic_list.at(1) ) );
            this->ui->label_StatsGlob_Traffic_Day_Count->setText( std::get<1>( traffic_list.at(1) ) );
            this->ui->label_StatsGlob_Traffic_Hour_String->setText( std::get<0>( traffic_list.at(2) ) );
            this->ui->label_StatsGlob_Traffic_Hour_Count->setText( std::get<1>( traffic_list.at(2) ) );

            this->ui->label_StatsGlob_Perf_Time_Mean->setText( std::get<0>( perf_list.at(0) ) );
            this->ui->label_StatsGlob_Perf_Time_Max->setText( std::get<1>( perf_list.at(0) ) );
            this->ui->label_StatsGlob_Perf_Sent_Mean->setText( std::get<0>( perf_list.at(1) ) );
            this->ui->label_StatsGlob_Perf_Sent_Max->setText( std::get<1>( perf_list.at(1) ) );
            this->ui->label_StatsGlob_Perf_Received_Mean->setText( std::get<0>( perf_list.at(2) ) );
            this->ui->label_StatsGlob_Perf_Received_Max->setText( std::get<1>( perf_list.at(2) ) );

            this->ui->label_StatsGlob_Work_Req_Count->setText( work_list.at(0) );
            this->ui->label_StatsGlob_Work_Time_Count->setText( work_list.at(1) );
            this->ui->label_StatsGlob_Work_Sent_Count->setText( work_list.at(2) );

            if ( this->glob_ws == "apache" ) {
                if ( this->ui->button_StatsGlob_Apache->isFlat() ) {
                    // un-flat
                    this->ui->button_StatsGlob_Apache->setFlat( false );
                    this->ui->button_StatsGlob_Nginx->setFlat( true );
                    this->ui->button_StatsGlob_Iis->setFlat( true );
                }
            } else if ( this->glob_ws == "nginx" ) {
                if ( this->ui->button_StatsGlob_Nginx->isFlat() ) {
                    // un-flat
                    this->ui->button_StatsGlob_Nginx->setFlat( false );
                    this->ui->button_StatsGlob_Apache->setFlat( true );
                    this->ui->button_StatsGlob_Iis->setFlat( true );
                }
            } else if ( this->glob_ws == "iis" ) {
                if ( this->ui->button_StatsGlob_Iis->isFlat() ) {
                    // un-flat
                    this->ui->button_StatsGlob_Iis->setFlat( false );
                    this->ui->button_StatsGlob_Apache->setFlat( true );
                    this->ui->button_StatsGlob_Nginx->setFlat( true );
                }
            }

        } else {
            this->resetStatsGlobals();
        }
        recur_list.clear(); traffic_list.clear();
        perf_list.clear();  work_list.clear();

    } else {
        this->resetStatsGlobals();
    }
    // restore
    this->setDbWorkingState( false );
}

void MainWindow::resetStatsGlobals()
{
    this->ui->label_StatsGlob_Recur_Protocol_String->setText( "-" );
    this->ui->label_StatsGlob_Recur_Protocol_Count->setText( "0" );
    this->ui->label_StatsGlob_Recur_Method_String->setText( "-" );
    this->ui->label_StatsGlob_Recur_Method_Count->setText( "0" );
    this->ui->label_StatsGlob_Recur_URI_String->setText( "-" );
    this->ui->label_StatsGlob_Recur_URI_Count->setText( "0" );
    this->ui->label_StatsGlob_Recur_UserAgent_String->setText( "-" );
    this->ui->label_StatsGlob_Recur_UserAgent_Count->setText( "0" );

    this->ui->label_StatsGlob_Traffic_Date_String->setText( "-" );
    this->ui->label_StatsGlob_Traffic_Date_Count->setText( "0" );
    this->ui->label_StatsGlob_Traffic_Day_String->setText( "-" );
    this->ui->label_StatsGlob_Traffic_Day_Count->setText( "0" );
    this->ui->label_StatsGlob_Traffic_Hour_String->setText( "-" );
    this->ui->label_StatsGlob_Traffic_Hour_Count->setText( "0" );

    this->ui->label_StatsGlob_Perf_Time_Mean->setText( "-" );
    this->ui->label_StatsGlob_Perf_Time_Max->setText( "-" );
    this->ui->label_StatsGlob_Perf_Sent_Mean->setText( "-" );
    this->ui->label_StatsGlob_Perf_Sent_Max->setText( "-" );
    this->ui->label_StatsGlob_Perf_Received_Mean->setText( "-" );
    this->ui->label_StatsGlob_Perf_Received_Max->setText( "-" );

    this->ui->label_StatsGlob_Work_Req_Count->setText( "-" );
    this->ui->label_StatsGlob_Work_Time_Count->setText( "-" );
    this->ui->label_StatsGlob_Work_Sent_Count->setText( "-" );

    if ( this->ui->button_StatsGlob_Apache->isChecked() ) {
        this->ui->button_StatsGlob_Apache->setChecked( false );
    } else if ( this->ui->button_StatsGlob_Nginx->isChecked() ) {
        this->ui->button_StatsGlob_Nginx->setChecked( false );
    } else if ( this->ui->button_StatsGlob_Iis->isChecked() ) {
        this->ui->button_StatsGlob_Iis->setChecked( false );
    }
}



void MainWindow::globalsButtonClicked()
{
    this->setDbWorkingState( true );
    delete this->crapview_timer;
    this->crapview_timer = new QTimer(this);
    this->crapview_timer->setSingleShot( true );
    connect(this->crapview_timer, SIGNAL(timeout()), this, SLOT(makeStatsGlobals()));
    this->crapview_timer->start(250);
}

void MainWindow::on_button_StatsGlob_Apache_clicked()
{
    if ( this->checkDataDB() ) {
        this->glob_ws = "apache";
        this->globalsButtonClicked();
    }
}


void MainWindow::on_button_StatsGlob_Nginx_clicked()
{
    if ( this->checkDataDB() ) {
        this->glob_ws = "nginx";
        this->globalsButtonClicked();
    }
}


void MainWindow::on_button_StatsGlob_Iis_clicked()
{
    if ( this->checkDataDB() ) {
        this->glob_ws = "iis";
        this->globalsButtonClicked();
    }
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
    this->window_theme_id = index;
    this->updateUiTheme();
}

void MainWindow::on_box_ConfWindow_Icons_currentIndexChanged(int index)
{
    this->icons_theme_id = index;
    this->updateUiIcons();
}


/////////////////
//// DIALOGS ////
void MainWindow::on_slider_ConfDialogs_General_sliderReleased()
{
    this->dialogs_level = this->ui->slider_ConfDialogs_General->value();
}
void MainWindow::on_slider_ConfDialogs_Logs_sliderReleased()
{
    this->craplog.setDialogsLevel( this->ui->slider_ConfDialogs_Logs->value() );
}
void MainWindow::on_slider_ConfDialogs_Stats_sliderReleased()
{
    this->crapview.setDialogsLevel( this->ui->slider_ConfDialogs_Stats->value() );
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
            throw GenericException( "Unexpected Font index: "+std::to_string(index), true );
    }
    this->TB.setFont( font );
    this->crapnote->setTextFont( font );
    this->ui->textBrowser_ConfTextBrowser_Preview->setFont( font );
    this->ui->preview_ConfApache_Format_Sample->setFont( this->TB.getFont() );
    this->ui->preview_ConfNginx_Format_Sample->setFont( this->TB.getFont() );
    this->ui->preview_ConfIis_Format_Sample->setFont( this->TB.getFont() );
}
void MainWindow::on_checkBox_ConfTextBrowser_WideLines_clicked(bool checked)
{
    this->TB.setWideLinesUsage( checked );
    this->refreshTextBrowserPreview();
}
void MainWindow::on_box_ConfTextBrowser_ColorScheme_currentIndexChanged(int index)
{
    this->TB.setColorScheme( index, this->TB_COLOR_SCHEMES.at( index ) );
    this->crapnote->setColorScheme( index );
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
    QColor col = Qt::GlobalColor::darkGreen;
    QBarSet *bars_1 = new QBarSet( "" );
    bars_1->setColor( col );
    QBarSet *bars_2 = new QBarSet( "" );
    bars_2->setColor( col );
    QBarSet *bars_3 = new QBarSet( "" );
    bars_3->setColor( col );
    QBarSet *bars_4 = new QBarSet( "" );
    bars_4->setColor( col );
    QBarSet *bars_5 = new QBarSet( "" );
    bars_5->setColor( col );
    QBarSet *bars_6 = new QBarSet( "" );
    bars_6->setColor( col );

    int aux, max=0;
    for ( int i=0; i<24; i++ ) {
        aux = rand() %100; *bars_1 << aux;
        if ( aux > max ) { max = aux; }
        aux = rand() %100; *bars_2 << aux;
        if ( aux > max ) { max = aux; }
        aux = rand() %100; *bars_3 << aux;
        if ( aux > max ) { max = aux; }
        aux = rand() %100; *bars_4 << aux;
        if ( aux > max ) { max = aux; }
        aux = rand() %100; *bars_5 << aux;
        if ( aux > max ) { max = aux; }
        aux = rand() %100; *bars_6 << aux;
        if ( aux > max ) { max = aux; }
    }

    QBarSeries *bars = new QBarSeries();
    bars->append( bars_1 ); bars->append( bars_2 ); bars->append( bars_3 );
    bars->append( bars_4 ); bars->append( bars_5 ); bars->append( bars_6 );
    bars->setBarWidth( 1 );

    QChart *t_chart = new QChart();
    // apply the theme
    t_chart->setTheme( this->CHARTS_THEMES.at( this->charts_theme_id ) );
    // add the bars
    t_chart->addSeries( bars );
    t_chart->setTitle( "Sample preview" );
    t_chart->setTitleFont( this->FONTS.at("main") );
    t_chart->setAnimationOptions( QChart::SeriesAnimations );

    QStringList categories;
    categories << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
               << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append( categories );
    axisX->setLabelsFont( this->FONTS.at( "main_small" ) );
    axisX->setTitleText( "Infoes" );
    axisX->setTitleFont( this->FONTS.at("main_small") );
    t_chart->addAxis( axisX, Qt::AlignBottom );
    bars->attachAxis( axisX );

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat( "%d" );
    axisY->setRange( 0, max );
    axisY->setLabelsFont( this->FONTS.at( "main_small" ) );
    t_chart->addAxis( axisY, Qt::AlignLeft );
    bars->attachAxis( axisY) ;

    t_chart->legend()->setVisible( false );
    /*t_chart->legend()->setFont( this->FONTS.at("main_small") );
    t_chart->legend()->setAlignment( Qt::AlignBottom );*/

    this->ui->chart_ConfCharts_Preview->setChart( t_chart );
    this->ui->chart_ConfCharts_Preview->setRenderHint( QPainter::Antialiasing );
}


///////////////////
//// DATABASES ////
// data collection
void MainWindow::on_inLine_ConfDatabases_Data_Path_textChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        std::string path = this->resolvePath( arg1.toStdString() );
        if ( IOutils::checkDir( path ) ) {
            this->ui->icon_ConfDatabases_Data_Wrong->setVisible( false );
            this->ui->button_ConfDatabases_Data_Save->setEnabled( true );
        } else {
            this->ui->icon_ConfDatabases_Data_Wrong->setVisible( true );
            this->ui->button_ConfDatabases_Data_Save->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfDatabases_Data_Wrong->setVisible( true );
        this->ui->button_ConfDatabases_Data_Save->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfDatabases_Data_Path_returnPressed()
{
    this->on_button_ConfDatabases_Data_Save_clicked();
}
void MainWindow::on_button_ConfDatabases_Data_Save_clicked()
{
    if ( ! this->ui->icon_ConfDatabases_Data_Wrong->isVisible() ) {
        // set the paths
        std::string path = this->resolvePath( this->ui->inLine_ConfDatabases_Data_Path->text().toStdString() );
        if ( StringOps::endsWith( path, "/" ) ) {
            path = StringOps::rstrip( path, "/" );
        }
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        if ( ! IOutils::checkDir( path, false, true ) ) {
            DialogSec::warnDirNotWritable( nullptr );
        }
        this->db_data_path = path;
        this->craplog.setStatsDatabasePath( path );
        this->crapview.setDbPath( path );
        this->ui->inLine_ConfDatabases_Data_Path->setText( QString::fromStdString( path ) );
    }
    this->ui->inLine_ConfDatabases_Data_Path->setFocus();
    this->ui->button_ConfDatabases_Data_Save->setEnabled( false );
}

// usef files hashes
void MainWindow::on_inLine_ConfDatabases_Hashes_Path_textChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        std::string path = this->resolvePath( arg1.toStdString() );
        if ( IOutils::checkDir( path ) ) {
            this->ui->icon_ConfDatabases_Hashes_Wrong->setVisible( false );
            this->ui->button_ConfDatabases_Hashes_Save->setEnabled( true );
        } else {
            this->ui->icon_ConfDatabases_Hashes_Wrong->setVisible( true );
            this->ui->button_ConfDatabases_Hashes_Save->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfDatabases_Hashes_Wrong->setVisible( true );
        this->ui->button_ConfDatabases_Hashes_Save->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfDatabases_Hashes_Path_returnPressed()
{
    this->on_button_ConfDatabases_Hashes_Save_clicked();
}
void MainWindow::on_button_ConfDatabases_Hashes_Save_clicked()
{
    if ( ! this->ui->icon_ConfDatabases_Hashes_Wrong->isVisible() ) {
        // set the paths
        std::string path = this->resolvePath( this->ui->inLine_ConfDatabases_Hashes_Path->text().toStdString() );
        if ( StringOps::endsWith( path, "/" ) ) {
            path = StringOps::rstrip( path, "/" );
        }
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        if ( ! IOutils::checkDir( path, false, true ) ) {
            DialogSec::warnDirNotWritable( nullptr );
        }
        this->db_hashes_path = path;
        this->craplog.setHashesDatabasePath( path );
        this->ui->inLine_ConfDatabases_Hashes_Path->setText( QString::fromStdString( path ) );
    }
    this->ui->inLine_ConfDatabases_Hashes_Path->setFocus();
    this->ui->button_ConfDatabases_Hashes_Save->setEnabled( false );
}

// backups
void MainWindow::on_checkBox_ConfDatabases_DoBackup_clicked(bool checked)
{
    this->db_do_backup = checked;
    this->ui->spinBox_ConfDatabases_NumBackups->setEnabled( checked );
    if ( checked && this->ui->spinBox_ConfDatabases_NumBackups->value() == 0 ) {
        this->ui->spinBox_ConfDatabases_NumBackups->setValue( 1 );
    }
}
void MainWindow::on_spinBox_ConfDatabases_NumBackups_valueChanged(int arg1)
{
    this->db_backups_number = arg1;
    if ( arg1 == 1 ) {
        this->ui->spinBox_ConfDatabases_NumBackups->setSuffix( " " + MainWindow::tr( "copy" ) );
    } else {
        this->ui->spinBox_ConfDatabases_NumBackups->setSuffix( " " + MainWindow::tr( "copies" ) );
        if ( arg1 == 0 ) {
            this->ui->checkBox_ConfDatabases_DoBackup->setChecked( false );
            this->on_checkBox_ConfDatabases_DoBackup_clicked( false );
        }
    }
}


//////////////
//// LOGS ////
//////////////

//////////////////
//// DEFAULTS ////
void MainWindow::on_radio_ConfDefaults_Apache_toggled(bool checked)
{
    this->default_ws = this->APACHE_ID;
}
void MainWindow::on_radio_ConfDefaults_Nginx_toggled(bool checked)
{
    this->default_ws = this->NGINX_ID;
}
void MainWindow::on_radio_ConfDefaults_Iis_toggled(bool checked)
{
    this->default_ws = this->IIS_ID;
}

/////////////////
//// CONTROL ////
void MainWindow::on_checkBox_ConfControl_Usage_clicked(bool checked)
{
    this->hide_used_files = checked;
}
void MainWindow::on_checkBox_ConfControl_Size_clicked(bool checked)
{
    if ( ! checked ) {
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
void MainWindow::on_inLine_ConfApache_Path_String_textChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        std::string path = this->resolvePath( arg1.toStdString() );
        if ( IOutils::checkDir( path ) ) {
            this->ui->icon_ConfApache_Path_Wrong->setVisible( false );
            this->ui->button_ConfApache_Path_Save->setEnabled( true );
        } else {
            this->ui->icon_ConfApache_Path_Wrong->setVisible( true );
            this->ui->button_ConfApache_Path_Save->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfApache_Path_Wrong->setVisible( true );
        this->ui->button_ConfApache_Path_Save->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfApache_Path_String_returnPressed()
{
    this->on_button_ConfApache_Path_Save_clicked();
}
void MainWindow::on_button_ConfApache_Path_Save_clicked()
{
    if ( ! this->ui->icon_ConfApache_Path_Wrong->isVisible() ) {
        // set the paths
        std::string path = this->resolvePath( this->ui->inLine_ConfApache_Path_String->text().toStdString() );
        if ( StringOps::endsWith( path, "/" ) ) {
            path = StringOps::rstrip( path, "/" );
        }
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        this->craplog.setLogsPath( this->APACHE_ID, path );
        this->ui->inLine_ConfApache_Path_String->setText( QString::fromStdString( path ) );
    }
    this->ui->button_ConfApache_Path_Save->setEnabled( false );
}

// formats
void MainWindow::on_inLine_ConfApache_Format_String_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfApache_Format_Save->setEnabled( true );
    } else {
        this->ui->button_ConfApache_Format_Save->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfApache_Format_String_returnPressed()
{
    if ( this->ui->button_ConfApache_Format_Save->isEnabled() ) {
        this->on_button_ConfApache_Format_Save_clicked();
    }
}
void MainWindow::on_button_ConfApache_Format_Save_clicked()
{
    const bool success = this->craplog.setApacheLogFormat(
        this->ui->inLine_ConfApache_Format_String->text().toStdString() );
    if ( success ) {
        this->ui->button_ConfApache_Format_Save->setEnabled( false );
    }
}
void MainWindow::on_button_ConfApache_Format_Sample_clicked()
{
    this->ui->preview_ConfApache_Format_Sample->setText(
        this->craplog.getLogsFormatSample( this->APACHE_ID ) );
}
void MainWindow::on_button_ConfApache_Format_Help_clicked()
{
    this->showHelp( "apache_format" );
}

// warnlists
void MainWindow::on_box_ConfApache_Warnlist_Field_currentTextChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        this->ui->inLine_ConfApache_Warnlist_String->clear();
        this->ui->list_ConfApache_Warnlist_List->clear();
        // update the list
        const std::vector<std::string>& list = this->craplog.getWarnlist(
            this->APACHE_ID, this->crapview.getLogFieldID( arg1 ) );
        for ( const std::string& item : list ) {
            this->ui->list_ConfApache_Warnlist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        const bool used = this->craplog.isWarnlistUsed(
            this->APACHE_ID,
            this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Field->currentText() ) );
        this->ui->checkBox_ConfApache_Warnlist_Used->setChecked( used );
        this->on_checkBox_ConfApache_Warnlist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfApache_Warnlist_Used_clicked(bool checked)
{
    this->craplog.setWarnlistUsed(
        this->APACHE_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfApache_Warnlist_String->setEnabled( true );
        this->ui->list_ConfApache_Warnlist_List->setEnabled( true );
    } else {
        this->ui->inLine_ConfApache_Warnlist_String->clear();
        this->ui->inLine_ConfApache_Warnlist_String->setEnabled( false );
        this->ui->list_ConfApache_Warnlist_List->clearSelection();
        this->ui->list_ConfApache_Warnlist_List->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfApache_Warnlist_String_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfApache_Warnlist_Add->setEnabled( true );
    } else {
        this->ui->button_ConfApache_Warnlist_Add->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfApache_Warnlist_String_returnPressed()
{
    this->on_button_ConfApache_Warnlist_Add_clicked();
}
void MainWindow::on_button_ConfApache_Warnlist_Add_clicked()
{
    const QString& item = this->ui->inLine_ConfApache_Warnlist_String->text();
    if ( this->ui->list_ConfApache_Warnlist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        try {
            this->craplog.warnlistAdd(
                this->APACHE_ID,
                this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
                item.toStdString() );
            this->ui->list_ConfApache_Warnlist_List->addItem( item );
        } catch ( const BWlistException& ) {
            DialogSec::warnInvalidItemBW();
            return;
        }
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfApache_Warnlist_List->clearSelection();
    this->ui->list_ConfApache_Warnlist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfApache_Warnlist_String->clear();
}

void MainWindow::on_list_ConfApache_Warnlist_List_itemSelectionChanged()
{
    if ( this->ui->list_ConfApache_Warnlist_List->selectedItems().size() == 1 ) {
        this->ui->button_ConfApache_Warnlist_Remove->setEnabled( true );
        this->ui->button_ConfApache_Warnlist_Up->setEnabled( true );
        this->ui->button_ConfApache_Warnlist_Down->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfApache_Warnlist_List->selectedItems().at(0);
        const int max = this->ui->list_ConfApache_Warnlist_List->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfApache_Warnlist_Up->setEnabled( false );
            this->ui->button_ConfApache_Warnlist_Down->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfApache_Warnlist_List->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfApache_Warnlist_Up->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfApache_Warnlist_Down->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfApache_Warnlist_Remove->setEnabled( false );
        this->ui->button_ConfApache_Warnlist_Up->setEnabled( false );
        this->ui->button_ConfApache_Warnlist_Down->setEnabled( false );
    }
}
void MainWindow::on_button_ConfApache_Warnlist_Remove_clicked()
{
    const auto& item = this->ui->list_ConfApache_Warnlist_List->selectedItems().at(0);
    this->craplog.warnlistRemove(
        this->APACHE_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Field_currentTextChanged( this->ui->box_ConfApache_Warnlist_Field->currentText() );
}
void MainWindow::on_button_ConfApache_Warnlist_Up_clicked()
{
    const auto& item = this->ui->list_ConfApache_Warnlist_List->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveUp(
        this->APACHE_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Field_currentTextChanged( this->ui->box_ConfApache_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfApache_Warnlist_List->setFocus();
}
void MainWindow::on_button_ConfApache_Warnlist_Down_clicked()
{
    const auto& item = this->ui->list_ConfApache_Warnlist_List->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveDown(
        this->APACHE_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Field_currentTextChanged( this->ui->box_ConfApache_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfApache_Warnlist_List->setFocus();
}


// blacklist
void MainWindow::on_box_ConfApache_Blacklist_Field_currentTextChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        this->ui->inLine_ConfApache_Blacklist_String->clear();
        this->ui->list_ConfApache_Blacklist_List->clear();
        // update the list
        const std::vector<std::string>& list = this->craplog.getBlacklist(
            this->APACHE_ID, this->crapview.getLogFieldID( arg1 ) );
        for ( const std::string& item : list ) {
            this->ui->list_ConfApache_Blacklist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        bool used = this->craplog.isBlacklistUsed(
            this->APACHE_ID,
            this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Field->currentText() ) );
        this->ui->checkBox_ConfApache_Blacklist_Used->setChecked( used );
        this->on_checkBox_ConfApache_Blacklist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfApache_Blacklist_Used_clicked(bool checked)
{
    this->craplog.setBlacklistUsed(
        this->APACHE_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfApache_Blacklist_String->setEnabled( true );
        this->ui->list_ConfApache_Blacklist_List->setEnabled( true );
    } else {
        this->ui->inLine_ConfApache_Blacklist_String->clear();
        this->ui->inLine_ConfApache_Blacklist_String->setEnabled( false );
        this->ui->list_ConfApache_Blacklist_List->clearSelection();
        this->ui->list_ConfApache_Blacklist_List->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfApache_Blacklist_String_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfApache_Blacklist_Add->setEnabled( true );
    } else {
        this->ui->button_ConfApache_Blacklist_Add->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfApache_Blacklist_String_returnPressed()
{
    this->on_button_ConfApache_Blacklist_Add_clicked();
}
void MainWindow::on_button_ConfApache_Blacklist_Add_clicked()
{
    const QString& item = this->ui->inLine_ConfApache_Blacklist_String->text();
    if ( this->ui->list_ConfApache_Blacklist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        try {
            this->craplog.blacklistAdd(
                this->APACHE_ID,
                this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
                item.toStdString() );
            this->ui->list_ConfApache_Blacklist_List->addItem( item );
        } catch ( const BWlistException& ) {
            DialogSec::warnInvalidItemBW();
            return;
        }
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfApache_Blacklist_List->clearSelection();
    this->ui->list_ConfApache_Blacklist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfApache_Blacklist_String->clear();
}

void MainWindow::on_list_ConfApache_Blacklist_List_itemSelectionChanged()
{
    if ( this->ui->list_ConfApache_Blacklist_List->selectedItems().size() == 1 ) {
        this->ui->button_ConfApache_Blacklist_Remove->setEnabled( true );
        this->ui->button_ConfApache_Blacklist_Up->setEnabled( true );
        this->ui->button_ConfApache_Blacklist_Down->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfApache_Blacklist_List->selectedItems().at(0);
        const int max = this->ui->list_ConfApache_Blacklist_List->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfApache_Blacklist_Up->setEnabled( false );
            this->ui->button_ConfApache_Blacklist_Down->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfApache_Blacklist_List->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfApache_Blacklist_Up->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfApache_Blacklist_Down->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfApache_Blacklist_Remove->setEnabled( false );
        this->ui->button_ConfApache_Blacklist_Up->setEnabled( false );
        this->ui->button_ConfApache_Blacklist_Down->setEnabled( false );
    }
}
void MainWindow::on_button_ConfApache_Blacklist_Remove_clicked()
{
    const auto& item = this->ui->list_ConfApache_Blacklist_List->selectedItems().at(0);
    this->craplog.blacklistRemove(
        this->APACHE_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Field_currentTextChanged( this->ui->box_ConfApache_Blacklist_Field->currentText() );
}
void MainWindow::on_button_ConfApache_Blacklist_Up_clicked()
{
    const auto& item = this->ui->list_ConfApache_Blacklist_List->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveUp(
        this->APACHE_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Field_currentTextChanged( this->ui->box_ConfApache_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfApache_Blacklist_List->setFocus();
}
void MainWindow::on_button_ConfApache_Blacklist_Down_clicked()
{
    const auto& item = this->ui->list_ConfApache_Blacklist_List->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveDown(
        this->APACHE_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Field_currentTextChanged( this->ui->box_ConfApache_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfApache_Blacklist_List->setFocus();
}


////////////////
//// NGINX ////
// paths
void MainWindow::on_inLine_ConfNginx_Path_String_textChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        std::string path = this->resolvePath( arg1.toStdString() );
        if ( IOutils::checkDir( path ) ) {
            this->ui->icon_ConfNginx_Path_Wrong->setVisible( false );
            this->ui->button_ConfNginx_Path_Save->setEnabled( true );
        } else {
            this->ui->icon_ConfNginx_Path_Wrong->setVisible( true );
            this->ui->button_ConfNginx_Path_Save->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfNginx_Path_Wrong->setVisible( true );
        this->ui->button_ConfNginx_Path_Save->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfNginx_Path_String_returnPressed()
{
    this->on_button_ConfNginx_Path_Save_clicked();
}
void MainWindow::on_button_ConfNginx_Path_Save_clicked()
{
    if ( ! this->ui->icon_ConfNginx_Path_Wrong->isVisible() ) {
        // set the paths
        std::string path = this->resolvePath( this->ui->inLine_ConfNginx_Path_String->text().toStdString() );
        if ( StringOps::endsWith( path, "/" ) ) {
            path = StringOps::rstrip( path, "/" );
        }
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        this->craplog.setLogsPath( this->NGINX_ID, path );
        this->ui->inLine_ConfNginx_Path_String->setText( QString::fromStdString( path ) );
    }
    this->ui->button_ConfNginx_Path_Save->setEnabled( false );
}

// formats
void MainWindow::on_inLine_ConfNginx_Format_String_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfNginx_Format_Save->setEnabled( true );
    } else {
        this->ui->button_ConfNginx_Format_Save->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfNginx_Format_String_returnPressed()
{
    if ( this->ui->button_ConfNginx_Format_Save->isEnabled() ) {
        this->on_button_ConfNginx_Format_Save_clicked();
    }
}
void MainWindow::on_button_ConfNginx_Format_Save_clicked()
{
    const bool success = this->craplog.setNginxLogFormat(
        this->ui->inLine_ConfNginx_Format_String->text().toStdString() );
    if ( success ) {
        this->ui->button_ConfNginx_Format_Save->setEnabled( false );
    }
}
void MainWindow::on_button_ConfNginx_Format_Sample_clicked()
{
    this->ui->preview_ConfNginx_Format_Sample->setText(
        this->craplog.getLogsFormatSample( this->NGINX_ID ) );
}
void MainWindow::on_button_ConfNginx_Format_Help_clicked()
{
    this->showHelp( "nginx_format" );
}

// warnlists
void MainWindow::on_box_ConfNginx_Warnlist_Field_currentTextChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        this->ui->inLine_ConfNginx_Warnlist_String->clear();
        this->ui->list_ConfNginx_Warnlist_List->clear();
        // update the list
        const std::vector<std::string>& list = this->craplog.getWarnlist(
            this->NGINX_ID, this->crapview.getLogFieldID( arg1 ) );
        for ( const std::string& item : list ) {
            this->ui->list_ConfNginx_Warnlist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        const bool used = this->craplog.isWarnlistUsed(
            this->NGINX_ID,
            this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Field->currentText() ) );
        this->ui->checkBox_ConfNginx_Warnlist_Used->setChecked( used );
        this->on_checkBox_ConfNginx_Warnlist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfNginx_Warnlist_Used_clicked(bool checked)
{
    this->craplog.setWarnlistUsed(
        this->NGINX_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfNginx_Warnlist_String->setEnabled( true );
        this->ui->list_ConfNginx_Warnlist_List->setEnabled( true );
    } else {
        this->ui->inLine_ConfNginx_Warnlist_String->clear();
        this->ui->inLine_ConfNginx_Warnlist_String->setEnabled( false );
        this->ui->list_ConfNginx_Warnlist_List->clearSelection();
        this->ui->list_ConfNginx_Warnlist_List->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfNginx_Warnlist_String_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfNginx_Warnlist_Add->setEnabled( true );
    } else {
        this->ui->button_ConfNginx_Warnlist_Add->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfNginx_Warnlist_String_returnPressed()
{
    this->on_button_ConfNginx_Warnlist_Add_clicked();
}
void MainWindow::on_button_ConfNginx_Warnlist_Add_clicked()
{
    const QString& item = this->ui->inLine_ConfNginx_Warnlist_String->text();
    if ( this->ui->list_ConfNginx_Warnlist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        try {
            this->craplog.warnlistAdd(
                this->NGINX_ID,
                this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
                item.toStdString() );
            this->ui->list_ConfNginx_Warnlist_List->addItem( item );
        } catch ( const BWlistException& ) {
            DialogSec::warnInvalidItemBW();
            return;
        }
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfNginx_Warnlist_List->clearSelection();
    this->ui->list_ConfNginx_Warnlist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfNginx_Warnlist_String->clear();
}

void MainWindow::on_list_ConfNginx_Warnlist_List_itemSelectionChanged()
{
    if ( this->ui->list_ConfNginx_Warnlist_List->selectedItems().size() == 1 ) {
        this->ui->button_ConfNginx_Warnlist_Remove->setEnabled( true );
        this->ui->button_ConfNginx_Warnlist_Up->setEnabled( true );
        this->ui->button_ConfNginx_Warnlist_Down->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfNginx_Warnlist_List->selectedItems().at(0);
        const int max = this->ui->list_ConfNginx_Warnlist_List->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfNginx_Warnlist_Up->setEnabled( false );
            this->ui->button_ConfNginx_Warnlist_Down->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfNginx_Warnlist_List->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfNginx_Warnlist_Up->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfNginx_Warnlist_Down->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfNginx_Warnlist_Remove->setEnabled( false );
        this->ui->button_ConfNginx_Warnlist_Up->setEnabled( false );
        this->ui->button_ConfNginx_Warnlist_Down->setEnabled( false );
    }
}
void MainWindow::on_button_ConfNginx_Warnlist_Remove_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Warnlist_List->selectedItems().at(0);
    this->craplog.warnlistRemove(
        this->NGINX_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Field_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Field->currentText() );
}
void MainWindow::on_button_ConfNginx_Warnlist_Up_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Warnlist_List->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveUp(
        this->NGINX_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Field_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Warnlist_List->setFocus();
}
void MainWindow::on_button_ConfNginx_Warnlist_Down_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Warnlist_List->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveDown(
        this->NGINX_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Field_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Warnlist_List->setFocus();
}


// blacklist
void MainWindow::on_box_ConfNginx_Blacklist_Field_currentTextChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        this->ui->inLine_ConfNginx_Blacklist_String->clear();
        this->ui->list_ConfNginx_Blacklist_List->clear();
        // update the list
        const std::vector<std::string>& list = this->craplog.getBlacklist(
            this->NGINX_ID, this->crapview.getLogFieldID( arg1 ) );
        for ( const std::string& item : list ) {
            this->ui->list_ConfNginx_Blacklist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        bool used = this->craplog.isBlacklistUsed(
            this->NGINX_ID,
            this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Field->currentText() ) );
        this->ui->checkBox_ConfNginx_Blacklist_Used->setChecked( used );
        this->on_checkBox_ConfNginx_Blacklist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfNginx_Blacklist_Used_clicked(bool checked)
{
    this->craplog.setBlacklistUsed(
        this->NGINX_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfNginx_Blacklist_String->setEnabled( true );
        this->ui->list_ConfNginx_Blacklist_List->setEnabled( true );
    } else {
        this->ui->inLine_ConfNginx_Blacklist_String->clear();
        this->ui->inLine_ConfNginx_Blacklist_String->setEnabled( false );
        this->ui->list_ConfNginx_Blacklist_List->clearSelection();
        this->ui->list_ConfNginx_Blacklist_List->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfNginx_Blacklist_String_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfNginx_Blacklist_Add->setEnabled( true );
    } else {
        this->ui->button_ConfNginx_Blacklist_Add->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfNginx_Blacklist_String_returnPressed()
{
    this->on_button_ConfNginx_Blacklist_Add_clicked();
}
void MainWindow::on_button_ConfNginx_Blacklist_Add_clicked()
{
    const QString& item = this->ui->inLine_ConfNginx_Blacklist_String->text();
    if ( this->ui->list_ConfNginx_Blacklist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        try {
            this->craplog.blacklistAdd(
                this->NGINX_ID,
                this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
                item.toStdString() );
            this->ui->list_ConfNginx_Blacklist_List->addItem( item );
        } catch ( const BWlistException& ) {
            DialogSec::warnInvalidItemBW();
            return;
        }
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfNginx_Blacklist_List->clearSelection();
    this->ui->list_ConfNginx_Blacklist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfNginx_Blacklist_String->clear();
}

void MainWindow::on_list_ConfNginx_Blacklist_List_itemSelectionChanged()
{
    if ( this->ui->list_ConfNginx_Blacklist_List->selectedItems().size() == 1 ) {
        this->ui->button_ConfNginx_Blacklist_Remove->setEnabled( true );
        this->ui->button_ConfNginx_Blacklist_Up->setEnabled( true );
        this->ui->button_ConfNginx_Blacklist_Down->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfNginx_Blacklist_List->selectedItems().at(0);
        const int max = this->ui->list_ConfNginx_Blacklist_List->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfNginx_Blacklist_Up->setEnabled( false );
            this->ui->button_ConfNginx_Blacklist_Down->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfNginx_Blacklist_List->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfNginx_Blacklist_Up->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfNginx_Blacklist_Down->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfNginx_Blacklist_Remove->setEnabled( false );
        this->ui->button_ConfNginx_Blacklist_Up->setEnabled( false );
        this->ui->button_ConfNginx_Blacklist_Down->setEnabled( false );
    }
}
void MainWindow::on_button_ConfNginx_Blacklist_Remove_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Blacklist_List->selectedItems().at(0);
    this->craplog.blacklistRemove(
        this->NGINX_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Field_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Field->currentText() );
}
void MainWindow::on_button_ConfNginx_Blacklist_Up_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Blacklist_List->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveUp(
        this->NGINX_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Field_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Blacklist_List->setFocus();
}
void MainWindow::on_button_ConfNginx_Blacklist_Down_clicked()
{
    const auto& item = this->ui->list_ConfNginx_Blacklist_List->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveDown(
        this->NGINX_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Field_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Blacklist_List->setFocus();
}


////////////////
//// IIS ////
// paths
void MainWindow::on_inLine_ConfIis_Path_String_textChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        std::string path = this->resolvePath( arg1.toStdString() );
        if ( IOutils::checkDir( path ) ) {
            this->ui->icon_ConfIis_Path_Wrong->setVisible( false );
            this->ui->button_ConfIis_Path_Save->setEnabled( true );
        } else {
            this->ui->icon_ConfIis_Path_Wrong->setVisible( true );
            this->ui->button_ConfIis_Path_Save->setEnabled( false );
        }
    } else {
        this->ui->icon_ConfIis_Path_Wrong->setVisible( true );
        this->ui->button_ConfIis_Path_Save->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfIis_Path_String_returnPressed()
{
    this->on_button_ConfIis_Path_Save_clicked();
}
void MainWindow::on_button_ConfIis_Path_Save_clicked()
{
    if ( ! this->ui->icon_ConfIis_Path_Wrong->isVisible() ) {
        // set the paths
        std::string path = this->resolvePath( this->ui->inLine_ConfIis_Path_String->text().toStdString() );
        if ( StringOps::endsWith( path, "/" ) ) {
            path = StringOps::rstrip( path, "/" );
        }
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        this->craplog.setLogsPath( this->IIS_ID, path );
        this->ui->inLine_ConfIis_Path_String->setText( QString::fromStdString( path ) );
    }
    this->ui->button_ConfIis_Path_Save->setEnabled( false );
}

// formats
const int MainWindow::getIisLogsModule()
{
    int module = 0;
    if ( this->ui->radio_ConfIis_Format_NCSA->isChecked() ) {
        module = 1;
    } else if ( this->ui->radio_ConfIis_Format_IIS->isChecked() ) {
        module = 2;
    }
    return module;
}

void MainWindow::on_radio_ConfIis_Format_W3C_toggled(bool checked)
{
    if ( checked ) {
        const bool success = this->craplog.setIisLogFormat( "", 0 );
        if ( success ) {
            this->ui->inLine_ConfIis_Format_String->clear();
            this->ui->inLine_ConfIis_Format_String->setEnabled( true );
            this->ui->inLine_ConfIis_Format_String->setFocus();
        }
    }
}
void MainWindow::on_radio_ConfIis_Format_NCSA_toggled(bool checked)
{
    if ( checked ) {
        const bool success = this->craplog.setIisLogFormat(
            "c-ip s-sitename s-computername [date:time] sc-status sc-bytes",
            1 );
        if ( success ) {
            this->ui->inLine_ConfIis_Format_String->clear();
            this->ui->inLine_ConfIis_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( this->IIS_ID ) ) );
            this->ui->inLine_ConfIis_Format_String->setEnabled( false );
            this->ui->button_ConfIis_Format_Save->setEnabled( false );
        }
    }
}
void MainWindow::on_radio_ConfIis_Format_IIS_toggled(bool checked)
{
    if ( checked ) {
        const bool success = this->craplog.setIisLogFormat(
            "c-ip, cs-username, date, time, s-sitename, s-computername, s-ip, time-taken, cs-bytes, sc-bytes, sc-status, sc-win32-status, cs-method, cs-uri-stem, cs-uri-query,",
            2 );
        if ( success ) {
            this->ui->inLine_ConfIis_Format_String->clear();
            this->ui->inLine_ConfIis_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( this->IIS_ID ) ) );
            this->ui->inLine_ConfIis_Format_String->setEnabled( false );
            this->ui->button_ConfIis_Format_Save->setEnabled( false );
        }
    }
}

void MainWindow::on_inLine_ConfIis_Format_String_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfIis_Format_Save->setEnabled( true );
    } else {
        this->ui->button_ConfIis_Format_Save->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfIis_Format_String_returnPressed()
{
    if ( this->ui->button_ConfIis_Format_Save->isEnabled() ) {
        this->on_button_ConfIis_Format_Save_clicked();
    }
}
void MainWindow::on_button_ConfIis_Format_Save_clicked()
{
    const bool success = this->craplog.setIisLogFormat(
        StringOps::strip( this->ui->inLine_ConfIis_Format_String->text().toStdString() ),
        this->getIisLogsModule() );
    if ( success ) {
        this->ui->button_ConfIis_Format_Save->setEnabled( false );
    }
}
void MainWindow::on_button_ConfIis_Format_Sample_clicked()
{
    this->ui->preview_ConfIis_Format_Sample->setText(
        this->craplog.getLogsFormatSample( this->IIS_ID ) );
}
void MainWindow::on_button_ConfIis_Format_Help_clicked()
{
    this->showHelp( "iis_format" );
}

// warnlists
void MainWindow::on_box_ConfIis_Warnlist_Field_currentTextChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        this->ui->inLine_ConfIis_Warnlist_String->clear();
        this->ui->list_ConfIis_Warnlist_List->clear();
        // update the list
        const std::vector<std::string>& list = this->craplog.getWarnlist(
            this->IIS_ID, this->crapview.getLogFieldID( arg1 ) );
        for ( const std::string& item : list ) {
            this->ui->list_ConfIis_Warnlist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        const bool used = this->craplog.isWarnlistUsed(
            this->IIS_ID,
            this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Field->currentText() ) );
        this->ui->checkBox_ConfIis_Warnlist_Used->setChecked( used );
        this->on_checkBox_ConfIis_Warnlist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfIis_Warnlist_Used_clicked(bool checked)
{
    this->craplog.setWarnlistUsed(
        this->IIS_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfIis_Warnlist_String->setEnabled( true );
        this->ui->list_ConfIis_Warnlist_List->setEnabled( true );
    } else {
        this->ui->inLine_ConfIis_Warnlist_String->clear();
        this->ui->inLine_ConfIis_Warnlist_String->setEnabled( false );
        this->ui->list_ConfIis_Warnlist_List->clearSelection();
        this->ui->list_ConfIis_Warnlist_List->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfIis_Warnlist_String_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfIis_Warnlist_Add->setEnabled( true );
    } else {
        this->ui->button_ConfIis_Warnlist_Add->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfIis_Warnlist_String_returnPressed()
{
    this->on_button_ConfIis_Warnlist_Add_clicked();
}
void MainWindow::on_button_ConfIis_Warnlist_Add_clicked()
{
    const QString& item = this->ui->inLine_ConfIis_Warnlist_String->text();
    if ( this->ui->list_ConfIis_Warnlist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        try {
            this->craplog.warnlistAdd(
                this->IIS_ID,
                this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
                item.toStdString() );
            this->ui->list_ConfIis_Warnlist_List->addItem( item );
        } catch ( const BWlistException& ) {
            DialogSec::warnInvalidItemBW();
            return;
        }
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfIis_Warnlist_List->clearSelection();
    this->ui->list_ConfIis_Warnlist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfIis_Warnlist_String->clear();
}

void MainWindow::on_list_ConfIis_Warnlist_List_itemSelectionChanged()
{
    if ( this->ui->list_ConfIis_Warnlist_List->selectedItems().size() == 1 ) {
        this->ui->button_ConfIis_Warnlist_Remove->setEnabled( true );
        this->ui->button_ConfIis_Warnlist_Up->setEnabled( true );
        this->ui->button_ConfIis_Warnlist_Down->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfIis_Warnlist_List->selectedItems().at(0);
        const int max = this->ui->list_ConfIis_Warnlist_List->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfIis_Warnlist_Up->setEnabled( false );
            this->ui->button_ConfIis_Warnlist_Down->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfIis_Warnlist_List->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfIis_Warnlist_Up->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfIis_Warnlist_Down->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfIis_Warnlist_Remove->setEnabled( false );
        this->ui->button_ConfIis_Warnlist_Up->setEnabled( false );
        this->ui->button_ConfIis_Warnlist_Down->setEnabled( false );
    }
}
void MainWindow::on_button_ConfIis_Warnlist_Remove_clicked()
{
    const auto& item = this->ui->list_ConfIis_Warnlist_List->selectedItems().at(0);
    this->craplog.warnlistRemove(
        this->IIS_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Field_currentTextChanged( this->ui->box_ConfIis_Warnlist_Field->currentText() );
}
void MainWindow::on_button_ConfIis_Warnlist_Up_clicked()
{
    const auto& item = this->ui->list_ConfIis_Warnlist_List->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveUp(
        this->IIS_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Field_currentTextChanged( this->ui->box_ConfIis_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfIis_Warnlist_List->setFocus();
}
void MainWindow::on_button_ConfIis_Warnlist_Down_clicked()
{
    const auto& item = this->ui->list_ConfIis_Warnlist_List->selectedItems().at(0);
    const int i = this->craplog.warnlistMoveDown(
        this->IIS_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Field_currentTextChanged( this->ui->box_ConfIis_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfIis_Warnlist_List->setFocus();
}


// blacklist
void MainWindow::on_box_ConfIis_Blacklist_Field_currentTextChanged(const QString &arg1)
{
    if ( arg1.size() > 0 ) {
        this->ui->inLine_ConfIis_Blacklist_String->clear();
        this->ui->list_ConfIis_Blacklist_List->clear();
        // update the list
        const std::vector<std::string>& list = this->craplog.getBlacklist(
            this->IIS_ID, this->crapview.getLogFieldID( arg1 ) );
        for ( const std::string& item : list ) {
            this->ui->list_ConfIis_Blacklist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        bool used = this->craplog.isBlacklistUsed(
            this->IIS_ID,
            this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Field->currentText() ) );
        this->ui->checkBox_ConfIis_Blacklist_Used->setChecked( used );
        this->on_checkBox_ConfIis_Blacklist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfIis_Blacklist_Used_clicked(bool checked)
{
    this->craplog.setBlacklistUsed(
        this->IIS_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfIis_Blacklist_String->setEnabled( true );
        this->ui->list_ConfIis_Blacklist_List->setEnabled( true );
    } else {
        this->ui->inLine_ConfIis_Blacklist_String->clear();
        this->ui->inLine_ConfIis_Blacklist_String->setEnabled( false );
        this->ui->list_ConfIis_Blacklist_List->clearSelection();
        this->ui->list_ConfIis_Blacklist_List->setEnabled( false );
    }
}

void MainWindow::on_inLine_ConfIis_Blacklist_String_cursorPositionChanged(int arg1, int arg2)
{
    if ( arg2 > 0 ) {
        this->ui->button_ConfIis_Blacklist_Add->setEnabled( true );
    } else {
        this->ui->button_ConfIis_Blacklist_Add->setEnabled( false );
    }
}
void MainWindow::on_inLine_ConfIis_Blacklist_String_returnPressed()
{
    this->on_button_ConfIis_Blacklist_Add_clicked();
}
void MainWindow::on_button_ConfIis_Blacklist_Add_clicked()
{
    const QString& item = this->ui->inLine_ConfIis_Blacklist_String->text();
    if ( this->ui->list_ConfIis_Blacklist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).size() == 0 ) {
        // not in the list yet, append
        try {
            this->craplog.blacklistAdd(
                this->IIS_ID,
                this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
                item.toStdString() );
            this->ui->list_ConfIis_Blacklist_List->addItem( item );
        } catch ( const BWlistException& ) {
            DialogSec::warnInvalidItemBW();
            return;
        }
    }
    // select the item in the list, in both cases it was already in or it has been just inserted
    this->ui->list_ConfIis_Blacklist_List->clearSelection();
    this->ui->list_ConfIis_Blacklist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).at(0)->setSelected( true );
    this->ui->inLine_ConfIis_Blacklist_String->clear();
}

void MainWindow::on_list_ConfIis_Blacklist_List_itemSelectionChanged()
{
    if ( this->ui->list_ConfIis_Blacklist_List->selectedItems().size() == 1 ) {
        this->ui->button_ConfIis_Blacklist_Remove->setEnabled( true );
        this->ui->button_ConfIis_Blacklist_Up->setEnabled( true );
        this->ui->button_ConfIis_Blacklist_Down->setEnabled( true );
        // polishing
        const auto& item = this->ui->list_ConfIis_Blacklist_List->selectedItems().at(0);
        const int max = this->ui->list_ConfIis_Blacklist_List->count() -1;
        if ( max == 0 ) {
            this->ui->button_ConfIis_Blacklist_Up->setEnabled( false );
            this->ui->button_ConfIis_Blacklist_Down->setEnabled( false );
        } else {
            for ( int i=0; i<=max; i++ ) {
                if ( this->ui->list_ConfIis_Blacklist_List->item(i) == item ) {
                    if ( i == 0 ) {
                        this->ui->button_ConfIis_Blacklist_Up->setEnabled( false );
                    } else if ( i == max ) {
                        this->ui->button_ConfIis_Blacklist_Down->setEnabled( false );
                    }
                }
            }
        }
    } else {
        this->ui->button_ConfIis_Blacklist_Remove->setEnabled( false );
        this->ui->button_ConfIis_Blacklist_Up->setEnabled( false );
        this->ui->button_ConfIis_Blacklist_Down->setEnabled( false );
    }
}
void MainWindow::on_button_ConfIis_Blacklist_Remove_clicked()
{
    const auto& item = this->ui->list_ConfIis_Blacklist_List->selectedItems().at(0);
    this->craplog.blacklistRemove(
        this->IIS_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Field_currentTextChanged( this->ui->box_ConfIis_Blacklist_Field->currentText() );
}
void MainWindow::on_button_ConfIis_Blacklist_Up_clicked()
{
    const auto& item = this->ui->list_ConfIis_Blacklist_List->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveUp(
        this->IIS_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Field_currentTextChanged( this->ui->box_ConfIis_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfIis_Blacklist_List->setFocus();
}
void MainWindow::on_button_ConfIis_Blacklist_Down_clicked()
{
    const auto& item = this->ui->list_ConfIis_Blacklist_List->selectedItems().at(0);
    const int i = this->craplog.blacklistMoveDown(
        this->IIS_ID,
        this->crapview.getLogFieldID( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Field_currentTextChanged( this->ui->box_ConfIis_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfIis_Blacklist_List->setFocus();
}

