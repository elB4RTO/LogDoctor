
#include "crapup.h"

#include "modules/dialogs.h"

#include "utilities/io.h"
#include "utilities/strings.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

#include <stdexcept>


Crapup::Crapup()
{

}


void Crapup::versionCheck( const float& v, const int& dialog_level )
{
    bool successful = false;
    float version = -1;
    int err;

    QByteArray ua = QByteArray::fromStdString("LogDoctor/"+std::to_string(v)+"(version check)");
    std::string content;

    const std::string links[3] = {"https://raw.githubusercontent.com/elB4RTO/LogDoctor/main/version.txt",
                                  "https://git.disroot.org/elB4RTO/LogDoctor/raw/branch//main/version.txt",
                                  "https://gitlab.com/elB4RTO/LogDoctor/-/raw/main/version.txt"};


    QNetworkAccessManager *networkMgr = new QNetworkAccessManager(this);


    for ( const std::string& URL : links ) {

        // reset the content
        content.clear();

        // set the URL and make the request
        QNetworkRequest request;
        request.setRawHeader( "User-Agent", ua );
        request.setUrl( QUrl( URL.c_str() ) );
        QNetworkReply *reply = networkMgr->get( request );

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
        loop.exec();


        if ( reply->error() ) {
            // failed
            if ( dialog_level == 2 ) {
                DialogSec::warnConnectionFailed(
                    QString::fromStdString( URL ),
                    QString::fromStdString( reply->errorString().toStdString() ) );
            }

        } else {

            // successful, get the content
            content = reply->readAll().toStdString();
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
                        err = 11;
                    }
                } else {
                    // second version mark not found
                    err = 10;
                }
            } else {
                // first version mark not found
                err = 10;
            }
        }

        delete reply;
    }

    if ( successful ) {
        // check the versions
        if ( version > v ) {
            // new version available
            DialogSec::msgNewVersion( nullptr );
        } else if ( version == v ) {
            // same version
            DialogSec::msgSameVersion( nullptr );
        } else if ( version > 0 )  {
            // this version is beyond the current upstream version
            DialogSec::warnFutureVersion( nullptr );
        } else {
            // something went wrong, can't be successful if version is less than 0
            successful = false;
            err = 20;
        }
    }
    if ( ! successful ) {
        DialogSec::errVersionCheckFailed( err );
    }
}
