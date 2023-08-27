
#include "crapup.h"
#include "ui_crapup.h"

#include "modules/stylesheets.h"

#include "modules/exceptions.h"

#include <QFontDatabase>
#include <QRegularExpression>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QPixmap>
#include <QTimer>


namespace /*private*/
{

enum class ErrCode : uint8_t {
    CONNECTION_ERROR,
    CONNECTION_TIMEOUT,
    VERSION_NOT_FOUND,
    VERSION_MALFORMED,
    UNEXPECTED_VERSION
};

} // namespacce (private)


Crapup::Crapup( const int window_theme_id, const QString& icons_theme, QWidget* parent )
    : QWidget{ parent }
    , ui{ new Ui::Crapup }
    , img_checking{ ":/icons/icons/"+icons_theme+"/checking.png" }
{
    this->ui->setupUi(this);

    QString stylesheet;
    StyleSec::Crapup::getStyleSheet( stylesheet, window_theme_id );
    this->setStyleSheet( stylesheet );

    // fonts
    const QString& ff{ QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/fonts/Metropolis")).at(0) };
    const QFont font{ ff, 13 };
    const QFont font_big{ ff, 16 };

    // apply the fonts
    this->ui->label_Title->setFont( font_big );
    this->ui->label_Message->setFont( font );

    this->ui->label_Title->setText( Crapup::tr("Checking for updates") );
    this->adjustSize();
}

Crapup::~Crapup()
{
    if ( this->ui != nullptr ) {
        delete this->ui;
        this->ui = nullptr;
    }
    if ( !this->reply.isNull() ) {
        this->deleteReply();
    }
}

void Crapup::closeEvent( QCloseEvent* event )
{
    this->quitting |= true;
    if ( !this->img_timer.isNull() ) {
        if ( this->img_timer->isActive() ) {
            this->img_timer->stop();
        }
    }
    if ( !this->request_timer.isNull() ) {
        if ( this->request_timer->isActive() ) {
            this->request_timer->stop();
        }
    }
    if ( !this->reply.isNull() ) {
        this->requestTimeout();
    }
}


void Crapup::versionCheck( const float v )
{
    bool successful{ false };
    float version{ -1.0 };
    ErrCode err{ ErrCode::CONNECTION_ERROR };

    this->img_timer.reset( new QTimer(this) );
    connect( this->img_timer.get(), &QTimer::timeout, this, &Crapup::rotateImg );
    this->img_timer->start(100);

    const QByteArray ua{ QByteArray::fromStdString("LogDoctor/"+std::to_string(v)+" (version check)") };

    const QStringList links{ "https://raw.githubusercontent.com/elB4RTO/LogDoctor/main/version.txt",
                             "https://git.disroot.org/elB4RTO/LogDoctor/raw/branch//main/version.txt",
                             "https://gitlab.com/elB4RTO/LogDoctor/-/raw/main/version.txt",
                             "https://bitbucket.org/elb4rto/logdoctor/src/main/version.txt" };

    static const QRegularExpression regex(".:!¦version¦!:.[0-9.]*.:!¦version¦!:.");

    QNetworkAccessManager networkMgr{ this };


    for ( const QString& URL : links ) {

        // reset the reply
        this->request_aborted &= false;

        // request timeout timer
        this->request_timer.reset( new QTimer(this) );
        this->request_timer->setSingleShot( true );
        connect( this->request_timer.get(), &QTimer::timeout, this, &Crapup::requestTimeout );

        // set the URL and make the request
        QNetworkRequest request;
        request.setRawHeader( "User-Agent", ua );
        request.setUrl( QUrl( URL ) );
        request.setTransferTimeout( this->timeout_msec );
        this->reply.reset( networkMgr.get( request ) );

        // reply waiter loop
        QEventLoop wait_reply;
        connect( this->reply.get(), &QNetworkReply::readyRead, &wait_reply, &QEventLoop::quit );
        connect( this, &Crapup::abortRequest, &wait_reply, &QEventLoop::quit );

        // make the request
        this->request_timer->start( this->timeout_msec+1000 );
        wait_reply.exec();

        // request is over, in a way or another
        if ( this->quitting ) {
            networkMgr.disconnect();
            return;
        }
        if ( this->request_timer->isActive() ) {
            this->request_timer->stop();
        }
        if ( this->reply.isNull() ) {
            err = ErrCode::CONNECTION_TIMEOUT;
            continue;
        } else if ( this->reply->error() ) {
            err = ErrCode::CONNECTION_ERROR;
            continue;
        } else {
            // connection successful, get the content
            const QString content{ this->reply->readAll() };
            // search for the version mark
            if ( content.contains( regex ) ) {
                // get the version
                QScopedPointer<bool> ok{ new bool(false) };
                version = content.section(".:!¦version¦!:.", 1, 1 ).toFloat(ok.get());
                if ( ok.get() ) {
                    successful |= true;
                    break;
                } else {
                    // failed to convert string to float
                    err = ErrCode::VERSION_MALFORMED;
                }
            } else {
                // version mark not found
                err = ErrCode::VERSION_NOT_FOUND;
            }
        }
    }
    if ( !this->reply.isNull() ) {
        this->deleteReply();
    }
    networkMgr.disconnect();
    this->img_timer->stop();
    this->ui->label_Image->setVisible( false );

    if ( successful ) {
        // check the versions
        if ( version > v ) {
            // new version available
            this->ui->label_Title->setText( Crapup::tr("New version available") );
            this->ui->label_Message->setText( Crapup::tr(
                "A new version is available!\n"
                "Please visit LogDoctor's git repository and follow the instruction about how to update" ) );
        } else if ( version == v ) {
            // same version
            this->ui->label_Title->setText( Crapup::tr("No update found") );
            this->ui->label_Message->setText( Crapup::tr(
                "LogDoctor is up-to-date" ) );
        } else if ( version > 0.0f )  {
            // this version is beyond the current upstream version
            this->ui->label_Title->setText( Crapup::tr(":/") );
            this->ui->label_Message->setText( Crapup::tr(
                "You're running a version from the future!\n"
                "Your version is beyond the current upstream version\n"
                "Are you running the original LogDoctor?\n"
                "Please visit the LogDoctor's repository and get a fresh version of it" ) );
        } else {
            // something went wrong, can't be successful if version is less than 0
            successful &= false;
            err = ErrCode::UNEXPECTED_VERSION;
        }
    }
    if ( !successful ) {
        switch ( err ) {
            case ErrCode::CONNECTION_ERROR:
                this->ui->label_Title->setText( Crapup::tr("Failed to establish a connection") );
                this->ui->label_Message->setText( Crapup::tr(
                    "Connection error, please try again later" ) );
                break;
            case ErrCode::CONNECTION_TIMEOUT:
                this->ui->label_Title->setText( Crapup::tr("Failed to establish a connection") );
                this->ui->label_Message->setText( Crapup::tr(
                    "Connection timed out" ) );
                break;
            case ErrCode::VERSION_NOT_FOUND:
                this->ui->label_Title->setText( Crapup::tr("Version check failed") );
                this->ui->label_Message->setText( Crapup::tr(
                    "An error occured while parsing:\n"
                    "version mark not found" ) );
                break;
            case ErrCode::VERSION_MALFORMED:
                this->ui->label_Title->setText( Crapup::tr("Version check failed") );
                this->ui->label_Message->setText( Crapup::tr(
                    "An error occured while parsing:\n"
                    "malformed version" ) );
                break;
            case ErrCode::UNEXPECTED_VERSION:
                this->ui->label_Title->setText( Crapup::tr("Version check failed") );
                this->ui->label_Message->setText( Crapup::tr(
                    "An error occured while comparing:\n"
                    "malformed upstream version" ) );
                break;
            default:
                throw GenericException("Unexpected ErrCode in VersionCheck: "+static_cast<uint8_t>(err));
                break;
        }
    }
    this->adjustSize();
}



void Crapup::requestTimeout()
{
    this->request_aborted |= true;
    this->deleteReply();
    emit this->abortRequest();
}

void Crapup::deleteReply()
{
    if ( this->reply->isOpen() ) {
        this->reply->abort();
    }
    this->reply.reset();
}



void Crapup::rotateImg()
{
    this->img_orientation += 36.0f;
    if ( this->img_orientation >= 360.0f ) {
        this->img_orientation = 0.0f;
    }
    this->ui->label_Image->setPixmap(
        this->img_checking.transformed(
            QTransform().rotate( this->img_orientation ) ) );
}
