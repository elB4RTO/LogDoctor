#ifndef CRAPUP_H
#define CRAPUP_H

#include <QObject>

#include <string>


class Crapup : public QObject
{
    Q_OBJECT
public:
    Crapup();

    // perform a GET request for a version-check
    void versionCheck( const float& current_version, const int& dialog_level );

};

#endif // CRAPUP_H
