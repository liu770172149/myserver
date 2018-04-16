#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QPixmap>
#include <QMovie>

namespace Ui {

class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:

    void myslgnal(QString);//自定义一个信号用于窗口之间通信

private slots:
    void on_dengluButton_clicked();

    void on_zhuceButton_clicked();

    void connnectedslots(); //连接函数  链接成功发送密码账号

    void slotRecvData();//接收服务器消息的函数函数

    void qtimeSlot();  //显示图片label


    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Widget *ui;

    QTcpSocket *socket;

    QPixmap pix;

    QPixmap pix1;

    bool flag; //设置一个标志位用于记住账号

    bool flag1;//设置一个记住密码的标志

};

#endif // WIDGET_H
