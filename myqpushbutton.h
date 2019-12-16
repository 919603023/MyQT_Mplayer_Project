#ifndef MYQPUSHBUTTON_H
#define MYQPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
class MyQPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyQPushButton(QWidget *parent = nullptr);

    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
signals:
void QPushButonMysignalsLeave(void);
void QPushButonMysignalsEnter(void);
public slots:
};

#endif // MYQPUSHBUTTON_H
