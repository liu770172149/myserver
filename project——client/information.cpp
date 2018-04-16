#include "information.h"
#include "ui_information.h"

#include "widget.h"
#include <QHostAddress>
#include <QTime>
#include <QTimer>
#include <QDate>
#include <QPixmap>
#include <QPalette>
#include <QImage>
#include <QBuffer>
#include <QInputDialog>

extern QString globaldata;      //声明外部变量
Information::Information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Information)
{
    ui->setupUi(this);
    this->setFixedSize(580,440);
    //time
    QTimer *timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(qtimeSlot()));
    timer->start(1000);

    //socket
    socket=new QTcpSocket(this);
    //建立连接
    socket->connectToHost(QHostAddress("192.168.0.110"),10086);
    connect(socket,SIGNAL(connected()),this,SLOT(connnectedslots()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(slotRecvData()));
    flag = false;

    socket1=new QTcpSocket(this);
    //建立连接
    socket1->connectToHost(QHostAddress("192.168.0.110"),10086);
    connect(socket1,SIGNAL(connected()),this,SLOT(connnectedslots1()));
    connect(socket1,SIGNAL(readyRead()),this,SLOT(slotRecvData1()));


    //设置背景
    QPixmap map = QPixmap(":/bluelight.jpg").scaled(this->size());
    QPalette pate(this->palette());
    pate.setBrush(QPalette::Background,QBrush(map));
    this->setPalette(pate);

    //初始化变量
     hasReadHead=false;
     //avalibleNum=0;

     //窗口之间通信
     data=new Widget;
     connect(data, SIGNAL(myslgnal(QString)), this, SLOT(receiveData(QString)));
     //data->show();
     qDebug()<<"globaldata:"<<globaldata;
     if(globaldata=="普通用户")
     {
        ui->sdpushButton->hide();
        ui->wdpushButton->hide();
        ui->lightpushButton->hide();
        ui->fmpushButton->hide();
        ui->fspushButton->hide();
        ui->kaidepushButton->hide();
        ui->sdpushButton->setEnabled(false);
        ui->wdpushButton->setEnabled(false);
        ui->lightpushButton->setEnabled(false);
        ui->kaidepushButton->setEnabled(false);
        ui->fmpushButton->setEnabled(false);
        ui->fspushButton->setEnabled(false);
        this->setWindowTitle("普通用户");
        pix.load(":/2.jpg");
        ui->superlabel->setScaledContents(true);
        ui->superlabel->setPixmap(pix);
     }
     else
     {

        this->setWindowTitle("超级用户");
        pix.load(":/1.jpg");
        ui->superlabel->setScaledContents(true);
        ui->superlabel->setPixmap(pix);

     }
     //开灯的标志 蜂鸣器 风扇
     flag1=false;
     flag2=false;
     flag3=false;

}

Information::~Information()
{
    delete ui;
}

//返回登录界面
void Information::on_pushButton_clicked()
{
    Widget *Chushi=new Widget();
    this->hide();
    Chushi->show();
}

//时间表的显示
void Information::qtimeSlot()
{
    QTime qtimeObj = QTime::currentTime();
    QString strTime = qtimeObj.toString("h:m ap");
    strTime.prepend(" 时间: ");
    QDate qdateObj = QDate::currentDate();
    QString strDate = qdateObj.toString("dddd, yyyy/MM/d"); //星期、月份、天、年
    strDate.prepend("日期: ");

    strDate.append(strTime);
    ui->timelabel->setText (strDate);
}
void Information::connnectedslots()
{

     QString str=QString("E");
     QByteArray arr;
     arr.append(str);
     socket->write(arr);
     qDebug()<<"connnect success E";


}
void Information::connnectedslots1()
{
    QString str=QString("C");
    QByteArray arr;
    arr.append(str);
    socket1->write(arr);
    qDebug()<<"connnect successC";

}
//接收数据
//当有数据包发送来时，解析第一个数据包并获取所有数据包的总长度，获取的第二个以上的数 据包不做此处理。
void Information::slotRecvData()
{
    //方法一
    //imageData.clear();   //第一个数据包发来时，先清空存储图片数据的空间
    /*  QByteArray by=socket->readAll();
        qDebug()<<"by:"<<by;
    if(false == flag)
    {
       //QString str = QString(by);
       //fileName = str.section("#",0,0);
       fileSize = by.toInt();// 得数据总长度。
       qDebug()<<"fileSize:"<<fileSize;
       flag = true;
     }
    else
    {
        qDebug()<<"by:"<<by.length();
        //QBuffer buffer(&by);
        //buffer.open(QIODevice::ReadOnly);
        imageData.append(by);

        qDebug()<<"imageData:"<<imageData.length();
        if(imageData.length()>=fileSize)
        {
             qDebug()<<"receive a image,length1="<<fileSize<<endl;
             qDebug()<<"receive a image,imageData.length()="<<imageData.length()<<endl;
             changeFace();
             hasReadHead=false;
       }
       else
       {
           qDebug()<<"error"<<endl;
       }
    }
*/

    //方法2

    QByteArray array;
    array.append(socket->readAll());
    qDebug()<<"bytesAvailable"<<array.length();
    while(socket->waitForReadyRead(100))
    {
        //array.append(by);
        array.append(socket->readAll());
        qDebug()<<"bytesAvailable"<<array.length();


        QPalette palette;
        QImage image=QImage::fromData(array,"jpg").scaled(ui->movilabel->size()); //从给定的QByteArray数据加载图像。
        palette.setBrush(QPalette::Background, QBrush(image));
        ui->movilabel->setAutoFillBackground(true);
        ui->movilabel->setPalette(palette);
        qDebug()<<"bytesAvailable22222"<<array.length();
    }


}

void Information::changeFace()
{
   qDebug()<<"显示照片"<<endl;
   QPalette palette;
   QImage image=QImage::fromData(imageData,"jpg").scaled(ui->movilabel->size()); //从给定的QByteArray数据加载图像。
   palette.setBrush(QPalette::Background, QBrush(image));
   ui->movilabel->setAutoFillBackground(true);
   ui->movilabel->setPalette(palette);
}
//SOCKET1 用于温湿度传送
void Information::slotRecvData1()
{
    QByteArray by=socket1->readAll();
    qDebug()<<"byaa:"<<by;
    QString str = QString(by);
    QString wd= str.section("#",0,0);
    QString sd= str.section("#",1,1);
    QString light= str.section("#",2,2);
    ui->wdlineEdit->setText(wd);
    ui->sdlineEdit->setText(sd);
    ui->lightlineEdit->setText(light);

}
void Information::receiveData(QString tmp)
{
   qDebug()<<"tmp:"<<tmp;
   qDebug()<<"globaldata:"<<globaldata;
}

//改变温度
void Information::on_wdpushButton_clicked()
{
    bool ok;
    QString str = QInputDialog::getText(this,"温度设置","请输入要设置的温度",QLineEdit::Normal,"17",&ok);
    if(true == ok)
    {
        ui->wdlineEdit->setText(str);
        qDebug() << str;
    }

}

void Information::on_sdpushButton_clicked()
{

    bool ok;
    QString str = QInputDialog::getText(this,"湿度设置","请输入要设置的湿度",QLineEdit::Normal,"17",&ok);
    if(true == ok)
    {
        ui->sdlineEdit->setText(str);
        qDebug() << str;
    }
}

void Information::on_lightpushButton_clicked()
{

    bool ok;
    QString str = QInputDialog::getText(this,"光照强度","请输入要设置的光照强度",QLineEdit::Normal,"17",&ok);
    if(true == ok)
    {
        ui->sdlineEdit->setText(str);
        qDebug() << str;
    }
}


//开灯
void Information::on_kaidepushButton_clicked()
{
    if(flag1==false)
    {
        ui->kaidepushButton->setText("关灯");
        socket1->write("OPEN");
        flag1=true;
    }
    else
    {
        flag1=false;
        socket1->write("CLOSE");
        ui->kaidepushButton->setText("开灯");
    }

}
//打开蜂鸣器
void Information::on_fmpushButton_clicked()
{
    if(flag2==false)
    {
        socket1->write("OPEN");
        ui->fmpushButton->setText("关闭");
        flag2=true;
    }
    else
    {
        socket1->write("CLOSE");
        flag2=false;
        ui->fmpushButton->setText("打开蜂鸣器");
    }
}
//打开风扇
void Information::on_fspushButton_clicked()
{
    if(flag3==false)
    {
        socket1->write("OPEN");
        ui->fspushButton->setText("关闭");
        flag3=true;
    }
    else
    {
        socket1->write("CLOSE");
        flag3=false;
        ui->fspushButton->setText("打开风扇");
    }
}
