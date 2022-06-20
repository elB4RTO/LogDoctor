#ifndef CRAPLOG_H
#define CRAPLOG_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Craplog; }
QT_END_NAMESPACE

class Craplog : public QMainWindow
{
    Q_OBJECT
    
public:
    Craplog(QWidget *parent = nullptr);
    ~Craplog();
    
private:
    Ui::Craplog *ui;
};
#endif // CRAPLOG_H
