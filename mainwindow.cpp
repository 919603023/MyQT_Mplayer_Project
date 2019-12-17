#include "mainwindow.h"
#include "ui_mainwindow.h"
//QString ARTIST;
//int LENGTH;
//int AllTime = 0;
int PuaesFlag= 0;
//QString ALBUMNAME;
//int setseekbarfindviewbyid = 0;
//int setnowtime = 0;
//float totaltime = 0;

//sem_t *sem;
pthread_mutex_t mutex;
QList<lyric*> Lyriclist;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//  ui->widget->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
ui->setupUi(this);
    Initialize();

QTimer *time = new QTimer(this);
time->start(100);

    connect(ui->pushButton_showalllyric,SIGNAL(clicked()),this,SLOT(SlotQPushButtonShowAllLyric()));
    connect(ui->pushButton_volume,SIGNAL(QPushButonMysignalsEnter()),this,SLOT(SlotQPushButtonMysignalsEnter()));
    connect(ui->pushButton_volume,SIGNAL(QPushButonMysignalsLeave()),this,SLOT(SlotQPushButtonMysignalsLeave()));
    connect(ui->groupBox,SIGNAL(QGroupMysignalsLeave()),this,SLOT(SlotQGroupBoxMysignalsLeave()));
    connect(ui->groupBox,SIGNAL(QGroupMysignalsEnter()),this,SLOT(SlotQGroupBoxMysignalsEnter()));
    connect(ui->pushButton_volume,SIGNAL(clicked()),this,SLOT(SlotQPushButtonvolume()));




    connect(ui->huds, &QSlider::valueChanged, ui->spinBox_huds, &QSpinBox::setValue);
       //当改变选值框的值时，同时进度条也改变位置
    connect(ui->pushButton_pause,SIGNAL(clicked()),this,SLOT(SlotMyClickedPlaying()));
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(SlotMyDoubleClickedList(QListWidgetItem *)));
    connect(ui->pushButton_last,SIGNAL(clicked()),this,SLOT(SlotMusicFront()));
    connect(ui->pushButton_next,SIGNAL(clicked()),this,SLOT(SlotMusicNext()));
  //  connect(time,SIGNAL(timeout()),this,SLOT(SlotTimeOut()));
//    connect(ui->pushButton_volume,SIGNAL(Mysignalsvulmehide()),this,SLOT(SlotVulmeHide()));
//    connect(ui->pushButton_volume,SIGNAL(Mysignalsvulmeshow()),this,SLOT(SlotVulmeShow()));
    connect(ui->progress_bar,SIGNAL(valueChanged(int)),this,SLOT(SlotProgressValue(int)));





}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::MyCutSong()
{


Lyriclist.clear();
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
   for(int a = 0;a < 3;a++)
   {
       Lyric[a]->row = a;
       strcpy(Lyric[a]->MyLyric,"\n");
      Lyriclist.push_back(Lyric[a]);
   }
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
//      printf("%s\n ",buff);
//      fflush(stdout);
      strcpy(Lyric[i-1]->MyLyric,buff) ;
      Lyric[i-1]->row = i-1;
      Lyric[i-1]->time = val1*600+val2*10;
//      printf("*\n ",buff);fflush(stdout);
       Lyriclist.push_back(Lyric[i-4]);
//       printf("&\n ",buff);fflush(stdout);
      }
      i++;
      }
  for(int b = 0;b < 6;b++)
  {
      Lyric[i]->row = i + b;
      strcpy(Lyric[i]->MyLyric,"\n");
      Lyriclist.push_back(Lyric[i]);
}
  HaveLyricFlag = 1;
  fclose(MyFd);
  viewinformation.lyric = buf;
  SetAllLyric();

}

char* MainWindow::MyFindLyric()
{
    strcpy(MyBuff,"__nohave");
    std::for_each(Lyriclist.begin(),Lyriclist.end(),[=](lyric* val ){
        if((int)(viewinformation.NowTime *10) == val->time)
        {
//            printf("%s\n",val->MyLyric);
            fflush(stdout);
            strcpy(MyBuff,val->MyLyric);
            ui->listWidget_2->setCurrentRow(val->row+5);
            ui->listWidget_2->setCurrentRow(val->row+3);
            ui->listWidget_2->item(val->row+3)->setTextColor(QColor(255,0,0,255));
            if(val->row != 0)
            {
                ui->listWidget_2->item(val->row+2)->setTextColor(QColor(0,0,0,255));
            }

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

void MainWindow::InsterListWidget(lyric *&val)
{
    ui->listWidget->addItem(new QListWidgetItem(val->MyLyric));
}


void MainWindow::SlotMusicFront()
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
//    printf("%s\n",buff);
//    fflush(stdout);
    MyCutSong();
    Unlock();
    ui->progress_bar->setMaximum(viewinformation.AllTime);

}
void MainWindow::SlotMusicNext()
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
//    printf("%s\n",buff);
//    fflush(stdout);
    MyCutSong();
    Unlock();
    ui->progress_bar->setMaximum(viewinformation.AllTime);

}
void MainWindow::SlotTimeOut()
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

void MainWindow::SlotVulmeHide()
{
    ui->groupBox->hide();
}

void MainWindow::SlotVulmeShow()
{
    ui->groupBox->show();
}

void MainWindow::SlotProgressValue(int val)
{
    int i ;
    char buff[128] = {0};
    i = val - viewinformation.NowTime;
    sprintf(buff,"seek %d\n",i);

    SendMsgToMplayer(buff);
    viewinformation.NowTime = val;

   SetTimeQstring(val,viewinformation.nowtime);

}

void MainWindow::SlotSliderPressed()
{
    SpliderPress = 0;
}

void MainWindow::SlotSliderReleased()
{
    SpliderPress = 1;
}

void MainWindow::SlotQPushButtonShowAllLyric()
{
    if(ShowAllLyric == 0)
    {
        ui->listWidget_2->show();
        ShowAllLyric = 1;
    }
    else
    {
        ui->listWidget_2->hide();
        ShowAllLyric = 0;
    }
}

void MainWindow::SlotQPushButtonvolume()
{


    if(MuteFlag == 0)
    {

        MuteFlag = 1;
        viewinformation.mutehub = ui->huds->value();
        ui->huds->setValue(0);

    }
    else
    {
        MuteFlag = 0;
        ui->huds->setValue(viewinformation.mutehub);
    }
}

void MainWindow::SlotQPushButtonMysignalsEnter()
{
    ui->groupBox->show();
}

void MainWindow::SlotQPushButtonMysignalsLeave()
{
    ui->groupBox->hide();
}

void MainWindow::SlotQGroupBoxMysignalsEnter()
{
    ui->groupBox->show();
}

void MainWindow::SlotQGroupBoxMysignalsLeave()
{
    ui->groupBox->hide();
}



// clicke the pause button
void MainWindow::SlotMyClickedPlaying()
{

    if(PuaesFlag == 0)
      {

        Lock();
    }
    else {

        Unlock();
    }

}

void MainWindow::SlotMyDoubleClickedList(QListWidgetItem *item)
{
            char buff[128]= "";
            strcpy(buf,QStringToChar(item->text()));
//            printf("%s\n",buf);
//            fflush(stdout);
            sprintf(buff,"loadfile \"../MyQT_Mplayer_Project/song/%s\"\n",buf);
            SendMsgToMplayer(buff);
//            printf("%s\n",buff);
//           fflush(stdout);
           MyCutSong();
           Unlock();

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
     MuteFlag = 0;

    for(int i = 0;i < 128;i++)
    {
        lyric *val = (lyric*)malloc(sizeof (int)+256);
        Lyric[i] = val;
    }
    fd = open("fifo_cmd",O_RDWR);
    if(fd < 0){
         perror("open wronly fifo");
    }
    OpenFlag = 0;
    CutSong = 0;
    pthread_mutex_init(&mutex,NULL);

    HaveLyricFlag = 0;
    this->setFixedSize(800, 450);
    ui->spinBox_huds->setValue(99);
    ui->huds->setValue(99);
//    ui->listWidget_2
    ui->label_lyric->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_song->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_album->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_singer->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_nowtime->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_songname->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_albumname->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_totaltime->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->label_singername->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->listWidget->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
    ui->pushButton_last->setShortcut(QKeySequence(Qt::Key_Left));
    ui->pushButton_next->setShortcut(QKeySequence(Qt::Key_Right));
    ui->pushButton_pause->setShortcut(QKeySequence(Qt::Key_Space));
    ShowAllLyric = 0;
    ui->listWidget_2->hide();
    ui->groupBox->hide();

    bzero(buf,sizeof(buf));
    ReadDir("../MyQT_Mplayer_Project/song/");

}
void MainWindow::PrintInformation()
{

  ui->progress_bar->setMaximum(viewinformation.AllTime);
printf("%d\n",viewinformation.NowTime);
fflush(stdout);
   ui->huds->setValue(viewinformation.hub);
   ui->label_songname->setText(viewinformation.song);
   ui->label_singername->setText(viewinformation.singer);
   ui->label_albumname->setText(viewinformation.album);
   ui->label_lyric->setText(viewinformation.lyric);
   ui->label_nowtime->setText(viewinformation.nowtime);
   ui->label_totaltime->setText(QString(viewinformation.alltime));

      ui->progress_bar->setValue(viewinformation.NowTime);
//             ui->progress_bar->setValue(0);
//      ui->listWidget_2->setCurrentRow(19);


}
void MainWindow::SetInformation()
{
    viewinformation.hub =  ui->spinBox_huds->value();
    viewinformation.song = buf;
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
void MainWindow::Lock()
{
    PuaesFlag = 1;
    pthread_mutex_lock(&mutex);
    SendMsgToMplayer("pause\n");
}
void MainWindow::Unlock()
{
    SendMsgToMplayer("pause\n");
    PuaesFlag = 0;
    pthread_mutex_unlock(&mutex);
}

void MainWindow::SetAllLyric()
{
    ui->listWidget_2->clear();
    if(Lyriclist.count() != 0)
    {
 //       std::for_each(Lyriclist.begin(),Lyriclist.end(),MainWindow::InsterListWidget);

        QList<lyric*>::iterator it = Lyriclist.begin();
        while(it != Lyriclist.end())
        {

//            printf("weifjowjefiajfijaewifj\n");
//            fflush(stdout);
            QListWidgetItem *val = new QListWidgetItem(QString(it.i->t()->MyLyric));
        ui->listWidget_2->addItem(val);
        val->setTextAlignment(Qt::AlignHCenter);
//        val->setFlags(Qt::NoItemFlags);

        val->setFlags(val->flags()&~Qt::ItemIsSelectable);


        it++;
        }

    }
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
   int val1;
    char cmd[128] = "";
     MainWindow *m = (MainWindow *)arg;
       int fd  = open("Myfifo",O_RDONLY);
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
                m->viewinformation.progress = percent_pos;

            }
             else if(strcmp(cmd,"ANS_TIME_POSITION") == 0)//当前时间
            {
                 float time_pos = 0;
                  sscanf(buf,"%*[^=]=%f", &time_pos);

                  m->SetTimeQstring(time_pos,m->viewinformation.nowtime);
                  m->viewinformation.NowTime = time_pos;
            }
            else if(strcmp(cmd,"ANS_META_ALBUM") == 0)
            {

                sscanf(buf,"%*[^=]='%s'",val);

                m->viewinformation.album = val;
            }
            else if(strcmp(cmd,"ANS_META_ARTIST") == 0)
            {

                sscanf(buf,"%*[^=]='%s'",val);

                m->viewinformation.singer = val;
            }
            else if(strcmp(cmd,"ANS_LENGTH") == 0)
            {

                sscanf(buf,"%*[^=]=%d",&val1);

                 m->SetTimeQstring(val1,m->viewinformation.alltime);
                 m->viewinformation.AllTime = val1;

            }
            else
            {
            }
              fflush(stdout);
        }
}
void *MyPrint(void *arg)
{
    MainWindow *m = (MainWindow *)arg;
    usleep(200*1000);
    while(1)
    {
        usleep(500*100);
        m->SetInformation();
        m->PrintInformation();
        if(PuaesFlag == 0)
        {
           char buff[128];
           sprintf(buff,"volume %d 1\n",m->viewinformation.hub);
           SendMsgToMplayer(buff);
        }
    }
}



