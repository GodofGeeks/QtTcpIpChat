#ifndef GETNAME_H
#define GETNAME_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class GetName;
}

class GetName : public QDialog
{
    Q_OBJECT

public:
    explicit GetName(QWidget *parent = nullptr);
    ~GetName();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GetName *ui;
};

#endif // GETNAME_H
