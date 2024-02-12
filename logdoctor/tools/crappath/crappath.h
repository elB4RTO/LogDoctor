#ifndef LOGDOCTOR__CRAPPATH_H
#define LOGDOCTOR__CRAPPATH_H


#include <QFileDialog>


class Crappath : public QFileDialog
{
    Q_OBJECT

public:
    explicit Crappath( QWidget* parent=nullptr );
    Q_DISABLE_COPY_MOVE(Crappath)
};

#endif // LOGDOCTOR__CRAPPATH_H
