#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OpenFlag = 0;
    CutSong = 0;
    fd = open("fifo",O_RDWR);
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
        write(fd,buff1,128);
    });
       //当改变选值框的值时，同时进度条也改变位置
       void (QSpinBox::*mysignal)(int) = &QSpinBox::valueChanged;
       connect(ui->spinBox_huds,mysignal, ui->huds, &QSlider::setValue);

    bzero(buf,sizeof(buf));
    connect(ui->pushButton_pause,SIGNAL(clicked()),this,SLOT(MyClickedPlaying()));

    //list fflush button
    connect(ui->pushButton,&QPushButton::clicked,[=]{
        DIR *dir = opendir("../MySystemProject/song");
        int i = 0;
        ui->listWidget->clear();
        while (1) {
            struct dirent* dirp = readdir(dir);

            if(dirp == NULL){
                break;
            }
            else if(dirp->d_type == DT_REG){
                ui->listWidget->addItem(new QListWidgetItem(dirp->d_name));
                strcpy(buf,dirp->d_name);
            }
            i++;
            }

        OpenFlag = 1;
            closedir(dir);
    });
    connect(ui->listWidget,&QListWidget::doubleClicked,[=]{


        char buff[128]= "loadfile ../MySystemProject/song/ ";
        QByteArray ba = ui->listWidget->currentItem()->text().toUtf8();
        strcpy(buf,ba.data());
        strcat(buff,buf);
        write(fd,buff,128);
        //printf("%s\n",buff);
       // fflush(stdout);

    });
    InitializeListFunction();

}

MainWindow::~MainWindow()
{
    delete ui;
}
//the song list initialize
void MainWindow::InitializeListFunction()
{
    DIR *dir = opendir("../MySystemProject/song");
    int i = 0;
    ui->listWidget->clear();
    while (1) {
        struct dirent* dirp = readdir(dir);

        if(dirp == NULL){
            break;
        }
        else if(dirp->d_type == DT_REG){
            ui->listWidget->addItem(new QListWidgetItem(dirp->d_name));
            strcpy(buf,dirp->d_name);
        }
        i++;
        }
    if(OpenFlag == 0){
        char buff[128] = "mplayer  -idle  -slave  -quiet ../MySystemProject/song/";
        strcat(buff,buf);
        write(fd,buff,128);
    }
    OpenFlag = 1;
    closedir(dir);
}

void MainWindow::MyCutSong()
{
   char buff[128] = "";

   int MyFd = open("../MySystemProject/lyrics",O_RDONLY);


}

// clicke the pause button
void MainWindow::MyClickedPlaying()
{
       write(fd,"pause",128);
}

//set the volume
void MainWindow::MyVolumeSet()
{
    int val = ui->huds->value();
    char buff1[128] ="volume ";
    char a[20];
    sprintf(a,"%d",val);
    strcat(buff1,a);
    write(fd,buff1,128);
}



