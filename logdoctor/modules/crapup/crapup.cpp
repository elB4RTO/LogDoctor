
#include "crapup.h"
#include "ui_crapup.h"

#include "modules/exceptions.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QPixmap>
#include <QTimer>

#include <string>
#include <stdexcept>
#include <unordered_map> // leave this for clang


namespace /*private*/
{

enum StyleId : uint32_t {
    TEXT,
    WINDOW_PRIMARY,
    WINDOW_SECONDARY
};

} // namespacce (private)


Crapup::Crapup( const int window_theme_id, const QString& icons_theme, QWidget* parent )
    : QWidget{ parent }
    , ui{ new Ui::Crapup }
    , img_checking{ ":/icons/icons/"+icons_theme+"/checking.png" }
{
    this->ui->setupUi(this);

    QString stylesheet;
    this->getStyleSheet( stylesheet, window_theme_id );
    this->setStyleSheet( stylesheet );

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
    int err{ 1 };

    this->img_timer.reset( new QTimer(this) );
    connect( this->img_timer.get(), &QTimer::timeout, this, &Crapup::rotateImg );
    this->img_timer->start(100);

    QByteArray ua{ QByteArray::fromStdString("LogDoctor/"+std::to_string(v)+" (version check)") };
    std::string content;

    const std::string links[4] {"https://raw.githubusercontent.com/elB4RTO/LogDoctor/main/version.txt",
                                "https://git.disroot.org/elB4RTO/LogDoctor/raw/branch//main/version.txt",
                                "https://gitlab.com/elB4RTO/LogDoctor/-/raw/main/version.txt",
                                "https://bitbucket.org/elb4rto/logdoctor/src/main/version.txt"};

    QNetworkAccessManager networkMgr{ this };


    for ( const std::string& URL : links ) {

        // reset the content
        content.clear();

        // reset the reply
        this->request_aborted &= false;

        // request timeout timer
        this->request_timer.reset( new QTimer(this) );
        this->request_timer->setSingleShot( true );
        connect( this->request_timer.get(), &QTimer::timeout, this, &Crapup::requestTimeout );

        // set the URL and make the request
        QNetworkRequest request;
        request.setRawHeader( "User-Agent", ua );
        request.setUrl( QUrl( URL.c_str() ) );
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
            err = 2;
            continue;
        }

        if ( this->reply->error() ) {
            continue;
        } else {
            // connection successful, get the content
            content = this->reply->readAll().toStdString();
            // search for the version mark
            const std::string version_mark{ ".:!¦version¦!:." };
            size_t start{ content.find( version_mark ) };
            if ( start != std::string::npos ) {
                // first found
                start += version_mark.size();
                const size_t stop{ content.find( version_mark, start ) };
                if ( stop != std::string::npos ) {
                    // second found too
                    try {
                        // get the version
                        version = std::stof( content.substr( start, stop-start ) );
                        successful |= true;
                        break;

                    } catch ( const std::invalid_argument&/*& e*/ ) {
                        // failed to convert string to float
                        err = 21;
                    }
                } else {
                    // second version mark not found
                    err = 12;
                }
            } else {
                // first version mark not found
                err = 11;
            }
        }
    }
    if ( !this->reply.isNull() ) {
        this->deleteReply();
    }
    networkMgr.disconnect();
    this->img_timer->stop();
    this->ui->label_Image->setVisible( false );

    if ( !successful && err < 10 ) {
        // connection failure
        this->ui->label_Title->setText( Crapup::tr("Failed to establish a connection") );
        switch ( err ) {
            case 1:
                this->ui->label_Message->setText( Crapup::tr(
                    "Connection error, please try again later" ) );
                break;
            case 2:
                this->ui->label_Message->setText( Crapup::tr(
                    "Connection timed out" ) );
                break;
            default:
                throw GenericException("Unexpected err_code in VersionCheck: "+std::to_string(err));
                break;
        }

    } else {
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
                err = 22;
            }
        }
        if ( !successful ) {
            // version error
            this->ui->label_Title->setText( Crapup::tr("Version check failed") );
            switch ( err ) {
                case 11:
                    this->ui->label_Message->setText( Crapup::tr(
                        "An error occured while parsing:\n"
                        "initial version mark not found" ) );
                    break;
                case 12:
                    this->ui->label_Message->setText( Crapup::tr(
                        "An error occured while parsing:\n"
                        "final version mark not found" ) );
                    break;
                case 21:
                    this->ui->label_Message->setText( Crapup::tr(
                        "An error occured while parsing:\n"
                        "malformed version" ) );
                    break;
                case 22:
                    this->ui->label_Message->setText( Crapup::tr(
                        "An error occured while comparing:\n"
                        "malformed upstream version" ) );
                    break;
                default:
                    throw GenericException("Unexpected err_code in VersionCheck: "+std::to_string(err));
                    break;
            }
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


void Crapup::getStyleSheet( QString& stylesheet, const int theme_id ) const
{
    std::unordered_map<StyleId, QString> style;
    switch ( theme_id ) {
        case 0: // native
            break;
        case 1: // light
            style = {
                {TEXT,
                 "rgb( 22, 11, 0 )"},
                {WINDOW_PRIMARY,
                 "rgb( 250, 250, 255 )"},
                {WINDOW_SECONDARY,
                 "rgb( 230, 230, 230 )"}
            };
            break;
        case 2: // dark
            style = {
                {TEXT,
                    "rgb( 248, 248, 248 )"},
                {WINDOW_PRIMARY,
                    "rgb( 13, 14, 15 )"},
                {WINDOW_SECONDARY,
                    "rgb( 27, 30, 33 )"}
            };
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
    if ( theme_id != 0 ) {
        stylesheet =
            "* {"
            "   color: "+style.at(TEXT)+";"
            "}"
            "QWidget#Crapup {"
            "   background-color: "+style.at(WINDOW_SECONDARY)+";"
            "}"
            "QFrame {"
            "   background-color: "+style.at(WINDOW_PRIMARY)+";"
            "}";
    }
}
