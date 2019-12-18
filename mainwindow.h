#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <QPixmap>
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
#include <QPainter>
#include <exception>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include<QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include<QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <ctime>

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

extern void *MyPrint(void *arg);

extern void *MyGetTimeAndBar(void *arg);

extern void *MySendMsgToMplayer(void *arg);

extern void SetSeekBarFindViewById(int val);

extern void TotalTime(float val);

extern void SetNowTimeQstring(float val);

extern void SendMsgToMplayer(char *val);

extern char *QStringToChar(QString val);


extern pthread_mutex_t mutex;

extern pthread_mutex_t mutex2;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    int imageRotate;

    QPixmap disc;

    int fd;

    char buf[128];

    int OpenFlag;

    int VolueButtonFlag;

    int MuteFlag;

    pid_t pid;

    char MyBuff[128];

    int HaveLyricFlag;

    int SpliderPress;

    int ShowAllLyric;

    int Frist;

    int Mode;

    QList<int> LyriclistTime;

    QList<int> lyriclistRow;

    QList<QString> LyriclistLyric;

    QString setnowtimeqstring ;

    ViewInformation viewinformation;


    QPixmap  pixmap;

    friend void *MyGetTimeAndBar(void *arg);

    void GetLyric();

    QString MyFindLyric();

    void ReadDir(char *val);

    void Initialize();

    void PrintInformation();

    void SetInformation();

    void SetTimeQstring(float val,QString &val1);

    void Lock();

    void Unlock();

    void SetAllLyric();

    void CutSong(char *val);


    void  closeEvent(QCloseEvent *event);

    void  resizeEvent(QResizeEvent *);

    void  paintEvent(QPaintEvent *);



signals:

public slots:
   void SlotMyClickedPlaying(void);

   void SlotMyDoubleClickedList(QListWidgetItem *item);

   void SlotMusicNext(void);

   void SlotMusicFront(void);

   void SlotTimeOut(void);

   void SlotProgressValue(int val);

   void SlotSliderPressed(void);

   void SlotSliderReleased(void);

   void SlotQPushButtonShowAllLyric(void);

   void SlotQPushButtonvolumeShow(void);

   void SlotQPushButtonMute(void);


private slots:
   void on_pushButton_clicked();

   void on_pushButton_3_clicked();

   void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;



};
#endif // MAINWINDOW_H
