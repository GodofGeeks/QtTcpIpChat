#include "getname.h"
#include "ui_getname.h"

GetName::GetName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetName)
{
    ui->setupUi(this);
}

GetName::~GetName()
{
    delete ui;
}

void GetName::on_buttonBox_accepted()
{


    delete this;
}
