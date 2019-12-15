#include "mainwindow.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    int ret = mkfifo("fifo_cmd",0664);
     int fd1[2];
     pipe(fd1);
    if(ret < 0){
        perror("mkfifo:");

    }
    pid_t pid = fork();
    if(pid < 0){
        perror("fork:");
        return 0;
    }
    //child
    else if(pid == 0){
       int fd = open("fifo_cmd",O_RDONLY);
       close(0);
       dup2(fd,0);
       dup2(fd1[1], 1);
        execlp("mplayer",
                    " mplayer ",
                    "-slave", "-quiet","-idle",
                    "-input", "file=./fifo_cmd",
                    "../MyQT_Mplayer_Project/res/stratvoice/hello_kugo.mp3", NULL);
    }
    else{
     int fd = open("fifo_cmd",O_WRONLY);
    pthread_t mplayer_ack;
    pthread_create(&mplayer_ack,NULL,MyGetTimeAndBar,(void *)(fd1[0]));
    pthread_detach(mplayer_ack);
    pthread_t send_mplayer;
    pthread_create(&send_mplayer,NULL,MySendMsgToMplayer,(void *)fd);
    pthread_detach(send_mplayer);
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("utf8");



      QTextCodec::setCodecForLocale(codec);


    MainWindow w;
    w.show();
    w.pid = pid;
    return a.exec();
    }
}
