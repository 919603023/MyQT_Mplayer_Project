#include "mainwindow.h"

#include <QApplication>


struct adsf
{
    int b;
    MainWindow * c;

};
int main(int argc, char *argv[])
{



    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutex2,NULL);
    int ret = mkfifo("fifo_cmd",0664);
    int ret2 = mkfifo("Myfifo",0664);
     int fd1[2];


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
       fd1[1]  = open("Myfifo",O_WRONLY);
       close(0);
       dup2(fd,0);
       dup2(fd1[1], 1);
        execlp("mplayer",
                    " mplayer ",
                    "-slave", "-quiet","-idle",
                    "-input", "file=./fifo_cmd",
                    "./res/stratvoice/hello_kugo.mp3", NULL);
    }
    else{
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        w.pid = pid;
     int fd = open("fifo_cmd",O_WRONLY);

    pthread_t mplayer_ack;
    pthread_create(&mplayer_ack,NULL,MyGetTimeAndBar,(void *)&w);
    pthread_detach(mplayer_ack);

    pthread_t send_mplayer;
    pthread_create(&send_mplayer,NULL,MySendMsgToMplayer,NULL);
    pthread_detach(send_mplayer);

//    pthread_t myprint;
//    pthread_create(&myprint,NULL,MyPrint,(void *)&w);
//    pthread_detach(myprint);

    QTextCodec *codec = QTextCodec::codecForName("utf8");
    adsf d;
    d.c = &w;


      QTextCodec::setCodecForLocale(codec);



    return a.exec();


    }
}
