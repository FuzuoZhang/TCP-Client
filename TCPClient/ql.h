#ifndef QL_H
#define QL_H

#include <QCloseEvent>
#include <QNetworkInterface>
#include <QIODevice>
#include <QMainWindow>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QFileDialog>
#include <QTextCursor>
#include <QTextTable>
#include <QHostInfo>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QTimer>
namespace Ui {
class QL;
}

class QL : public QMainWindow
{
    Q_OBJECT

public:
    explicit QL(QString ip_me, QString xh_me,QWidget *parent = 0);
    ~QL();
    void Init();
    void sendmessage();
    void closeEvent(QCloseEvent *event);
public slots:
    void check();
    void readmessage();
private slots:
    void on_pushButton_clicked();

private:
    QString ip_me,xh_me;
    QString ip[3],xh[3],name[3];
    QUdpSocket *UDP_Socket;
    QTcpSocket *mySocket;
    QString IP_Server;
    int Port_Server;
    QTimer *timer;
    Ui::QL *ui;
};

#endif // QL_H
