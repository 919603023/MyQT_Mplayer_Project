#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    int ret = mkfifo("fifo_cmd",0664);
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
       int fd = open("fifo_cmd",O_RDWR);
       close(0);
       dup2(fd,0);
        execlp("mplayer",
                    " mplayer ",
                    "-slave", "-quiet","-idle",
                    "-input", "file=./fifo_cmd",
                    "../MyQT_Mplayer_Project/song/coffee.mp3", NULL);

//        int FLag = 0;
//        int fd = open("fifo",O_RDWR);
//        if(fd < 0){
//            perror("open fifo");
//        }
//        char buf[128] = {0};
//        while(1){
//            bzero(buf,sizeof (buf));
//            read(fd,buf,128);
//            if(FLag == 0){
//                system(buf);
//                FLag++;
//                close(0);
//                dup2(1,0);
//            }
//             else {

//                 printf("%s\n",buf);
//                 fflush(stdout);
//             }
//        }
    }
    else{



    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    }
}
