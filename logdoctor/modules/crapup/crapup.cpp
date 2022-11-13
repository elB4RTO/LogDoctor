
#include "crapup.h"
#include "ui_crapup.h"

#include "modules/exceptions.h"

#include <QNetworkAccessManager>
#include <QEventLoop>

#include <string>
#include <stdexcept>


Crapup::Crapup( const int& window_theme_id, const QString& icons_theme, QWidget* parent ) :
    QWidget(parent),
    ui(new Ui::Crapup)
{
    this->ui->setupUi(this);

    QString stylesheet = "";
    this->getStyleSheet( stylesheet, window_theme_id );
    this->setStyleSheet( stylesheet );

    this->img_checking = QPixmap(":/icons/icons/"+icons_theme+"/checking.png");

    this->ui->label_Title->setText( Crapup::tr("Checking for updates") );
    this->adjustSize();
}

Crapup::~Crapup()
{
    delete this->ui;
    if ( this->reply ) {
        delete this->reply;
    }
    if ( this->img_timer ) {
        delete this->img_timer;
    }
    if ( this->request_timer ) {
        delete this->request_timer;
    }
}

void Crapup::closeEvent( QCloseEvent* event )
{
    this->quitting = true;
    if ( this->img_timer->isActive() ) {
        this->img_timer->stop();
    }
    if ( this->request_timer->isActive() ) {
        this->request_timer->stop();
    }
    if ( this->reply ) {
        if ( !this->reply->isFinished() ) {
            this->requestTimeout();
        }
    }
}


void Crapup::versionCheck( const float& v )
{
    bool successful = false;
    float version = -1;
    int err = 1;

    this->img_timer = new QTimer(this);
    connect(this->img_timer, SIGNAL(timeout()), this, SLOT(rotateImg()));
    this->img_timer->start(100);

    QByteArray ua = QByteArray::fromStdString("LogDoctor/"+std::to_string(v)+" (version check)");
    std::string content;

    const std::string links[3] = {"https://raw.githubusercontent.com/elB4RTO/LogDoctor/main/version.txt",
                                  "https://git.disroot.org/elB4RTO/LogDoctor/raw/branch//main/version.txt",
                                  "https://gitlab.com/elB4RTO/LogDoctor/-/raw/main/version.txt"};

    QNetworkAccessManager networkMgr = QNetworkAccessManager(this);


    for ( const std::string& URL : links ) {

        // reset the content
        content.clear();

        // reset the reply
        this->request_aborted = false;

        // request timeout timer
        if ( this->request_timer ) {
            delete this->request_timer;
        }
        this->request_timer = new QTimer(this);
        this->request_timer->setSingleShot( true );
        connect(this->request_timer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

        // set the URL and make the request
        QNetworkRequest request;
        request.setRawHeader( "User-Agent", ua );
        request.setUrl( QUrl( URL.c_str() ) );
        request.setTransferTimeout( 15'000 );
        this->reply = networkMgr.get( request );

        // reply waiter loop
        QEventLoop wait_reply;
        connect(this->reply, SIGNAL(readyRead()), &wait_reply, SLOT(quit()));
        connect(this, SIGNAL(abortRequest()), &wait_reply, SLOT(quit()));

        // make the request
        this->request_timer->start( 16'000 );
        wait_reply.exec();

        // request is over, in a way or another
        if ( this->quitting ) {
            networkMgr.disconnect();
            return;
        }
        if ( this->request_timer->isActive() ) {
            this->request_timer->stop();
        }
        if ( !this->reply ) {
            err = 2;
            continue;
        }

        if ( this->reply->error() ) {
            continue;
        } else {
            // connection successful, get the content
            content = this->reply->readAll().toStdString();
            // search for the version mark
            const std::string version_mark = ".:!¦version¦!:.";
            int start = content.find( version_mark );
            if ( start != std::string::npos ) {
                // first found
                start += version_mark.size();
                const int stop = content.find( version_mark, start );
                if ( stop != std::string::npos ) {
                    // second found too
                    try {
                        // get the version
                        version = std::stof( content.substr( start, stop-start ) );
                        successful = true;
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
    if ( this-> reply ) {
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
            } else if ( version > 0 )  {
                // this version is beyond the current upstream version
                this->ui->label_Title->setText( Crapup::tr(":/") );
                this->ui->label_Message->setText( Crapup::tr(
                    "You're running a version from the future!\n"
                    "Your version is beyond the current upstream version\n"
                    "Are you running the original LogDoctor?\n"
                    "Please visit the LogDoctor's repository and get a fresh version of it" ) );
            } else {
                // something went wrong, can't be successful if version is less than 0
                successful = false;
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
    this->request_aborted = true;
    this->deleteReply();
    emit this->abortRequest();
}

void Crapup::deleteReply()
{
    if ( !this->reply->isFinished() ) {
        this->reply->abort();
    }
    if ( this->reply->isOpen() ) {
        this->reply->close();
    }
    delete this->reply;
    this->reply = nullptr;
}



void Crapup::rotateImg()
{
    this->img_orientation += 36.0;
    if ( this->img_orientation >= 360.0 ) {
        this->img_orientation = 0.0;
    }
    this->ui->label_Image->setPixmap(
        this->img_checking.transformed(
            QTransform().rotate( this->img_orientation ) ) );
}


void Crapup::getStyleSheet( QString& stylesheet, const int& theme_id )
{
    std::unordered_map<std::string, QString> style;
    switch ( theme_id ) {
        case 0:
            break;
        case 1:
            style = {
                {"text",
                    "rgb( 248, 248, 248 )"},
                {"window_primary",
                    "rgb( 16, 16, 16 )"},
                {"window_secondary",
                    "rgb( 32, 32, 32 )"}
            };
            break;
        case 2:
            style = {
                {"text",
                    "rgb( 45, 0, 30 )"},
                {"window_primary",
                    "rgb( 255, 204, 143 )"},
                {"window_secondary",
                    "rgb( 255, 140, 141 )"}
            };
            break;
        case 3:
            style = {
                {"text",
                    "rgb( 220, 211, 187 )"},
                {"window_primary",
                    "rgb( 14, 28, 0 )"},
                {"window_secondary",
                    "rgb( 24, 48, 0 )"}
            };
            break;
        case 4:
            style = {
                {"text",
                    "rgb( 30, 21, 0 )"},
                {"window_primary",
                    "rgb( 230, 221, 197 )"},
                {"window_secondary",
                    "rgb( 170, 161, 137 )"}
            };
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
    if ( theme_id != 0 ) {
        stylesheet =
            "* {"
            "   color: "+style.at("text")+";"
            "}"
            "QWidget#Crapup {"
            "   background-color: "+style.at("window_primary")+";"
            "}"
            "QFrame {"
            "   background-color: "+style.at("window_secondary")+";"
            "}";
    }
}
