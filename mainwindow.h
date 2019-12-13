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
typedef   struct
{
    double time;
    char *MyLyric;
}lyric;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
extern void *Mydeal_fun(void *arg);
extern void *deal_fun2(void *arg);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InitializeListFunction();
    void MyCutSong();
    int GetPuaesFlag();
    int fd;
//    sem_t *sem;
    char buf[128];
    int OpenFlag;
    int CutSong;
    pid_t pid;
    QList<lyric> Lyriclist;
//    int PuaesFlag;
    QList<char*> QListSongName;
    lyric StructLyric;
    void  closeEvent(QCloseEvent *event);
    void SetSeekbarfindViewById(int val);



signals:

public slots:
   void MyClickedPlaying();
   void MyDoubleClickedList(const QModelIndex &index);


   void MyVolumeSet();

private:
    Ui::MainWindow *ui;



};
#endif // MAINWINDOW_H
