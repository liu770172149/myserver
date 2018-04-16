#ifndef TISHI_H
#define TISHI_H

#include <QWidget>

namespace Ui {
class Tishi;
}

class Tishi : public QWidget
{
    Q_OBJECT

public:
    explicit Tishi(QWidget *parent = 0);
    ~Tishi();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Tishi *ui;
};

#endif // TISHI_H
