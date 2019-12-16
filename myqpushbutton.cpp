#include "myqpushbutton.h"

MyQPushButton::MyQPushButton(QWidget *parent) : QPushButton(parent)
{

}

void MyQPushButton::enterEvent(QEvent *e)
{
    emit this->QPushButonMysignalsEnter();

}

void MyQPushButton::leaveEvent(QEvent *e)
{
    emit this->QPushButonMysignalsLeave();
}
