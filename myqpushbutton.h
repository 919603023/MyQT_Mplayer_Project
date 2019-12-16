#ifndef MYQPUSHBUTTON_H
#define MYQPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
class MyQPushbutton : public QPushButton
{
    Q_OBJECT
public:
    int leaveflag;
    int hideorshow;
    int enterflag;
    explicit MyQPushbutton(QWidget *parent = nullptr);
void enterEvent(QEvent *e);
void leaveEvent(QEvent *e);
void mouseMoveEvent(QMouseEvent *e);
signals:
void Mysignalsvulmeshow(void);
void Mysignalsvulmehide(void);
public slots:
};

#endif // MYQPUSHBUTTON_H
