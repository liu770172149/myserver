#ifndef ZHUCE_H
#define ZHUCE_H

#include "widget.h"
#include <QWidget>
#include <QTcpSocket>
namespace Ui {
class Zhuce;
}

class Zhuce : public QWidget
{
    Q_OBJECT

public:
    explicit Zhuce(QWidget *parent = 0);
    ~Zhuce();

    //void sendmessge();//发消息


private slots:
    void on_pushButton_clicked();

    void slotRecvData();

    void on_pushButton_2_clicked();

    void connnectedslots(); //连接函数

private:
    Ui::Zhuce *ui;
    QTcpSocket *socket;

};

#endif // ZHUCE_H
