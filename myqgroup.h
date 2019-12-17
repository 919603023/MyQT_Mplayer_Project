#ifndef MYQGROUP_H
#define MYQGROUP_H

#include <QWidget>
#include <QString>
#include <QGroupBox>
class MyQGroup : public QGroupBox
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
