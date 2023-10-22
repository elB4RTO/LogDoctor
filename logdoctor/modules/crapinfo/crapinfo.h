#ifndef LOGDOCTOR__CRAPINFO_H
#define LOGDOCTOR__CRAPINFO_H


#include <QWidget>


namespace Ui {
    class Crapinfo;
}


//! Crapinfo
/*!
    Displays informations
*/
class Crapinfo : public QWidget
{
    Q_OBJECT

public:

    //! Class constructor
    /*!
        \param version The version of LogDoctor actually running
        \param exec_path The path of the executable
        \param conf_path The path of the configuration file
        \param logdoc_path The path of the additional resources folder
        \param parent The parent Widget
    */
    explicit Crapinfo( const QString& version, const QString& exec_path, const QString& conf_path, const QString& logdoc_path, QWidget* parent=nullptr );

private:
    QSharedPointer<Ui::Crapinfo> ui;
};


#endif // LOGDOCTOR__CRAPINFO_H
