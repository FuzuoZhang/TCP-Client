#ifndef LOG_H
#define LOG_H

#include <QDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QByteArray>
#include <QFile>
#include <QDebug>

namespace Ui {
class Log;
}

class Log : public QDialog
{
    Q_OBJECT

public:
    explicit Log(QString name, QFile *file, QWidget *parent = 0);
    ~Log();

  void Init();
  void show();

private slots:
  void on_pushButton_clicked();

private:
    QFile *file;
    QString name;
    Ui::Log *ui;
};

#endif // LOG_H
