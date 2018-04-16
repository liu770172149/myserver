#ifndef INFORMATION_H
#define INFORMATION_H

#include <QWidget>
#include <QTcpSocket>
#include <QPixmap>
#include "widget.h"

namespace Ui {
class Information;
}

class Information : public QWidget
{
    Q_OBJECT

public:
    explicit Information(QWidget *parent = 0);
    ~Information();

    void changeFace();  //label上显示图片

private slots:
    void on_pushButton_clicked();  //界面返回

    void qtimeSlot();  //时间显示槽函数

    void connnectedslots(); //连接函数

    void slotRecvData();//接收服务器图片的函数函数

    void connnectedslots1(); //连接函数

    void slotRecvData1();//接收服务温度湿度等。。。

    void receiveData(QString tmp);   //接收另外窗口传递过来的数据的槽

    void on_wdpushButton_clicked();

    void on_sdpushButton_clicked();

    void on_lightpushButton_clicked();

    void on_kaidepushButton_clicked();

    void on_fmpushButton_clicked();

    void on_fspushButton_clicked();

private:
    Ui::Information *ui;

    QTcpSocket *socket;  //用于传送图片

    QTcpSocket *socket1;//用于接收温度 湿度 光照

    bool flag;

    bool flag1;//开灯的标志

    bool flag2;//开风扇的标志

    bool flag3;//打开蜂鸣器

    //QString fileName;

    qint64 fileSize;  //接收图片的长度

    //qint64 fileLen;

    QString filePath;

    QByteArray imageData;   //存放接收到的图片数据的QByteArray

    bool hasReadHead;   //是否接收到了当前所接收图片的第一个数据包。

    Widget *data;

    QPixmap pix;
};

#endif // INFORMATION_H
