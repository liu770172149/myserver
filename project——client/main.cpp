#include "widget.h"
#include "information.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //Information h;
    //把不同是界面之间的槽函数和信号之间connect链接起来
    //QObject::connect(&w,SIGNAL(myslgnal(QString )),&h,SLOT(receiveData(QString)));
    w.show();

    return a.exec();
}
