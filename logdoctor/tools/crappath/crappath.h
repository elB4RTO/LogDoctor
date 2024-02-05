#ifndef LOGDOCTOR__CRAPPATH_H
#define LOGDOCTOR__CRAPPATH_H


#include <QFileDialog>


class Crappath : public QFileDialog
{
    Q_OBJECT

public:
    explicit Crappath( QWidget* parent=nullptr );
};

#endif // LOGDOCTOR__CRAPPATH_H
