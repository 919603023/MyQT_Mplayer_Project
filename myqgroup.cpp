#include "myqgroup.h"

MyQGroup::MyQGroup(QWidget *parent) : QWidget(parent)
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
