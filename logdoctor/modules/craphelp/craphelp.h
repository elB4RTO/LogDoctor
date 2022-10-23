#ifndef CRAPHELP_H
#define CRAPHELP_H

#include <QWidget>


namespace Ui {
class Craphelp;
}

class Craphelp : public QWidget
{
    Q_OBJECT

public:
    explicit Craphelp(QWidget *parent = nullptr);
    ~Craphelp();

    void helpLogsFormat( const std::string& path, const QFont& font, const int& color_scheme_id );

private:
    Ui::Craphelp *ui;

    const std::unordered_map<std::string, QString> getColorScheme( const int& scheme_id );
};

#endif // CRAPHELP_H
