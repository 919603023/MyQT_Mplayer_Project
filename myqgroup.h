#ifndef MYQGROUP_H
#define MYQGROUP_H

#include <QWidget>

class MyQGroup : public QWidget
{
    Q_OBJECT
public:
    explicit MyQGroup(QWidget *parent = nullptr);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
signals:
    void QGroupMysignalsLeave(void);
    void QGroupMysignalsEnter(void);
public slots:
};

#endif // MYQGROUP_H
