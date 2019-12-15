#include "mainwindow.h"
#include "ui_mainwindow.h"
QString ARTIST;
int LENGTH;
int AllTime = 0;
int PuaesFlag= 0;
QString ALBUMNAME;
int setseekbarfindviewbyid = 0;
int setnowtime = 0;
float totaltime = 0;

//sem_t *sem;
pthread_mutex_t mutex;
QList<lyric*> Lyriclist;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//  ui->widget->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));

    Initialize();

    QTimer *time = new QTimer(this);
    time->start(100);
    connect(ui->huds, &QSlider::valueChanged, ui->spinBox_huds, &QSpinBox::setValue);
       //当改变选值框的值时，同时进度条也改变位置
    connect(ui->pushButton_pause,SIGNAL(clicked()),this,SLOT(MyClickedPlaying()));
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(MyDoubleClickedList(QListWidgetItem *)));
    connect(ui->pushButton_last,SIGNAL(clicked()),this,SLOT(MusicFront()));
    connect(ui->pushButton_next,SIGNAL(clicked()),this,SLOT(MusicNext()));
    connect(time,SIGNAL(timeout()),this,SLOT(TimeOut()));

}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::MyCutSong()
{


   printf("a wo si la\n");fflush(stdout);
   char buff[128] = "";
   char Site[128] = "";
   int val1,val2,val3;
   char buff1[128];
    strcpy(buff,buf);
    strcpy(&(buff[strlen(buff)-4]),".lrc");
    sprintf(Site,"../MyQT_Mplayer_Project/lyrics/%s",buff);
   printf("%s",Site);
   fflush(stdout);
   FILE *MyFd;
   HaveLyricFlag = 0;
  if( (MyFd = fopen(Site,"r+")) == NULL){
    perror("fopen the lyric");

    return;
  }
  int i = 0;
  while(fgets(buff1, sizeof(buff1), MyFd) != NULL)
      {
      if(i > 3)
      {

     sscanf(buff1,"[%02d:%02d.%02d]",&val1,&val2,&val3);
      strcpy(buff,&(buff1[10]));
      strcpy(&(buff[strlen(buff)-2]),"\n\0\0");
      printf("%s\n ",buff);
      fflush(stdout);
      strcpy(Lyric[i-4]->MyLyric,buff) ;
      Lyric[i-4]->time = val1*600+val2*10;
      printf("*\n ",buff);fflush(stdout);
       Lyriclist.push_back(Lyric[i-4]);
       printf("&\n ",buff);fflush(stdout);
      }
      i++;
      }
  HaveLyricFlag = 1;
  fclose(MyFd);
  viewinformation.lyric = buf;
}

char* MainWindow::MyFindLyric()
{
    strcpy(MyBuff,"__nohave");
    std::for_each(Lyriclist.begin(),Lyriclist.end(),[=](lyric* val ){
        if((int)(setnowtime*10) == val->time)
        {
            printf("%s\n",val->MyLyric);
            fflush(stdout);
            strcpy(MyBuff,val->MyLyric);
        }
    });
    return MyBuff;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    char buff[128] = "";
    sprintf(buff,"kill -9 %d",pid);
    system(buff);
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
    char buff[128]= {0};
    strcpy(buf,QStringToChar(ui->listWidget->currentItem()->text()));
    sprintf(buff,"loadfile \"../MyQT_Mplayer_Project/song/%s\"\n",buf);
    SendMsgToMplayer(buff);
    printf("%s\n",buff);
    fflush(stdout);
    MyCutSong();

}

void MainWindow::TimeOut()
{
    SetInformation();
    PrintInformation();
    if(PuaesFlag == 0)
    {
       char buff[128];
       sprintf(buff,"volume %d 1\n",viewinformation.hub);
       SendMsgToMplayer(buff);
    }
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
    char buff[128]= {0};
    strcpy(buf,QStringToChar(ui->listWidget->currentItem()->text()));
    sprintf(buff,"loadfile \"../MyQT_Mplayer_Project/song/%s\"\n",buf);
    SendMsgToMplayer(buff);
    printf("%s\n",buff);
    fflush(stdout);
    MyCutSong();

}
// clicke the pause button
void MainWindow::MyClickedPlaying()
{

    if(PuaesFlag == 0)
      {
        PuaesFlag = 1;
        pthread_mutex_lock(&mutex);
        SendMsgToMplayer("pause\n");
        printf("wo gai le");
        fflush(stdout);
    }
    else {
        SendMsgToMplayer("pause\n");
        PuaesFlag = 0;
       pthread_mutex_unlock(&mutex);
        printf("%d \n",PuaesFlag);
        fflush(stdout);
    }
    //    MyCutSong();
}

void MainWindow::MyDoubleClickedList(QListWidgetItem *item)
{
            char buff[128]= "";
            strcpy(buf,QStringToChar(item->text()));
            printf("%s\n",buf);
            fflush(stdout);
            sprintf(buff,"loadfile \"../MyQT_Mplayer_Project/song/%s\"\n",buf);
            SendMsgToMplayer(buff);
            printf("%s\n",buff);
           fflush(stdout);
           MyCutSong();

}
void MainWindow::resizeEvent(QResizeEvent *)
{
    QPalette    palette = this->palette();
    QPixmap    pixmap(":/res/img/bg.jpg");
       palette.setBrush(this->backgroundRole(),
                         QBrush(pixmap.scaled(this->size(),
                                Qt::IgnoreAspectRatio,
                                Qt::SmoothTransformation)));
       this->setPalette(palette);
}

void MainWindow::ReadDir(char *val)
{
    DIR *dir = opendir(val);
    int i = 0;
    ui->listWidget->clear();
    while (1) {
        struct dirent* dirp = readdir(dir);
        if(dirp == NULL){
            break;
        }
        else if(dirp->d_type ==  DT_REG){
            if(strcmp(&(dirp->d_name[strlen(dirp->d_name)-3]),".mp3"))
            {
                ui->listWidget->addItem(new QListWidgetItem(dirp->d_name));
                if(i == 0)
                {
                    i++;
                    strcpy(buf,dirp->d_name);
                }

            if(strcmp(dirp->d_name,buf) == 0)
            {
                ui->listWidget->setCurrentRow(3);
            }
            else
            {
                ui->listWidget->setCurrentRow(0);
            }
            }

        }

        }


    closedir(dir);
}

void MainWindow::Initialize()
{
    for(int i = 0;i < 128;i++)
    {
        lyric *val = (lyric*)malloc(sizeof (int)+256);
        Lyric[i] = val;
    }
    HaveLyricFlag = 0;
    fd = open("fifo_cmd",O_RDWR);

    pthread_mutex_init(&mutex,NULL);
    ui->setupUi(this);
    OpenFlag = 0;
    CutSong = 0;
    if(fd < 0){
         perror("open wronly fifo");
    }
    ui->spinBox_huds->setValue(99);
    ui->huds->setValue(99);
    ui->label_lyric->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_song->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_album->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_singer->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_nowtime->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_songname->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_albumname->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_totaltime->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_singername->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    bzero(buf,sizeof(buf));
    ReadDir("../MyQT_Mplayer_Project/song/");

}

void MainWindow::PrintInformation()
{
   ui->huds->setValue(viewinformation.hub);
   ui->label_songname->setText(viewinformation.song);
   ui->label_singername->setText(viewinformation.singer);
   ui->label_albumname->setText(viewinformation.album);
   ui->label_lyric->setText(viewinformation.lyric);
   ui->label_nowtime->setText(viewinformation.nowtime);
   ui->label_totaltime->setText(QString(viewinformation.alltime));
   ui->progress_bar->setValue(viewinformation.progress);
}

void MainWindow::SetInformation()
{
    viewinformation.hub =  ui->spinBox_huds->value();
    SetTimeQstring(setnowtime,viewinformation.nowtime);
    SetTimeQstring(LENGTH,viewinformation.alltime);



    viewinformation.progress = setseekbarfindviewbyid;
    viewinformation.song = buf;
    viewinformation.album = ALBUMNAME;
    viewinformation.singer = ARTIST;

    if(HaveLyricFlag == 1)
    {
        if(strcmp(MyFindLyric(), "__nohave") != 0)
        {
           viewinformation.lyric= MyFindLyric();
        }
    }
    else
    {
      viewinformation.lyric = "No have lyric";
    }
}
void SetSeekBarFindViewById(int val)
{
    setseekbarfindviewbyid = val;
}
void SetNowTime(float val)
{
    setnowtime = val;
}
void TotalTime(float val)
{
    totaltime = val;
}
void MainWindow::SetTimeQstring(float val,QString &val1)
{
    char buff[128] = {0};
   int i = val;

   int minute = i/60;
   int second = i%60;
   int msec = ((int)(val*10))%10;
   sprintf(buff,"%02d:%02d",minute,second);
   val1 = QString(buff);
}
void SendMsgToMplayer(char *val)
{
   int fd = open("fifo_cmd",O_RDWR);
    write(fd,val,strlen(val));
    close(fd);
}
char *QStringToChar(QString val)
{
    QByteArray ba = val.toUtf8();
    return ba.data();
}

void *MySendMsgToMplayer(void *arg)
{
    usleep(200*10000+800000);
        //不停的给fifo_cmd发送获取当前时间以及进度
        while(1)
        {
            pthread_mutex_lock(&mutex);
            usleep(500*100);//0.05秒发指令
            SendMsgToMplayer("get_percent_pos\n");
            usleep(500*100);//0.05秒发指令
            SendMsgToMplayer("get_time_pos\n");
            usleep(500*100);//0.05秒发指令
            SendMsgToMplayer("get_time_length\n");
            usleep(500*100);//0.05秒发指令
            SendMsgToMplayer("get_meta_artist\n");
           pthread_mutex_unlock(&mutex);
           usleep(500*100);//0.05秒发指令
        }
}
void *MyGetTimeAndBar(void *arg)
{
    char val[128] = "";
    char buf[128] = "";
    float val1;
    char cmd[128] = "";
        int fd = (int)(long)arg;
        while(1)
        {
            bzero(val,sizeof(val));
            bzero(buf,sizeof (buf));
            bzero(cmd,sizeof(cmd));
            val1 =0;
            read(fd,buf,sizeof (buf));

            sscanf(buf,"%[^=]",cmd);
            if(strcmp(cmd,"ANS_PERCENT_POSITION") == 0)//百分比
            {
                int percent_pos = 0;
                sscanf(buf,"%*[^=]=%d",&percent_pos);
                SetSeekBarFindViewById(percent_pos);
            }
             else if(strcmp(cmd,"ANS_TIME_POSITION") == 0)//当前时间
            {
                 float time_pos = 0;
                  sscanf(buf,"%*[^=]=%f", &time_pos);
                  SetNowTime(time_pos);
            }
            else if(strcmp(cmd,"ANS_META_ALBUM") == 0)
            {

                sscanf(buf,"%*[^=]='%s'",val);
                ALBUMNAME = val;
            }
            else if(strcmp(cmd,"ANS_META_ARTIST") == 0)
            {

                sscanf(buf,"%*[^=]='%s'",val);
                ARTIST = val;
            }
            else if(strcmp(cmd,"ANS_LENGTH") == 0)
            {

                sscanf(buf,"%*[^=]=%f",&val1);
                LENGTH = val1;
            }
            else
            {
            }
              fflush(stdout);
        }
}
