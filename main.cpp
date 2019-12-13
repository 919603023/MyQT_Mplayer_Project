#include "mainwindow.h"

#include <QApplication>

void *Mydeal_fun(void *arg)
{
   sem_t *sem = sem_open("mysem",O_RDWR|O_CREAT,0664,1);
   sem_init(sem,0,1);
    int fd = (int)(long)arg;
    while(1)
    {
        sem_wait(sem);
        char buf[128] = "";
        read(fd,buf,sizeof (buf));
        char cmd[128] = "";
        sscanf(buf,"%[^=]",cmd);
        if(strcmp(cmd,"ANS_PERCENT_POSITION") == 0)//百分比
        {
            int percent_pos = 0;
            sscanf(buf,"%*[^=]=%d",&percent_pos);
            printf("\r当前的百分比为:%%%d \t", percent_pos);
        }
         else if(strcmp(cmd,"ANS_TIME_POSITION") == 0)//当前时间
        {
             float time_pos = 0;
              sscanf(buf,"%*[^=]=%f", &time_pos);
              printf("当前的时间为:%02d:%02d", (int)(time_pos+0.5)/60, (int)(time_pos+0.5)%60);
        }
          fflush(stdout);
          sem_post(sem);

    }
}

void *deal_fun2(void *arg)

{
sem_t *sem = sem_open("mysem",O_RDWR|O_CREAT,0664,1);
sem_init(sem,0,1);
    int fifo_fd = (int)(long)arg;

    //不停的给fifo_cmd发送获取当前时间以及进度

    while(1)

    {
sem_wait(sem);
        usleep(500*1000);//0.5秒发指令

        write(fifo_fd,"get_percent_pos\n", strlen("get_percent_pos\n"));

        usleep(500*1000);//0.5秒发指令

        write(fifo_fd,"get_time_pos\n", strlen("get_time_pos\n"));

sem_post(sem);
}
    close(fifo_fd);


}
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
                    "../MyQT_Mplayer_Project/song/coffee.mp3", NULL);
    }
    else{
     int fd = open("fifo_cmd",O_WRONLY);
       pthread_t mplayer_ack;
       pthread_create(&mplayer_ack,NULL,Mydeal_fun , (void *)(fd1[0]));
    pthread_detach(mplayer_ack);
    pthread_t send_mplayer;
    pthread_create(&send_mplayer,NULL,deal_fun2 , (void *)fd);
    pthread_detach(send_mplayer);
    QApplication a(argc, argv);
MainWindow w;
    w.show();
    w.pid = pid;
    return a.exec();
    }
}
