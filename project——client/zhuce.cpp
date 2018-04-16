#include "zhuce.h"
#include "ui_zhuce.h"

#include "widget.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QMessageBox>
#include <QPixmap>
#include <QPalette>

Zhuce::Zhuce(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Zhuce)
{
    ui->setupUi(this);

    socket=new QTcpSocket(this);

    //建立连接
    socket->connectToHost(QHostAddress("192.168.0.110"),10086);
    //密码不可见
    ui->rmlineEdit->setEchoMode(QLineEdit::Password);
    ui->mmlineEdit->setEchoMode(QLineEdit::Password);
    connect(socket,SIGNAL(readyRead()),this,SLOT(slotRecvData()));
    connect(socket,SIGNAL(connected()),this,SLOT(connnectedslots()));
    //设置背景
    QPixmap map = QPixmap(":/bluelight.jpg").scaled(this->size());
    QPalette pate(this->palette());
    pate.setBrush(QPalette::Background,QBrush(map));
    this->setPalette(pate);

    this->setWindowTitle("注册");
}

Zhuce::~Zhuce()
{
    delete ui;
}

//
void Zhuce:: connnectedslots()
{
   qDebug()<<"connnect successA";

}


//返回
void Zhuce::on_pushButton_clicked()
{

    //首先判断昵称是不是存在  账户是不是存在  存在 弹出对话华框
    //判断密码是不是两次输入的一直

    //判读密码和账号是不是正确
    QString str = ui->zzlineEdit->text();
    if((ui->zzlineEdit->text()=="")||(ui->rmlineEdit->text()=="")||(ui->mmlineEdit->text()==""))
    {
        qDebug()<<"请正确输入。。。";
        QMessageBox::information(this, "提示", "您的输入不能为空");
        return;
    }
    if(ui->rmlineEdit->text()==ui->mmlineEdit->text())
    {
        QString str1 = ui->mmlineEdit->text();
        QString str2=QString("A")+"@"+QString(str)+"@"+QString(str1);
        QByteArray arr;
        arr.append(str2);
        socket->write(arr);
        qDebug()<<"clices:"<<arr;

        /*QMessageBox::information(this, "提示", "注册成功");
        Widget *Chushi=new Widget();
        this->hide();
        Chushi->show();*/

    }
    else
    {
            QMessageBox::information(this, "信息", "你输入的密码不一致");

    }

}

void Zhuce::slotRecvData()
{
    QByteArray arr = socket->readAll();
    //服务器返回的是正确 那就会执行跳转界面  不然弹出对话框
    qDebug()<<"server:"<<arr;
    if(QString(arr)=="OK")
    {
        //注册成功
        QMessageBox::information(this, "提示", "注册成功");
        Widget *Chushi=new Widget();
        this->hide();
        Chushi->show();
    }
    else
    {
         QMessageBox::information(this, "提示", "注册失败");
    }

}

void Zhuce::on_pushButton_2_clicked()
{
    Widget *Chushi=new Widget();
    this->hide();
    Chushi->show();
}
