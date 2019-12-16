#include "myqpushbutton.h"

MyQPushbutton::MyQPushbutton(QWidget *parent) : QPushButton(parent)
{
    //is not enter
    //is leave
    leaveflag  = 0;
    hideorshow = 0;

}

void MyQPushbutton::enterEvent(QEvent *e)
{
     //is enter
    enterflag = 1;
    //can hide or show flag
    //can show
    hideorshow = 1;


}

void MyQPushbutton::leaveEvent(QEvent *e)
{
    leaveflag = 0;
}

void MyQPushbutton::mouseMoveEvent(QMouseEvent *e)
{
    if(e->x() > 670 && e->x() < 721 && e->y() >140 && e->y() < 301)
    {
        if(leaveflag == 1 && enterflag == 1)
        {
            emit Mysignalsvulmeshow();
        }
    }
    else
    {
        hideorshow = 0;
        enterflag  = 0;
        emit Mysignalsvulmehide();
    }
}
