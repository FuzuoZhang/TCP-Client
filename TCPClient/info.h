#ifndef INFO_H
#define INFO_H

#include <QDialog>

namespace Ui {
class info;
}

class info : public QDialog
{
    Q_OBJECT

public:
    explicit info(QString name,QString xh, QString ip, QWidget *parent = 0);
    ~info();
    void myshow();
private:
    QString name,xh,ip;
    Ui::info *ui;
};

#endif // INFO_H
