#ifndef CRAPINFO_H
#define CRAPINFO_H

#include <QWidget>

namespace Ui {
class Crapinfo;
}

class Crapinfo : public QWidget
{
    Q_OBJECT

public:
    explicit Crapinfo( const QString& version, const QString& exec_path, const QString& conf_path, const QString& logdoc_path, QWidget *parent=nullptr);
    ~Crapinfo();

private:
    Ui::Crapinfo *ui;
};

#endif // CRAPINFO_H
