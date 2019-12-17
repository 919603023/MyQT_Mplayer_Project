#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <dirent.h>
#include <QTimer>
#include <stdlib.h>
#include <QByteArray>
#include <QDebug>
#include <QVector>
#include <QList>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <QTextCodec>
#include <QListWidgetItem>
#include "myqpushbutton.h"
#include "myqgroup.h"

typedef   struct
{
    int time;
    int row;
    char MyLyric[256];
}lyric;
struct ViewInformation
{
    QString song;
    QString singer;
    QString album;
    QString nowtime;
    QString alltime;
    QString lyric;
    int progress;
    int mutehub;
    int hub;
    int NowTime;
    int AllTime;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
extern sem_t sem1,sem2,sem3;
extern void *MyPrint(void *arg);
extern void *MyGetTimeAndBar(void *arg);
extern void *MySendMsgToMplayer(void *arg);
extern void SetSeekBarFindViewById(int val);
extern void TotalTime(float val);
extern void SetNowTimeQstring(float val);
extern void SendMsgToMplayer(char *val);
extern char *QStringToChar(QString val);
extern pthread_mutex_t mutex;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void MyCutSong();
    char*MyFindLyric();
    int fd;
    char buf[128];
    int OpenFlag;
    int CutSong;
    int Volue;
    int MuteFlag;
    pid_t pid;
    char MyBuff[128];
    int HaveLyricFlag;
    int SpliderPress;
    lyric *Lyric[128];
    int ShowAllLyric;
    QString setnowtimeqstring ="";
    ViewInformation viewinformation;
    lyric StructLyric;
    void  closeEvent(QCloseEvent *event);

    void  resizeEvent(QResizeEvent *);
    void ReadDir(char *val);
    void Initialize();
    void PrintInformation();
    void SetInformation();
    void SetTimeQstring(float val,QString &val1);
    void Lock();
    void Unlock();
    void SetAllLyric();
signals:

public slots:
   void SlotMyClickedPlaying(void);
   void SlotMyDoubleClickedList(QListWidgetItem *item);
   void SlotMusicNext(void);
   void SlotMusicFront(void);
   void SlotTimeOut(void);
//   void SlotVulmeHide(void);
//   void SlotVulmeShow(void);
   void SlotProgressValue(int val);
   void SlotSliderPressed(void);
   void SlotSliderReleased(void);
   void SlotQPushButtonShowAllLyric(void);
   void SlotQPushButtonvolume(void);

//   void SlotQPushButtonMysignalsEnter(void);
//   void SlotQPushButtonMysignalsLeave(void);

   void SlotQGroupBoxMysignalsEnter(void);
   void SlotQGroupBoxMysignalsLeave(void);

private:
    Ui::MainWindow *ui;



};
#endif // MAINWINDOW_H
