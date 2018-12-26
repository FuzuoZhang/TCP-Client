#include "log.h"
#include "ui_log.h"

Log::Log(QString name, QFile *file, QWidget *parent) :
  name(name), file(file), QDialog(parent),
    ui(new Ui::Log)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("聊天记录"));
    Init();
    show();
}

Log::~Log()
{
    delete ui;
}


void Log::Init(){
    setStyleSheet("background-color:#5599ff");
}

void Log::show(){
    QFile temp(file->fileName());
    if(temp.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       QTextStream in(&temp);
        qDebug()<<"readling";
        QString linestr;
        in.setCodec("utf8");
        while(!in.atEnd()){

           linestr=in.readLine();
            if(linestr.isEmpty()) continue;
            if(linestr.startsWith(name)){
                ui->textBrowser->setTextColor(QColor("blue"));
            }
            else if(linestr.startsWith("我")){
                ui->textBrowser->setTextColor(QColor(0,0,0));
            }
            else{
                ui->textBrowser->setTextColor(QColor(128,128,128));
            }
            ui->textBrowser->append(linestr+"\n");
       }

    }
    else {
        qDebug()<<"打不开文件";
        return ;
    }
    temp.close();
}

void Log::on_pushButton_clicked()
{
    close();
}
