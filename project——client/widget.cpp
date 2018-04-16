#include "widget.h"
#include "ui_widget.h"
#include "zhuce.h"
#include "tishi.h"
#include "information.h"
#include <QHostAddress>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QIcon>
#include <QPixmap>

QString username;
QString passwd;
QString globaldata;      //声明一个全局变量用于窗体间传值

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    socket=new QTcpSocket(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //图标
    /*QIcon icon;
    icon.addFile(tr(":/CloseBtn.bmp"));
    ui->pushButton_3->setFlat(true);
    ui->pushButton_3->setIcon(icon);
    */
    QPixmap icon(tr(":/CloseBtn.bmp"));
    ui->pushButton_3->setIcon(icon);
    ui->pushButton_3->setFixedSize(icon.size());

    QPixmap icon1(tr(":/MinBtn.bmp"));
    ui->pushButton_4->setIcon(icon1);
    ui->pushButton_4->setFixedSize(icon1.size());

    QPixmap icon2(tr(":/about.bmp"));
    ui->pushButton_5->setIcon(icon2);
    ui->pushButton_5->setFixedSize(icon2.size());


    //建立连接
    this->setFixedSize(560,460);
    socket->connectToHost(QHostAddress("192.168.0.110"),10086);

    ui->milineEdit->setEchoMode(QLineEdit::Password);//隐藏密码不可见


    //设置背景
    QPixmap map = QPixmap(":/bluelight.jpg").scaled(this->size());
    QPalette pate(this->palette());
    pate.setBrush(QPalette::Background,QBrush(map));
    this->setPalette(pate);

    connect(socket,SIGNAL(connected()),this,SLOT(connnectedslots()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(slotRecvData()));


    //label显示图片图片pige
    QTimer *timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(qtimeSlot()));
    timer->start(1000);

    pix1.load(":/4.jpg");
    ui->giflabel->setScaledContents(true);
    ui->giflabel->setPixmap(pix1);


    ui->zhlineEdit->setText(username);
    ui->milineEdit->setText(passwd);
    flag=false; //设置一个标志位用于记住账号
    flag1=false;//设置一个记住密码的标志

    /*//显示一个gif
    movie=new QMovie(this);
    movie->setFileName(":/11.gif");
    ui->giflabel->setMovie(movie);
    ui->giflabel->setScaledContents(true);
    movie->start();
    */

}

Widget::~Widget()
{
    delete ui;
}
//显示账号的头像
void Widget::qtimeSlot()
{
    if(ui->comboBox->currentIndex()==0)//
    {
        if(""==ui->zhlineEdit->text())
        {
            ui->label->clear();
        }
        else
        {
            pix.load(":/2.jpg");
            ui->label->setScaledContents(true);
            ui->label->setPixmap(pix);
        }
    }
    else
    {
        if(""==ui->zhlineEdit->text())
        {
            ui->label->clear();
        }
        else
        {
            pix.load(":/1.jpg");
            ui->label->setScaledContents(true);
            ui->label->setPixmap(pix);
        }
    }
}

//登录的按钮
void Widget::on_dengluButton_clicked()
{

       //qDebug()<<"comboBox"<<ui->comboBox->currentIndex();//comboBox打印是第几条指令
       //发送内容 密码  账号
       QString str = ui->zhlineEdit->text();
       qDebug()<<"str:"<<str;
       QString str1 = ui->milineEdit->text();
       qDebug()<<"str1:"<<str1;
       if((str=="")||(str1==""))
       {
           qDebug()<<"请正确输入。。。";
           QMessageBox::information(this, "提示", "请重新输入");
           return;
       }

       QString str2=QString("B")+"@"+QString(str)+"@"+QString(str1);
       QByteArray arr;
       arr.append(str2);
       socket->write(arr);
       qDebug()<<"clices:"<<arr;

       /*if(ui->comboBox->currentIndex()==0)
       {
            qDebug()<<"comboBox"<<ui->comboBox->itemText(0);  //判断是不是超级用户 ，是超级用户就会跳到

            globaldata=ui->comboBox->itemText(0);      //声明一个全局变量用于窗体间传值
            //窗口切换
            Information *inform=new Information();
            emit myslgnal(ui->comboBox->itemText(0));  //获取comboBox的输入并且传递出去
            qDebug()<<"emit:ok";
            this->hide();
            inform->show();
       }
       else
       {
           qDebug()<<"comboBox"<<ui->comboBox->itemText(1); //判断是不是超级用户 ，是超级用户就会跳到
           globaldata=ui->comboBox->itemText(1);      //声明一个全局变量用于窗体间传值

          //窗口切换
           Information *inform=new Information();
           emit myslgnal(ui->comboBox->itemText(1));  //获取comboBox的输入并且传递出去
           qDebug()<<"emit:ok";
           this->hide();
           inform->show();
       }
        */

}
//跳转界面到注册
void Widget::on_zhuceButton_clicked()
{
    //点击注册的时候 胡跳转到另一个界面
    Zhuce *zhuce=new Zhuce();
    this->hide();
    zhuce->show();

}
//链接成功  发送密码和账号还有昵称
void Widget::connnectedslots()
{
     qDebug()<<"connnect successB";

     //sendmessage();
}

//接收数据
void Widget::slotRecvData()
{
    QByteArray arr = socket->readAll();
    //服务器返回的是正确 那就会执行跳转界面  不然弹出对话框
    qDebug()<<"server:"<<arr;
    if(QString(arr)=="OK")
    {
        if(ui->comboBox->currentIndex()==0)
        {
             qDebug()<<"comboBox"<<ui->comboBox->itemText(0);  //判断是不是超级用户 ，是超级用户就会跳到

             globaldata=ui->comboBox->itemText(0);      //声明一个全局变量用于窗体间传值
             //窗口切换
             Information *inform=new Information();
             emit myslgnal(ui->comboBox->itemText(0));  //获取comboBox的输入并且传递出去
             qDebug()<<"emit:ok";
             this->hide();
             inform->show();

        }
        else
        {
            qDebug()<<"comboBox"<<ui->comboBox->itemText(1); //判断是不是超级用户 ，是超级用户就会跳到


            globaldata=ui->comboBox->itemText(1);      //声明一个全局变量用于窗体间传值

           //窗口切换
            Information *inform=new Information();
            emit myslgnal(ui->comboBox->itemText(1));  //获取comboBox的输入并且传递出去
            qDebug()<<"emit:ok";
            this->hide();
            inform->show();
        }
    }
    else
    {
        QMessageBox::information(this, "提示", "您输入输入错误");
        return;
    }

}
//记住账号
void Widget::on_checkBox_clicked()
{

    if(flag== false)
    {
        username=ui->zhlineEdit->text();
        flag= true;
        qDebug()<<"记住账号";
    }
    else
    {
        //ui->zhlineEdit->clear();
        flag= false;
        username="";
        qDebug()<<"不记住账号";


    }

}
//记住密码
void Widget::on_checkBox_2_clicked()
{
    if( flag1== false)
    {
        passwd=ui->milineEdit->text();
        flag1= true;

        qDebug()<<"记住密码";

    }
    else
    {
        //ui->milineEdit->clear();
        flag1= false;

        passwd="";
        qDebug()<<"不记住密码";
    }

}

void Widget::on_pushButton_3_clicked()
{
    exit(0);
}
//最小化
void Widget::on_pushButton_4_clicked()
{

   //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
   setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}
//弹出一个框
void Widget::on_pushButton_5_clicked()
{
        Tishi *tishi=new Tishi;
        this->hide();
        tishi->show();

}
