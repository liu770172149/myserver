#include "tishi.h"
#include "ui_tishi.h"
#include "widget.h"

Tishi::Tishi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tishi)
{
    ui->setupUi(this);
}

Tishi::~Tishi()
{
    delete ui;
}

void Tishi::on_pushButton_clicked()
{
    Widget *Chushi=new Widget();
    this->hide();
    Chushi->show();
}
