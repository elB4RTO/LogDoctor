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
    explicit Crapinfo( const int& window_theme_id, const QString& version, const QString& exec_path, const QString& conf_path, const QString& logdoc_path, QWidget *parent=nullptr );
    ~Crapinfo();

private:
    Ui::Crapinfo *ui;

    void getStyleSheet( QString& stylesheet, const int& theme_id );
};

#endif // CRAPINFO_H
