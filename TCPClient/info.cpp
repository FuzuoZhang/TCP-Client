#include "info.h"
#include "ui_info.h"

info::info(QString name,QString xh, QString ip, QWidget *parent) :
   name(name),xh(xh),ip(ip), QDialog(parent),
    ui(new Ui::info)
{
    ui->setupUi(this);
    myshow();
}

info::~info()
{
    delete ui;
}

void info::myshow(){
    ui->label_5->setText(name);
    ui->label_6->setText(xh);
    if(ip.isEmpty()){
        ui->label_7->setText("NULL");
    }
    else ui->label_7->setText(ip);
}
