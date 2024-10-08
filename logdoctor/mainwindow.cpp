
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "main_lib.h"

#include "globals/db_names.h"
#include "globals/global_configs.h"
#include "globals/security.h"

#include "customs/logfile_treewidgetitem.h"
#include "customs/models/logfields_listmodel.h"
#include "customs/models/languages_listmodel.h"

#include "utilities/checks.h"
#include "utilities/colors.h"
#include "utilities/gzip.h"
#include "utilities/io.h"
#include "utilities/printables.h"
#include "utilities/rtf.h"
#include "utilities/stylesheets.h"
#include "utilities/vectors.h"

#include "modules/dialogs.h"
#include "modules/exceptions.h"
#include "modules/shared.h"

#include "modules/craphelp/craphelp.h"
#include "modules/crapup/crapup.h"
#include "modules/crapinfo/crapinfo.h"

#include "modules/changelog/changelog.h"

#include "modules/craplog/lib.h"

#include "modules/crapview/lib.h"
#include "modules/crapview/utilities/filters.h"

#include "modules/crapconf/lib.h"

#include "tools/crappath/crappath.h"
#include "tools/crapnote/crapnote.h"

#include "games/crisscross/game.h"
#include "games/snake/game.h"

#include <QCloseEvent>
#include <QTranslator>
#include <QThread>
#include <QTimer>
#include <QTreeWidget>
#include <QFontDatabase>
#include <QChartView>

#include <chrono>
#include <filesystem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{ parent }
    , ui{ new Ui::MainWindow }
    , tb_colors_schemes{ ColorSec::getColorSchemes() }
    , colors{ ColorSec::getColors() }
{
    //////////////////
    //// GRAPHICS ////
    this->ui->setupUi(this);

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
    this->fonts.emplace( "main", QFont(
        this->main_font_family,
        this->font_size ) );
    this->fonts.emplace( "main_italic", QFont(
        this->main_font_family,
        this->font_size,
        -1, true ) );
    this->fonts.emplace( "main_bold", QFont(
        this->main_font_family,
        this->font_size,
        1 ) );
    this->fonts.emplace( "main_big", QFont(
        this->main_font_family,
        this->font_size_big ) );
    this->fonts.emplace( "main_small", QFont(
        this->main_font_family,
        this->font_size_small ) );
    this->fonts.emplace( "alternative", QFont(
        this->alternative_font_family,
        this->font_size ) );
    this->fonts.emplace( "script", QFont(
        this->script_font_family,
        this->font_size ) );

    // parent fonts
    this->ui->mainwidget->setFont( this->fonts.at( "main_big" ) );

    // TextBrowser for the LogFiles
    this->TB.setColorScheme( ColorsScheme::Breeze, this->tb_colors_schemes.at( ColorsScheme::Breeze ) );
    this->TB.setFont( this->fonts.at( "main" ) );
    this->ui->textLogFiles->setFont( this->TB.getFont() );

    // adjust LogsList headers width
    this->ui->listLogFiles->header()->resizeSection(0,200);
    this->ui->listLogFiles->header()->resizeSection(1,100);

    // text browser's default message
    {
        QString rich_text;
        RichText::richLogsDefault( rich_text );
        this->ui->textLogFiles->setText( rich_text );
        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
    }


    //////////////
    //// MENU ////
    // tools
    connect( this->ui->actionBlockNote, &QAction::triggered, this, &MainWindow::menu_actionBlockNote_triggered );
    // utilities
    connect( this->ui->actionInfos, &QAction::triggered, this, &MainWindow::menu_actionInfos_triggered );
    connect( this->ui->actionChangelog, &QAction::triggered, this, &MainWindow::menu_actionChangelog_triggered );
    connect( this->ui->actionCheckUpdates, &QAction::triggered, this, &MainWindow::menu_actionCheckUpdates_triggered );
    // games
    connect( this->ui->actionCrissCross, &QAction::triggered, this, &MainWindow::menu_actionCrissCross_triggered );
    connect( this->ui->actionSnake, &QAction::triggered, this, &MainWindow::menu_actionSnake_triggered );


    /////////////////
    //// CONFIGS ////
    this->defineOSspec();
    this->readConfigs();


    /////////////////
    //// CRAPLOG ////
    connect( this, &MainWindow::refreshLogs, &this->craplog, &Craplog::scanLogsDir);
    connect( &this->craplog, &Craplog::pushLogFile, this, &MainWindow::appendToLogsList);
    connect( &this->craplog, &Craplog::finishedRefreshing, this, &MainWindow::refreshFinished);
    connect( this, &MainWindow::runCraplog, &this->craplog, &Craplog::startWorking);
    connect( &this->craplog, &Craplog::finishedWorking, this, &MainWindow::craplogFinished);


    //////////////////
    //// CRAPVIEW ////
    this->ui->box_StatsDay_LogsField->setModel( new DaytimeLogFieldsListModel() );
    this->ui->box_StatsRelat_LogsField_1->setModel( new RelationslLogFieldsListModel() );
    this->ui->box_StatsRelat_LogsField_2->setModel( new RelationslLogFieldsListModel() );


    //////////////////
    //// CRAPCONF ////
    {
        QSignalBlocker blocker( this->ui->box_ConfWindow_Language );
        this->ui->box_ConfWindow_Language->setModel( new LanguagesListModel() );
    }


    ///////////////////
    //// POLISHING ////
    // default tabs
    this->switchMainTab( 0 );
    this->switchStatsTab( 0 );

    // set the default WS as the current one
    switch ( this->default_web_server ) {
        case WS_APACHE:
            this->ui->button_LogFiles_Apache->setFlat( false );
            this->ui->radio_ConfDefaults_Apache->setChecked( true );
            break;
        case WS_NGINX:
            this->ui->button_LogFiles_Nginx->setFlat( false );
            this->ui->radio_ConfDefaults_Nginx->setChecked( true );
            break;
        case WS_IIS:
            this->ui->button_LogFiles_Iis->setFlat( false );
            this->ui->radio_ConfDefaults_Iis->setChecked( true );
            break;
        default:
            // shouldn't be here
            throw DoNotCatchException( "Unexpected WebServer", std::to_string(static_cast<int>(this->default_web_server)) );
    }
    this->craplog.setCurrentWebServer( this->default_web_server );


    // initialize the Configs
    this->ui->splitter_Conf->setSizes( {128,1} );
    this->ui->tree_ConfSections->expandAll();
    this->ui->tree_ConfSections->itemAt(0,0)->child(0)->setSelected( true );
    // window
    this->ui->checkBox_ConfWindow_Geometry->setChecked( this->remember_window );
    this->ui->box_ConfWindow_Theme->setCurrentIndex( static_cast<int>(GlobalConfigs::window_theme) );
    this->ui->box_ConfWindow_Icons->setCurrentIndex( static_cast<int>(GlobalConfigs::icons_theme) );
    this->ui->box_ConfWindow_Language->setCurrentIndex(
        dynamic_cast<LanguagesListModel*>( this->ui->box_ConfWindow_Language->model() )->indexOfLanguage( this->language )
    );
    // dialogs
    this->ui->slider_ConfDialogs_General->setValue( static_cast<int>(this->dialogs_level) );
    this->ui->slider_ConfDialogs_Logs->setValue( static_cast<int>(this->craplog.getDialogsLevel()) );
    this->ui->slider_ConfDialogs_Stats->setValue( static_cast<int>(this->crapview.getDialogsLevel()) );
    // text browser
    this->ui->box_ConfTextBrowser_Font->setCurrentText( this->TB.getFontFamily() );
    this->ui->checkBox_ConfTextBrowser_WideLines->setChecked( this->TB.getWideLinesUsage() );
    this->ui->box_ConfTextBrowser_ColorScheme->setCurrentIndex( static_cast<int>(this->TB.getColorSchemeID()) );
    this->refreshTextBrowserPreview();
    // charts
    this->ui->box_ConfCharts_Theme->setCurrentIndex( static_cast<int>(GlobalConfigs::charts_theme) );
    this->refreshChartsPreview();
    this->ui->box_ConfCharts_Speed_TimeInterval->setCurrentText( QString::number(this->crapview.getSpeedTimeInterval()) );
    this->ui->box_ConfCharts_Speed_TimeFormat->setCurrentText( this->crapview.getSpeedTimeFormat() );
    this->ui->doubleSpinBox_ConfCharts_Count_PieSize->setValue( this->crapview.getCountPieSize() );
    this->ui->spinBox_ConfCharts_Count_MaxSlices->setValue( this->crapview.getCountMaxSlices() );
    this->ui->box_ConfCharts_Relat_TimeFormat->setCurrentText( this->crapview.getRelatTimeFormat() );
    // databases
    this->ui->inLine_ConfDatabases_Data_Path->setText( QString::fromStdString( this->db_data_path.toString() ) );
    this->ui->button_ConfDatabases_Data_Save->setEnabled( false );
    this->ui->inLine_ConfDatabases_Hashes_Path->setText( QString::fromStdString( this->db_hashes_path.toString() ) );
    this->ui->button_ConfDatabases_Hashes_Save->setEnabled( false );
    this->ui->spinBox_ConfDatabases_NumBackups->setValue( this->db_backups_number );
    this->ui->checkBox_ConfDatabases_DoBackup->setChecked( this->db_do_backup );
    // security
    this->ui->checkBox_ConfSecurity_Paths_Symlinks->setChecked( GlobalConfigs::Security::follow_symlinks );
    // logs control
    this->ui->checkBox_ConfControl_Usage->setChecked( this->hide_used_files );
    this->ui->spinBox_ConfControl_Size->setValue( static_cast<int>(this->craplog.getWarningSize() / 1'048'576ul) );
    if ( this->craplog.getWarningSize() > 0 ) {
        this->ui->checkBox_ConfControl_Size->setChecked( true );
    } else {
        this->ui->checkBox_ConfControl_Size->setChecked( false );
    }
    // apache paths
    this->ui->inLine_ConfApache_Path_String->setText( QString::fromStdString(this->craplog.getLogsPath( WS_APACHE ).toString()) );
    this->ui->button_ConfApache_Path_Save->setEnabled( false );
    // apache formats
    this->ui->inLine_ConfApache_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( WS_APACHE ) ) );
    this->ui->button_ConfApache_Format_Save->setEnabled( false );
    // apache warnlists
    this->on_box_ConfApache_Warnlist_Field_currentTextChanged( this->ui->box_ConfApache_Warnlist_Field->currentText() );
    // apache blacklists
    this->on_box_ConfApache_Blacklist_Field_currentTextChanged( this->ui->box_ConfApache_Blacklist_Field->currentText() );
    // nginx paths
    this->ui->inLine_ConfNginx_Path_String->setText( QString::fromStdString(this->craplog.getLogsPath( WS_NGINX ).toString()) );
    this->ui->button_ConfNginx_Path_Save->setEnabled( false );
    // nginx formats
    this->ui->inLine_ConfNginx_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( WS_NGINX ) ) );
    this->ui->button_ConfNginx_Format_Save->setEnabled( false );
    // nginx warnlists
    this->on_box_ConfNginx_Warnlist_Field_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Field->currentText() );
    // nginx blacklists
    this->on_box_ConfNginx_Blacklist_Field_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Field->currentText() );
    // iis paths
    this->ui->inLine_ConfIis_Path_String->setText( QString::fromStdString(this->craplog.getLogsPath( WS_IIS ).toString()) );
    this->ui->button_ConfIis_Path_Save->setEnabled( false );
    // iis formats
    this->ui->inLine_ConfIis_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( WS_IIS ) ) );
    this->ui->button_ConfIis_Format_Save->setEnabled( false );
    // iis warnlists
    this->on_box_ConfIis_Warnlist_Field_currentTextChanged( this->ui->box_ConfIis_Warnlist_Field->currentText() );
    // iis blacklists
    this->on_box_ConfIis_Blacklist_Field_currentTextChanged( this->ui->box_ConfIis_Blacklist_Field->currentText() );


    ///////////////////
    //// INTERFACE ////
    this->updateUiLanguage();
    this->updateUiTheme();
    QChartView* charts[]{
        this->ui->chart_MakeStats_Size,
        this->ui->chart_ConfCharts_Preview,
        this->ui->chart_StatsWarn,
        this->ui->chart_StatsSpeed,
        this->ui->chart_StatsCount,
        this->ui->chart_StatsDay,
        this->ui->chart_StatsRelat
    };
    for ( auto chart : charts ) {
        ColorSec::applyChartTheme(
            this->fonts, chart );
    }
    this->ui->listLogFiles->sortByColumn( 0, Qt::SortOrder::AscendingOrder );


    ///////////////
    //// START ////
    // get a fresh list of LogFiles
    this->waiter_timer.reset( new QTimer(this) );
    connect( this->waiter_timer.get(), &QTimer::timeout,
             this, &MainWindow::waitActiveWindow);
    this->waiter_timer->start(250);
}

void MainWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event)
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
    #if defined( Q_OS_MACOS )
        this->db_data_path   = PathHandler( this->logdoc_path );
        this->db_hashes_path = PathHandler( this->logdoc_path );

    #elif defined( Q_OS_WINDOWS )
        this->db_data_path   = PathHandler( this->logdoc_path );
        this->db_hashes_path = PathHandler( this->logdoc_path );

    #elif defined( Q_OS_LINUX ) || defined( Q_OS_BSD4 )
        this->db_data_path   = PathHandler( this->home_path + "/.local/share/LogDoctor" );
        this->db_hashes_path = PathHandler( this->home_path + "/.local/share/LogDoctor" );

    #else
        // shouldn't be here
        static_assert( false, "Unexpected OS" );
    #endif
}

void MainWindow::readConfigs()
{
    std::error_code err;
    bool proceed{true};
    QString err_msg;

    if ( const auto path_exp{ this->configs_path.getPath() };
         !path_exp.has_value() && path_exp.error().isReasonSymlink() ) {
        const auto& invalid{ path_exp.error() };
        QString invalid_component;
        if ( this->dialogs_level > DL_ESSENTIAL ) {
            invalid_component = toQString( invalid.invalid_component );
        }
        DialogSec::errConfPathHasSymlink( toQString( invalid.full_path ), invalid_component );
        proceed &= false;
    } else {
        const path_t& conf_file{ this->configs_path.getPathUnchecked() };
        if ( ! IOutils::exists( conf_file ) ) {
            // configuration file not found
            QString file;
            if ( this->dialogs_level == DL_EXPLANATORY ) {
                file = toQString( conf_file );
            }
            DialogSec::warnConfFileNotFound( file );
            proceed &= false;

        } else if ( ! IOutils::checkFile( conf_file ) ) {
            // the given path doesn't point to a file
            const QString path_msg{ toQString( conf_file ) };
            if ( DialogSec::choiceFileNotFile( path_msg ) ) {
                if ( IOutils::renameAsCopy( conf_file, err ) ) {
                    return;
                }
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    err_msg = QString::fromStdString( err.message() );
                }
                DialogSec::errRenaming( path_msg, err_msg );
            }
            proceed &= false;

        } else if ( ! IOutils::checkFile( conf_file, true ) ) {
            // file not readable
            QString file;
            if ( this->dialogs_level > DL_ESSENTIAL ) {
                file = toQString( conf_file );
            }
            DialogSec::errConfFileNotReadable( file, err_msg );
            proceed &= false;
        }
    }
    if ( ! proceed ) {
        if ( ! DialogSec::choiceFailedApplyingConfigs( err_msg ) ) {
            // choosed to abort
            std::exit( 1 );
        }
        return;
    }

    QStringList invalid_lines;
    try {
        bool iis_module_set{ false };
        std::string iis_format_w3c;
        const std::string iis_format_ncsa{ "c-ip s-sitename s-computername [date:time] sc-status sc-bytes" };
        const std::string iis_format_iis{  "c-ip, cs-username, date, time, s-sitename, s-computername, s-ip, time-taken, cs-bytes, sc-bytes, sc-status, sc-win32-status, cs-method, cs-uri-stem, cs-uri-query," };
        const auto apply_iis_format{ [this,&invalid_lines](const std::string& line, const std::string& format, const IISLogsModule module)
            {
                if ( ! this->craplog.setIisLogFormat( format, module ) ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }
            }
        };

        std::string content;
        IOutils::readFile( this->configs_path.getPathUnchecked(), content );
        std::vector<std::string> configs;
        StringOps::splitrip( configs, content );
        for ( const std::string& line : configs ) {
            if ( StringOps::startsWith( line, '[') ) {
                // section descriptor
                continue;
            }
            std::vector<std::string> aux;
            StringOps::splitrip( aux, line, '=' );
            if ( aux.size() < 2 ) {
                // nothing to do
                continue;
            }
            // if here, a value is present
            const std::string& var{ aux.at( 0 ) };
            const std::string& val{ aux.at( 1 ) };

            if ( val.empty() ) {
                // nothing to do, no value stored
                continue;
            }

            if ( var == "Language" ) {
                if ( val.size() != 5 ) {
                    // not a valid locale, keep the default
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errLangLocaleInvalid( QString::fromStdString( val ) );
                } else {
                    if ( VecOps::contains( this->available_languages, val ) ) {
                        this->language = val;
                    } else {
                        invalid_lines.append( QString::fromStdString( line ) );
                        DialogSec::errLangNotAccepted( QString::fromStdString( val ) );
                    }
                }

            } else if ( var == "RememberGeometry" ) {
                try {
                    this->remember_window = this->s2b.at( val );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "Geometry" ) {
                this->setGeometryFromString( val );

            } else if ( var == "WindowTheme" ) {
                try {
                    GlobalConfigs::window_theme = static_cast<WindowTheme>( std::stoi( val ) );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "IconsTheme" ) {
                try {
                    GlobalConfigs::icons_theme = static_cast<IconsTheme>( std::stoi( val ) );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "ChartsTheme" ) {
                try {
                    GlobalConfigs::charts_theme = static_cast<ChartsTheme>( std::stoi( val ) );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "MainDialogsLevel" ) {
                try {
                    this->setDialogsLevelFromString( val );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "DefaultWebServer" ) {
                try {
                    this->setWebServerFromString( val );
                } catch ( const GenericException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        TR::tr("Unexpected WebServer"),
                        QString::fromStdString( val ) ) );
                }

            } else if ( var == "DatabaseDataPath" ) {
                this->db_data_path = PathHandler( val );

            } else if ( var == "DatabaseHashesPath" ) {
                this->db_hashes_path = PathHandler( val );

            } else if ( var == "DatabaseDoBackup" ) {
                try {
                    this->db_do_backup = this->s2b.at( val );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "DatabaseBackupsNumber" ) {
                try {
                    this->db_backups_number = std::stoi( val );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "FollowSymlinks" ) {
                try {
                    GlobalConfigs::Security::follow_symlinks = this->s2b.at( val );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "Font" ) {
                try {
                    this->on_box_ConfTextBrowser_Font_currentIndexChanged( std::stoi( val ) );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "WideLines" ) {
                try {
                    this->TB.setWideLinesUsage( this->s2b.at( val ) );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "ColorScheme" ) {
                try {
                    const int v{ std::stoi( val ) };
                    GlobalConfigs::colors_scheme = static_cast<ColorsScheme>( v );
                    this->on_box_ConfTextBrowser_ColorScheme_currentIndexChanged( v );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "CraplogDialogsLevel" ) {
                try {
                    this->craplog.setDialogsLevel( this->dialogsLevelFromInt( std::stoi( val ) ) );
                } catch ( ... ) { // std::exception / GenericException
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "HideUsedFiles" ) {
                try {
                    hide_used_files = this->s2b.at( val );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "WarningSize" ) {
                try {
                    this->craplog.setWarningSize( std::stoul( val ) );
                } catch ( const std::exception& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "ApacheLogsPath" ) {
                this->craplog.setLogsPath( WS_APACHE, PathHandler( val ) );

            } else if ( var == "ApacheLogsFormat" ) {
                if ( ! this->craplog.setApacheLogFormat( val ) ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "ApacheWarnlistMethod" ) {
                try {
                    this->warnlists.setList( WS_APACHE, WarnlistField::Method, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Apache -> %1 (%2)").arg(
                            TR::tr(FIELDS__METHOD.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "ApacheWarnlistMethodUsed" ) {
                this->warnlists.setUsed( WS_APACHE, WarnlistField::Method, this->s2b.at( val ) );

            } else if ( var == "ApacheWarnlistURI" ) {
                try {
                    this->warnlists.setList( WS_APACHE, WarnlistField::Uri, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Apache -> %1 (%2)").arg(
                            TR::tr(FIELDS__URI.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "ApacheWarnlistURIUsed" ) {
                this->warnlists.setUsed( WS_APACHE, WarnlistField::Uri, this->s2b.at( val ) );

            } else if ( var == "ApacheWarnlistClient" ) {
                try {
                    this->warnlists.setList( WS_APACHE, WarnlistField::Client, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Apache -> %1 (%2)").arg(
                            TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "ApacheWarnlistClientUsed" ) {
                this->warnlists.setUsed( WS_APACHE, WarnlistField::Client, this->s2b.at( val ) );

            } else if ( var == "ApacheWarnlistUserAgent" ) {
                try {
                    this->warnlists.setList( WS_APACHE, WarnlistField::UserAgent, this->string2list( val, true ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Apache -> %1 (%2)").arg(
                            TR::tr(FIELDS__USER_AGENT.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "ApacheWarnlistUserAgentUsed" ) {
                this->warnlists.setUsed( WS_APACHE, WarnlistField::UserAgent, this->s2b.at( val ) );

            } else if ( var == "ApacheBlacklistClient" ) {
                try {
                    this->blacklists.setList( WS_APACHE, BlacklistField::Client, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Apache -> %1 (%2)").arg(
                            TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("blacklist") ) ) );
                }

            } else if ( var == "ApacheBlacklistClientUsed" ) {
                this->blacklists.setUsed( WS_APACHE, BlacklistField::Client, this->s2b.at( val ) );

            } else if ( var == "NginxLogsPath" ) {
                this->craplog.setLogsPath( WS_NGINX, PathHandler( val ) );

            } else if ( var == "NginxLogsFormat" ) {
                if ( ! this->craplog.setNginxLogFormat( val ) ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "NginxWarnlistMethod" ) {
                try {
                    this->warnlists.setList( WS_NGINX, WarnlistField::Method, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Nginx -> %1 (%2)").arg(
                            TR::tr(FIELDS__METHOD.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "NginxWarnlistMethodUsed" ) {
                this->warnlists.setUsed( WS_NGINX, WarnlistField::Method, this->s2b.at( val ) );

            } else if ( var == "NginxWarnlistURI" ) {
                try {
                    this->warnlists.setList( WS_NGINX, WarnlistField::Uri, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Nginx -> %1 (%2)").arg(
                            TR::tr(FIELDS__URI.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "NginxWarnlistURIUsed" ) {
                this->warnlists.setUsed( WS_NGINX, WarnlistField::Uri, this->s2b.at( val ) );

            } else if ( var == "NginxWarnlistClient" ) {
                try {
                    this->warnlists.setList( WS_NGINX, WarnlistField::Client, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Nginx -> %1 (%2)").arg(
                            TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "NginxWarnlistClientUsed" ) {
                this->warnlists.setUsed( WS_NGINX, WarnlistField::Client, this->s2b.at( val ) );

            } else if ( var == "NginxWarnlistUserAgent" ) {
                try {
                    this->warnlists.setList( WS_NGINX, WarnlistField::UserAgent, this->string2list( val, true ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Nginx -> %1 (%2)").arg(
                            TR::tr(FIELDS__USER_AGENT.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "NginxWarnlistUserAgentUsed" ) {
                this->warnlists.setUsed( WS_NGINX, WarnlistField::UserAgent, this->s2b.at( val ) );

            } else if ( var == "NginxBlacklistClient" ) {
                try {
                    this->blacklists.setList( WS_NGINX, BlacklistField::Client, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("Nginx -> %1 (%2)").arg(
                            TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("blacklist") ) ) );
                }

            } else if ( var == "NginxBlacklistClientUsed" ) {
                this->blacklists.setUsed( WS_NGINX, BlacklistField::Client, this->s2b.at( val ) );

            } else if ( var == "IisLogsPath" ) {
                this->craplog.setLogsPath( WS_IIS, PathHandler( val ) );

            } else if ( var == "IisLogsModule" ) {
                if ( val == "0" ) {
                    this->ui->radio_ConfIis_Format_W3C->setChecked( true );
                    if ( ! iis_format_w3c.empty() ) {
                        apply_iis_format( line, iis_format_w3c, IISLogsModule::W3C );
                    }
                } else if ( val == "1" ) {
                    iis_format_w3c.clear();
                    this->ui->radio_ConfIis_Format_NCSA->setChecked( true );
                    apply_iis_format( line, iis_format_ncsa, IISLogsModule::NCSA );
                } else if ( val == "2" ) {
                    iis_format_w3c.clear();
                    this->ui->radio_ConfIis_Format_IIS->setChecked( true );
                    apply_iis_format( line, iis_format_iis, IISLogsModule::IIS );
                } else {
                    invalid_lines.append( QString::fromStdString( line ) );
                    iis_module_set &= false;
                    continue;
                }
                iis_module_set |= true;

            } else if ( var == "IisLogsFormat" ) {
                iis_format_w3c = val;
                if ( iis_module_set ) {
                    if ( this->ui->radio_ConfIis_Format_W3C->isChecked() ) {
                        apply_iis_format( line, iis_format_w3c, IISLogsModule::W3C );
                    } else if ( this->ui->radio_ConfIis_Format_NCSA->isChecked() ) {
                        iis_format_w3c.clear();
                        apply_iis_format( line, iis_format_ncsa, IISLogsModule::NCSA );
                    } else if ( this->ui->radio_ConfIis_Format_IIS->isChecked() ) {
                        iis_format_w3c.clear();
                        apply_iis_format( line, iis_format_iis, IISLogsModule::IIS );
                    } else {
                        // ... wtf?
                        throw("No radio button checked for the IIS logs module");
                    }
                }

            } else if ( var == "IisWarnlistMethod" ) {
                try {
                    this->warnlists.setList( WS_IIS, WarnlistField::Method, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("IIS -> %1 (%2)").arg(
                            TR::tr(FIELDS__METHOD.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "IisWarnlistMethodUsed" ) {
                this->warnlists.setUsed( WS_IIS, WarnlistField::Method, this->s2b.at( val ) );

            } else if ( var == "IisWarnlistURI" ) {
                try {
                    this->warnlists.setList( WS_IIS, WarnlistField::Uri, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("IIS -> %1 (%2)").arg(
                            TR::tr(FIELDS__URI.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "IisWarnlistURIUsed" ) {
                this->warnlists.setUsed( WS_IIS, WarnlistField::Uri, this->s2b.at( val ) );

            } else if ( var == "IisWarnlistClient" ) {
                try {
                    this->warnlists.setList( WS_IIS, WarnlistField::Client, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("IIS -> %1 (%2)").arg(
                            TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "IisWarnlistClientUsed" ) {
                this->warnlists.setUsed( WS_IIS, WarnlistField::Client, this->s2b.at( val ) );

            } else if ( var == "IisWarnlistUserAgent" ) {
                try {
                    this->warnlists.setList( WS_IIS, WarnlistField::UserAgent, this->string2list( val, true ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("IIS -> %1 (%2)").arg(
                            TR::tr(FIELDS__USER_AGENT.c_str()), MainWindow::tr("warnlist") ) ) );
                }

            } else if ( var == "IisWarnlistUserAgentUsed" ) {
                this->warnlists.setUsed( WS_IIS, WarnlistField::UserAgent, this->s2b.at( val ) );

            } else if ( var == "IisBlacklistClient" ) {
                try {
                    this->blacklists.setList( WS_IIS, BlacklistField::Client, this->string2list( val ) );
                } catch ( const BWlistException& ) {
                    invalid_lines.append( QString::fromStdString( line ) );
                    DialogSec::errFailedApplyingConfigsItem( QStringLiteral("%1:\n%2").arg(
                        DialogSec::tr("One of the lists has an invalid item"),
                        QStringLiteral("IIS -> %1 (%2)").arg(
                            TR::tr(FIELDS__CLIENT.c_str()), MainWindow::tr("blacklist") ) ) );
                }

            } else if ( var == "IisBlacklistClientUsed" ) {
                this->blacklists.setUsed( WS_IIS, BlacklistField::Client, this->s2b.at( val ) );

            } else if ( var == "CrapviewDialogsLevel" ) {
                try {
                    this->crapview.setDialogsLevel( this->dialogsLevelFromInt( std::stoi( val ) ) );
                } catch ( ... ) { // std::exception / GenericException
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "CrapviewSpeedTimeInterval" ) {
                try {
                    const qint64 value{ std::stoll( val ) };
                    if ( const int index{this->ui->box_ConfCharts_Speed_TimeInterval->findText( QString::fromStdString( val ) )}; index >= 0 ) {
                        this->crapview.setSpeedTimeInterval( value );
                    } else {
                        invalid_lines.append( QString::fromStdString( line ) );
                    }
                } catch ( ... ) { // std::exception
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "CrapviewSpeedTimeFormat" ) {
                if ( val == "hh:mm" || val == "hh" ) {
                    this->crapview.setSpeedTimeFormat( QString::fromStdString( val ) );
                } else {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "CrapviewCountPieSize" ) {
                try {
                    const qreal value{ std::stod( val ) };
                    if ( value >= 0.6 && value <= 0.8 ) {
                        this->crapview.setCountPieSize( value );
                    } else {
                        invalid_lines.append( QString::fromStdString( line ) );
                    }
                } catch ( ... ) { // std::exception
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "CrapviewCountMaxSlices" ) {
                try {
                    const int value{ std::stoi( val ) };
                    if ( value >= 1 && value <= 31 ) {
                        this->crapview.setCountMaxSlices( value );
                    } else {
                        invalid_lines.append( QString::fromStdString( line ) );
                    }
                } catch ( ... ) { // std::exception
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            } else if ( var == "CrapviewRelationalTimeFormat" ) {
                if ( val == "hh:mm" || val == "hh" ) {
                    this->crapview.setRelatTimeFormat( QString::fromStdString( val ) );
                } else {
                    invalid_lines.append( QString::fromStdString( line ) );
                }

            }/* else {
                // not valid
            }*/
        }

    } catch ( const std::ios_base::failure& ) {
        // failed reading
        proceed &= false;
        invalid_lines.clear();
        err_msg = DialogSec::tr("An error occured while reading the configuration file");
    } catch ( const LogDoctorException& ) {
        // something failed
        proceed &= false;
        invalid_lines.clear();
        err_msg = DialogSec::tr("An error occured while parsing configuration file's data");
    }

    if ( ! invalid_lines.isEmpty() ) {
        proceed &= false;
        DialogSec::warnInvalidConfigsList( invalid_lines );
        err_msg = DialogSec::tr("An error occured while parsing configuration file's data");
    }
    if ( ! proceed ) {
        if ( ! DialogSec::choiceFailedApplyingConfigs( err_msg ) ) {
            // choosed to abort
            std::exit( 1 );
        }
    }
}

void MainWindow::writeConfigs()
{
    std::error_code err;
    bool proceed{true};
    QString msg, err_msg;

    if ( const auto path_exp{ this->configs_path.getPath() };
         !path_exp.has_value() && path_exp.error().isReasonSymlink() ) {
        const auto& invalid{ path_exp.error() };
        QString invalid_component;
        if ( this->dialogs_level > DL_ESSENTIAL ) {
            invalid_component = toQString( invalid.invalid_component );
        }
        DialogSec::errConfPathHasSymlink( toQString( invalid.full_path ), invalid_component );
        proceed &= false;
    } else {
        const path_t& conf_file{ this->configs_path.getPathUnchecked() };
        if ( ! IOutils::exists( conf_file ) ) {
            // file does not exists, check if at least the folder exists
            const auto base_path{ this->configs_path.getParentUnchecked() };
            if ( ! IOutils::exists( base_path ) ) {
                // the configs folder does not exist too
                if ( ! IOutils::makeDir( base_path, err ) ) {
                    msg = DialogSec::tr("Failed to create the configuration file's directory");
                    if ( this->dialogs_level > DL_ESSENTIAL ) {
                        msg += QLatin1String(":\n%1").arg( base_path.c_str() );
                        err_msg = QString::fromStdString( err.message() );
                    }
                    proceed &= false;
                }

            } else if ( ! IOutils::isDir( base_path ) ) {
                // not a directory
                const QString path_msg{ toQString( base_path ) };
                if ( DialogSec::choiceDirNotDir( path_msg ) ) {
                    if ( ! IOutils::renameAsCopy( base_path, err ) ) {
                        if ( this->dialogs_level > DL_ESSENTIAL ) {
                            err_msg = QString::fromStdString( err.message() );
                        }
                        DialogSec::errRenaming( path_msg, err_msg );
                        proceed &= false;
                    } else {
                        // make the new folder
                        if ( ! IOutils::makeDir( base_path, err ) ) {
                            msg = DialogSec::tr("Failed to create the configuration file's directory");
                            if ( this->dialogs_level > DL_ESSENTIAL ) {
                                msg += QLatin1String(":\n%1").arg( path_msg );
                                err_msg = QString::fromStdString( err.message() );
                            }
                            proceed &= false;
                        }
                    }
                } else {
                    // choosed not to rename the entry as '.copy'
                    msg = DialogSec::tr("Failed to create the configuration file");
                    if ( this->dialogs_level > DL_ESSENTIAL ) {
                        msg += QLatin1String(":\n%1").arg( path_msg );
                    }
                    proceed &= false;
                }

            } else if ( ! IOutils::checkDir( base_path, false, true ) ) {
                QString file;
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    file = toQString( base_path );
                }
                DialogSec::errConfDirNotWritable( file, err_msg );
                proceed &= false;
            }

        } else if ( ! IOutils::checkFile( conf_file ) ) {
            // the given path doesn't point to a file
            const QString path_msg{ toQString( conf_file ) };
            if ( DialogSec::choiceFileNotFile( path_msg ) ) {
                if ( ! IOutils::renameAsCopy( conf_file, err ) ) {
                    if ( this->dialogs_level > DL_ESSENTIAL ) {
                        err_msg = QString::fromStdString( err.message() );
                    }
                    DialogSec::errRenaming( path_msg, err_msg );
                    return;
                }
            } else {
                // choosed not to rename the entry as '.copy'
                msg = DialogSec::tr("Failed to create the configuration file");
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    msg += QLatin1String(":\n%1").arg( path_msg );
                }
                proceed &= false;
            }

        } else if ( ! IOutils::checkFile( conf_file, false, true ) ) {
            // file not writable
            QString file;
            if ( this->dialogs_level > DL_ESSENTIAL ) {
                file = toQString( conf_file );
            }
            DialogSec::errConfFileNotWritable( file, err_msg );
            return;
        }
    }
    if ( !proceed ) {
        DialogSec::errConfFailedWriting( msg, err_msg );
        return;
    }

    //// USER INTERFACE ////
    std::string configs;
    configs += "\n[UI]";
    configs += "\nLanguage=" + this->language;
    configs += "\nRememberGeometry=" + this->b2s.at( this->remember_window );
    configs += "\nGeometry=" + this->geometryToString();
    configs += "\nWindowTheme=" + std::to_string( static_cast<themes_t>(GlobalConfigs::window_theme) );
    configs += "\nIconsTheme=" + std::to_string( static_cast<themes_t>(GlobalConfigs::icons_theme) );
    configs += "\nChartsTheme=" + std::to_string( static_cast<themes_t>(GlobalConfigs::charts_theme) );
    configs += "\nMainDialogsLevel=" + toString( this->dialogs_level );
    configs += "\nDefaultWebServer=" + toString( this->default_web_server );
    configs += "\nDatabaseDataPath=" + this->db_data_path.toString();
    configs += "\nDatabaseHashesPath=" + this->db_hashes_path.toString();
    configs += "\nDatabaseDoBackup=" + this->b2s.at( this->db_do_backup );
    configs += "\nDatabaseBackupsNumber=" + std::to_string( this->db_backups_number );
    //// SECURITY ////
    configs += "\n\n[Security]";
    configs += "\nFollowSymlinks=" + this->b2s.at(GlobalConfigs::Security::follow_symlinks);
    //// TEXT BROWSER ////
    configs += "\n\n[TextBrowser]";
    configs += "\nFont=" + std::to_string( this->ui->box_ConfTextBrowser_Font->currentIndex() );
    configs += "\nWideLines=" + this->b2s.at( this->TB.getWideLinesUsage() );
    configs += "\nColorScheme=" + std::to_string( static_cast<themes_t>(GlobalConfigs::colors_scheme) );
    //// CRAPLOG ////
    configs += "\n\n[Craplog]";
    configs += "\nCraplogDialogsLevel=" + toString( this->craplog.getDialogsLevel() );
    configs += "\nHideUsedFiles=" + this->b2s.at( this->hide_used_files );
    configs += "\nWarningSize=" + std::to_string( this->craplog.getWarningSize() );
    //// APACHE2 ////
    configs += "\n\n[Apache2]";
    configs += "\nApacheLogsPath=" + this->craplog.getLogsPath( WS_APACHE ).getPathUnchecked().string();
    configs += "\nApacheLogsFormat=" + this->craplog.getLogsFormatString( WS_APACHE );
    configs += "\nApacheWarnlistMethod=" + this->list2string( this->warnlists.getListConst( WS_APACHE, WarnlistField::Method ) );
    configs += "\nApacheWarnlistMethodUsed=" + this->b2s.at( this->warnlists.isUsed( WS_APACHE, WarnlistField::Method ) );
    configs += "\nApacheWarnlistURI=" + this->list2string( this->warnlists.getListConst( WS_APACHE, WarnlistField::Uri ) );
    configs += "\nApacheWarnlistURIUsed=" + this->b2s.at( this->warnlists.isUsed( WS_APACHE, WarnlistField::Uri ) );
    configs += "\nApacheWarnlistClient=" + this->list2string( this->warnlists.getListConst( WS_APACHE, WarnlistField::Client ) );
    configs += "\nApacheWarnlistClientUsed=" + this->b2s.at( this->warnlists.isUsed( WS_APACHE, WarnlistField::Client ) );
    configs += "\nApacheWarnlistUserAgent=" + this->list2string( this->warnlists.getListConst( WS_APACHE, WarnlistField::UserAgent ), true );
    configs += "\nApacheWarnlistUserAgentUsed=" + this->b2s.at( this->warnlists.isUsed( WS_APACHE, WarnlistField::UserAgent ) );
    configs += "\nApacheBlacklistClient=" + this->list2string( this->blacklists.getListConst( WS_APACHE, BlacklistField::Client ) );
    configs += "\nApacheBlacklistClientUsed=" + this->b2s.at( this->blacklists.isUsed( WS_APACHE, BlacklistField::Client ) );
    //// NGINX ////
    configs += "\n\n[Nginx]";
    configs += "\nNginxLogsPath=" + this->craplog.getLogsPath( WS_NGINX ).getPathUnchecked().string();
    configs += "\nNginxLogsFormat=" + this->craplog.getLogsFormatString( WS_NGINX );
    configs += "\nNginxWarnlistMethod=" + this->list2string( this->warnlists.getListConst( WS_NGINX, WarnlistField::Method ) );
    configs += "\nNginxWarnlistMethodUsed=" + this->b2s.at( this->warnlists.isUsed( WS_NGINX, WarnlistField::Method ) );
    configs += "\nNginxWarnlistURI=" + this->list2string( this->warnlists.getListConst( WS_NGINX, WarnlistField::Uri ) );
    configs += "\nNginxWarnlistURIUsed=" + this->b2s.at( this->warnlists.isUsed( WS_NGINX, WarnlistField::Uri ) );
    configs += "\nNginxWarnlistClient=" + this->list2string( this->warnlists.getListConst( WS_NGINX, WarnlistField::Client ) );
    configs += "\nNginxWarnlistClientUsed=" + this->b2s.at( this->warnlists.isUsed( WS_NGINX, WarnlistField::Client ) );
    configs += "\nNginxWarnlistUserAgent=" + this->list2string( this->warnlists.getListConst( WS_NGINX, WarnlistField::UserAgent ), true );
    configs += "\nNginxWarnlistUserAgentUsed=" + this->b2s.at( this->warnlists.isUsed( WS_NGINX, WarnlistField::UserAgent ) );
    configs += "\nNginxBlacklistClient=" + this->list2string( this->blacklists.getListConst( WS_NGINX, BlacklistField::Client ) );
    configs += "\nNginxBlacklistClientUsed=" + this->b2s.at( this->blacklists.isUsed( WS_NGINX, BlacklistField::Client ) );
    //// IIS ////
    configs += "\n\n[IIS]";
    configs += "\nIisLogsPath=" + this->craplog.getLogsPath( WS_IIS ).getPathUnchecked().string();
    std::string module{ "0" };
    if ( this->ui->radio_ConfIis_Format_NCSA->isChecked() ) {
        module = "1";
    } else if ( this->ui->radio_ConfIis_Format_IIS->isChecked() ) {
        module = "2";
    }
    configs += "\nIisLogsModule=" + module;
    configs += "\nIisLogsFormat=" + this->craplog.getLogsFormatString( WS_IIS );
    configs += "\nIisWarnlistMethod=" + this->list2string( this->warnlists.getListConst( WS_IIS, WarnlistField::Method ) );
    configs += "\nIisWarnlistMethodUsed=" + this->b2s.at( this->warnlists.isUsed( WS_IIS, WarnlistField::Method ) );
    configs += "\nIisWarnlistURI=" + this->list2string( this->warnlists.getListConst( WS_IIS, WarnlistField::Uri ) );
    configs += "\nIisWarnlistURIUsed=" + this->b2s.at( this->warnlists.isUsed( WS_IIS, WarnlistField::Uri ) );
    configs += "\nIisWarnlistClient=" + this->list2string( this->warnlists.getListConst( WS_IIS, WarnlistField::Client ) );
    configs += "\nIisWarnlistClientUsed=" + this->b2s.at( this->warnlists.isUsed( WS_IIS, WarnlistField::Client ) );
    configs += "\nIisWarnlistUserAgent=" + this->list2string( this->warnlists.getListConst( WS_IIS, WarnlistField::UserAgent ), true );
    configs += "\nIisWarnlistUserAgentUsed=" + this->b2s.at( this->warnlists.isUsed( WS_IIS, WarnlistField::UserAgent ) );
    configs += "\nIisBlacklistClient=" + this->list2string( this->blacklists.getListConst( WS_IIS, BlacklistField::Client ) );
    configs += "\nIisBlacklistClientUsed=" + this->b2s.at( this->blacklists.isUsed( WS_IIS, BlacklistField::Client ) );
    //// CRAPVIEW ////
    configs += "\n\n[Crapview]";
    configs += "\nCrapviewDialogsLevel=" + toString( this->crapview.getDialogsLevel() );
    configs += "\nCrapviewSpeedTimeInterval=" + std::to_string( this->crapview.getSpeedTimeInterval() );
    configs += "\nCrapviewSpeedTimeFormat=" + this->crapview.getSpeedTimeFormat().toStdString();
    configs += "\nCrapviewCountPieSize=" + std::to_string( this->crapview.getCountPieSize() );
    configs += "\nCrapviewCountMaxSlices=" + std::to_string( this->crapview.getCountMaxSlices() );
    configs += "\nCrapviewRelationalTimeFormat=" + this->crapview.getRelatTimeFormat().toStdString();

    try {
        IOutils::writeOnFile( this->configs_path.getPath().value(), configs );

    } catch ( const std::bad_expected_access<InvalidPath>& /*err*/ ) {
        // invalid path
        DialogSec::errGeneric( DialogSec::tr("An error occured while writing the configuration file") );
    } catch ( const std::ios_base::failure& /*err*/ ) {
        // failed writing
        DialogSec::errGeneric( DialogSec::tr("An error occured while writing the configuration file") );
    } catch (...) {
        // something failed
        DialogSec::errGeneric( DialogSec::tr("An error occured while preparing the configuration file's data") );
    }
}


void MainWindow::backupDatabase() const
{
    std::error_code err;
    QString err_msg;
    const PathHandler db_path{ this->db_data_path / std::string(DatabasesNames::data) };
    const auto path_exp{ db_path.getPath() };
    if ( ! path_exp.has_value() ) {
        const auto invalid{ path_exp.error() };
        if ( invalid.isReasonSymlink() ) {
            QString invalid_component;
            if ( this->dialogs_level > DL_ESSENTIAL ) {
                invalid_component = toQString( invalid.invalid_component );
            }
            DialogSec::errDatabaseFailedBackup( DialogSec::tr("The path contains a symlink"), invalid_component, false );
            return;
        }
    }
    const path_t& db_file{ path_exp.value() };
    if ( ! IOutils::checkFile( db_file ) ) {
        // database file does not exists or is not a file
        QString file;
        if ( this->dialogs_level > DL_ESSENTIAL ) {
            file = toQString( db_file );
        }
        DialogSec::errDatabaseFailedBackup( DialogSec::tr("The file does not exist"), file, false );
        return;
    }
    const path_t backups_dir{ this->db_data_path.getPathUnchecked() / "backups" };
    if ( IOutils::isSymlink( backups_dir ) ) {
        DialogSec::errDatabaseFailedBackup( DialogSec::tr("The path contains a symlink"), toQString( backups_dir ), false );
        return;
    } else if ( ! IOutils::exists( backups_dir ) ) {
        // backups directory doesn't exists, make it
        if ( ! IOutils::makeDir( backups_dir, err ) ) {
            QString msg = DialogSec::tr("Failed to create the database backups' directory");
            if ( this->dialogs_level > DL_ESSENTIAL ) {
                msg += QLatin1String(":\n%1").arg( backups_dir.c_str() );
                if ( err.value() ) {
                    err_msg = QString::fromStdString( err.message() );
                }
            }
            DialogSec::errFailedMakeDir( msg, err_msg );
            DialogSec::errDatabaseFailedBackup( QString(), QString() );
            return;
        }
    } else if ( ! IOutils::isDir( backups_dir ) ) {
        // exists but it's not a directory, rename as copy and make a new one
        if ( DialogSec::choiceDirNotDir( toQString( backups_dir ) ) ) {
            if ( ! IOutils::renameAsCopy( backups_dir, err ) ) {
                QString path_msg;
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    path_msg = toQString( backups_dir );
                    if ( err.value() ) {
                        err_msg = QString::fromStdString( err.message() );
                    }
                }
                DialogSec::errRenaming( path_msg, err_msg );
                DialogSec::errDatabaseFailedBackup( QString(), QString() );
                return;
            }
            // sucesfully renamed, make the new one
            if ( ! IOutils::makeDir( backups_dir, err ) ) {
                QString msg = DialogSec::tr("Failed to create the database backups' directory");
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    msg += QLatin1String(":\n%1").arg( backups_dir.c_str() );
                    if ( err.value() ) {
                        err_msg = QString::fromStdString( err.message() );
                    }
                }
                DialogSec::errFailedMakeDir( msg, err_msg );
                DialogSec::errDatabaseFailedBackup( QString(), QString() );
                return;
            }
        } else {
            // choosed not to rename the entry as '.copy'
            DialogSec::errDatabaseFailedBackup( QString(), QString() );
            return;
        }
    }

    const path_t base_file{(backups_dir / DatabasesNames::data).concat(".0") };
    bool proceed = std::filesystem::copy_file(
        db_file,
        base_file,
        std::filesystem::copy_options::update_existing,
        err );
    if ( ! proceed ) {
        // failed to copy
        if ( this->dialogs_level > DL_ESSENTIAL && err.value() ) {
            err_msg = QString::fromStdString( err.message() );
        }
        DialogSec::errDatabaseFailedBackup( DialogSec::tr( "Failed to copy the database file" ), err_msg );
        return;
    }

    // succesfully copied, now rename the already existing copies (up to the choosen number of copies)
    const path_t backup_file{ path_t(base_file).replace_extension(std::to_string(this->db_backups_number)) };
    if ( std::filesystem::exists( backup_file ) ) {
        std::ignore = std::filesystem::remove_all( backup_file, err );
        if ( err.value() ) {
            err_msg = QString::fromStdString( err.message() );
            proceed &= false;
        } else {
            proceed = ! std::filesystem::exists( backup_file );
        }
        if ( ! proceed ) {
            DialogSec::errDatabaseFailedBackup( DialogSec::tr( "Failed to update the backups" ), err_msg );
            return;
        }
    }
    // cascade rename
    for ( int n=this->db_backups_number-1; n>=0; --n ) {
        const path_t path{ path_t(base_file).replace_extension(std::to_string(n)) };
        if ( std::filesystem::exists( path ) ) {
            const path_t new_path{ path_t(base_file).replace_extension(std::to_string(n+1)) };
            std::filesystem::rename( path, new_path, err );
            if ( err.value() ) {
                err_msg = QString::fromStdString( err.message() );
                proceed &= false;
            } else {
                proceed = ! std::filesystem::exists( path );
            }
            if ( ! proceed ) {
                DialogSec::errDatabaseFailedBackup( DialogSec::tr( "Failed to update the backups" ), err_msg );
                return;
            }
        }
    }
}


std::string MainWindow::geometryToString() const
{
    const QRect& geometry{ this->geometry() };
    std::string string;
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
void MainWindow::setGeometryFromString( const std::string& geometry )
{
    std::vector<std::string> aux;
    StringOps::splitrip( aux, geometry, ',' );
    QRect new_geometry;
    new_geometry.setRect( std::stoi(aux.at(0)), std::stoi(aux.at(1)), std::stoi(aux.at(2)), std::stoi(aux.at(3)) );
    this->setGeometry( new_geometry );
    if ( aux.at(4) == "true" ) {
        this->showMaximized();
    }
}

void MainWindow::setDialogsLevelFromString( const std::string& dialogs_level )
{
    this->dialogs_level = this->dialogsLevelFromInt( std::stoi( dialogs_level ) );
}
DialogsLevel MainWindow::dialogsLevelFromInt( const int dialogs_level )
{
    if ( dialogs_level >= 0 && dialogs_level <= 2) {
        return static_cast<DialogsLevel>( dialogs_level );
    } else {
        throw DoNotCatchException( "Unexpected DialogsLevel", std::to_string(dialogs_level) );
    }
}

void MainWindow::setWebServerFromString(const std::string& web_server )
{
    // 11,12,13 are kept for retro-compatibility, to be removed in a later stage
    if ( web_server == "Apache" || web_server == "11" ) {
        this->default_web_server = WebServer::Apache;
    } else if ( web_server == "Nginx" || web_server == "12" ) {
        this->default_web_server = WebServer::Nginx;
    } else if ( web_server == "IIS" || web_server == "13" ) {
        this->default_web_server = WebServer::IIS;
    } else {
        throw DoNotCatchException( "Unexpected WebServer", web_server );
    }
}


std::string MainWindow::list2string( const std::vector<std::string>& list, const bool user_agent ) const
{
    if ( user_agent ) {
        return std::accumulate(
            list.begin(), list.end(), std::string{},
            [&]( auto s, auto str ) -> std::string
               { return std::move(s) + StringOps::replace( str, " ", "%@#" ) + " "; } );

    } else {
        return std::accumulate(
            list.begin(), list.end(), std::string{},
            []( auto s, auto str ) -> std::string
              { return std::move(s) + std::move(str) + " "; } );
    }
}
std::vector<std::string> MainWindow::string2list( const std::string& string, const bool user_agent ) const
{
    std::vector<std::string> list, aux;
    StringOps::splitrip( aux, string, ' ' );
    if ( user_agent ) {
        list.reserve( aux.size() );
        std::transform( aux.cbegin(), aux.cend(),
                        std::back_inserter( list ),
                        [&]( auto str )
                           { return StringOps::replace( str, "%@#", " " ); } );
        return list;
    } else {
        return aux;
    }
}


//////////////////
//// GRAPHICS ////
//////////////////
void MainWindow::detectIconsTheme()
{
    switch ( GlobalConfigs::window_theme ) {
        case WindowTheme::Native:
            // use window color to determine the theme
            if ( this->palette().window().color().black() > 127 ) {
                GlobalConfigs::icons_set = "light_native";
            } else {
                GlobalConfigs::icons_set = "dark_native";
            }
            break;
        case WindowTheme::Light:
            GlobalConfigs::icons_set = "dark";
            break;
        case WindowTheme::Dark:
            GlobalConfigs::icons_set = "light";
            break;
        default:
            throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
            break;
    }
}


void MainWindow::updateUiTheme()
{
    // window and fonts
    switch ( GlobalConfigs::window_theme ) {
        case WindowTheme::Native:
            // window first
            this->setStyleSheet("");
            // icons last
            this->updateUiIcons();
            break;
        case WindowTheme::Light:
            [[fallthrough]];
        case WindowTheme::Dark:
            // icons first
            this->updateUiIcons();
            // window last
            this->setStyleSheet( StyleSec::getStyleSheet() );
            break;
        default:
            // wrong
            throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
            break;
    }
    // fallback stylesheets
    this->stylesheet_lineedit = QLineEdit().styleSheet();
    // fonts
    this->updateUiFonts();
}

void MainWindow::updateUiIcons()
{
    const QString old_icons_set{ GlobalConfigs::icons_set };
    switch ( GlobalConfigs::icons_theme ) {
        case IconsTheme::Auto:
            this->detectIconsTheme();
            break;
        case IconsTheme::Light:
            GlobalConfigs::icons_set = ( GlobalConfigs::window_theme == WindowTheme::Native )
                                       ? "light_native"
                                       : "light";
            break;
        case IconsTheme::Dark:
            GlobalConfigs::icons_set = ( GlobalConfigs::window_theme == WindowTheme::Native )
                                       ? "dark_native"
                                       : "dark";
            break;
        default:
            throw DoNotCatchException( "Unexpected IconsTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::icons_theme)) );
            break;
    }

    if ( GlobalConfigs::icons_set != old_icons_set ) {
        // main tabs
        const int m_index{ this->ui->stackedPages_Sections->currentIndex() };
        this->ui->button_Tab_Log->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/log_%2.png").arg(
                GlobalConfigs::icons_set,
                (m_index==0) ? "on" : "off" )) );
        this->ui->button_Tab_View->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/view_%2.png").arg(
                GlobalConfigs::icons_set,
                (m_index==1) ? "on" : "off" )) );
        this->ui->button_Tab_Conf->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/conf_%2.png").arg(
                GlobalConfigs::icons_set,
                (m_index==2) ? "on" : "off" )) );
        // view logs
        this->ui->button_LogFiles_ViewFile->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/show_file.png").arg(GlobalConfigs::icons_set)) );
        this->ui->button_LogFiles_RefreshList->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/refresh.png").arg(GlobalConfigs::icons_set)) );
        // parse logs
        this->ui->icon_MakeStats_Size->setPixmap(
            QPixmap(QStringLiteral(":/icons/icons/%1/mk_size.png").arg(GlobalConfigs::icons_set)) );
        this->ui->icon_MakeStats_Lines->setPixmap(
            QPixmap(QStringLiteral(":/icons/icons/%1/mk_lines.png").arg(GlobalConfigs::icons_set)) );
        this->ui->icon_MakeStats_Time->setPixmap(
            QPixmap(QStringLiteral(":/icons/icons/%1/mk_time.png").arg(GlobalConfigs::icons_set)) );
        this->ui->icon_MakeStats_Speed->setPixmap(
            QPixmap(QStringLiteral(":/icons/icons/%1/mk_speed.png").arg(GlobalConfigs::icons_set)) );
        // stats
        const int s_index{ this->ui->stackedPages_Stats->currentIndex() };
        // stats warn
        this->ui->button_Tab_StatsWarn->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/warn_%2.png").arg(
                GlobalConfigs::icons_set,
                (s_index==0) ? "on" : "off" )) );
        this->ui->button_StatsWarn_Draw->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/draw.png").arg(GlobalConfigs::icons_set)) );
        // stats speed
        this->ui->button_Tab_StatsSpeed->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/speed_%2.png").arg(
                GlobalConfigs::icons_set,
                (s_index==1) ? "on" : "off" )) );
        this->ui->button_StatsSpeed_Draw->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/draw.png").arg(GlobalConfigs::icons_set)) );
        // stats count
        this->ui->button_Tab_StatsCount->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/count_%2.png").arg(
                GlobalConfigs::icons_set,
                (s_index==2) ? "on" : "off" )) );
        // stats daytime
        this->ui->button_Tab_StatsDay->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/daytime_%2.png").arg(
                GlobalConfigs::icons_set,
                (s_index==3) ? "on" : "off" )) );
        this->ui->button_StatsDay_Draw->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/draw.png").arg(GlobalConfigs::icons_set)) );
        // stats relational
        this->ui->button_Tab_StatsRelat->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/relational_%2.png").arg(
                GlobalConfigs::icons_set,
                (s_index==4) ? "on" : "off" )) );
        this->ui->button_StatsRelat_Draw->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/draw.png").arg(GlobalConfigs::icons_set)) );
        // stats globals
        this->ui->button_Tab_StatsGlob->setIcon(
            QIcon(QStringLiteral(":/icons/icons/%1/global_%2.png").arg(
                GlobalConfigs::icons_set,
                (s_index==5) ? "on" : "off" )) );
        // configs tree
        QTreeWidgetItemIterator it{ this->ui->tree_ConfSections };
        while (*it) {
            QString icon_name;
            const QString text{ (*it)->text(0) };
            if ( text == tr("General") ) {
                icon_name += "conf_general";
            } else if ( text == tr("Window") ) {
                icon_name += "conf_window";
            } else if ( text == tr("Dialogs") ) {
                icon_name += "conf_dialogs";
            } else if ( text == tr("Charts") ) {
                icon_name += "conf_charts";
            } else if ( text == tr("Appearance") ) {
                icon_name += "conf_charts_style";
            } else if ( text == tr("Speed") ) {
                icon_name += "conf_speed";
            } else if ( text == tr("Count") ) {
                icon_name += "conf_count";
            } else if ( text == tr("Relational") ) {
                icon_name += "conf_relational";
            } else if ( text == tr("TextBrowser") ) {
                icon_name += "conf_textbrowser";
            } else if ( text == tr("Databases") ) {
                icon_name += "conf_databases";
            } else if ( text == tr("Security") ) {
                icon_name += "conf_security";
            } else if ( text == tr("Logs") ) {
                icon_name += "conf_logs";
            } else if ( text == tr("Defaults") ) {
                icon_name += "conf_defaults";
            } else if ( text == tr("Control") ) {
                icon_name += "conf_control";
            } else if ( text == tr("Path") ) {
                icon_name += "conf_path";
            } else if ( text == tr("Format") ) {
                icon_name += "conf_format";
            } else if ( text == tr("Warnlists") ) {
                icon_name += "conf_warnlists";
            } else if ( text == tr("Blacklists") ) {
                icon_name += "conf_blacklists";
            } else if ( text == tr("Apache2")
                     || text == tr("Nginx")
                     || text == tr("IIS") ) {
                icon_name += "conf_webservers";
            } else {
                throw DoNotCatchException( "Unexpected Configs section", text.toStdString() );
            }
            (*it)->setIcon(0,
                QIcon(QStringLiteral(":/icons/icons/%1/%2.png").arg(GlobalConfigs::icons_set, icon_name)) );
            ++it;
        }
        // conf databases
        {
            const QString save_template{ QStringLiteral(":/icons/icons/%1/save.png")      };
            const QString help_template{ QStringLiteral(":/icons/icons/%1/help.png")      };
            const QString add_template{  QStringLiteral(":/icons/icons/%1/list_add.png")  };
            const QString rem_template{  QStringLiteral(":/icons/icons/%1/list_rem.png")  };
            const QString up_template{   QStringLiteral(":/icons/icons/%1/list_up.png")   };
            const QString down_template{ QStringLiteral(":/icons/icons/%1/list_down.png") };

            this->ui->button_ConfDatabases_Data_Save->setIcon(
                QIcon(save_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfDatabases_Hashes_Save->setIcon(
                QIcon(save_template.arg(GlobalConfigs::icons_set)) );
            // conf apache
            this->ui->button_ConfApache_Path_Save->setIcon(
                QIcon(save_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Format_Save->setIcon(
                QIcon(save_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Format_Help->setIcon(
                QIcon(help_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Warnlist_Add->setIcon(
                QIcon(add_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Warnlist_Remove->setIcon(
                QIcon(rem_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Warnlist_Up->setIcon(
                QIcon(up_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Warnlist_Down->setIcon(
                QIcon(down_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Blacklist_Add->setIcon(
                QIcon(add_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Blacklist_Remove->setIcon(
                QIcon(rem_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Blacklist_Up->setIcon(
                QIcon(up_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfApache_Blacklist_Down->setIcon(
                QIcon(down_template.arg(GlobalConfigs::icons_set)) );
            // conf nginx
            this->ui->button_ConfNginx_Path_Save->setIcon(
                QIcon(save_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Format_Save->setIcon(
                QIcon(save_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Format_Help->setIcon(
                QIcon(help_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Warnlist_Add->setIcon(
                QIcon(add_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Warnlist_Remove->setIcon(
                QIcon(rem_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Warnlist_Up->setIcon(
                QIcon(up_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Warnlist_Down->setIcon(
                QIcon(down_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Blacklist_Add->setIcon(
                QIcon(add_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Blacklist_Remove->setIcon(
                QIcon(rem_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Blacklist_Up->setIcon(
                QIcon(up_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfNginx_Blacklist_Down->setIcon(
                QIcon(down_template.arg(GlobalConfigs::icons_set)) );
            // conf iis
            this->ui->button_ConfIis_Path_Save->setIcon(
                QIcon(save_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Format_Save->setIcon(
                QIcon(save_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Format_Help->setIcon(
                QIcon(help_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Warnlist_Add->setIcon(
                QIcon(add_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Warnlist_Remove->setIcon(
                QIcon(rem_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Warnlist_Up->setIcon(
                QIcon(up_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Warnlist_Down->setIcon(
                QIcon(down_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Blacklist_Add->setIcon(
                QIcon(add_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Blacklist_Remove->setIcon(
                QIcon(rem_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Blacklist_Up->setIcon(
                QIcon(up_template.arg(GlobalConfigs::icons_set)) );
            this->ui->button_ConfIis_Blacklist_Down->setIcon(
                QIcon(down_template.arg(GlobalConfigs::icons_set)) );
        }
    }
}

void MainWindow::updateUiFonts()
{
    const QFont& small_font{ this->fonts.at( "main_small" ) };
    const QFont& font{ this->fonts.at( "main" ) };
    const QFont& big_font{ this->fonts.at( "main_big" ) };
    const QFont& tb_font{ this->TB.getFont() };
    QFont menu_font{ small_font };
    menu_font.setPointSizeF( this->font_size_small+1.5 );
    QFont header_font{ small_font };
    header_font.setPointSizeF( this->font_size_small+2 );
    // menu
    this->ui->menuTools->setFont( menu_font );
    this->ui->actionBlockNote->setFont( menu_font );
    this->ui->menuUtilities->setFont( menu_font );
    this->ui->actionInfos->setFont( menu_font );
    this->ui->actionChangelog->setFont( menu_font );
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
    this->ui->textLogFiles->setFont( tb_font );
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
    // conf window
    this->ui->label_ConfWindow_Geometry->setFont( big_font );
    this->ui->checkBox_ConfWindow_Geometry->setFont( font );
    this->ui->label_ConfWindow_Theme->setFont( big_font );
    this->ui->box_ConfWindow_Theme->setFont( font );
    this->ui->label_ConfWindow_Icons->setFont( big_font );
    this->ui->box_ConfWindow_Icons->setFont( font );
    this->ui->label_ConfWindow_Language->setFont( big_font );
    this->ui->box_ConfWindow_Language->setFont( font );
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
    this->ui->textBrowser_ConfTextBrowser_Preview->setFont( tb_font );
    // conf charts
    this->ui->label_ConfCharts_Theme->setFont( big_font );
    this->ui->box_ConfCharts_Theme->setFont( font );
    this->ui->label_ConfCharts_Preview->setFont( big_font );
    this->ui->label_ConfCharts_Speed_TimeInterval->setFont( big_font );
    this->ui->box_ConfCharts_Speed_TimeInterval->setFont( font );
    this->ui->label_ConfCharts_Speed_TimeFormat->setFont( big_font );
    this->ui->box_ConfCharts_Speed_TimeFormat->setFont( font );
    this->ui->label_ConfCharts_Count_PieSize->setFont( big_font );
    this->ui->doubleSpinBox_ConfCharts_Count_PieSize->setFont( font );
    this->ui->label_ConfCharts_Count_MaxSlices->setFont( big_font );
    this->ui->spinBox_ConfCharts_Count_MaxSlices->setFont( font );
    this->ui->label_ConfCharts_Relat_TimeFormat->setFont( big_font );
    this->ui->box_ConfCharts_Relat_TimeFormat->setFont( font );
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
    this->ui->preview_ConfApache_Format_Sample->setFont( tb_font );
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
    this->ui->preview_ConfNginx_Format_Sample->setFont( tb_font );
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
    this->ui->preview_ConfIis_Format_Sample->setFont( tb_font );
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
    if ( this->translator.load( QStringLiteral(":/translations/%1").arg(QString::fromStdString( this->language )) ) ) {
        // apply the new translator
        QCoreApplication::installTranslator( &this->translator );
        this->ui->retranslateUi( this );
        // stats warn table header
        {
            const QStringList h{ this->crapview.getWarnHeaderColumns() };
            this->ui->table_StatsWarn->setColumnCount( h.size() );
            this->ui->table_StatsWarn->setHorizontalHeaderLabels( h );
        }
        // stats speed table header
        {
            const QStringList h{ this->crapview.getSpeedHeaderColumns() };
            this->ui->table_StatsSpeed->setColumnCount( h.size() );
            this->ui->table_StatsSpeed->setHorizontalHeaderLabels( h );
        }
        // stats count buttons
        this->ui->button_StatsCount_Protocol->setText(  TR::tr( FIELDS__PROTOCOL.c_str()      ) );
        this->ui->button_StatsCount_Method->setText(    TR::tr( FIELDS__METHOD.c_str()        ) );
        this->ui->button_StatsCount_Uri->setText(       TR::tr( FIELDS__URI.c_str()           ) );
        this->ui->button_StatsCount_Query->setText(     TR::tr( FIELDS__QUERY.c_str()         ) );
        this->ui->button_StatsCount_Response->setText(  TR::tr( FIELDS__RESPONSE_CODE.c_str() ) );
        this->ui->button_StatsCount_Referrer->setText(  TR::tr( FIELDS__REFERRER.c_str()      ) );
        this->ui->button_StatsCount_Cookie->setText(    TR::tr( FIELDS__COOKIE.c_str()        ) );
        this->ui->button_StatsCount_UserAgent->setText( TR::tr( FIELDS__USER_AGENT.c_str()    ) );
        this->ui->button_StatsCount_Client->setText(    TR::tr( FIELDS__CLIENT.c_str()        ) );
        // stats day log fields
        dynamic_cast<LogFieldsListModel*>(this->ui->box_StatsDay_LogsField->model())->translate();
        // stats relat log fields
        dynamic_cast<LogFieldsListModel*>(this->ui->box_StatsRelat_LogsField_1->model())->translate();
        dynamic_cast<LogFieldsListModel*>(this->ui->box_StatsRelat_LogsField_2->model())->translate();
        // configs
        {
            QTreeWidgetItemIterator it( this->ui->tree_ConfSections );
            while (*it) {
                (*it)->setText( 0, tr((*it)->text(0).toStdString().c_str()) );
                ++it;
            }

        }
        // configs warn/black-lists
        {
            const QStringList wl{
                TR::tr( FIELDS__METHOD.c_str()     ),
                TR::tr( FIELDS__URI.c_str()        ),
                TR::tr( FIELDS__USER_AGENT.c_str() ),
                TR::tr( FIELDS__CLIENT.c_str()     ) };
            const QStringList bl{
                TR::tr( FIELDS__CLIENT.c_str() ) };
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
void MainWindow::waitActiveWindow()
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
    std::error_code err;
    QString err_msg;

    // check that the sqlite plugin is available
    if ( ! this->dbHandler.checkDriver() ) {
        DialogSec::errSqlDriverNotFound( "QSQLITE" );
        std::exit( 1 );
    }

    // check LogDoctor's folders paths
    const std::vector<PathHandler> paths{
        PathHandler(this->configs_path.getParentUnchecked()),
        this->logdoc_path,
        this->db_data_path,
        this->db_hashes_path
    };
    for ( const auto& path_handler : paths ) {
        const auto path_exp{ path_handler.getPath() };
        if ( !path_exp.has_value() && path_exp.error().isReasonSymlink() ) {
            path_exp.error().showDialogMessage();
            std::exit( 1 );
        }
        const auto& path{ path_exp.value() };
        if ( ! IOutils::exists( path ) ) {
            if ( ! IOutils::makeDir( path, err ) ) {
                QString msg{ DialogSec::tr("Failed to create the directory") };
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    msg += QLatin1String(":\n%1").arg( path.c_str() );
                    if ( err.value() ) {
                        err_msg = QString::fromStdString( err.message() );
                    }
                }
                DialogSec::errFailedMakeDir( msg, err_msg );
                std::exit( 1 );
            }

        } else if ( ! IOutils::isDir( path ) ) {
            // not a directory, rename as copy a make a new one
            if ( ! DialogSec::choiceDirNotDir( toQString( path ) ) ) {
                // choosed not to rename the entry as '.copy'
                std::exit( 1 );
            } else if ( ! IOutils::renameAsCopy( path, err ) ) {
                QString p;
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    p = toQString( path );
                    if ( err.value() ) {
                        err_msg = QString::fromStdString( err.message() );
                    }
                }
                DialogSec::errRenaming( p, err_msg );
                std::exit( 1 );
            } else if ( ! IOutils::makeDir( path, err ) ) {
                QString msg{ DialogSec::tr("Failed to create the directory") };
                if ( this->dialogs_level > DL_ESSENTIAL ) {
                    msg += QLatin1String(":\n%1").arg( path.c_str() );
                    if ( err.value() ) {
                        err_msg = QString::fromStdString( err.message() );
                    }
                }
                DialogSec::errFailedMakeDir( msg, err_msg );
                std::exit( 1 );
            }

        } else if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::errDirNotReadable( toQString( path ), err_msg );
            std::exit( 1 );

        } else if ( ! IOutils::checkDir( path, false, true ) ) {
            DialogSec::errDirNotWritable( toQString( path ), err_msg );
            std::exit( 1 );
        }
    }

    // statistics' database
    if ( ! CheckSec::checkCollectionDatabase( this->db_data_path.getPathUnchecked() / DatabasesNames::data )
      || ! CheckSec::checkHashesDatabase( this->db_hashes_path.getPathUnchecked() / DatabasesNames::hashes ) ) {
        std::exit( 1 );
    }
    this->crapview.setDbPath( this->db_data_path );
    this->craplog.setStatsDatabasePath( this->db_data_path );
    this->craplog.setHashesDatabasePath( this->db_hashes_path );
    if ( ! this->craplog.hasher.loadUsedHashesLists( this->db_hashes_path.getPathUnchecked() / DatabasesNames::hashes ) ) {
        std::exit( 1 );
    }

    // get available stats dates
    this->refreshStatsDates();
    // get a fresh list of log files
    this->on_button_LogFiles_RefreshList_clicked();
    // set the default WS as the current one
    switch ( this->craplog.getCurrentWebServer() ) {
        case WS_APACHE:
            this->ui->box_StatsWarn_WebServer->setCurrentIndex(  0 );
            this->ui->box_StatsCount_WebServer->setCurrentIndex( 0 );
            this->ui->box_StatsSpeed_WebServer->setCurrentIndex( 0 );
            this->ui->box_StatsDay_WebServer->setCurrentIndex(   0 );
            this->ui->box_StatsRelat_WebServer->setCurrentIndex( 0 );
            break;
        case WS_NGINX:
            this->ui->box_StatsWarn_WebServer->setCurrentIndex(  1 );
            this->ui->box_StatsCount_WebServer->setCurrentIndex( 1 );
            this->ui->box_StatsSpeed_WebServer->setCurrentIndex( 1 );
            this->ui->box_StatsDay_WebServer->setCurrentIndex(   1 );
            this->ui->box_StatsRelat_WebServer->setCurrentIndex( 1 );
            break;
        case WS_IIS:
            this->ui->box_StatsWarn_WebServer->setCurrentIndex(  2 );
            this->ui->box_StatsCount_WebServer->setCurrentIndex( 2 );
            this->ui->box_StatsSpeed_WebServer->setCurrentIndex( 2 );
            this->ui->box_StatsDay_WebServer->setCurrentIndex(   2 );
            this->ui->box_StatsRelat_WebServer->setCurrentIndex( 2 );
            break;
        default:
            // shouldn't be here
            throw DoNotCatchException( "Unexpected WebServer", std::to_string(static_cast<int>(this->default_web_server)) );
    }
    this->initiating &= false;
    // effectively check if draw buttons can be enabled
    this->checkStatsWarnDrawable();
    this->checkStatsSpeedDrawable();
    this->checkStatsCountDrawable();
    this->checkStatsDayDrawable();
    this->checkStatsRelatDrawable();
}


bool MainWindow::checkDataDB()
{
    if ( this->initiating ) { // avoid recursions
        return false;
    }

    const auto path{ this->db_data_path / DatabasesNames::data };
    const auto path_exp{ path.getPath() };
    if ( !path_exp.has_value() && path_exp.error().isReasonSymlink() ) {
        path_exp.error().showDialogMessage();
        return false;
    } else if ( ! CheckSec::checkCollectionDatabase( path ) ) {
        // db invalid and failed to create a new one or user refused to do so
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
        this->db_ok &= false;

    } else if ( ! this->db_ok ) {
        // db was invalid but is now fine and ready to use
        this->db_ok |= true;
    }

    return this->db_ok;
}


/////////////////////
//// GENERAL USE ////
/////////////////////
QString MainWindow::wsFromIndex( const int index ) const
{
    switch (index) {
        case 0:
            return QStringLiteral("apache");
        case 1:
            return QStringLiteral("nginx");
        case 2:
            return QStringLiteral("iis");
        default:
            throw DoNotCatchException( "Unexpected WebServer index", std::to_string(index) );
    }
}
WebServer MainWindow::wsEnumFromIndex( const int index ) const
{
    switch (index) {
        case 0:
            return WS_APACHE;
        case 1:
            return WS_NGINX;
        case 2:
            return WS_IIS;
        default:
            throw DoNotCatchException( "Unexpected WebServer index", std::to_string(index) );
    }
}


//////////////
//// HELP ////
//////////////
void MainWindow::showHelp( const std::string& file_name )
{
    bool fallback{ false };
    const QString link{ "https://github.com/elB4RTO/LogDoctor/tree/main/installation_stuff/logdocdata/help/" };
    const auto path_exp{ (this->logdoc_path / "help" / this->language/ (file_name+".html")).getPath() };
    if ( !path_exp.has_value() && path_exp.error().isReasonSymlink() ) {
        path_exp.error().showDialogMessage();
        fallback |= true;
    } else {
        const path_t& file_path{ path_exp.value() };
        if ( ! IOutils::exists( file_path ) ) {
            DialogSec::errHelpNotFound( link );
            fallback |= true;
        } else if ( ! IOutils::isFile( file_path ) ) {
            DialogSec::errHelpFailed( link, DialogSec::tr("Unrecognized entry") );
            fallback |= true;
        } else if ( ! IOutils::checkFile( file_path, true ) ) {
            DialogSec::errHelpNotReadable( link );
            fallback |= true;
        } else {
            // everything ok, open a new window
            this->craphelp.reset( new Craphelp() );
            this->craphelp->helpLogsFormat(
                file_path,
                this->TB.getFont(),
                this->TB.getColorSchemeID() );
            if ( this->isMaximized() ) {
                this->craphelp->showMaximized();
            } else {
                this->craphelp->show();
            }
            return;
        }
    }
    if ( fallback ) {
        // help file not found for the current locale, fallback to the default version
        this->craphelp.reset( new Craphelp() );
        this->craphelp->helpLogsFormatDefault(
            file_name,
            this->TB.getFont(),
            this->TB.getColorSchemeID() );
        if ( this->isMaximized() ) {
            this->craphelp->showMaximized();
        } else {
            this->craphelp->show();
        }
    }
}

BlacklistField MainWindow::blacklistFieldFromString( const QString& str )
{
    if ( TR::tr(FIELDS__CLIENT.c_str()) == str ) {
        return BlacklistField::Client;
    }
    throw DoNotCatchException( "Unexpected BlacklistField string", str.toStdString() );
}

WarnlistField MainWindow::warnlistFieldFromString( const QString& str )
{
    if ( TR::tr(FIELDS__METHOD.c_str()) == str ) {
        return WarnlistField::Method;
    } else if ( TR::tr(FIELDS__URI.c_str()) == str ) {
        return WarnlistField::Uri;
    } else if ( TR::tr(FIELDS__CLIENT.c_str()) == str ) {
        return WarnlistField::Client;
    } else if ( TR::tr(FIELDS__USER_AGENT.c_str()) == str ) {
        return WarnlistField::UserAgent;
    }
    throw DoNotCatchException( "Unexpected WarnlistField string", str.toStdString() );
}



/****************************************************************
 ************** UI HANDLING LOGIC START FROM HERE ***************
 ****************************************************************/


//////////////
//// MENU ////
//////////////
// use a tool
void MainWindow::menu_actionBlockNote_triggered()
{
    if ( !this->crapnote.isNull() && this->crapnote->isVisible() ) {
        this->crapnote->activateWindow();

    } else {
        this->crapnote.reset( new Crapnote( this->TB.getColorSchemeID(), this->TB.getFont() ) );
        this->crapnote->show();
    }
}

void MainWindow::menu_actionInfos_triggered()
{
    if ( !this->crapinfo.isNull() && this->crapinfo->isVisible() ) {
        this->crapinfo->activateWindow();

    } else {
        this->crapinfo.reset( new Crapinfo(
            QString::number( this->version ),
            QString::fromStdString( PathHandler(std::string("./")).toString() ),
            QString::fromStdString( this->configs_path.toString() ),
            QString::fromStdString( this->logdoc_path.toString() ) ) );
        this->crapinfo->show();
    }
}

void MainWindow::menu_actionChangelog_triggered()
{
    if ( !this->changelog.isNull() && this->changelog->isVisible() ) {
        this->changelog->activateWindow();

    } else {
        this->changelog.reset( new Changelog(
            this->fonts.at( "main" ),
            this->TB.getFont() ) );
        this->changelog->show();
    }
}

void MainWindow::menu_actionCheckUpdates_triggered()
{
    if ( !this->crapup.isNull() && this->crapup->isVisible() ) {
        this->crapup->activateWindow();

    } else {
        this->crapup.reset( new Crapup() );
        this->crapup->show();
        this->crapup->versionCheck( this->version );
    }
}

// play a game
void MainWindow::menu_actionCrissCross_triggered()
{
    if ( !this->crisscross.isNull() && this->crisscross->isVisible() ) {
        this->crisscross->activateWindow();

    } else {
        this->crisscross.reset( new CrissCross() );
        this->crisscross->show();
    }
}

void MainWindow::menu_actionSnake_triggered()
{
    if ( !this->snake.isNull() && this->snake->isVisible() ) {
        this->snake->activateWindow();

    } else {
        this->snake.reset( new SnakeGame(
            this->fonts.at("script") ) );
        this->snake->show();
    }
}



//////////////
//// TABS ////
//////////////
void MainWindow::switchMainTab( const int new_index )
{
    const int old_index{ this->ui->stackedPages_Sections->currentIndex() };
    // turn off the old icon
    switch ( old_index ) {
        case 0:
            // make
            this->ui->button_Tab_Log->setFlat( true );
            this->ui->button_Tab_Log->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/log_off.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 1:
            // view
            this->ui->button_Tab_View->setFlat( true );
            this->ui->button_Tab_View->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/view_off.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 2:
            // config
            this->ui->button_Tab_Conf->setFlat( true );
            this->ui->button_Tab_Conf->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/conf_off.png").arg(GlobalConfigs::icons_set)) );
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
                QIcon(QStringLiteral(":/icons/icons/%1/log_on.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 1:
            // view
            this->ui->button_Tab_View->setFlat( false );
            this->ui->button_Tab_View->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/view_on.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 2:
            // config
            this->ui->button_Tab_Conf->setFlat( false );
            this->ui->button_Tab_Conf->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/conf_on.png").arg(GlobalConfigs::icons_set)) );
            break;
        default:
            throw("Unexpected MainTabs index: "+std::to_string(new_index));
            break;
    }
    this->ui->stackedPages_Sections->setCurrentIndex( new_index );
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
void MainWindow::switchStatsTab( const int new_index )
{
    const int old_index{ this->ui->stackedPages_Stats->currentIndex() };
    // turn off the old icon
    switch ( old_index ) {
        case 0:
            // warning
            this->ui->button_Tab_StatsWarn->setFlat( true );
            this->ui->button_Tab_StatsWarn->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/warn_off.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 1:
            // speed
            this->ui->button_Tab_StatsSpeed->setFlat( true );
            this->ui->button_Tab_StatsSpeed->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/speed_off.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 2:
            // counts
            this->ui->button_Tab_StatsCount->setFlat( true );
            this->ui->button_Tab_StatsCount->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/count_off.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 3:
            // daytime
            this->ui->button_Tab_StatsDay->setFlat( true );
            this->ui->button_Tab_StatsDay->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/daytime_off.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 4:
            // relational
            this->ui->button_Tab_StatsRelat->setFlat( true );
            this->ui->button_Tab_StatsRelat->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/relational_off.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 5:
            // globals
            this->ui->button_Tab_StatsGlob->setFlat( true );
            this->ui->button_Tab_StatsGlob->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/global_off.png").arg(GlobalConfigs::icons_set)) );
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
                QIcon(QStringLiteral(":/icons/icons/%1/warn_on.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 1:
            // speed
            this->ui->button_Tab_StatsSpeed->setFlat( false );
            this->ui->button_Tab_StatsSpeed->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/speed_on.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 2:
            // counts
            this->ui->button_Tab_StatsCount->setFlat( false );
            this->ui->button_Tab_StatsCount->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/count_on.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 3:
            // daytime
            this->ui->button_Tab_StatsDay->setFlat( false );
            this->ui->button_Tab_StatsDay->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/daytime_on.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 4:
            // relational
            this->ui->button_Tab_StatsRelat->setFlat( false );
            this->ui->button_Tab_StatsRelat->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/relational_on.png").arg(GlobalConfigs::icons_set)) );
            break;
        case 5:
            // globals
            this->ui->button_Tab_StatsGlob->setFlat( false );
            this->ui->button_Tab_StatsGlob->setIcon(
                QIcon(QStringLiteral(":/icons/icons/%1/global_on.png").arg(GlobalConfigs::icons_set)) );
            break;
        default:
            throw("Unexpected StatsTabs index: "+std::to_string(new_index));
            break;
    }
    this->ui->stackedPages_Stats->setCurrentIndex( new_index );
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
void MainWindow::setDbWorkingState( const bool working )
{
    this->db_working = working;
    if ( ! working ) {
        this->checkMakeStats_Makable();
        this->checkStatsWarnDrawable();
        this->checkStatsCountDrawable();
        this->checkStatsSpeedDrawable();
        this->checkStatsDayDrawable();
        this->checkStatsRelatDrawable();
        this->ui->button_StatsGlob_Apache->setEnabled( true );
        this->ui->button_StatsGlob_Nginx->setEnabled(  true );
        this->ui->button_StatsGlob_Iis->setEnabled(    true );
        this->ui->page_Section_Conf->setEnabled( true );
    } else {
        this->ui->button_MakeStats_Start->setEnabled(  false );
        this->ui->button_StatsWarn_Draw->setEnabled(   false );
        this->ui->scrollArea_StatsCount->setEnabled(   false );
        this->ui->button_StatsSpeed_Draw->setEnabled(  false );
        this->ui->button_StatsDay_Draw->setEnabled(    false );
        this->ui->button_StatsRelat_Draw->setEnabled(  false );
        this->ui->button_StatsGlob_Apache->setEnabled( false );
        this->ui->button_StatsGlob_Nginx->setEnabled(  false );
        this->ui->button_StatsGlob_Iis->setEnabled(    false );
        this->ui->page_Section_Conf->setEnabled( false );
    }
}

bool MainWindow::dbUsable()
{
    if ( this->db_working ) {
        return false;
    } else if ( ! this->db_ok ) {
        // db is invalid, attempt to renew
        return this->checkDataDB();
    }

    const auto path_exp{ (this->db_data_path / DatabasesNames::data).getPath() };
    if ( path_exp.has_value() ) {
        return IOutils::checkFile( path_exp.value(), true );
    }
    return false;
}


//////////////
//// LOGS ////
//////////////
// switch pages
void MainWindow::on_button_Logs_Down_clicked()
{
    this->ui->stackedPages_Logs->setCurrentIndex( 1 );
}


void MainWindow::on_button_Logs_Up_clicked()
{
    this->ui->stackedPages_Logs->setCurrentIndex( 0 );
}


// check
void MainWindow::checkMakeStats_Makable()
{
    bool state{ false };
    if ( this->dbUsable() ) {
        if ( this->ui->checkBox_LogFiles_CheckAll->checkState() == Qt::CheckState::Checked ) {
            // all checked
            state |= true;
        } else if ( this->ui->checkBox_LogFiles_CheckAll->checkState() == Qt::CheckState::PartiallyChecked ) {
            // at least one should be checked
            QTreeWidgetItemIterator i(this->ui->listLogFiles);
            while ( *i ) {
                if ( (*i)->checkState(0) == Qt::CheckState::Checked ) {
                    // an entry is checked
                    state |= true;
                    break;
                }
                ++i;
            }
            if ( ! state ) {
                this->ui->checkBox_LogFiles_CheckAll->setTristate( false );
            }
        }
    }
    this->ui->button_MakeStats_Start->setEnabled( state );
}


// switch to apache web server
void MainWindow::on_button_LogFiles_Apache_clicked()
{
    if ( this->craplog.getCurrentWebServer() != WS_APACHE ) {
        // flat/unflat
        this->ui->button_LogFiles_Apache->setFlat( false );
        this->ui->button_LogFiles_Nginx->setFlat( true );
        this->ui->button_LogFiles_Iis->setFlat( true );
        // set the WebServer
        this->craplog.setCurrentWebServer( WS_APACHE );
        // reset the log files viewer
        {
            QString rich_text;
            RichText::richLogsDefault( rich_text );
            this->ui->textLogFiles->setText( rich_text );
            this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        }
        // load the list
        this->on_button_LogFiles_RefreshList_clicked();
    }
}
// switch to nginx web server
void MainWindow::on_button_LogFiles_Nginx_clicked()
{
    if ( this->craplog.getCurrentWebServer() != WS_NGINX ) {
        // flat/unflat
        this->ui->button_LogFiles_Nginx->setFlat( false );
        this->ui->button_LogFiles_Apache->setFlat( true );
        this->ui->button_LogFiles_Iis->setFlat( true );
        // set the WebServer
        this->craplog.setCurrentWebServer( WS_NGINX );
        // reset the log files viewer
        {
            QString rich_text;
            RichText::richLogsDefault( rich_text );
            this->ui->textLogFiles->setText( rich_text );
            this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        }
        // load the list
        this->on_button_LogFiles_RefreshList_clicked();
    }
}
// switch to iis web server
void MainWindow::on_button_LogFiles_Iis_clicked()
{
    if ( this->craplog.getCurrentWebServer() != WS_IIS ) {
        // flat/unflat
        this->ui->button_LogFiles_Iis->setFlat( false );
        this->ui->button_LogFiles_Apache->setFlat( true );
        this->ui->button_LogFiles_Nginx->setFlat( true );
        // set the WebServer
        this->craplog.setCurrentWebServer( WS_IIS );
        // reset the log files viewer
        {
            QString rich_text;
            RichText::richLogsDefault( rich_text );
            this->ui->textLogFiles->setText( rich_text );
            this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        }
        // load the list
        this->on_button_LogFiles_RefreshList_clicked();
    }
}

// refresh the log files list
void MainWindow::on_button_LogFiles_RefreshList_clicked()
{
    if ( ! this->refreshing_list ) {
        this->refreshing_list |= true;
        // clear the current tree
        this->ui->listLogFiles->clear();
        this->ui->checkBox_LogFiles_CheckAll->setCheckState( Qt::CheckState::Unchecked );
        // disable elements
        this->ui->listLogFiles->setEnabled( false );
        this->ui->checkBox_LogFiles_CheckAll->setEnabled( false );
        this->ui->button_LogFiles_RefreshList->setEnabled( false );
        this->ui->button_LogFiles_ViewFile->setEnabled( false );
        this->ui->button_LogFiles_Apache->setEnabled( false );
        this->ui->button_LogFiles_Nginx->setEnabled( false );
        this->ui->button_LogFiles_Iis->setEnabled( false );
        // start refreshing as thread
        emit this->refreshLogs();
    }
}

void MainWindow::appendToLogsList( const LogFile& log_file )
{
    // new entry for the tree widget
    LogFileTreeWidgetItem* item{ new LogFileTreeWidgetItem() };

    // preliminary check for file usage display
    if ( log_file.hasBeenUsed() ) {
        if ( this->hide_used_files ) {
            // do not display
            delete item;
            return;
        }
        // display with red foreground
        item->setForeground( 0, this->colors.at( "red" ) );
    }

    // preliminary check on file size
    item->setForeground( 1, this->colors.at( (log_file.size() > this->craplog.getWarningSize())
                                             ? "orange"
                                             : "grey" ) );

    // set the name
    item->setText( 0, log_file.name() );
    item->setFont( 0, this->fonts.at("main") );
    // set the size
    item->setText( 1, PrintSec::printableSize( log_file.size() ) );
    item->setFont( 1, this->fonts.at("main_italic") );
    // append the item (on top, forced)
    item->setCheckState(0, Qt::CheckState::Unchecked );
    this->ui->listLogFiles->addTopLevelItem( item );
}

void MainWindow::refreshFinished()
{
    // refresh finished, back to normal state
    if ( this->craplog.getLogsListSize() > 0ul ) {
        this->ui->checkBox_LogFiles_CheckAll->setEnabled( true );
        this->ui->button_LogFiles_RefreshList->setEnabled( true );
        this->ui->button_LogFiles_ViewFile->setEnabled( true );
        this->ui->listLogFiles->setEnabled( true );
    }
    this->ui->button_LogFiles_Apache->setEnabled( true );
    this->ui->button_LogFiles_Nginx->setEnabled( true );
    this->ui->button_LogFiles_Iis->setEnabled( true );
    this->refreshing_list &= false;
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
    if ( ! this->ui->listLogFiles->selectedItems().isEmpty() ) {
        bool proceed{ true };
        const QString item_name{ this->ui->listLogFiles->selectedItems().takeFirst()->text(0) };
        LogFile item;
        // retrieve the file item
        try {
            item = this->craplog.getLogFileItem( item_name );

        } catch ( const GenericException& ) {
            // failed to find file
            proceed &= false;
            DialogSec::errFileNotFound( item_name, true );
        }

        // check the size
        if ( proceed ) {
            const std::size_t warn_size{ this->craplog.getWarningSize() };
            if ( warn_size > 0ul ) {
                if ( item.size() > warn_size ) {
                    // exceeds the warning size
                    QString msg{ item.name() };
                    if ( this->dialogs_level >= DL_NORMAL ) {
                        msg += QStringLiteral("\n\n%1:\n%2").arg(
                            DialogSec::tr("Size of the file"),
                            PrintSec::printableSize( item.size() ) );
                        if ( this->dialogs_level == DL_EXPLANATORY ) {
                            msg += QStringLiteral("\n\n%1:\n%2").arg(
                                DialogSec::tr("Warning size parameter"),
                                PrintSec::printableSize( warn_size ) );
                        }
                    }
                    // ask the user what to do
                    if ( ! DialogSec::choiceFileSizeWarning2( msg ) ) {
                        QString rich_text;
                        RichText::richLogsDefault( rich_text );
                        this->ui->textLogFiles->setText( rich_text );
                        this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
                        return;
                    }
                }
            }
        }

        // check the path
        if ( proceed ) {
            const auto path_exp{ item.path().getPath() };
            if ( ! path_exp.has_value() ) {
                path_exp.error().showDialogMessage();
                proceed &= false;
            }
        }

        if ( proceed ) {
            // get the current log format
            const LogsFormat& format{ this->craplog.getCurrentLogFormat() };
            // read the content
            std::string content;
            const path_t path{ item.path().getPathUnchecked() };
            try {
                try {
                    // try reading as gzip compressed file
                    GZutils::readFile( path, content );

                } catch ( const GenericException& ) {
                    // failed closing file pointer
                    throw;

                } catch (...) {
                    // failed as gzip, try as text file
                    if ( content.size() > 0 ) {
                        content.clear();
                    }
                    IOutils::readFile( path, content );
                }

            } catch ( const GenericException& ) {
                // failed closing gzip file pointer
                proceed &= false;
                // >> e.what() << //
                DialogSec::errGeneric( QStringLiteral("%1:\n%2").arg(
                    DialogSec::tr("Failed to read gzipped file"),
                    item.name()) );

            /*} catch ( const std::ios_base::failure& err ) {
                // failed reading as text
                proceed &= false;
                // >> err.what() << //
                DialogSec::errFailedReadFile( item.name );*/
            } catch (...) {
                // failed somehow
                proceed &= false;
                DialogSec::errFailedReadFile( item.name() );
            }

            if ( proceed ) {
                // succesfully read, now enriched and display
                QString rich_content;
                RichText::enrichLogs(
                    rich_content, content,
                    format, this->TB );
                this->ui->textLogFiles->setText( rich_content );
                this->ui->textLogFiles->setFont( this->TB.getFont() );
            }
        }
        if ( ! proceed ) {
            // failed
            QString rich_text;
            RichText::richLogsFailure( rich_text );
            this->ui->textLogFiles->setText( rich_text );
            this->ui->textLogFiles->setAlignment( Qt::AlignHCenter );
        }
    }
}


void MainWindow::on_listLogFiles_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item)
    Q_UNUSED(column)
    this->on_button_LogFiles_ViewFile_clicked();
}


void MainWindow::on_listLogFiles_itemChanged(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item)
    Q_UNUSED(column)
    // control checked
    std::size_t n_checked{ 0ul };
    QTreeWidgetItemIterator i(this->ui->listLogFiles);
    while ( *i ) {
        if ( (*i)->checkState(0) == Qt::CheckState::Checked ) {
            ++ n_checked;
        }
        ++ i;
    }
    if ( n_checked == 0ul ) {
        this->ui->checkBox_LogFiles_CheckAll->setCheckState(Qt::CheckState::Unchecked);
    } else if ( n_checked == this->craplog.getLogsListSize() ) {
        this->ui->checkBox_LogFiles_CheckAll->setCheckState(Qt::CheckState::Checked);
    } else {
        this->ui->checkBox_LogFiles_CheckAll->setCheckState(Qt::CheckState::PartiallyChecked);
    }
}


void MainWindow::on_button_MakeStats_Start_clicked()
{
    if ( !( this->dbUsable() && this->checkDataDB() ) ) {
        return;
    }

    // check that the format has been set and is consistent
    if ( ! craplog.checkCurrentLogsFormat() ) {
        return;
    }

    // take actions on Craplog's start
    this->craplogStarted();

    // feed craplog with the checked files
    QTreeWidgetItemIterator i{ this->ui->listLogFiles };
    while ( *i ) {
        if ( (*i)->checkState(0) == Qt::CheckState::Checked ) {
            // tell Craplog to set this file as selected
            if ( ! this->craplog.setLogFileSelected( (*i)->text(0) ) ) {
                // failed to retrieve the file. this shouldn't be, but...
                const int choice{ DialogSec::choiceSelectedFileNotFound( (*i)->text(0) ) };
                if ( choice == 0 ) {
                    // choosed to abort all
                    this->craplogFinished();
                    return;
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

    if ( this->craplog.checkStuff() ) {
        // periodically update perfs
        this->waiter_timer.reset( new QTimer(this) );
        this->waiter_timer->setInterval(250);
        this->waiter_timer->setTimerType( Qt::PreciseTimer );
        connect( this->waiter_timer.get(), &QTimer::timeout,
                 this, &MainWindow::updatePerfsLabels );
        // start processing
        this->waiter_timer_start = std::chrono::system_clock::now();
        this->waiter_timer->start();
        emit runCraplog( this->blacklists );
    } else {
        this->craplogFinished();
    }
}

void MainWindow::resetPerfsLabels()
{
    this->ui->label_MakeStats_Size->setText( "0 B" );
    this->ui->label_MakeStats_Lines->setText( "0" );
    this->ui->label_MakeStats_Time->setText( "00:00" );
    this->ui->label_MakeStats_Speed->setText( "0 B/s" );
}

void MainWindow::updatePerfsLabels()
{
    if ( this->craplog.isParsing() || this->force_updating_labels ) {
        const std::size_t size{ this->craplog.getParsedSize() };
        this->ui->label_MakeStats_Size->setText( PrintSec::printableSize( size ) );
        this->ui->label_MakeStats_Lines->setText( QString::number( this->craplog.getParsedLines() ) );
        this->ui->label_MakeStats_Speed->setText( this->craplog.getParsingSpeed() );
    }
    std::chrono::duration<float, std::milli> timer_elapsed =
        std::chrono::system_clock::now() - this->waiter_timer_start;
    this->ui->label_MakeStats_Time->setText(
        PrintSec::printableTime( static_cast<unsigned>(
            timer_elapsed.count() * 0.001 ) ) );
}

void MainWindow::craplogStarted()
{
    this->resetPerfsLabels();
    this->ui->stackedPages_Logs->setEnabled( false );
    this->setDbWorkingState( true );
}

void MainWindow::craplogFinished()
{
    if ( this->waiter_timer->isActive() ) {
        this->waiter_timer->stop();
        this->force_updating_labels |= true;
        this->updatePerfsLabels();
        this->force_updating_labels &= false;
        // draw the chart
        this->craplog.makeChart(
            this->CHARTS_THEMES.at( static_cast<std::size_t>(GlobalConfigs::charts_theme) ),
            this->fonts,
            this->ui->chart_MakeStats_Size );
        ColorSec::applyChartTheme(
            this->fonts,
            this->ui->chart_MakeStats_Size );
        this->db_edited = this->craplog.editedDatabase();
        // refresh the logs section
        this->waiter_timer.reset( new QTimer(this) );
        this->waiter_timer->setSingleShot( true );
        connect( this->waiter_timer.get(), &QTimer::timeout,
                 this, &MainWindow::afterCraplogFinished);
        this->waiter_timer->start(1000);
    } else {
        this->afterCraplogFinished();
    }
    this->craplog.clearLogFilesSelection();
}

void MainWindow::afterCraplogFinished()
{
    this->ui->stackedPages_Logs->setEnabled( true );
    this->setDbWorkingState( false );
    if ( this->craplog.editedDatabase() ) {
        this->on_button_LogFiles_RefreshList_clicked();
        this->refreshStatsDates();
    }
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
    this->ui->button_StatsWarn_Draw->setEnabled(
        this->dbUsable()
        && this->ui->box_StatsWarn_Year->currentIndex() >= 0
        && this->ui->box_StatsWarn_Month->currentIndex() >= 0
        && this->ui->box_StatsWarn_Day->currentIndex() >= 0
    );
}

void MainWindow::on_box_StatsWarn_WebServer_currentIndexChanged(int index)
{
    this->ui->box_StatsWarn_Year->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsWarn_Year->addItems(
            this->crapview.getYears( this->wsFromIndex( index ) ));
        this->ui->box_StatsWarn_Year->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsWarn_Year->count() > 0 ) {
        this->ui->box_StatsWarn_Year->setEnabled( true );
        this->on_box_StatsWarn_Year_currentIndexChanged(
            this->ui->box_StatsWarn_Year->currentIndex() );
    } else {
        this->ui->box_StatsWarn_Year->setEnabled( false );
        this->on_box_StatsWarn_Year_currentIndexChanged( -1 );
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_box_StatsWarn_Year_currentIndexChanged(int index)
{
    this->ui->box_StatsWarn_Month->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsWarn_Month->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsWarn_WebServer->currentIndex() ),
                this->ui->box_StatsWarn_Year->currentText() ) );
        this->ui->box_StatsWarn_Month->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsWarn_Month->count() > 0
      && this->ui->box_StatsWarn_Year->isEnabled() ) {
        this->ui->box_StatsWarn_Month->setEnabled( true );
        this->on_box_StatsWarn_Month_currentIndexChanged(
            this->ui->box_StatsWarn_Month->currentIndex() );
    } else {
        this->ui->box_StatsWarn_Month->setEnabled( false );
        this->on_box_StatsWarn_Month_currentIndexChanged( -1 );
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_box_StatsWarn_Month_currentIndexChanged(int index)
{
    this->ui->box_StatsWarn_Day->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsWarn_Day->addItems(
            this->crapview.getDays(
                this->wsFromIndex( this->ui->box_StatsWarn_WebServer->currentIndex() ),
                this->ui->box_StatsWarn_Year->currentText(),
                this->ui->box_StatsWarn_Month->currentText() ) );
        this->ui->box_StatsWarn_Day->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsWarn_Day->count() > 0
      && this->ui->box_StatsWarn_Month->isEnabled() ) {
        this->ui->box_StatsWarn_Day->setEnabled( true );
        this->on_box_StatsWarn_Day_currentIndexChanged(
            this->ui->box_StatsWarn_Day->currentIndex() );
    } else {
        this->ui->box_StatsWarn_Day->setEnabled( false );
        this->on_box_StatsWarn_Day_currentIndexChanged( -1 );
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_box_StatsWarn_Day_currentIndexChanged(int index)
{
    this->ui->box_StatsWarn_Hour->clear();
    if ( this->ui->checkBox_StatsWarn_Hour->isChecked()
      && this->ui->box_StatsWarn_Day->isEnabled() ) {
        if ( index >= 0 ) {
            this->ui->box_StatsWarn_Hour->addItems( this->crapview.getHours() );
            this->ui->box_StatsWarn_Hour->setCurrentIndex( 0 );
            this->ui->box_StatsWarn_Hour->setEnabled( true );
        }
    } else {
        this->ui->box_StatsWarn_Hour->setEnabled( false );
    }
    this->checkStatsWarnDrawable();
}

void MainWindow::on_checkBox_StatsWarn_Hour_stateChanged(int state)
{
    if ( state == Qt::CheckState::Checked
      && this->ui->box_StatsWarn_Day->isEnabled() ) {
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
    Q_UNUSED(index)
    this->checkStatsWarnDrawable();
}

void MainWindow::on_button_StatsWarn_Draw_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->setDbWorkingState( true );
        this->crapview_timer.reset( new QTimer(this) );
        this->crapview_timer->setSingleShot( true );
        connect( this->crapview_timer.get(), &QTimer::timeout,
                 this, &MainWindow::drawStatsWarn);
        this->crapview_timer->start(250);
    }
}
void MainWindow::drawStatsWarn()
{
    const int wsIndex{ this->ui->box_StatsWarn_WebServer->currentIndex() };
    this->ui->table_StatsWarn->horizontalHeader()->setSortIndicator( -1, Qt::SortOrder::AscendingOrder );
    this->ui->table_StatsWarn->setRowCount( 0 );
    this->ui->chart_StatsWarn->setChart( new QChart() );
    this->crapview.drawWarn(
        this->ui->table_StatsWarn, this->ui->chart_StatsWarn,
        this->CHARTS_THEMES.at( static_cast<std::size_t>(GlobalConfigs::charts_theme) ),
        this->wsFromIndex( wsIndex ),
        this->ui->box_StatsWarn_Year->currentText(),
        this->ui->box_StatsWarn_Month->currentText(),
        this->ui->box_StatsWarn_Day->currentText(),
        this->ui->checkBox_StatsWarn_Hour->isChecked() ? this->ui->box_StatsWarn_Hour->currentText() : "",
        this->warnlists.get( this->wsEnumFromIndex( wsIndex ) ) );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsWarn );
    this->setDbWorkingState( false );
}


///////////////
//// SPEED ////
void MainWindow::checkStatsSpeedDrawable()
{
    this->ui->button_StatsSpeed_Draw->setEnabled(
        this->dbUsable()
        && this->ui->box_StatsSpeed_Year->currentIndex() >= 0
        && this->ui->box_StatsSpeed_Month->currentIndex() >= 0
        && this->ui->box_StatsSpeed_Day->currentIndex() >= 0
        && FilterOps::parseTextualFilter( this->ui->inLine_StatsSpeed_Protocol->text() ).has_value()
        && FilterOps::parseTextualFilter( this->ui->inLine_StatsSpeed_Method->text() ).has_value()
        && FilterOps::parseTextualFilter( this->ui->inLine_StatsSpeed_Uri->text() ).has_value()
        && FilterOps::parseTextualFilter( this->ui->inLine_StatsSpeed_Query->text() ).has_value()
        && FilterOps::parseNumericFilter( this->ui->inLine_StatsSpeed_Response->text() ).has_value()
    );
}

void MainWindow::on_box_StatsSpeed_WebServer_currentIndexChanged(int index)
{
    this->ui->box_StatsSpeed_Year->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsSpeed_Year->addItems(
            this->crapview.getYears( this->wsFromIndex( index ) ) );
        this->ui->box_StatsSpeed_Year->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsSpeed_Year->count() > 0 ) {
        this->ui->box_StatsSpeed_Year->setEnabled( true );
        this->on_box_StatsSpeed_Year_currentIndexChanged(
            this->ui->box_StatsSpeed_Year->currentIndex() );
    } else {
        this->ui->box_StatsSpeed_Year->setEnabled( false );
        this->on_box_StatsSpeed_Year_currentIndexChanged( -1 );
    }
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_box_StatsSpeed_Year_currentIndexChanged(int index)
{
    this->ui->box_StatsSpeed_Month->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsSpeed_Month->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsSpeed_WebServer->currentIndex() ),
                this->ui->box_StatsSpeed_Year->currentText() ) );
        this->ui->box_StatsSpeed_Month->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsSpeed_Month->count() > 0 ) {
        this->ui->box_StatsSpeed_Month->setEnabled( true );
        this->on_box_StatsSpeed_Month_currentIndexChanged(
            this->ui->box_StatsSpeed_Month->currentIndex() );
    } else {
        this->ui->box_StatsSpeed_Month->setEnabled( false );
        this->on_box_StatsSpeed_Month_currentIndexChanged( -1 );
    }
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_box_StatsSpeed_Month_currentIndexChanged(int index)
{
    this->ui->box_StatsSpeed_Day->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsSpeed_Day->addItems(
            this->crapview.getDays(
                this->wsFromIndex( this->ui->box_StatsSpeed_WebServer->currentIndex() ),
                this->ui->box_StatsSpeed_Year->currentText(),
                this->ui->box_StatsSpeed_Month->currentText() ) );
        this->ui->box_StatsSpeed_Day->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsSpeed_Day->count() > 0 ) {
        this->ui->box_StatsSpeed_Day->setEnabled( true );
        this->on_box_StatsSpeed_Day_currentIndexChanged(
            this->ui->box_StatsSpeed_Day->currentIndex() );
    } else {
        this->ui->box_StatsSpeed_Day->setEnabled( false );
        this->on_box_StatsSpeed_Day_currentIndexChanged( -1 );
    }
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_box_StatsSpeed_Day_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_inLine_StatsSpeed_Protocol_textChanged(const QString& arg1)
{
    if ( FilterOps::parseTextualFilter( arg1 ).has_value() ) {
        this->ui->inLine_StatsSpeed_Protocol->setStyleSheet(
            this->stylesheet_lineedit );
    } else {
        this->ui->inLine_StatsSpeed_Protocol->setStyleSheet(
            this->stylesheet_lineedit_err );
    }
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_inLine_StatsSpeed_Method_textChanged(const QString& arg1)
{
    if ( FilterOps::parseTextualFilter( arg1 ).has_value() ) {
        this->ui->inLine_StatsSpeed_Method->setStyleSheet(
            this->stylesheet_lineedit );
    } else {
        this->ui->inLine_StatsSpeed_Method->setStyleSheet(
            this->stylesheet_lineedit_err );
    }
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_inLine_StatsSpeed_Uri_textChanged(const QString& arg1)
{
    if ( FilterOps::parseTextualFilter( arg1 ).has_value() ) {
        this->ui->inLine_StatsSpeed_Uri->setStyleSheet(
            this->stylesheet_lineedit );
    } else {
        this->ui->inLine_StatsSpeed_Uri->setStyleSheet(
            this->stylesheet_lineedit_err );
    }
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_inLine_StatsSpeed_Query_textChanged(const QString& arg1)
{
    if ( FilterOps::parseTextualFilter( arg1 ).has_value() ) {
        this->ui->inLine_StatsSpeed_Query->setStyleSheet(
            this->stylesheet_lineedit );
    } else {
        this->ui->inLine_StatsSpeed_Query->setStyleSheet(
            this->stylesheet_lineedit_err );
    }
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_inLine_StatsSpeed_Response_textChanged(const QString& arg1)
{
    if ( FilterOps::parseNumericFilter( arg1 ).has_value() ) {
        this->ui->inLine_StatsSpeed_Response->setStyleSheet(
            this->stylesheet_lineedit );
    } else {
        this->ui->inLine_StatsSpeed_Response->setStyleSheet(
            this->stylesheet_lineedit_err );
    }
    this->checkStatsSpeedDrawable();
}

void MainWindow::on_button_StatsSpeed_Draw_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->setDbWorkingState( true );
        this->crapview_timer.reset( new QTimer(this) );
        this->crapview_timer->setSingleShot( true );
        connect( this->crapview_timer.get(), &QTimer::timeout,
                 this, &MainWindow::drawStatsSpeed);
        this->crapview_timer->start(250);
    }
}
void MainWindow::drawStatsSpeed()
{
    this->ui->table_StatsSpeed->horizontalHeader()->setSortIndicator( -1, Qt::SortOrder::AscendingOrder );
    this->ui->table_StatsSpeed->setRowCount( 0 );
    this->ui->chart_StatsSpeed->setChart( new QChart() );
    this->crapview.drawSpeed(
        this->ui->table_StatsSpeed,
        this->ui->chart_StatsSpeed,
        this->CHARTS_THEMES.at( static_cast<std::size_t>(GlobalConfigs::charts_theme) ),
        this->wsFromIndex( this->ui->box_StatsSpeed_WebServer->currentIndex() ),
        this->ui->box_StatsSpeed_Year->currentText(),
        this->ui->box_StatsSpeed_Month->currentText(),
        this->ui->box_StatsSpeed_Day->currentText(),
        FilterOps::parseTextualFilter( this->ui->inLine_StatsSpeed_Protocol->text() ).value(),
        FilterOps::parseTextualFilter( this->ui->inLine_StatsSpeed_Method->text() ).value(),
        FilterOps::parseTextualFilter( this->ui->inLine_StatsSpeed_Uri->text() ).value(),
        FilterOps::parseTextualFilter( this->ui->inLine_StatsSpeed_Query->text() ).value(),
        FilterOps::parseNumericFilter( this->ui->inLine_StatsSpeed_Response->text() ).value() );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsSpeed );
    this->setDbWorkingState( false );
}


///////////////
//// COUNT ////
void MainWindow::checkStatsCountDrawable()
{
    this->ui->scrollArea_StatsCount->setEnabled(
        this->dbUsable()
        && this->ui->box_StatsCount_Year->currentIndex() >= 0
        && this->ui->box_StatsCount_Month->currentIndex() >= 0
        && this->ui->box_StatsCount_Day->currentIndex() >= 0
    );
}

void MainWindow::on_box_StatsCount_WebServer_currentIndexChanged(int index)
{
    this->ui->box_StatsCount_Year->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsCount_Year->addItems(
            this->crapview.getYears( this->wsFromIndex( index ) ));
        this->ui->box_StatsCount_Year->setCurrentIndex( 0 );
        this->resetStatsCountButtons();
    }
    if ( this->ui->box_StatsCount_Year->count() > 0 ) {
        this->ui->box_StatsCount_Year->setEnabled( true );
        this->on_box_StatsCount_Year_currentIndexChanged(
            this->ui->box_StatsCount_Year->currentIndex() );
    } else {
        this->ui->box_StatsCount_Year->setEnabled( false );
        this->on_box_StatsCount_Year_currentIndexChanged( -1 );
    }
    this->checkStatsCountDrawable();
}

void MainWindow::on_box_StatsCount_Year_currentIndexChanged(int index)
{
    this->ui->box_StatsCount_Month->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsCount_Month->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsCount_WebServer->currentIndex() ),
                this->ui->box_StatsCount_Year->currentText() ) );
        this->ui->box_StatsCount_Month->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsCount_Month->count() > 0 ) {
        this->ui->box_StatsCount_Month->setEnabled( true );
        this->on_box_StatsCount_Month_currentIndexChanged(
            this->ui->box_StatsCount_Month->currentIndex() );
    } else {
        this->ui->box_StatsCount_Month->setEnabled( false );
        this->on_box_StatsCount_Month_currentIndexChanged( -1 );
    }
    this->checkStatsCountDrawable();
}

void MainWindow::on_box_StatsCount_Month_currentIndexChanged(int index)
{
    this->ui->box_StatsCount_Day->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsCount_Day->addItems(
            this->crapview.getDays(
                this->wsFromIndex( this->ui->box_StatsCount_WebServer->currentIndex() ),
                this->ui->box_StatsCount_Year->currentText(),
                this->ui->box_StatsCount_Month->currentText() ) );
        this->ui->box_StatsCount_Day->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsCount_Day->count() > 0 ) {
        this->ui->box_StatsCount_Day->setEnabled( true );
        this->on_box_StatsCount_Day_currentIndexChanged(
            this->ui->box_StatsCount_Day->currentIndex() );
    } else {
        this->ui->box_StatsCount_Day->setEnabled( false );
        this->on_box_StatsCount_Day_currentIndexChanged( -1 );
    }
    this->checkStatsCountDrawable();
}

void MainWindow::on_box_StatsCount_Day_currentIndexChanged(int index)
{
    Q_UNUSED(index)
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

void MainWindow::makeStatsCount()
{
    this->setDbWorkingState( true );
    this->crapview_timer.reset( new QTimer(this) );
    this->crapview_timer->setSingleShot( true );
    connect( this->crapview_timer.get(), &QTimer::timeout,
             this, &MainWindow::drawStatsCount);
    this->crapview_timer->start(250);
}

void MainWindow::on_button_StatsCount_Protocol_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->ui->button_StatsCount_Protocol->setFlat( false );
        this->count_fld = this->ui->button_StatsCount_Protocol->text();
        this->makeStatsCount();
    }
}

void MainWindow::on_button_StatsCount_Method_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Method->text();
        this->ui->button_StatsCount_Method->setFlat( false );
        this->makeStatsCount();
    }
}

void MainWindow::on_button_StatsCount_Uri_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Uri->text();
        this->ui->button_StatsCount_Uri->setFlat( false );
        this->makeStatsCount();
    }
}

void MainWindow::on_button_StatsCount_Query_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Query->text();
        this->ui->button_StatsCount_Query->setFlat( false );
        this->makeStatsCount();
    }
}

void MainWindow::on_button_StatsCount_Response_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Response->text();
        this->ui->button_StatsCount_Response->setFlat( false );
        this->makeStatsCount();
    }
}

void MainWindow::on_button_StatsCount_Referrer_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Referrer->text();
        this->ui->button_StatsCount_Referrer->setFlat( false );
        this->makeStatsCount();
    }
}

void MainWindow::on_button_StatsCount_Cookie_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Cookie->text();
        this->ui->button_StatsCount_Cookie->setFlat( false );
        this->makeStatsCount();
    }
}

void MainWindow::on_button_StatsCount_UserAgent_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_UserAgent->text();
        this->ui->button_StatsCount_UserAgent->setFlat( false );
        this->makeStatsCount();
    }
}

void MainWindow::on_button_StatsCount_Client_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->resetStatsCountButtons();
        this->count_fld = this->ui->button_StatsCount_Client->text();
        this->ui->button_StatsCount_Client->setFlat( false );
        this->makeStatsCount();
    }
}

void MainWindow::drawStatsCount()
{
    this->ui->table_StatsCount->horizontalHeader()->setSortIndicator( -1, Qt::SortOrder::AscendingOrder );
    this->ui->table_StatsCount->setRowCount( 0 );
    this->ui->chart_StatsCount->setChart( new QChart() );
    this->crapview.drawCount(
        this->ui->table_StatsCount, this->ui->chart_StatsCount,
        this->CHARTS_THEMES.at( static_cast<std::size_t>(GlobalConfigs::charts_theme) ),
        this->wsFromIndex( this->ui->box_StatsCount_WebServer->currentIndex() ),
        this->ui->box_StatsCount_Year->currentText(),
        this->ui->box_StatsCount_Month->currentText(),
        this->ui->box_StatsCount_Day->currentText(),
        this->count_fld );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsCount );
    this->ui->chart_StatsCount->chart()->setTitleFont(
        this->fonts.at( "main_big" ) );
    this->setDbWorkingState( false );
}


/////////////
//// DAY ////
void MainWindow::checkStatsDayDrawable()
{
    if ( ! this->dbUsable() ) {
        this->ui->button_StatsDay_Draw->setEnabled( false );
        return;
    }

    bool aux{ true };
    // primary date
    if ( this->ui->box_StatsDay_FromYear->currentIndex() < 0
      || this->ui->box_StatsDay_FromMonth->currentIndex() < 0
      || this->ui->box_StatsDay_FromDay->currentIndex() < 0 ) {
        aux &= false;
    }
    // secondary date (period)
    if ( this->ui->checkBox_StatsDay_Period->isChecked() ) {
        if ( this->ui->box_StatsDay_ToYear->currentIndex() < 0
          || this->ui->box_StatsDay_ToMonth->currentIndex() < 0
          || this->ui->box_StatsDay_ToDay->currentIndex() < 0 ) {
            aux &= false;
        } else {
            int a{ this->ui->box_StatsDay_ToYear->currentText().toInt() };
            int b{ this->ui->box_StatsDay_FromYear->currentText().toInt() };
            if ( a < b ) {
                // year 'to' is less than 'from'
                aux &= false;
            } else if ( a == b ) {
                a = this->crapview.getMonthNumber( this->ui->box_StatsDay_ToMonth->currentText() );
                b = this->crapview.getMonthNumber( this->ui->box_StatsDay_FromMonth->currentText() );
                if ( a < b ) {
                    // month 'to' is less than 'from'
                    aux &= false;
                } else if ( a == b ) {
                    a = this->ui->box_StatsDay_ToDay->currentText().toInt();
                    b = this->ui->box_StatsDay_FromDay->currentText().toInt();
                    if ( a < b ) {
                        // day 'to' is less than 'from'
                        aux &= false;
                    }
                }
            }
        }
    }
    // check log field validity
    if ( this->ui->box_StatsDay_LogsField->currentIndex() < 0 ) {
        aux &= false;
    }
    // check filter string validity
    if ( ! this->getStatsDayParsedFilter().has_value() ) {
        aux &= false;
    }
    this->ui->button_StatsDay_Draw->setEnabled( aux );
}

LogField MainWindow::getStatsDayLogField() const
{
    const QModelIndex idx{ this->ui->box_StatsDay_LogsField->model()->index(
        this->ui->box_StatsDay_LogsField->currentIndex(), 0 ) };
    return this->ui->box_StatsDay_LogsField->model()->data(idx,Qt::UserRole).value<LogField>();
}
std::optional<QString> MainWindow::getStatsDayParsedFilter() const
{
    if ( this->getStatsDayLogField() == LogField::ResponseCode ) {
        return FilterOps::parseNumericFilter( this->ui->inLine_StatsDay_Filter->text() );
    } else {
        return FilterOps::parseTextualFilter( this->ui->inLine_StatsDay_Filter->text() );
    }
}

void MainWindow::on_box_StatsDay_WebServer_currentIndexChanged(int index)
{
    this->ui->box_StatsDay_FromYear->clear();
    this->ui->box_StatsDay_ToYear->clear();
    if ( index >= 0 ) {
        // refresh dates
        const QStringList years{ this->crapview.getYears( this->wsFromIndex( index ) ) };
        this->ui->box_StatsDay_FromYear->addItems( years );
        this->ui->box_StatsDay_FromYear->setCurrentIndex( 0 );
        if ( this->ui->checkBox_StatsDay_Period->isChecked() ) {
            this->ui->box_StatsDay_ToYear->addItems( years );
            this->ui->box_StatsDay_ToYear->setCurrentIndex( 0 );
        }
    }
    if ( this->ui->box_StatsDay_FromYear->count() > 0 ) {
        this->ui->box_StatsDay_FromYear->setEnabled( true );
        this->on_box_StatsDay_FromYear_currentIndexChanged(
            this->ui->box_StatsDay_FromYear->currentIndex() );
    } else {
        this->ui->box_StatsDay_FromYear->setEnabled( false );
        this->on_box_StatsDay_FromYear_currentIndexChanged( -1 );
    }
    if ( this->ui->checkBox_StatsDay_Period->isChecked()
      && this->ui->box_StatsDay_ToYear->count() > 0 ) {
        this->ui->box_StatsDay_ToYear->setEnabled( true );
        this->on_box_StatsDay_ToYear_currentIndexChanged(
            this->ui->box_StatsDay_ToYear->currentIndex() );
    } else {
        this->ui->box_StatsDay_ToYear->setEnabled( false );
        this->on_box_StatsDay_ToYear_currentIndexChanged( -1 );
    }
    this->checkStatsDayDrawable();
}


void MainWindow::on_box_StatsDay_LogsField_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->ui->inLine_StatsDay_Filter->clear();
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_FromYear_currentIndexChanged(int index)
{
    this->ui->box_StatsDay_FromMonth->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsDay_FromMonth->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
                this->ui->box_StatsDay_FromYear->currentText() ) );
        this->ui->box_StatsDay_FromMonth->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsDay_FromMonth->count() > 0 ) {
        this->ui->box_StatsDay_FromMonth->setEnabled( true );
        this->on_box_StatsDay_FromMonth_currentIndexChanged(
            this->ui->box_StatsDay_FromMonth->currentIndex() );
    } else {
        this->ui->box_StatsDay_FromMonth->setEnabled( false );
        this->on_box_StatsDay_FromMonth_currentIndexChanged( -1 );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_FromMonth_currentIndexChanged(int index)
{
    this->ui->box_StatsDay_FromDay->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsDay_FromDay->addItems(
            this->crapview.getDays(
                this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
                this->ui->box_StatsDay_FromYear->currentText(),
                this->ui->box_StatsDay_FromMonth->currentText() ) );
        this->ui->box_StatsDay_FromDay->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsDay_FromDay->count() > 0 ) {
        this->ui->box_StatsDay_FromDay->setEnabled( true );
        this->on_box_StatsDay_FromDay_currentIndexChanged(
            this->ui->box_StatsDay_FromDay->currentIndex() );
    } else {
        this->ui->box_StatsDay_FromDay->setEnabled( false );
        this->on_box_StatsDay_FromDay_currentIndexChanged( -1 );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_FromDay_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->checkStatsDayDrawable();
}

void MainWindow::on_checkBox_StatsDay_Period_stateChanged(int state)
{
    if ( state == Qt::CheckState::Checked ) {
        this->ui->box_StatsDay_ToYear->setEnabled( true );
        this->ui->box_StatsDay_ToMonth->setEnabled( true );
        this->ui->box_StatsDay_ToDay->setEnabled( true );
        // add available dates
        this->ui->box_StatsDay_ToYear->clear();
        this->ui->box_StatsDay_ToYear->addItems( this->crapview.getYears(
            this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ) ) );
        this->ui->box_StatsDay_ToYear->setCurrentIndex( 0 );
        if ( this->ui->box_StatsDay_ToYear->count() > 0 ) {
            this->ui->box_StatsDay_ToYear->setEnabled( true );
            this->on_box_StatsDay_ToYear_currentIndexChanged(
                this->ui->box_StatsDay_ToYear->currentIndex() );
        } else {
            this->ui->box_StatsDay_ToYear->setEnabled( false );
            this->on_box_StatsDay_ToYear_currentIndexChanged( -1 );
        }
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
    if ( index >= 0 ) {
        this->ui->box_StatsDay_ToMonth->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
                this->ui->box_StatsDay_ToYear->currentText() ) );
        this->ui->box_StatsDay_ToMonth->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsDay_ToMonth->count() > 0 ) {
        this->ui->box_StatsDay_ToMonth->setEnabled( true );
        this->on_box_StatsDay_ToMonth_currentIndexChanged(
            this->ui->box_StatsDay_ToMonth->currentIndex() );
    } else {
        this->ui->box_StatsDay_ToMonth->setEnabled( false );
        this->on_box_StatsDay_ToMonth_currentIndexChanged( -1 );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_ToMonth_currentIndexChanged(int index)
{
    this->ui->box_StatsDay_ToDay->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsDay_ToDay->addItems(
            this->crapview.getDays(
                this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
                this->ui->box_StatsDay_ToYear->currentText(),
                this->ui->box_StatsDay_ToMonth->currentText() ) );
        this->ui->box_StatsDay_ToDay->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsDay_ToDay->count() > 0 ) {
        this->ui->box_StatsDay_ToDay->setEnabled( true );
        this->on_box_StatsDay_ToDay_currentIndexChanged(
            this->ui->box_StatsDay_ToDay->currentIndex() );
    } else {
        this->ui->box_StatsDay_ToDay->setEnabled( false );
        this->on_box_StatsDay_ToDay_currentIndexChanged( -1 );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_box_StatsDay_ToDay_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->checkStatsDayDrawable();
}

void MainWindow::on_inLine_StatsDay_Filter_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1)
    if ( this->getStatsDayParsedFilter().has_value() ) {
        this->ui->inLine_StatsDay_Filter->setStyleSheet(
            this->stylesheet_lineedit );
    } else {
        this->ui->inLine_StatsDay_Filter->setStyleSheet(
            this->stylesheet_lineedit_err );
    }
    this->checkStatsDayDrawable();
}

void MainWindow::on_button_StatsDay_Draw_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->setDbWorkingState( true );
        this->crapview_timer.reset( new QTimer(this) );
        this->crapview_timer->setSingleShot( true );
        connect( this->crapview_timer.get(), &QTimer::timeout,
                 this, &MainWindow::drawStatsDay);
        this->crapview_timer->start(250);
    }
}
void MainWindow::drawStatsDay()
{
    const bool period{ this->ui->checkBox_StatsDay_Period->isChecked() };
    this->ui->chart_StatsDay->setChart( new QChart() );
    this->crapview.drawDay(
        this->ui->chart_StatsDay,
        this->CHARTS_THEMES.at( static_cast<std::size_t>(GlobalConfigs::charts_theme) ),
        this->wsFromIndex( this->ui->box_StatsDay_WebServer->currentIndex() ),
        this->ui->box_StatsDay_FromYear->currentText(),
        this->ui->box_StatsDay_FromMonth->currentText(),
        this->ui->box_StatsDay_FromDay->currentText(),
        period ? this->ui->box_StatsDay_ToYear->currentText()  : QString(),
        period ? this->ui->box_StatsDay_ToMonth->currentText() : QString(),
        period ? this->ui->box_StatsDay_ToDay->currentText()   : QString(),
        this->ui->box_StatsDay_LogsField->currentText(),
        this->getStatsDayLogField(),
        this->getStatsDayParsedFilter().value() );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsDay );
    this->setDbWorkingState( false );
}



////////////////////
//// RELATIONAL ////
void MainWindow::checkStatsRelatDrawable()
{
    if ( ! this->dbUsable() ) {
        this->ui->button_StatsRelat_Draw->setEnabled( false );
        return;
    }

    bool aux{ true };
    if ( this->ui->box_StatsRelat_FromYear->currentIndex() < 0
      || this->ui->box_StatsRelat_FromMonth->currentIndex() < 0
      || this->ui->box_StatsRelat_FromDay->currentIndex() < 0
      || this->ui->box_StatsRelat_ToYear->currentIndex() < 0
      || this->ui->box_StatsRelat_ToMonth->currentIndex() < 0
      || this->ui->box_StatsRelat_ToDay->currentIndex() < 0 ) {
        aux &= false;
    } else {
        // check period validity
        int a{ this->ui->box_StatsRelat_ToYear->currentText().toInt() };
        int b{ this->ui->box_StatsRelat_FromYear->currentText().toInt() };
        if ( a < b ) {
            // year 'to' is less than 'from'
            aux &= false;
        } else if ( a == b ) {
            a = this->crapview.getMonthNumber( this->ui->box_StatsRelat_ToMonth->currentText() );
            b = this->crapview.getMonthNumber( this->ui->box_StatsRelat_FromMonth->currentText() );
            if ( a < b ) {
                // month 'to' is less than 'from'
                aux &= false;
            } else if ( a == b ) {
                a = this->ui->box_StatsRelat_ToDay->currentText().toInt();
                b = this->ui->box_StatsRelat_FromDay->currentText().toInt();
                if ( a < b ) {
                    // day 'to' is less than 'from'
                    aux &= false;
                }
            }
        }
    }
    // check log field validity
    if ( this->ui->box_StatsRelat_LogsField_1->currentIndex() < 0
      || this->ui->box_StatsRelat_LogsField_2->currentIndex() < 0 ) {
        aux &= false;
    }
    // check filter string validity
    if ( !this->getStatsRelatParsedFilter( 1 ).has_value()
      || !this->getStatsRelatParsedFilter( 2 ).has_value() ) {
        aux &= false;
    }
    this->ui->button_StatsRelat_Draw->setEnabled( aux );
}


LogField MainWindow::getStatsRelatLogField( const int filter_num ) const
{
    const QComboBox* cb{ ( filter_num == 1 )
                         ? this->ui->box_StatsRelat_LogsField_1
                         : this->ui->box_StatsRelat_LogsField_2 };

    const QModelIndex idx{ cb->model()->index( cb->currentIndex(), 0 ) };
    return cb->model()->data(idx,Qt::UserRole).value<LogField>();
}
std::optional<QString> MainWindow::getStatsRelatParsedFilter( const int filter_num ) const
{
    const QString fld_t{ ( filter_num == 1 )
                         ? this->ui->inLine_StatsRelat_Filter_1->text()
                         : this->ui->inLine_StatsRelat_Filter_2->text() };

    switch ( this->getStatsRelatLogField( filter_num ) ) {
        case LogField::ResponseCode:
            [[fallthrough]];
        case LogField::TimeTaken:
            [[fallthrough]];
        case LogField::BytesReceived:
            [[fallthrough]];
        case LogField::BytesSent:
            return FilterOps::parseNumericFilter( fld_t );
        default:
            return FilterOps::parseTextualFilter( fld_t );
    }
}

void MainWindow::on_box_StatsRelat_WebServer_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_FromYear->clear();
    this->ui->box_StatsRelat_ToYear->clear();
    if ( index >= 0 ) {
        // refresh dates
        const QStringList years{ this->crapview.getYears( this->wsFromIndex( index ) ) };
        // from
        this->ui->box_StatsRelat_FromYear->clear();
        this->ui->box_StatsRelat_FromYear->addItems( years );
        this->ui->box_StatsRelat_FromYear->setCurrentIndex( 0 );
        // to
        this->ui->box_StatsRelat_ToYear->clear();
        this->ui->box_StatsRelat_ToYear->addItems( years );
        this->ui->box_StatsRelat_ToYear->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsRelat_FromYear->count() > 0 ) {
        this->ui->box_StatsRelat_FromYear->setEnabled( true );
        this->on_box_StatsRelat_FromYear_currentIndexChanged(
            this->ui->box_StatsRelat_FromYear->currentIndex() );
    } else {
        this->ui->box_StatsRelat_FromYear->setEnabled( false );
        this->on_box_StatsRelat_FromYear_currentIndexChanged( -1 );
    }
    if ( this->ui->box_StatsRelat_ToYear->count() > 0 ) {
        this->ui->box_StatsRelat_ToYear->setEnabled( true );
        this->on_box_StatsRelat_ToYear_currentIndexChanged(
            this->ui->box_StatsRelat_ToYear->currentIndex() );
    } else {
        this->ui->box_StatsRelat_ToYear->setEnabled( false );
        this->on_box_StatsRelat_ToYear_currentIndexChanged( -1 );
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_LogsField_1_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->ui->inLine_StatsRelat_Filter_1->clear();
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_LogsField_2_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->ui->inLine_StatsRelat_Filter_2->clear();
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_FromYear_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_FromMonth->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsRelat_FromMonth->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
                this->ui->box_StatsRelat_FromYear->currentText() ) );
        this->ui->box_StatsRelat_FromMonth->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsRelat_FromMonth->count() > 0 ) {
        this->ui->box_StatsRelat_FromMonth->setEnabled( true );
        this->on_box_StatsRelat_FromMonth_currentIndexChanged(
            this->ui->box_StatsRelat_FromMonth->currentIndex() );
    } else {
        this->ui->box_StatsRelat_FromMonth->setEnabled( false );
        this->on_box_StatsRelat_FromMonth_currentIndexChanged( -1 );
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_FromMonth_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_FromDay->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsRelat_FromDay->addItems(
            this->crapview.getDays(
                this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
                this->ui->box_StatsRelat_FromYear->currentText(),
                this->ui->box_StatsRelat_FromMonth->currentText() ) );
        this->ui->box_StatsRelat_FromDay->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsRelat_FromDay->count() > 0 ) {
        this->ui->box_StatsRelat_FromDay->setEnabled( true );
        this->on_box_StatsRelat_FromDay_currentIndexChanged(
            this->ui->box_StatsRelat_FromDay->currentIndex() );
    } else {
        this->ui->box_StatsRelat_FromDay->setEnabled( false );
        this->on_box_StatsRelat_FromDay_currentIndexChanged( -1 );
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_FromDay_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_ToYear_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_ToMonth->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsRelat_ToMonth->addItems(
            this->crapview.getMonths(
                this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
                this->ui->box_StatsRelat_ToYear->currentText() ) );
        this->ui->box_StatsRelat_ToMonth->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsRelat_ToMonth->count() > 0 ) {
        this->ui->box_StatsRelat_ToMonth->setEnabled( true );
        this->on_box_StatsRelat_ToMonth_currentIndexChanged(
            this->ui->box_StatsRelat_ToMonth->currentIndex() );
    } else {
        this->ui->box_StatsRelat_ToMonth->setEnabled( false );
        this->on_box_StatsRelat_ToMonth_currentIndexChanged( -1 );
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_ToMonth_currentIndexChanged(int index)
{
    this->ui->box_StatsRelat_ToDay->clear();
    if ( index >= 0 ) {
        this->ui->box_StatsRelat_ToDay->addItems(
            this->crapview.getDays(
                this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
                this->ui->box_StatsRelat_ToYear->currentText(),
                this->ui->box_StatsRelat_ToMonth->currentText() ) );
        this->ui->box_StatsRelat_ToDay->setCurrentIndex( 0 );
    }
    if ( this->ui->box_StatsRelat_ToDay->count() > 0 ) {
        this->ui->box_StatsRelat_ToDay->setEnabled( true );
        this->on_box_StatsRelat_ToDay_currentIndexChanged(
            this->ui->box_StatsRelat_ToDay->currentIndex() );
    } else {
        this->ui->box_StatsRelat_ToDay->setEnabled( false );
        this->on_box_StatsRelat_ToDay_currentIndexChanged( -1 );
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_box_StatsRelat_ToDay_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->checkStatsRelatDrawable();
}

void MainWindow::on_inLine_StatsRelat_Filter_1_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    if ( this->getStatsRelatParsedFilter( 1 ).has_value() ) {
        this->ui->inLine_StatsRelat_Filter_1->setStyleSheet(
            this->stylesheet_lineedit );
    } else {
        this->ui->inLine_StatsRelat_Filter_1->setStyleSheet(
            this->stylesheet_lineedit_err );
    }
    this->checkStatsRelatDrawable();
}

void MainWindow::on_inLine_StatsRelat_Filter_2_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    if ( this->getStatsRelatParsedFilter( 2 ).has_value() ) {
        this->ui->inLine_StatsRelat_Filter_2->setStyleSheet(
            this->stylesheet_lineedit );
    } else {
        this->ui->inLine_StatsRelat_Filter_2->setStyleSheet(
            this->stylesheet_lineedit_err );
    }
    this->checkStatsRelatDrawable();
}


void MainWindow::on_button_StatsRelat_Draw_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->setDbWorkingState( true );
        this->crapview_timer.reset( new QTimer(this) );
        this->crapview_timer->setSingleShot( true );
        connect( this->crapview_timer.get(), &QTimer::timeout,
                 this, &MainWindow::drawStatsRelat);
        this->crapview_timer->start(250);
    }
}
void MainWindow::drawStatsRelat()
{
    this->ui->chart_StatsRelat->setChart( new QChart() );
    this->crapview.drawRelat(
        this->ui->chart_StatsRelat,
        this->CHARTS_THEMES.at( static_cast<std::size_t>(GlobalConfigs::charts_theme) ),
        this->wsFromIndex( this->ui->box_StatsRelat_WebServer->currentIndex() ),
        this->ui->box_StatsRelat_FromYear->currentText(),
        this->ui->box_StatsRelat_FromMonth->currentText(),
        this->ui->box_StatsRelat_FromDay->currentText(),
        this->ui->box_StatsRelat_ToYear->currentText(),
        this->ui->box_StatsRelat_ToMonth->currentText(),
        this->ui->box_StatsRelat_ToDay->currentText(),
        this->ui->box_StatsRelat_LogsField_1->currentText(),
        this->getStatsRelatLogField( 1 ),
        this->getStatsRelatParsedFilter( 1 ).value(),
        this->ui->box_StatsRelat_LogsField_2->currentText(),
        this->getStatsRelatLogField( 2 ),
        this->getStatsRelatParsedFilter( 2 ).value() );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsRelat );
    this->setDbWorkingState( false );
}



////////////////
//// GLOBAL ////
void MainWindow::drawStatsGlobals()
{
    std::vector<std::tuple<QString,QString>> recur_list;
    std::vector<std::tuple<QString,QString>> traffic_list;
    std::vector<std::tuple<QString,QString>> perf_list;
    std::vector<QString> work_list;

    const bool result{ this->crapview.calcGlobals(
        recur_list, traffic_list, perf_list, work_list,
        this->glob_ws ) };

    if ( ! result ) {
        this->resetStatsGlob();
    } else {
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
    }
    this->setDbWorkingState( false );
}

void MainWindow::resetStatsGlob()
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

    this->ui->button_StatsGlob_Apache->setFlat( true );
    this->ui->button_StatsGlob_Nginx->setFlat( true );
    this->ui->button_StatsGlob_Iis->setFlat( true );
}

void MainWindow::makeStatsGlob()
{
    this->setDbWorkingState( true );
    this->crapview_timer.reset( new QTimer(this) );
    this->crapview_timer->setSingleShot( true );
    connect( this->crapview_timer.get(), &QTimer::timeout,
             this, &MainWindow::drawStatsGlobals);
    this->crapview_timer->start(250);
}

void MainWindow::on_button_StatsGlob_Apache_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->glob_ws = "apache";
        this->makeStatsGlob();
    }
}

void MainWindow::on_button_StatsGlob_Nginx_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->glob_ws = "nginx";
        this->makeStatsGlob();
    }
}

void MainWindow::on_button_StatsGlob_Iis_clicked()
{
    if ( this->dbUsable() && this->checkDataDB() ) {
        this->glob_ws = "iis";
        this->makeStatsGlob();
    }
}



/////////////////////////
//////// CONFIGS ////////
/////////////////////////

void MainWindow::on_tree_ConfSections_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    const QString section{ item->text(0) };
    if ( section == tr("General") ) {
        return;
    } else if ( section == tr("Window") ) {
        this->setConfigsPage( General_Window );
    } else if ( section == tr("Dialogs") ) {
        this->setConfigsPage( General_Dialogs );
    } else if ( section == tr("Charts") ) {
        return;
    } else if ( section == tr("Appearance") ) {
        this->setConfigsPage( General_Charts_Appearance );
    } else if ( section == tr("Speed") ) {
        this->setConfigsPage( General_Charts_Speed );
    } else if ( section == tr("Count") ) {
        this->setConfigsPage( General_Charts_Count );
    } else if ( section == tr("Relational") ) {
        this->setConfigsPage( General_Charts_Relational );
    } else if ( section == tr("TextBrowser") ) {
        this->setConfigsPage( General_TextBrowser );
    } else if ( section == tr("Databases") ) {
        this->setConfigsPage( General_Databases );
    } else if ( section == tr("Security") ) {
        this->setConfigsPage( General_Security );
    } else if ( section == tr("Logs") ) {
        return;
    } else if ( section == tr("Defaults") ) {
        this->setConfigsPage( Logs_Defaults );
    } else if ( section == tr("Control") ) {
        this->setConfigsPage( Logs_Control );
    } else if ( section == tr("Apache2") || section == tr("Nginx") || section == tr("IIS") ) {
        return;
    } else if ( section == tr("Path") ) {
        const QString parent{ item->parent()->text(0) };
        if ( parent == tr("Apache2") ) {
            this->setConfigsPage( Logs_Apache2_Path );
        } else if ( parent == tr("Nginx") ) {
            this->setConfigsPage( Logs_Nginx_Path );
        } else if ( parent == tr("IIS") ) {
            this->setConfigsPage( Logs_IIS_Path );
        }
    } else if ( section == tr("Format") ) {
        const QString parent{ item->parent()->text(0) };
        if ( parent == tr("Apache2") ) {
            this->setConfigsPage( Logs_Apache2_Format );
        } else if ( parent == tr("Nginx") ) {
            this->setConfigsPage( Logs_Nginx_Format );
        } else if ( parent == tr("IIS") ) {
            this->setConfigsPage( Logs_IIS_Format );
        }
    } else if ( section == tr("Warnlists") ) {
        const QString parent{ item->parent()->text(0) };
        if ( parent == tr("Apache2") ) {
            this->setConfigsPage( Logs_Apache2_Warnlists );
        } else if ( parent == tr("Nginx") ) {
            this->setConfigsPage( Logs_Nginx_Warnlists );
        } else if ( parent == tr("IIS") ) {
            this->setConfigsPage( Logs_IIS_Warnlists );
        }
    } else if ( section == tr("Blacklists") ) {
        const QString parent{ item->parent()->text(0) };
        if ( parent == tr("Apache2") ) {
            this->setConfigsPage( Logs_Apache2_Blacklists );
        } else if ( parent == tr("Nginx") ) {
            this->setConfigsPage( Logs_Nginx_Blacklists );
        } else if ( parent == tr("IIS") ) {
            this->setConfigsPage( Logs_IIS_Blacklists );
        }
    } else {
        throw("Unexpected Conf section: "+section.toStdString());
    }
}

void MainWindow::on_tree_ConfSections_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    this->on_tree_ConfSections_itemClicked( item, column );
}

void MainWindow::setConfigsPage( const ConfigsPage page )
{
    this->ui->stackedPages_Conf->setCurrentIndex( page );
}

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
    GlobalConfigs::window_theme = static_cast<WindowTheme>(index);
    this->updateUiTheme();
}

void MainWindow::on_box_ConfWindow_Icons_currentIndexChanged(int index)
{
    GlobalConfigs::icons_theme = static_cast<IconsTheme>(index);
    this->updateUiIcons();
}

void MainWindow::on_box_ConfWindow_Language_currentIndexChanged(int index)
{
    const QModelIndex idx{ this->ui->box_ConfWindow_Language->model()->index(
        index, 0 ) };
    this->language = this->ui->box_ConfWindow_Language->model()->data(
        idx, Qt::UserRole).value<std::string>();
    this->updateUiLanguage();
}


/////////////////
//// DIALOGS ////
void MainWindow::on_slider_ConfDialogs_General_sliderReleased()
{
    this->dialogs_level = this->dialogsLevelFromInt(
        this->ui->slider_ConfDialogs_General->value() );
}
void MainWindow::on_slider_ConfDialogs_Logs_sliderReleased()
{
    this->craplog.setDialogsLevel( this->dialogsLevelFromInt(
        this->ui->slider_ConfDialogs_Logs->value() ) );
}
void MainWindow::on_slider_ConfDialogs_Stats_sliderReleased()
{
    this->crapview.setDialogsLevel( this->dialogsLevelFromInt(
        this->ui->slider_ConfDialogs_Stats->value() ) );
}


//////////////////////
//// TEXT BROWSER ////
void MainWindow::on_box_ConfTextBrowser_Font_currentIndexChanged(int index)
{
    std::string f;
    switch ( index ) {
        case 0:
            f = "main"; break;
        case 1:
            f = "alternative"; break;
        case 2:
            f = "script"; break;
        default:
            throw DoNotCatchException( "Unexpected Font index", std::to_string(index) );
    }
    const QFont& font{ this->fonts.at( f ) };
    this->TB.setFont( font );
    this->ui->textBrowser_ConfTextBrowser_Preview->setFont( font );
    this->ui->preview_ConfApache_Format_Sample->setFont( font );
    this->ui->preview_ConfNginx_Format_Sample->setFont( font );
    this->ui->preview_ConfIis_Format_Sample->setFont( font );
    if ( !this->crapnote.isNull() ) {
        this->crapnote->setTextFont( font );
    }
    if ( !this->changelog.isNull() ) {
        this->changelog->setTextFont( font );
    }
}
void MainWindow::on_checkBox_ConfTextBrowser_WideLines_clicked(bool checked)
{
    this->TB.setWideLinesUsage( checked );
    this->refreshTextBrowserPreview();
}
void MainWindow::on_box_ConfTextBrowser_ColorScheme_currentIndexChanged(int index)
{
    ColorsScheme cs{ static_cast<ColorsScheme>( index ) };
    GlobalConfigs::colors_scheme = cs;
    this->TB.setColorScheme( cs, this->tb_colors_schemes.at( cs ) );
    if ( !this->crapnote.isNull() ) {
        this->crapnote->setColorScheme( cs );
    }
    this->refreshTextBrowserPreview();
}
void MainWindow::refreshTextBrowserPreview()
{
    QString content;
    this->TB.makePreview( content );
    this->ui->textBrowser_ConfTextBrowser_Preview->setText( content );
    this->ui->textBrowser_ConfTextBrowser_Preview->setFont( this->TB.getFont() );
}


////////////////
//// CHARTS ////
void MainWindow::on_box_ConfCharts_Theme_currentIndexChanged(int index)
{
    GlobalConfigs::charts_theme = static_cast<ChartsTheme>(index);
    this->refreshChartsPreview();

    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_MakeStats_Size );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsWarn );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsSpeed );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsCount );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsDay );
    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_StatsRelat );
}
void MainWindow::refreshChartsPreview()
{
    QColor col{ Qt::GlobalColor::darkGreen };
    QBarSet* bars_1{ new QBarSet( "" ) };
    bars_1->setColor( col );
    QBarSet* bars_2{ new QBarSet( "" ) };
    bars_2->setColor( col );
    QBarSet* bars_3{ new QBarSet( "" ) };
    bars_3->setColor( col );
    QBarSet* bars_4{ new QBarSet( "" ) };
    bars_4->setColor( col );
    QBarSet* bars_5{ new QBarSet( "" ) };
    bars_5->setColor( col );
    QBarSet* bars_6{ new QBarSet( "" ) };
    bars_6->setColor( col );

    const auto random_value = [] () -> int {
        return (rand()%10 > 8) ? rand()%100 : (rand()%10 > 6) ? rand()%50 : rand()%30;
    };
    int aux, max{0};
    for ( int i{0}; i<24; ++i ) {
        aux = random_value();
        *bars_1 << aux;
        if ( aux > max ) {
            max = aux;
        }
        aux = random_value();
        *bars_2 << aux;
        if ( aux > max ) {
            max = aux;
        }
        aux = random_value();
        *bars_3 << aux;
        if ( aux > max ) {
            max = aux;
        }
        aux = random_value();
        *bars_4 << aux;
        if ( aux > max ) {
            max = aux;
        }
        aux = random_value();
        *bars_5 << aux;
        if ( aux > max ) {
            max = aux;
        }
        aux = random_value();
        *bars_6 << aux;
        if ( aux > max ) {
            max = aux;
        }
    }

    QBarSeries* bars{ new QBarSeries() };
    bars->append( bars_1 ); bars->append( bars_2 ); bars->append( bars_3 );
    bars->append( bars_4 ); bars->append( bars_5 ); bars->append( bars_6 );
    bars->setBarWidth( 1 );

    QChart* t_chart{ new QChart() };
    // apply the theme
    t_chart->setTheme( this->CHARTS_THEMES.at( static_cast<std::size_t>(GlobalConfigs::charts_theme) ) );
    // add the bars
    t_chart->addSeries( bars );
    t_chart->setTitle( "Sample preview" );
    t_chart->setTitleFont( this->fonts.at("main") );
    t_chart->setAnimationOptions( QChart::SeriesAnimations );

    QStringList categories;
    categories << "00" << "01" << "02" << "03" << "04" << "05" << "06" << "07" << "08" << "09" << "10" << "11"
               << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23";

    QBarCategoryAxis* axisX{ new QBarCategoryAxis() };
    axisX->append( categories );
    axisX->setLabelsFont( this->fonts.at( "main_small" ) );
    axisX->setTitleText( "Infoes" );
    axisX->setTitleFont( this->fonts.at("main_small") );
    t_chart->addAxis( axisX, Qt::AlignBottom );
    bars->attachAxis( axisX );

    QValueAxis* axisY{ new QValueAxis() };
    axisY->setLabelFormat( "%d" );
    axisY->setRange( 0, max );
    axisY->setLabelsFont( this->fonts.at( "main_small" ) );
    t_chart->addAxis( axisY, Qt::AlignLeft );
    bars->attachAxis( axisY) ;

    t_chart->legend()->setVisible( false );
    /*t_chart->legend()->setFont( this->FONTS.at("main_small") );
    t_chart->legend()->setAlignment( Qt::AlignBottom );*/

    this->ui->chart_ConfCharts_Preview->setChart( t_chart );
    this->ui->chart_ConfCharts_Preview->setRenderHint( QPainter::Antialiasing );

    ColorSec::applyChartTheme(
        this->fonts,
        this->ui->chart_ConfCharts_Preview );
}

void MainWindow::on_box_ConfCharts_Speed_TimeInterval_currentTextChanged(const QString& arg1)
{
    this->crapview.setSpeedTimeInterval( arg1.toLong() );
}

void MainWindow::on_box_ConfCharts_Speed_TimeFormat_currentTextChanged(const QString& arg1)
{
    this->crapview.setSpeedTimeFormat( arg1 );
}

void MainWindow::on_doubleSpinBox_ConfCharts_Count_PieSize_valueChanged(double arg1)
{
    this->crapview.setCountPieSize( arg1 );
}

void MainWindow::on_spinBox_ConfCharts_Count_MaxSlices_valueChanged(int arg1)
{
    this->crapview.setCountMaxSlices( arg1 );
}

void MainWindow::on_box_ConfCharts_Relat_TimeFormat_currentTextChanged(const QString& arg1)
{
    this->crapview.setRelatTimeFormat( arg1 );
}


///////////////////
//// DATABASES ////
// data collection
void MainWindow::on_tool_ConfDatabases_Data_Dialog_clicked()
{
    this->crappath.reset( new Crappath( this ) );

    const QString current_path{ this->ui->inLine_ConfDatabases_Data_Path->text() };
    if ( ! current_path.isEmpty() ) {
        crappath->setDirectory( current_path );
    } else if ( ! this->db_data_path.getPathUnchecked().empty() ) {
        crappath->setDirectory( QString::fromStdString( this->db_data_path.toString() ) );
    } else {
        crappath->setDirectory( toQString( this->home_path ) );
    }

    if ( crappath->exec() ) {
        const QStringList selection{ crappath->selectedFiles() };
        if ( ! selection.isEmpty() ) {
            this->ui->inLine_ConfDatabases_Data_Path->setText( selection[0] );
        }
    }
}
void MainWindow::on_inLine_ConfDatabases_Data_Path_textChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        const auto path{ PathHandler( arg1.toStdString() ).getPath() };
        if ( !( path.has_value() && IOutils::checkDir( path.value() ) ) ) {
            this->ui->icon_ConfDatabases_Data_Wrong->setVisible( true );
            this->ui->button_ConfDatabases_Data_Save->setEnabled( false );
        } else {
            this->ui->icon_ConfDatabases_Data_Wrong->setVisible( false );
            this->ui->button_ConfDatabases_Data_Save->setEnabled( true );
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
        const auto path_handler{ PathHandler( this->ui->inLine_ConfDatabases_Data_Path->text().toStdString() ) };
        const auto path_exp{ path_handler.getPath() };
        if ( ! path_exp.has_value() ) {
            path_exp.error().showDialogMessage();
            this->ui->icon_ConfDatabases_Data_Wrong->setVisible( true );
            this->ui->button_ConfDatabases_Data_Save->setEnabled( false );
            return;
        }
        const path_t& path{ path_exp.value() };
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        if ( ! IOutils::checkDir( path, false, true ) ) {
            DialogSec::warnDirNotWritable( nullptr );
        }
        this->db_data_path = path_handler;
        this->craplog.setStatsDatabasePath( path_handler );
        this->crapview.setDbPath( path_handler );
        this->ui->inLine_ConfDatabases_Data_Path->setText( toQString( path ) );
    }
    this->ui->inLine_ConfDatabases_Data_Path->setFocus();
    this->ui->button_ConfDatabases_Data_Save->setEnabled( false );
}

// used files hashes
void MainWindow::on_tool_ConfDatabases_Hashes_Dialog_clicked()
{
    this->crappath.reset( new Crappath( this ) );

    const QString current_path{ this->ui->inLine_ConfDatabases_Hashes_Path->text() };
    if ( ! current_path.isEmpty() ) {
        crappath->setDirectory( current_path );
    } else if ( ! this->db_hashes_path.getPathUnchecked().empty() ) {
        crappath->setDirectory( QString::fromStdString( this->db_hashes_path.toString() ) );
    } else {
        crappath->setDirectory( toQString( this->home_path ) );
    }

    if ( crappath->exec() ) {
        const QStringList selection{ crappath->selectedFiles() };
        if ( ! selection.isEmpty() ) {
            this->ui->inLine_ConfDatabases_Hashes_Path->setText( selection[0] );
        }
    }
}
void MainWindow::on_inLine_ConfDatabases_Hashes_Path_textChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        const auto path{ PathHandler( arg1.toStdString() ).getPath() };
        if ( !( path.has_value() && IOutils::checkDir( path.value() ) ) ) {
            this->ui->icon_ConfDatabases_Hashes_Wrong->setVisible( true );
            this->ui->button_ConfDatabases_Hashes_Save->setEnabled( false );
        } else {
            this->ui->icon_ConfDatabases_Hashes_Wrong->setVisible( false );
            this->ui->button_ConfDatabases_Hashes_Save->setEnabled( true );
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
        const auto path_handler{ PathHandler( this->ui->inLine_ConfDatabases_Hashes_Path->text().toStdString() ) };
        const auto path_exp{ path_handler.getPath() };
        if ( ! path_exp.has_value() ) {
            path_exp.error().showDialogMessage();
            this->ui->icon_ConfDatabases_Hashes_Wrong->setVisible( true );
            this->ui->button_ConfDatabases_Hashes_Save->setEnabled( false );
            return;
        }
        const path_t& path{ path_exp.value() };
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        if ( ! IOutils::checkDir( path, false, true ) ) {
            DialogSec::warnDirNotWritable( nullptr );
        }
        this->db_hashes_path = path_handler;
        this->craplog.setHashesDatabasePath( path_handler );
        this->ui->inLine_ConfDatabases_Hashes_Path->setText( toQString( path ) );
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


//////////////////
//// SECURITY ////
void MainWindow::on_checkBox_ConfSecurity_Paths_Symlinks_toggled(bool checked)
{
    GlobalConfigs::Security::follow_symlinks = checked;
}


//////////////
//// LOGS ////
//////////////

//////////////////
//// DEFAULTS ////
void MainWindow::on_radio_ConfDefaults_Apache_toggled(bool checked)
{
    Q_UNUSED(checked)
    this->default_web_server = WS_APACHE;
}
void MainWindow::on_radio_ConfDefaults_Nginx_toggled(bool checked)
{
    Q_UNUSED(checked)
    this->default_web_server = WS_NGINX;
}
void MainWindow::on_radio_ConfDefaults_Iis_toggled(bool checked)
{
    Q_UNUSED(checked)
    this->default_web_server = WS_IIS;
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
        this->craplog.setWarningSize( 0ul );
    } else {
        // enable warning
        this->ui->spinBox_ConfControl_Size->setEnabled( true );
        this->craplog.setWarningSize(
            (static_cast<std::size_t>(this->ui->spinBox_ConfControl_Size->value()) * 1'048'576ul) +1ul );
    }
}
void MainWindow::on_spinBox_ConfControl_Size_editingFinished()
{
    this->craplog.setWarningSize(
        (static_cast<std::size_t>(this->ui->spinBox_ConfControl_Size->value()) * 1'048'576ul) +1ul );
}


////////////////
//// APACHE ////
// paths
void MainWindow::on_tool_ConfApache_Path_Dialog_clicked()
{
    this->crappath.reset( new Crappath( this ) );

    const QString current_path{ this->ui->inLine_ConfApache_Path_String->text() };
    const PathHandler& logs_path{ this->craplog.getLogsPath( WS_APACHE ) };
    if ( ! current_path.isEmpty() ) {
        crappath->setDirectory( current_path );
    } else if ( logs_path.getPath().has_value() ) {
        crappath->setDirectory( QString::fromStdString( logs_path.toString() ) );
    } else {
        crappath->setDirectory( QString::fromStdString( this->home_path ) );
    }

    if ( crappath->exec() ) {
        const QStringList selection{ crappath->selectedFiles() };
        if ( ! selection.isEmpty() ) {
            this->ui->inLine_ConfApache_Path_String->setText( selection[0] );
        }
    }
}
void MainWindow::on_inLine_ConfApache_Path_String_textChanged(const QString& arg1)
{
    if ( arg1.size() > 0 ) {
        const auto path{ PathHandler( arg1.toStdString() ).getPath() };
        if ( !( path.has_value() && IOutils::checkDir( path.value() ) ) ) {
            this->ui->icon_ConfApache_Path_Wrong->setVisible( true );
            this->ui->button_ConfApache_Path_Save->setEnabled( false );
        } else {
            this->ui->icon_ConfApache_Path_Wrong->setVisible( false );
            this->ui->button_ConfApache_Path_Save->setEnabled( true );
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
        const auto path_handler{ PathHandler( this->ui->inLine_ConfApache_Path_String->text().toStdString() ) };
        const auto path_exp{ path_handler.getPath() };
        if ( ! path_exp.has_value() ) {
            path_exp.error().showDialogMessage();
            this->ui->icon_ConfApache_Path_Wrong->setVisible( true );
            this->ui->button_ConfApache_Path_Save->setEnabled( false );
            return;
        }
        const path_t& path{ path_exp.value() };
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        this->craplog.setLogsPath( WS_APACHE, path_handler );
        this->ui->inLine_ConfApache_Path_String->setText( toQString( path ) );
    }
    this->ui->button_ConfApache_Path_Save->setEnabled( false );
}

// formats
void MainWindow::on_inLine_ConfApache_Format_String_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1)
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
    const bool success{ this->craplog.setApacheLogFormat(
        this->ui->inLine_ConfApache_Format_String->text().trimmed().toStdString() ) };
    if ( success ) {
        this->ui->button_ConfApache_Format_Save->setEnabled( false );
        if ( this->craplog.getCurrentWebServer() == WS_APACHE ) {
            this->craplog.setCurrentLogFormat();
        }
    }
}
void MainWindow::on_button_ConfApache_Format_Sample_clicked()
{
    this->ui->preview_ConfApache_Format_Sample->setText(
        this->craplog.getLogsFormatSample( WS_APACHE ) );
}
void MainWindow::on_button_ConfApache_Format_Help_clicked()
{
    this->showHelp( "apache_format" );
}

// warnlists
void MainWindow::on_box_ConfApache_Warnlist_Field_currentTextChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        this->ui->inLine_ConfApache_Warnlist_String->clear();
        this->ui->list_ConfApache_Warnlist_List->clear();
        // update the list
        const WarnlistField field{ this->warnlistFieldFromString( arg1 ) };
        const std::vector<std::string>& list{ this->warnlists.getListConst(
            WS_APACHE, field ) };
        for ( const std::string& item : list ) {
            this->ui->list_ConfApache_Warnlist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        const bool used{ this->warnlists.isUsed(
            WS_APACHE, field ) };
        this->ui->checkBox_ConfApache_Warnlist_Used->setChecked( used );
        this->on_checkBox_ConfApache_Warnlist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfApache_Warnlist_Used_clicked(bool checked)
{
    this->warnlists.setUsed(
        WS_APACHE,
        this->warnlistFieldFromString( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfApache_Warnlist_String->setEnabled( true );
        this->ui->list_ConfApache_Warnlist_List->setEnabled( true );
        this->ui->button_ConfApache_Warnlist_Add->setFlat( false );
        this->ui->button_ConfApache_Warnlist_Remove->setFlat( false );
        this->ui->button_ConfApache_Warnlist_Up->setFlat( false );
        this->ui->button_ConfApache_Warnlist_Down->setFlat( false );
    } else {
        this->ui->inLine_ConfApache_Warnlist_String->clear();
        this->ui->inLine_ConfApache_Warnlist_String->setEnabled( false );
        this->ui->list_ConfApache_Warnlist_List->clearSelection();
        this->ui->list_ConfApache_Warnlist_List->setEnabled( false );
        this->ui->button_ConfApache_Warnlist_Add->setFlat( true );
        this->ui->button_ConfApache_Warnlist_Remove->setFlat( true );
        this->ui->button_ConfApache_Warnlist_Up->setFlat( true );
        this->ui->button_ConfApache_Warnlist_Down->setFlat( true );
    }
}

void MainWindow::on_inLine_ConfApache_Warnlist_String_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1)
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
    const QString& item{ this->ui->inLine_ConfApache_Warnlist_String->text() };
    if ( this->ui->list_ConfApache_Warnlist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).isEmpty() ) {
        // not in the list yet, append
        try {
            this->warnlists.addItem(
                WS_APACHE,
                this->warnlistFieldFromString( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
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
        const auto item{ this->ui->list_ConfApache_Warnlist_List->selectedItems().at(0) };
        const int max{ this->ui->list_ConfApache_Warnlist_List->count() -1 };
        if ( max == 0 ) {
            this->ui->button_ConfApache_Warnlist_Up->setEnabled( false );
            this->ui->button_ConfApache_Warnlist_Down->setEnabled( false );
        } else {
            for ( int i{0}; i<=max; ++i ) {
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
    const auto item{ this->ui->list_ConfApache_Warnlist_List->selectedItems().at(0) };
    this->warnlists.removeItem(
        WS_APACHE,
        this->warnlistFieldFromString( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Warnlist_Field_currentTextChanged( this->ui->box_ConfApache_Warnlist_Field->currentText() );
}
void MainWindow::on_button_ConfApache_Warnlist_Up_clicked()
{
    const auto item{ this->ui->list_ConfApache_Warnlist_List->selectedItems().at(0) };
    const int i{ this->warnlists.moveUpItem(
        WS_APACHE,
        this->warnlistFieldFromString( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfApache_Warnlist_Field_currentTextChanged( this->ui->box_ConfApache_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfApache_Warnlist_List->setFocus();
}
void MainWindow::on_button_ConfApache_Warnlist_Down_clicked()
{
    const auto item{ this->ui->list_ConfApache_Warnlist_List->selectedItems().at(0) };
    const int i{ this->warnlists.moveDownItem(
        WS_APACHE,
        this->warnlistFieldFromString( this->ui->box_ConfApache_Warnlist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfApache_Warnlist_Field_currentTextChanged( this->ui->box_ConfApache_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfApache_Warnlist_List->setFocus();
}


// blacklist
void MainWindow::on_box_ConfApache_Blacklist_Field_currentTextChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        this->ui->inLine_ConfApache_Blacklist_String->clear();
        this->ui->list_ConfApache_Blacklist_List->clear();
        // update the list
        const BlacklistField field{ this->blacklistFieldFromString( arg1 ) };
        const std::vector<std::string>& list{ this->blacklists.getListConst(
            WS_APACHE, field ) };
        for ( const std::string& item : list ) {
            this->ui->list_ConfApache_Blacklist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        const bool used{ this->blacklists.isUsed(
            WS_APACHE, field ) };
        this->ui->checkBox_ConfApache_Blacklist_Used->setChecked( used );
        this->on_checkBox_ConfApache_Blacklist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfApache_Blacklist_Used_clicked(bool checked)
{
    this->blacklists.setUsed(
        WS_APACHE,
        this->blacklistFieldFromString( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfApache_Blacklist_String->setEnabled( true );
        this->ui->list_ConfApache_Blacklist_List->setEnabled( true );
        this->ui->button_ConfApache_Blacklist_Add->setFlat( false );
        this->ui->button_ConfApache_Blacklist_Remove->setFlat( false );
        this->ui->button_ConfApache_Blacklist_Up->setFlat( false );
        this->ui->button_ConfApache_Blacklist_Down->setFlat( false );
    } else {
        this->ui->inLine_ConfApache_Blacklist_String->clear();
        this->ui->inLine_ConfApache_Blacklist_String->setEnabled( false );
        this->ui->list_ConfApache_Blacklist_List->clearSelection();
        this->ui->list_ConfApache_Blacklist_List->setEnabled( false );
        this->ui->button_ConfApache_Blacklist_Add->setFlat( true );
        this->ui->button_ConfApache_Blacklist_Remove->setFlat( true );
        this->ui->button_ConfApache_Blacklist_Up->setFlat( true );
        this->ui->button_ConfApache_Blacklist_Down->setFlat( true );
    }
}

void MainWindow::on_inLine_ConfApache_Blacklist_String_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1)
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
    const QString& item{ this->ui->inLine_ConfApache_Blacklist_String->text() };
    if ( this->ui->list_ConfApache_Blacklist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).isEmpty() ) {
        // not in the list yet, append
        try {
            this->blacklists.addItem(
                WS_APACHE,
                this->blacklistFieldFromString( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
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
        const auto item{ this->ui->list_ConfApache_Blacklist_List->selectedItems().at(0) };
        const int max{ this->ui->list_ConfApache_Blacklist_List->count() -1 };
        if ( max == 0 ) {
            this->ui->button_ConfApache_Blacklist_Up->setEnabled( false );
            this->ui->button_ConfApache_Blacklist_Down->setEnabled( false );
        } else {
            for ( int i{0}; i<=max; ++i ) {
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
    const auto item{ this->ui->list_ConfApache_Blacklist_List->selectedItems().at(0) };
    this->blacklists.removeItem(
        WS_APACHE,
        this->blacklistFieldFromString( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfApache_Blacklist_Field_currentTextChanged( this->ui->box_ConfApache_Blacklist_Field->currentText() );
}
void MainWindow::on_button_ConfApache_Blacklist_Up_clicked()
{
    const auto item{ this->ui->list_ConfApache_Blacklist_List->selectedItems().at(0) };
    const int i{ this->blacklists.moveUpItem(
        WS_APACHE,
        this->blacklistFieldFromString( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfApache_Blacklist_Field_currentTextChanged( this->ui->box_ConfApache_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfApache_Blacklist_List->setFocus();
}
void MainWindow::on_button_ConfApache_Blacklist_Down_clicked()
{
    const auto item{ this->ui->list_ConfApache_Blacklist_List->selectedItems().at(0) };
    const int i{ this->blacklists.moveDownItem(
        WS_APACHE,
        this->blacklistFieldFromString( this->ui->box_ConfApache_Blacklist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfApache_Blacklist_Field_currentTextChanged( this->ui->box_ConfApache_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfApache_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfApache_Blacklist_List->setFocus();
}


////////////////
//// NGINX ////
// paths
void MainWindow::on_tool_ConfNginx_Path_Dialog_clicked()
{
    this->crappath.reset( new Crappath( this ) );

    const QString current_path{ this->ui->inLine_ConfNginx_Path_String->text() };
    const PathHandler& logs_path{ this->craplog.getLogsPath( WS_NGINX ) };
    if ( ! current_path.isEmpty() ) {
        crappath->setDirectory( current_path );
    } else if ( logs_path.getPath().has_value() ) {
        crappath->setDirectory( QString::fromStdString( logs_path.toString() ) );
    } else {
        crappath->setDirectory( QString::fromStdString( this->home_path ) );
    }

    if ( crappath->exec() ) {
        const QStringList selection{ crappath->selectedFiles() };
        if ( ! selection.isEmpty() ) {
            this->ui->inLine_ConfNginx_Path_String->setText( selection[0] );
        }
    }
}
void MainWindow::on_inLine_ConfNginx_Path_String_textChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        const auto path{ PathHandler( arg1.toStdString() ).getPath() };
        if ( !( path.has_value() && IOutils::checkDir( path.value() ) ) ) {
            this->ui->icon_ConfNginx_Path_Wrong->setVisible( true );
            this->ui->button_ConfNginx_Path_Save->setEnabled( false );
        } else {
            this->ui->icon_ConfNginx_Path_Wrong->setVisible( false );
            this->ui->button_ConfNginx_Path_Save->setEnabled( true );
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
        const auto path_handler{ PathHandler( this->ui->inLine_ConfNginx_Path_String->text().toStdString() ) };
        const auto path_exp{ path_handler.getPath() };
        if ( ! path_exp.has_value() ) {
            path_exp.error().showDialogMessage();
            this->ui->icon_ConfNginx_Path_Wrong->setVisible( true );
            this->ui->button_ConfNginx_Path_Save->setEnabled( false );
            return;
        }
        const path_t& path{ path_exp.value() };
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        this->craplog.setLogsPath( WS_NGINX, path_handler );
        this->ui->inLine_ConfNginx_Path_String->setText( toQString( path ) );
    }
    this->ui->button_ConfNginx_Path_Save->setEnabled( false );
}

// formats
void MainWindow::on_inLine_ConfNginx_Format_String_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1)
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
    const bool success{ this->craplog.setNginxLogFormat(
        this->ui->inLine_ConfNginx_Format_String->text().trimmed().toStdString() ) };
    if ( success ) {
        this->ui->button_ConfNginx_Format_Save->setEnabled( false );
        if ( this->craplog.getCurrentWebServer() == WS_NGINX ) {
            this->craplog.setCurrentLogFormat();
        }
    }
}
void MainWindow::on_button_ConfNginx_Format_Sample_clicked()
{
    this->ui->preview_ConfNginx_Format_Sample->setText(
        this->craplog.getLogsFormatSample( WS_NGINX ) );
}
void MainWindow::on_button_ConfNginx_Format_Help_clicked()
{
    this->showHelp( "nginx_format" );
}

// warnlists
void MainWindow::on_box_ConfNginx_Warnlist_Field_currentTextChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        this->ui->inLine_ConfNginx_Warnlist_String->clear();
        this->ui->list_ConfNginx_Warnlist_List->clear();
        // update the list
        const WarnlistField field{ this->warnlistFieldFromString( arg1 ) };
        const std::vector<std::string>& list{ this->warnlists.getListConst(
            WS_NGINX, field ) };
        for ( const std::string& item : list ) {
            this->ui->list_ConfNginx_Warnlist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        const bool used{ this->warnlists.isUsed(
            WS_NGINX, field ) };
        this->ui->checkBox_ConfNginx_Warnlist_Used->setChecked( used );
        this->on_checkBox_ConfNginx_Warnlist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfNginx_Warnlist_Used_clicked(bool checked)
{
    this->warnlists.setUsed(
        WS_NGINX,
        this->warnlistFieldFromString( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfNginx_Warnlist_String->setEnabled( true );
        this->ui->list_ConfNginx_Warnlist_List->setEnabled( true );
        this->ui->button_ConfNginx_Warnlist_Add->setFlat( false );
        this->ui->button_ConfNginx_Warnlist_Remove->setFlat( false );
        this->ui->button_ConfNginx_Warnlist_Up->setFlat( false );
        this->ui->button_ConfNginx_Warnlist_Down->setFlat( false );
    } else {
        this->ui->inLine_ConfNginx_Warnlist_String->clear();
        this->ui->inLine_ConfNginx_Warnlist_String->setEnabled( false );
        this->ui->list_ConfNginx_Warnlist_List->clearSelection();
        this->ui->list_ConfNginx_Warnlist_List->setEnabled( false );
        this->ui->button_ConfNginx_Warnlist_Add->setFlat( true );
        this->ui->button_ConfNginx_Warnlist_Remove->setFlat( true );
        this->ui->button_ConfNginx_Warnlist_Up->setFlat( true );
        this->ui->button_ConfNginx_Warnlist_Down->setFlat( true );
    }
}

void MainWindow::on_inLine_ConfNginx_Warnlist_String_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1)
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
    const QString& item{ this->ui->inLine_ConfNginx_Warnlist_String->text() };
    if ( this->ui->list_ConfNginx_Warnlist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).isEmpty() ) {
        // not in the list yet, append
        try {
            this->warnlists.addItem(
                WS_NGINX,
                this->warnlistFieldFromString( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
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
        const auto item{ this->ui->list_ConfNginx_Warnlist_List->selectedItems().at(0) };
        const int max{ this->ui->list_ConfNginx_Warnlist_List->count() -1 };
        if ( max == 0 ) {
            this->ui->button_ConfNginx_Warnlist_Up->setEnabled( false );
            this->ui->button_ConfNginx_Warnlist_Down->setEnabled( false );
        } else {
            for ( int i{0}; i<=max; ++i ) {
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
    const auto item{ this->ui->list_ConfNginx_Warnlist_List->selectedItems().at(0) };
    this->warnlists.removeItem(
        WS_NGINX,
        this->warnlistFieldFromString( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Field_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Field->currentText() );
}
void MainWindow::on_button_ConfNginx_Warnlist_Up_clicked()
{
    const auto item{ this->ui->list_ConfNginx_Warnlist_List->selectedItems().at(0) };
    const int i{ this->warnlists.moveUpItem(
        WS_NGINX,
        this->warnlistFieldFromString( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Field_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Warnlist_List->setFocus();
}
void MainWindow::on_button_ConfNginx_Warnlist_Down_clicked()
{
    const auto item{ this->ui->list_ConfNginx_Warnlist_List->selectedItems().at(0) };
    const int i{ this->warnlists.moveDownItem(
        WS_NGINX,
        this->warnlistFieldFromString( this->ui->box_ConfNginx_Warnlist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfNginx_Warnlist_Field_currentTextChanged( this->ui->box_ConfNginx_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Warnlist_List->setFocus();
}


// blacklist
void MainWindow::on_box_ConfNginx_Blacklist_Field_currentTextChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        this->ui->inLine_ConfNginx_Blacklist_String->clear();
        this->ui->list_ConfNginx_Blacklist_List->clear();
        // update the list
        const BlacklistField field{ this->blacklistFieldFromString( arg1 ) };
        const std::vector<std::string>& list{ this->blacklists.getListConst(
            WS_NGINX, field ) };
        for ( const std::string& item : list ) {
            this->ui->list_ConfNginx_Blacklist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        const bool used{ this->blacklists.isUsed(
            WS_NGINX, field ) };
        this->ui->checkBox_ConfNginx_Blacklist_Used->setChecked( used );
        this->on_checkBox_ConfNginx_Blacklist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfNginx_Blacklist_Used_clicked(bool checked)
{
    this->blacklists.setUsed(
        WS_NGINX,
        this->blacklistFieldFromString( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfNginx_Blacklist_String->setEnabled( true );
        this->ui->list_ConfNginx_Blacklist_List->setEnabled( true );
        this->ui->button_ConfNginx_Blacklist_Add->setFlat( false );
        this->ui->button_ConfNginx_Blacklist_Remove->setFlat( false );
        this->ui->button_ConfNginx_Blacklist_Up->setFlat( false );
        this->ui->button_ConfNginx_Blacklist_Down->setFlat( false );
    } else {
        this->ui->inLine_ConfNginx_Blacklist_String->clear();
        this->ui->inLine_ConfNginx_Blacklist_String->setEnabled( false );
        this->ui->list_ConfNginx_Blacklist_List->clearSelection();
        this->ui->list_ConfNginx_Blacklist_List->setEnabled( false );
        this->ui->button_ConfNginx_Blacklist_Add->setFlat( true );
        this->ui->button_ConfNginx_Blacklist_Remove->setFlat( true );
        this->ui->button_ConfNginx_Blacklist_Up->setFlat( true );
        this->ui->button_ConfNginx_Blacklist_Down->setFlat( true );
    }
}

void MainWindow::on_inLine_ConfNginx_Blacklist_String_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1)
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
    const QString& item{ this->ui->inLine_ConfNginx_Blacklist_String->text() };
    if ( this->ui->list_ConfNginx_Blacklist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).isEmpty() ) {
        // not in the list yet, append
        try {
            this->blacklists.addItem(
                WS_NGINX,
                this->blacklistFieldFromString( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
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
        const auto item{ this->ui->list_ConfNginx_Blacklist_List->selectedItems().at(0) };
        const int max{ this->ui->list_ConfNginx_Blacklist_List->count() -1 };
        if ( max == 0 ) {
            this->ui->button_ConfNginx_Blacklist_Up->setEnabled( false );
            this->ui->button_ConfNginx_Blacklist_Down->setEnabled( false );
        } else {
            for ( int i{0}; i<=max; ++i ) {
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
    const auto item{ this->ui->list_ConfNginx_Blacklist_List->selectedItems().at(0) };
    this->blacklists.removeItem(
        WS_NGINX,
        this->blacklistFieldFromString( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Field_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Field->currentText() );
}
void MainWindow::on_button_ConfNginx_Blacklist_Up_clicked()
{
    const auto item{ this->ui->list_ConfNginx_Blacklist_List->selectedItems().at(0) };
    const int i{ this->blacklists.moveUpItem(
        WS_NGINX,
        this->blacklistFieldFromString( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Field_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Blacklist_List->setFocus();
}
void MainWindow::on_button_ConfNginx_Blacklist_Down_clicked()
{
    const auto item{ this->ui->list_ConfNginx_Blacklist_List->selectedItems().at(0) };
    const int i{ this->blacklists.moveDownItem(
        WS_NGINX,
        this->blacklistFieldFromString( this->ui->box_ConfNginx_Blacklist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfNginx_Blacklist_Field_currentTextChanged( this->ui->box_ConfNginx_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfNginx_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfNginx_Blacklist_List->setFocus();
}


////////////////
//// IIS ////
// paths
void MainWindow::on_tool_ConfIis_Path_Dialog_clicked()
{
    this->crappath.reset( new Crappath( this ) );

    const QString current_path{ this->ui->inLine_ConfIis_Path_String->text() };
    const PathHandler& logs_path{ this->craplog.getLogsPath( WS_IIS ) };
    if ( ! current_path.isEmpty() ) {
        crappath->setDirectory( current_path );
    } else if ( logs_path.getPath().has_value() ) {
        crappath->setDirectory( QString::fromStdString( logs_path.toString() ) );
    } else {
        crappath->setDirectory( QString::fromStdString( this->home_path ) );
    }

    if ( crappath->exec() ) {
        const QStringList selection{ crappath->selectedFiles() };
        if ( ! selection.isEmpty() ) {
            this->ui->inLine_ConfIis_Path_String->setText( selection[0] );
        }
    }
}
void MainWindow::on_inLine_ConfIis_Path_String_textChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        const auto path{ PathHandler( arg1.toStdString() ).getPath() };
        if ( !( path.has_value() && IOutils::checkDir( path.value() ) ) ) {
            this->ui->icon_ConfIis_Path_Wrong->setVisible( true );
            this->ui->button_ConfIis_Path_Save->setEnabled( false );
        } else {
            this->ui->icon_ConfIis_Path_Wrong->setVisible( false );
            this->ui->button_ConfIis_Path_Save->setEnabled( true );
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
        const auto path_handler{ PathHandler( this->ui->inLine_ConfIis_Path_String->text().toStdString() ) };
        const auto path_exp{ path_handler.getPath() };
        if ( ! path_exp.has_value() ) {
            path_exp.error().showDialogMessage();
            this->ui->icon_ConfIis_Path_Wrong->setVisible( true );
            this->ui->button_ConfIis_Path_Save->setEnabled( false );
            return;
        }
        const path_t& path{ path_exp.value() };
        if ( ! IOutils::checkDir( path, true ) ) {
            DialogSec::warnDirNotReadable( nullptr );
        }
        this->craplog.setLogsPath( WS_IIS, path_handler );
        this->ui->inLine_ConfIis_Path_String->setText( toQString( path ) );
    }
    this->ui->button_ConfIis_Path_Save->setEnabled( false );
}

// formats
IISLogsModule MainWindow::getIisLogsModule() const
{
    return this->ui->radio_ConfIis_Format_W3C->isChecked() ? IISLogsModule::W3C
         : this->ui->radio_ConfIis_Format_IIS->isChecked() ? IISLogsModule::IIS
                                                           : IISLogsModule::NCSA;
}

void MainWindow::on_radio_ConfIis_Format_W3C_toggled(bool checked)
{
    if ( checked ) {
        const bool success{ this->craplog.setIisLogFormat( "", IISLogsModule::W3C ) };
        if ( success ) {
            this->ui->inLine_ConfIis_Format_String->clear();
            this->ui->inLine_ConfIis_Format_String->setEnabled( true );
            this->ui->inLine_ConfIis_Format_String->setFocus();
            if ( this->craplog.getCurrentWebServer() == WS_IIS ) {
                this->craplog.setCurrentLogFormat();
                this->on_button_LogFiles_RefreshList_clicked();
            }
        }
    }
}
void MainWindow::on_radio_ConfIis_Format_NCSA_toggled(bool checked)
{
    if ( checked ) {
        const bool success{ this->craplog.setIisLogFormat(
            "c-ip s-sitename s-computername [date:time] sc-status sc-bytes",
            IISLogsModule::NCSA ) };
        if ( success ) {
            this->ui->inLine_ConfIis_Format_String->clear();
            this->ui->inLine_ConfIis_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( WS_IIS ) ) );
            this->ui->inLine_ConfIis_Format_String->setEnabled( false );
            this->ui->button_ConfIis_Format_Save->setEnabled( false );
            if ( this->craplog.getCurrentWebServer() == WS_IIS ) {
                this->craplog.setCurrentLogFormat();
                this->on_button_LogFiles_RefreshList_clicked();
            }
        }
    }
}
void MainWindow::on_radio_ConfIis_Format_IIS_toggled(bool checked)
{
    if ( checked ) {
        const bool success{ this->craplog.setIisLogFormat(
            "c-ip, cs-username, date, time, s-sitename, s-computername, s-ip, time-taken, cs-bytes, sc-bytes, sc-status, sc-win32-status, cs-method, cs-uri-stem, cs-uri-query,",
            IISLogsModule::IIS ) };
        if ( success ) {
            this->ui->inLine_ConfIis_Format_String->clear();
            this->ui->inLine_ConfIis_Format_String->setText( QString::fromStdString( this->craplog.getLogsFormatString( WS_IIS ) ) );
            this->ui->inLine_ConfIis_Format_String->setEnabled( false );
            this->ui->button_ConfIis_Format_Save->setEnabled( false );
            if ( this->craplog.getCurrentWebServer() == WS_IIS ) {
                this->craplog.setCurrentLogFormat();
                this->on_button_LogFiles_RefreshList_clicked();
            }
        }
    }
}

void MainWindow::on_inLine_ConfIis_Format_String_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1)
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
    const bool success{ this->craplog.setIisLogFormat(
        this->ui->inLine_ConfIis_Format_String->text().trimmed().toStdString(),
        this->getIisLogsModule() ) };
    if ( success ) {
        this->ui->button_ConfIis_Format_Save->setEnabled( false );
        if ( this->craplog.getCurrentWebServer() == WS_IIS ) {
            this->craplog.setCurrentLogFormat();
        }
    }
}
void MainWindow::on_button_ConfIis_Format_Sample_clicked()
{
    this->ui->preview_ConfIis_Format_Sample->setText(
        this->craplog.getLogsFormatSample( WS_IIS ) );
}
void MainWindow::on_button_ConfIis_Format_Help_clicked()
{
    this->showHelp( "iis_format" );
}

// warnlists
void MainWindow::on_box_ConfIis_Warnlist_Field_currentTextChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        this->ui->inLine_ConfIis_Warnlist_String->clear();
        this->ui->list_ConfIis_Warnlist_List->clear();
        // update the list
        const WarnlistField field{ this->warnlistFieldFromString( arg1 ) };
        const std::vector<std::string>& list{ this->warnlists.getListConst(
            WS_IIS, field ) };
        for ( const std::string& item : list ) {
            this->ui->list_ConfIis_Warnlist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        const bool used{ this->warnlists.isUsed(
            WS_IIS, field ) };
        this->ui->checkBox_ConfIis_Warnlist_Used->setChecked( used );
        this->on_checkBox_ConfIis_Warnlist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfIis_Warnlist_Used_clicked(bool checked)
{
    this->warnlists.setUsed(
        WS_IIS,
        this->warnlistFieldFromString( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfIis_Warnlist_String->setEnabled( true );
        this->ui->list_ConfIis_Warnlist_List->setEnabled( true );
        this->ui->button_ConfIis_Warnlist_Add->setFlat( false );
        this->ui->button_ConfIis_Warnlist_Remove->setFlat( false );
        this->ui->button_ConfIis_Warnlist_Up->setFlat( false );
        this->ui->button_ConfIis_Warnlist_Down->setFlat( false );
    } else {
        this->ui->inLine_ConfIis_Warnlist_String->clear();
        this->ui->inLine_ConfIis_Warnlist_String->setEnabled( false );
        this->ui->list_ConfIis_Warnlist_List->clearSelection();
        this->ui->list_ConfIis_Warnlist_List->setEnabled( false );
        this->ui->button_ConfIis_Warnlist_Add->setFlat( true );
        this->ui->button_ConfIis_Warnlist_Remove->setFlat( true );
        this->ui->button_ConfIis_Warnlist_Up->setFlat( true );
        this->ui->button_ConfIis_Warnlist_Down->setFlat( true );
    }
}

void MainWindow::on_inLine_ConfIis_Warnlist_String_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1)
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
    const QString& item{ this->ui->inLine_ConfIis_Warnlist_String->text() };
    if ( this->ui->list_ConfIis_Warnlist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).isEmpty() ) {
        // not in the list yet, append
        try {
            this->warnlists.addItem(
                WS_IIS,
                this->warnlistFieldFromString( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
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
        const auto item{ this->ui->list_ConfIis_Warnlist_List->selectedItems().at(0) };
        const int max{ this->ui->list_ConfIis_Warnlist_List->count() -1 };
        if ( max == 0 ) {
            this->ui->button_ConfIis_Warnlist_Up->setEnabled( false );
            this->ui->button_ConfIis_Warnlist_Down->setEnabled( false );
        } else {
            for ( int i{0}; i<=max; ++i ) {
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
    const auto item{ this->ui->list_ConfIis_Warnlist_List->selectedItems().at(0) };
    this->warnlists.removeItem(
        WS_IIS,
        this->warnlistFieldFromString( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Warnlist_Field_currentTextChanged( this->ui->box_ConfIis_Warnlist_Field->currentText() );
}
void MainWindow::on_button_ConfIis_Warnlist_Up_clicked()
{
    const auto item{ this->ui->list_ConfIis_Warnlist_List->selectedItems().at(0) };
    const int i{ this->warnlists.moveUpItem(
        WS_IIS,
        this->warnlistFieldFromString( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfIis_Warnlist_Field_currentTextChanged( this->ui->box_ConfIis_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfIis_Warnlist_List->setFocus();
}
void MainWindow::on_button_ConfIis_Warnlist_Down_clicked()
{
    const auto item{ this->ui->list_ConfIis_Warnlist_List->selectedItems().at(0) };
    const int i{ this->warnlists.moveDownItem(
        WS_IIS,
        this->warnlistFieldFromString( this->ui->box_ConfIis_Warnlist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfIis_Warnlist_Field_currentTextChanged( this->ui->box_ConfIis_Warnlist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Warnlist_List->item( i )->setSelected( true );
    this->ui->list_ConfIis_Warnlist_List->setFocus();
}


// blacklist
void MainWindow::on_box_ConfIis_Blacklist_Field_currentTextChanged(const QString& arg1)
{
    if ( ! arg1.isEmpty() ) {
        this->ui->inLine_ConfIis_Blacklist_String->clear();
        this->ui->list_ConfIis_Blacklist_List->clear();
        // update the list
        const BlacklistField field{ this->blacklistFieldFromString( arg1 ) };
        const std::vector<std::string>& list{ this->blacklists.getListConst(
            WS_IIS, field ) };
        for ( const std::string& item : list ) {
            this->ui->list_ConfIis_Blacklist_List->addItem( QString::fromStdString( item ) );
        }
        // check/uncheck the usage option
        const bool used{ this->blacklists.isUsed(
            WS_IIS, field ) };
        this->ui->checkBox_ConfIis_Blacklist_Used->setChecked( used );
        this->on_checkBox_ConfIis_Blacklist_Used_clicked( used );
    }
}
void MainWindow::on_checkBox_ConfIis_Blacklist_Used_clicked(bool checked)
{
    this->blacklists.setUsed(
        WS_IIS,
        this->blacklistFieldFromString( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
        checked );
    if ( checked ) {
        this->ui->inLine_ConfIis_Blacklist_String->setEnabled( true );
        this->ui->list_ConfIis_Blacklist_List->setEnabled( true );
        this->ui->button_ConfIis_Blacklist_Add->setFlat( false );
        this->ui->button_ConfIis_Blacklist_Remove->setFlat( false );
        this->ui->button_ConfIis_Blacklist_Up->setFlat( false );
        this->ui->button_ConfIis_Blacklist_Down->setFlat( false );
    } else {
        this->ui->inLine_ConfIis_Blacklist_String->clear();
        this->ui->inLine_ConfIis_Blacklist_String->setEnabled( false );
        this->ui->list_ConfIis_Blacklist_List->clearSelection();
        this->ui->list_ConfIis_Blacklist_List->setEnabled( false );
        this->ui->button_ConfIis_Blacklist_Add->setFlat( true );
        this->ui->button_ConfIis_Blacklist_Remove->setFlat( true );
        this->ui->button_ConfIis_Blacklist_Up->setFlat( true );
        this->ui->button_ConfIis_Blacklist_Down->setFlat( true );
    }
}

void MainWindow::on_inLine_ConfIis_Blacklist_String_cursorPositionChanged(int arg1, int arg2)
{
    Q_UNUSED(arg1)
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
    const QString& item{ this->ui->inLine_ConfIis_Blacklist_String->text() };
    if ( this->ui->list_ConfIis_Blacklist_List->findItems( item, Qt::MatchFlag::MatchCaseSensitive ).isEmpty() ) {
        // not in the list yet, append
        try {
            this->blacklists.addItem(
                WS_IIS,
                this->blacklistFieldFromString( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
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
        const auto item{ this->ui->list_ConfIis_Blacklist_List->selectedItems().at(0) };
        const int max{ this->ui->list_ConfIis_Blacklist_List->count() -1 };
        if ( max == 0 ) {
            this->ui->button_ConfIis_Blacklist_Up->setEnabled( false );
            this->ui->button_ConfIis_Blacklist_Down->setEnabled( false );
        } else {
            for ( int i{0}; i<=max; ++i ) {
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
    const auto item{ this->ui->list_ConfIis_Blacklist_List->selectedItems().at(0) };
    this->blacklists.removeItem(
        WS_IIS,
        this->blacklistFieldFromString( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
        item->text().toStdString() );
    // refresh the list
    this->on_box_ConfIis_Blacklist_Field_currentTextChanged( this->ui->box_ConfIis_Blacklist_Field->currentText() );
}
void MainWindow::on_button_ConfIis_Blacklist_Up_clicked()
{
    const auto item{ this->ui->list_ConfIis_Blacklist_List->selectedItems().at(0) };
    const int i{ this->blacklists.moveUpItem(
        WS_IIS,
        this->blacklistFieldFromString( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfIis_Blacklist_Field_currentTextChanged( this->ui->box_ConfIis_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfIis_Blacklist_List->setFocus();
}
void MainWindow::on_button_ConfIis_Blacklist_Down_clicked()
{
    const auto item{ this->ui->list_ConfIis_Blacklist_List->selectedItems().at(0) };
    const int i{ this->blacklists.moveDownItem(
        WS_IIS,
        this->blacklistFieldFromString( this->ui->box_ConfIis_Blacklist_Field->currentText() ),
        item->text().toStdString() ) };
    // refresh the list
    this->on_box_ConfIis_Blacklist_Field_currentTextChanged( this->ui->box_ConfIis_Blacklist_Field->currentText() );
    // re-select the item
    this->ui->list_ConfIis_Blacklist_List->item( i )->setSelected( true );
    this->ui->list_ConfIis_Blacklist_List->setFocus();
}
