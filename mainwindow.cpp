#include "mainwindow.h"
#include "ui_mainwindow.h"

int PuaesFlag= 0;

#define N 0

pthread_mutex_t mutex;
pthread_mutex_t mutex2;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//  ui->widget->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));
             ui->setupUi(this);
            Initialize();

//            QMenuBar *mBar = menuBar();
//             setMenuBar(mBar);
//             QMenu *menu = mBar->addMenu("对话框");
//             QAction *p5 = menu->addAction("文件对话框");
//                connect(p5,&QAction::triggered,[=](){

//                    /*打开一个文件，显示内容，显示路径，
//                     * 通过查看getOpenFileName里面的内容，（其实里面已经有写好的例子）
//                     * QFileDialog::getOpenFileName(this,"open","../");会有一个返回值，
//                     * QString path = QFileDialog::getOpenFileName(this,"open","../");
//                     * 如果想打开指定格式的文件怎么写？
//                     * 注意：当想有多种类型的文件打开时，使用两个;;来进行表示
//                     *
//                    */
//                    QString path = QFileDialog::getOpenFileName(this,"open","../","JPG(*.jpg)");
//                    qDebug()<<path;
//                });















            QTimer *time = new QTimer(this);
            time->start(50);
            pixmap.load(":/res/img/bg.jpg");

            connect(ui->pushButton_showalllyric,SIGNAL(clicked()),this,SLOT(SlotQPushButtonShowAllLyric()));

            connect(ui->pushButton_volume,SIGNAL(clicked()),this,SLOT(SlotQPushButtonvolumeShow()));

            connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT( SlotQPushButtonMute()));

            connect(ui->pushButton_volume,SIGNAL(clicked()),this,SLOT(SlotQGroupBoxMysignalsLeave()));

            connect(ui->pushButton_volume,SIGNAL(clicked()),this,SLOT(SlotQPushButtonvolume()));

            connect(ui->huds, &QSlider::valueChanged, ui->spinBox_huds, &QSpinBox::setValue);

       //当改变选值框的值时，同时进度条也改变位置
            connect(ui->pushButton_pause,SIGNAL(clicked()),this,SLOT(SlotMyClickedPlaying()));

            connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(SlotMyDoubleClickedList(QListWidgetItem *)));

            connect(ui->pushButton_last,SIGNAL(clicked()),this,SLOT(SlotMusicFront()));

            connect(ui->pushButton_next,SIGNAL(clicked()),this,SLOT(SlotMusicNext()));

            connect(time,SIGNAL(timeout()),this,SLOT(SlotTimeOut()));

            connect(ui->progress_bar,SIGNAL(valueChanged(int)),this,SLOT(SlotProgressValue(int)));

            connect(ui->pushButton_2,&QPushButton::clicked,[=](){

         QString val = QFileDialog::getOpenFileName(this,"open","../","JPG(*.jpg)");
         if(val == "")
         {
            pixmap.load(":/res/img/bg.jpg");
         }
         else
            {
             pixmap.load(val);
         }
         this->setFixedSize(800,451);
         this->setFixedSize(800,450);

    });

        //this->setWindowFlags(Qt::FramelessWindowHint);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Initialize()
{

    pthread_mutex_init(&mutex,NULL);

    fd = open("fifo_cmd",O_RDWR);
    if(fd < 0){
         perror("open wronly fifo");
    }
    OpenFlag = 0;

    Mode = 0;

    Frist = 0;

    imageRotate = 0;

    VolueButtonFlag = 0;

    MuteFlag = 0;

    HaveLyricFlag = 0;

    ShowAllLyric = 0;

    viewinformation.hub = 0;

    viewinformation.song = "";


    viewinformation.album = "";

    viewinformation.lyric = "";

    viewinformation.singer = "";

    viewinformation.AllTime = 0;

    viewinformation.NowTime = 0;

    viewinformation.alltime = "";

    viewinformation.mutehub = 0;

    viewinformation.nowtime = "";

    viewinformation.progress = 0;


    this->setFixedSize(800, 450);

    ui->spinBox_huds->setMaximum(100);

    ui->spinBox_huds->setValue(50);

    ui->huds->setMaximum(100);

    ui->huds->setValue(50);

    ui->pushButton_3->hide();

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

    ui->listWidget_2->setStyleSheet(QString("background-color: rgba(255, 255, 255, 55%);"));

    ui->pushButton_last->setShortcut(QKeySequence(Qt::Key_Left));

    ui->pushButton_next->setShortcut(QKeySequence(Qt::Key_Right));

    ui->pushButton_pause->setShortcut(QKeySequence(Qt::Key_Space));

    ui->listWidget_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //垂直滚动条

    ui->listWidget_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    ui->listWidget_2->hide();

    ui->groupBox->hide();

    bzero(buf,sizeof(buf));

    this->setWindowTitle("奥利给播放器");

    this->setWindowIcon(QIcon(":/res/img/gei.png"));

    ReadDir("../MyQT_Mplayer_Project/song/");

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

void MainWindow::GetLyric()
{

    usleep(500);


            LyriclistTime.clear();

            LyriclistLyric.clear();


            char buff[128] = "";
            char Site[128] = "";
            int val1,val2,val3;
            char buff1[128];
            strcpy(buff,buf);
            strcpy(&(buff[strlen(buff)-4]),".lrc");
            sprintf(Site,"../MyQT_Mplayer_Project/lyrics/%s",buff);
//            printf("Site=================%s\n",Site);
            fflush(stdout);
           FILE *MyFd;
           HaveLyricFlag = 0;
            for(int a = 0;a < 3;a++)
            {
                LyriclistTime.push_back(0);
                LyriclistLyric.push_back("\n");
             }
          if( (MyFd = fopen(Site,"r+")) != NULL)
          {
            int i = 0;
            while(fgets(buff1, sizeof(buff1), MyFd) != NULL)
            {

                if(i > 3)
                {
                sscanf(buff1,"[%02d:%02d.%02d]",&val1,&val2,&val3);
                strcpy(buff,&(buff1[10]));
//                printf("%d----%s\n",val1*60+val2,buff);
                fflush(stdout);


                LyriclistTime.push_back(val1*60+val2);

                LyriclistLyric.push_back(buff);

                usleep(5000);
                }
                 i++;

             }
       for(int b = 0;b < 6;b++)
        {
            LyriclistTime.push_back(0);
            LyriclistLyric.push_back("\n");
        }

            HaveLyricFlag = 1;
            fclose(MyFd);

            SetAllLyric();

        }
        else
        {

              LyriclistTime.clear();
              LyriclistLyric.clear();
        }
        viewinformation.lyric = buf;
    return;
}

QString MainWindow::MyFindLyric()
{



    if(LyriclistLyric.count()<10)
    {

        strcpy(MyBuff,"__nohave");

        return "__nohave";
    }
    usleep(5000);

if(LyriclistTime.indexOf(viewinformation.NowTime) != -1)
{
    int a =LyriclistTime.indexOf(viewinformation.NowTime);
//    printf("wo zhao dao la ");
    ui->listWidget_2->setCurrentRow(a+2);
    ui->listWidget_2->setCurrentRow(a);
    bzero(MyBuff,sizeof (MyBuff));
    QByteArray ba = LyriclistLyric[a].toUtf8(); // must
    strcpy(MyBuff,ba.data());

    for(int i = 0;i < ui->listWidget_2->count();i++)
   {

       ui->listWidget_2->item(i)->setTextColor(QColor(0,0,0,255));
    }
//printf("awkfjwlllllllllllll\n");
//fflush(stdout);
      ui->listWidget_2->item(a)->setTextColor(QColor(255,0,0,255));
      return LyriclistLyric[a];
}

#if 0
    std::for_each(LyriclistTime.begin(),LyriclistTime.end(),[=](int val ){
        if(viewinformation.NowTime *10 == val)        {

            strcpy(MyBuff,val->MyLyric);

            ui->listWidget_2->setCurrentRow(val->row+5);

            ui->listWidget_2->setCurrentRow(val->row+3);
          for(int i = 0;i < ui->listWidget_2->count();i++)
         {

             ui->listWidget_2->item(i)->setTextColor(QColor(0,0,0,255));
          }

            ui->listWidget_2->item(val->row+3)->setTextColor(QColor(255,0,0,255));
            printf("bengle \n");

#endif



    return MyBuff;
}


void MainWindow::SetAllLyric()
{
    usleep(100*100);
    ui->listWidget_2->clear();
    if(LyriclistLyric.count()<10)
    {

    for(int i = 0;i < ui->listWidget_2->count();i++)
    {

       ui->listWidget_2->item(i-1)->setText("NO_HAVE");
    }
    return ;
    }

#if 1

        QList<QString>::iterator it = LyriclistLyric.begin();

        while(it != LyriclistLyric.end())
        {

        QListWidgetItem *val = new QListWidgetItem(it.i->t());
        ui->listWidget_2->addItem(val);
        val->setTextAlignment(Qt::AlignHCenter);
        val->setFlags(val->flags()&~Qt::ItemIsSelectable);
//         qDebug() << it.i->t();
        it++;
        }
#endif
}

void MainWindow::CutSong(char *val)
{
    switch (Mode) {
    case 0:
        if(QString(val) == "last")
        {
            if(ui->listWidget->currentRow() == 0)//当光标在第一个文件时，点击上一个光标移动到最下面的文件，不播放
            {
                  ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
             }
            else
              {
                  ui->listWidget->setCurrentRow(ui->listWidget->currentRow()-1);
               }
        }
        else
        {
            if(ui->listWidget->currentRow() == ui->listWidget->count()-1)
            {
                ui->listWidget->setCurrentRow(0);//当光标在最后一个文件时，点击下一个，光标移动到第一个，不播放
            }
            else
            {
                ui->listWidget->setCurrentRow(ui->listWidget->currentRow()+1);
            }
        }
        break;
    case 1:
       ui->listWidget->setCurrentRow( ui->listWidget->currentRow());
        break;
    case 2:
        ui->listWidget->setCurrentRow(qrand()%ui->listWidget->count());
        break;
    case 3:
        return;




    }

    char buff[128]= {0};
    strcpy(buf,QStringToChar(ui->listWidget->currentItem()->text()));
    sprintf(buff,"loadfile \"../MyQT_Mplayer_Project/song/%s\"\n",buf);
    SendMsgToMplayer(buff);

    GetLyric();
    Unlock();

    Lock();
    viewinformation.NowTime = 0;
    usleep(2300000);
    Unlock();
    ui->progress_bar->setMaximum(viewinformation.AllTime);
}

void MainWindow::SetInformation()
{
    viewinformation.hub =  ui->spinBox_huds->value();
    viewinformation.song = buf;
    viewinformation.progress = viewinformation.NowTime;
    if(HaveLyricFlag == 1)
    {
        if(MyFindLyric() != "__nohave")
        {

           viewinformation.lyric= MyFindLyric();
        }
    }
    else
    {
      viewinformation.lyric = "No have lyric";
    }

}

void MainWindow::PrintInformation()
{

   ui->progress_bar->setMaximum(viewinformation.AllTime);
   qDebug()<<viewinformation.AllTime;

   ui->huds->setValue(viewinformation.hub);

   ui->label_songname->setText(viewinformation.song);

   ui->label_singername->setText(viewinformation.singer);

   ui->label_albumname->setText(viewinformation.album);

   ui->label_lyric->setText(viewinformation.lyric);

   ui->label_nowtime->setText(viewinformation.nowtime);

   ui->label_totaltime->setText(QString(viewinformation.alltime));
    usleep(6666);
   ui->progress_bar->setValue(viewinformation.NowTime);
   qDebug()<<viewinformation.NowTime;



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

void SendMsgToMplayer(char *val)
{
    pthread_mutex_lock(&mutex2);
   int fd = open("fifo_cmd",O_RDWR);
    write(fd,val,strlen(val));
    close(fd);
    pthread_mutex_unlock(&mutex2);
}

char *QStringToChar(QString val)
{
    QByteArray ba = val.toUtf8();
    return ba.data();
}

void *MySendMsgToMplayer(void *arg)
{



        //不停的给fifo_cmd发送获取当前时间以及进度
        while(1)
        {
            pthread_mutex_lock(&mutex);


            usleep(800*100);//0.05秒发指令
            SendMsgToMplayer("get_percent_pos\n");
            usleep(800*100);//0.05秒发指令
            SendMsgToMplayer("get_time_pos\n");
            usleep(800*100);//0.05秒发指令
            SendMsgToMplayer("get_time_length\n");
            usleep(800*100);//0.05秒发指令
            SendMsgToMplayer("get_meta_artist\n");
           pthread_mutex_unlock(&mutex);

           usleep(800*100);//0.05秒发指令

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
                 int time_pos = 0;
                  sscanf(buf,"%*[^=]=%d", &time_pos);

                  m->SetTimeQstring(time_pos,m->viewinformation.nowtime);
                  m->viewinformation.NowTime = time_pos;
                  if(m->viewinformation.NowTime == m->viewinformation.AllTime-2 )
                  {








                  }
//                  printf("%d\n",m->viewinformation.NowTime);
//                  fflush(stdout);
//                  printf("%d\n",time_pos);
//                  fflush(stdout);
            }
            else if(strcmp(cmd,"ANS_META_ALBUM") == 0)
            {

                sscanf(buf,"%*[^=]='%s'",val);
                QByteArray ba = QString(val).toUtf8();
                m->viewinformation.album = ba.data();
            }
            else if(strcmp(cmd,"ANS_META_ARTIST") == 0)
            {

                sscanf(buf,"%*[^=]='%s'",val);

                 QByteArray ba = QString(val).toUtf8();

                m->viewinformation.singer = ba.data();
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
//    MainWindow *m = (MainWindow *)arg;
//    usleep(200*1000);
//    while(1)
//    {
//        usleep(500*100);
//        m->SetInformation();
//        m->PrintInformation();
//        if(PuaesFlag == 0)
//        {
//           char buff[128];
//           sprintf(buff,"volume %d 1\n",m->viewinformation.hub);
//           SendMsgToMplayer(buff);
//        }
//    }
}

void MainWindow::SlotMusicFront()
{
//    if(ui->listWidget->currentRow() == 0)//当光标在第一个文件时，点击上一个光标移动到最下面的文件，不播放
//    {
//        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
//    }
//    else
//    {
//        ui->listWidget->setCurrentRow(ui->listWidget->currentRow()-1);
//    }
//    char buff[128]= {0};
//    strcpy(buf,QStringToChar(ui->listWidget->currentItem()->text()));
//    sprintf(buff,"loadfile \"../MyQT_Mplayer_Project/song/%s\"\n",buf);
//    SendMsgToMplayer(buff);

//    GetLyric();
//    Unlock();
//    usleep(500);
//    ui->progress_bar->setMaximum(viewinformation.AllTime);

    CutSong("last");

}

void MainWindow::SlotMusicNext()
{
//    if(ui->listWidget->currentRow() == ui->listWidget->count()-1)
//    {
//        ui->listWidget->setCurrentRow(0);//当光标在最后一个文件时，点击下一个，光标移动到第一个，不播放
//    }
//    else
//    {
//        ui->listWidget->setCurrentRow(ui->listWidget->currentRow()+1);
//    }
//    char buff[128]= {0};
//    strcpy(buf,QStringToChar(ui->listWidget->currentItem()->text()));
//    sprintf(buff,"loadfile \"../MyQT_Mplayer_Project/song/%s\"\n",buf);
//    SendMsgToMplayer(buff);
//    GetLyric();
//    Unlock();
//    usleep(500);
//    ui->progress_bar->setMaximum(viewinformation.AllTime);
    CutSong("next");

}

void MainWindow::SlotTimeOut()
{
//if(Frist == 0)
//{
//    sleep(2);
//   Frist = 1;
//}
    SetInformation();
    PrintInformation();
    if(PuaesFlag == 0)
    {

        fflush(stdout);
       char buff[128];
       sprintf(buff,"volume %d 1\n",viewinformation.hub);

       fflush(stdout);
       SendMsgToMplayer(buff);

       fflush(stdout);

    }
usleep(500*10);
    if(viewinformation.NowTime == viewinformation.AllTime-2 && buf != "")
    {


            CutSong("next");
            viewinformation.NowTime = 0;





    }
    update();


}

void MainWindow::SlotProgressValue(int val)
{
    int i ;
    char buff[128] = {0};
    usleep(20000);
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
        ui->label_lyric->hide();
        ShowAllLyric = 1;
    }
    else
    {
        ui->listWidget_2->hide();
        ui->label_lyric->show();
        ShowAllLyric = 0;
    }
}

void MainWindow::SlotQPushButtonvolumeShow()
{


    if(VolueButtonFlag == 0)
    {

        VolueButtonFlag = 1;
        ui->groupBox->show();


    }
    else
    {
        VolueButtonFlag = 0;
        ui->groupBox->hide();
    }
}
void MainWindow::SlotQPushButtonMute()
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
            sprintf(buff,"loadfile \"../MyQT_Mplayer_Project/song/%s\"\n",buf);
            SendMsgToMplayer(buff);
            GetLyric();
            Unlock();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    SendMsgToMplayer("quit\n");
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    QPalette    palette = this->palette();

       palette.setBrush(this->backgroundRole(),
                         QBrush(pixmap.scaled(this->size(),
                                Qt::IgnoreAspectRatio,
                                Qt::SmoothTransformation)));
       this->setPalette(palette);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(PuaesFlag == 0)
    {
        disc.load(":/res/img/gei.png");

        /* 碟机转动 */
        if(imageRotate++ == 360)
            imageRotate = 0;
    }
    else
    {
        disc.load(":/res/img/laoba.png");
    }
        /* 设定旋转中心点 */
        painter.translate(130,150);
        /* 旋转的角度 */
        painter.rotate(imageRotate);
        /* 恢复中心点 */
        painter.translate(-130,-150);
        /* 画图操作 */
        painter.drawPixmap(40,60,180,180, disc);

}


void MainWindow::on_pushButton_clicked()
{
   ui->stackedWidget->setCurrentIndex(1);
   ui->pushButton_3->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->pushButton_3->hide();
}

void MainWindow::on_pushButton_5_clicked()
{
    switch (Mode) {
    case 0 : Mode = 1;break;
    case 1 : Mode = 2;break;
    case 2 : Mode = 3;break;
    case 3 : Mode = 0;break;


    }
}
