#include "mainwindow.h"
#include "ui_mainwindow.h"


int PuaesFlag= 0;
//sem_t *sem;
pthread_mutex_t mutex;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    pthread_mutex_init(&mutex,NULL);
//   sem_init(sem,0,1);
    ui->setupUi(this);
    OpenFlag = 0;
    CutSong = 0;
  //  PuaesFlag = 0;
    fd = open("fifo_cmd",O_RDWR);
    if(fd < 0){
         perror("open wronly fifo");
    }
    connect(ui->huds, &QSlider::valueChanged, ui->spinBox_huds, &QSpinBox::setValue);
    connect(ui->huds,&QSlider::valueChanged,[=]{
        int val = ui->spinBox_huds->value();
        char buff1[128] ="volume ";
        char a[20];
        sprintf(a,"%d",val);
        strcat(buff1,a);
        strcat(buff1," 1");
        strcat(buff1,"\n");
        if(PuaesFlag == 0)
        {
        write(fd,buff1,strlen(buff1));
        }



    });
       //当改变选值框的值时，同时进度条也改变位置
       void (QSpinBox::*mysignal)(int) = &QSpinBox::valueChanged;


    bzero(buf,sizeof(buf));
    connect(ui->pushButton_pause,SIGNAL(clicked()),this,SLOT(MyClickedPlaying()));

    //list fflush button
    connect(ui->pushButton,&QPushButton::clicked,[=]{
        DIR *dir = opendir("../MyQT_Mplayer_Project/song");
        int i = 0;
        ui->listWidget->clear();
        while (1) {
            struct dirent* dirp = readdir(dir);

            if(dirp == NULL){
                break;
            }
            else if(dirp->d_type == DT_REG){
                ui->listWidget->addItem(new QListWidgetItem(dirp->d_name));

            }
            i++;
            }

        OpenFlag = 1;
            closedir(dir);
 //           MyCutSong();
    });
    connect(ui->listWidget,&QListWidget::doubleClicked,[=]{


        char buff[128]= "loadfile ../MyQT_Mplayer_Project/song/";
        QByteArray ba = ui->listWidget->currentItem()->text().toUtf8();
        strcpy(buf,ba.data());
        strcat(buff,buf);
        strcat(buff,"\n");
        write(fd,buff,strlen(buff));
        printf("%s\n",buff);
       fflush(stdout);

    });
//    connect(ui->listWidget,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(MyDoubleClickedList(const QModelIndex &index)));
    InitializeListFunction();
//    MyCutSong();
    connect(ui->pushButton_last,&QPushButton::clicked,[=]{
        MusicFront();
    });
    connect(ui->pushButton_next,&QPushButton::clicked,[=]{
        MusicNext();
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
//the song list initialize
void MainWindow::InitializeListFunction()
{
    DIR *dir = opendir("../MyQT_Mplayer_Project/song");
    int i = 0;
    ui->listWidget->clear();
    ui->listWidget->setCurrentRow(0);
    while (1) {
        struct dirent* dirp = readdir(dir);

        if(dirp == NULL){
            break;
        }
        else if(dirp->d_type == DT_REG){
            ui->listWidget->addItem(new QListWidgetItem(dirp->d_name));
            strcpy(buf,dirp->d_name);
            char *buff1 = (char*)malloc(128);
            strcpy(buff1,buf);
            QListSongName.push_back(buff1);


        }
        i++;
        }
    closedir(dir);
}

void MainWindow::MyCutSong()
{

   char buff[128] = "";
   char Site[128] = "../MyQT_Mplayer_Project/lyrics/";
   char *buff1,*buff2,*buff3,buff4[128] = {0};

   sscanf(buf,"%s.",buff4);
   strcpy(buff4,strtok(buf,"."));
   strcat(buff4,".lrc");
   strcat(Site,buff4);
   printf("%s",Site);
   fflush(stdout);
   FILE *MyFd;
  if( (MyFd = fopen(Site,"r+")) == NULL){
    perror("fopen the lyric");
    return;
  }
  while(fgets(buff, sizeof(buff), MyFd) != NULL)
      {
          sscanf(buff,"[%s:",buff1);
          sscanf(buff,":%s.",buff2);
          sscanf(buff,".%s]",buff3);
          sscanf(buff,"]%s\n",StructLyric.MyLyric);

          lyric *temp = (lyric *)malloc(sizeof(lyric));
          temp->time = atof(buff1)*60 + atof(buff2) + atof(buff3)*0.01;
          Lyriclist.push_back(*temp);


      }




}

int MainWindow::GetPuaesFlag()
{
    return PuaesFlag;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    char buff[128] = "";
    char buff1[128] = "kill -9 ";
    sprintf(buff,"%d",pid);
    strcat(buff1,buff);
    system(buff1);
}

void MainWindow::SetSeekbarfindViewById(int val)
{
    ui->progress_bar->setValue(val);
}

void *Mydeal_fun(void *arg)
{

        int fd = (int)(long)arg;
        while(1)
        {
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

        }
}

void *deal_fun2(void *arg)
{
    usleep(500*1000);
    pthread_mutex_init(&mutex,NULL);
        int fifo_fd1 = (int)(long)arg;

        //不停的给fifo_cmd发送获取当前时间以及进度
        while(1)

        {
            pthread_mutex_lock(&mutex);
            printf("%d **************\n",PuaesFlag);
            fflush(stdout);

//           sem_wait(sem);
            usleep(500*100);//0.5秒发指令

            write(fifo_fd1,"get_percent_pos\n", strlen("get_percent_pos\n"));

            usleep(500*100);//0.5秒发指令

            write(fifo_fd1,"get_time_pos\n", strlen("get_time_pos\n"));
//            sem_post(sem);
           pthread_mutex_unlock(&mutex);
           usleep(500*100);

        }
    }


void MainWindow::MusicFront()
{
    if(ui->listWidget->currentRow() == 0)//当光标在第一个文件时，点击上一个光标移动到最下面的文件，不播放
    {
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    }
    else
    {
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow()-1);
    }
    char buff[128]= "loadfile ../MyQT_Mplayer_Project/song/";
    QByteArray ba = ui->listWidget->currentItem()->text().toUtf8();
    strcpy(buf,ba.data());
    strcat(buff,buf);
    strcat(buff,"\n");
    write(fd,buff,strlen(buff));
    printf("%s\n",buff);
    fflush(stdout);
}
void MainWindow::MusicNext()
{
    if(ui->listWidget->currentRow() == ui->listWidget->count()-1)
    {
        ui->listWidget->setCurrentRow(0);//当光标在最后一个文件时，点击下一个，光标移动到第一个，不播放
    }
    else
    {
        ui->listWidget->setCurrentRow(ui->listWidget->currentRow()+1);
    }
    char buff[128]= "loadfile ../MyQT_Mplayer_Project/song/";
    QByteArray ba = ui->listWidget->currentItem()->text().toUtf8();
    strcpy(buf,ba.data());
    strcat(buff,buf);
    strcat(buff,"\n");
    write(fd,buff,strlen(buff));
    printf("%s\n",buff);
    fflush(stdout);
}

// clicke the pause button
void MainWindow::MyClickedPlaying()
{

    if(PuaesFlag == 0)
      {
        PuaesFlag = 1;
        pthread_mutex_lock(&mutex);
        write(fd,"pause\n",strlen ("pause\n"));
 //       sem_wait(sem);
        printf("wo gai le");
        fflush(stdout);
    }
    else {
        write(fd,"pause\n",strlen ("pause\n"));
        PuaesFlag = 0;
       pthread_mutex_unlock(&mutex);
 //       sem_post(sem);
        printf("%d \n",PuaesFlag);
        fflush(stdout);
    }
    std::for_each(QListSongName.begin(),QListSongName.end(),[=](char *val){
        printf("%s\n",val);
        fflush(stdout);

    });

}

void MainWindow::MyDoubleClickedList(const QModelIndex &index)
{
    char buff[128]= "loadfile ../MyQT_Mplayer_Project/song/";
    QByteArray ba = ui->listWidget->currentItem()->text().toUtf8();
    strcpy(buf,ba.data());
    strcat(buff,buf);
    strcat(buff,"\n");
    write(fd,buff,strlen(buff));
    printf("%s\n",buff);
   fflush(stdout);
}










