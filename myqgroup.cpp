#include "myqgroup.h"

MyQGroup::MyQGroup(QWidget *parent) : QGroupBox(parent)
{

}

void MyQGroup::enterEvent(QEvent *e)
{
    emit this->QGroupMysignalsEnter();

}

void MyQGroup::leaveEvent(QEvent *e)
{
    emit this->QGroupMysignalsLeave();
}


