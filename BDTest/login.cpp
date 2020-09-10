#include "login.h"
#include "mainwindow.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{

    delete ui;
}

void Login::on_buttonBox_accepted()
{
    MainWindow w;
    this->hide();
    w.show();
}
