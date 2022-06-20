#include "craplog.h"
#include "./ui_craplog.h"

Craplog::Craplog(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Craplog)
{
    ui->setupUi(this);
}

Craplog::~Craplog()
{
    delete ui;
}

