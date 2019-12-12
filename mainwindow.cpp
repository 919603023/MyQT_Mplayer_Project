#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OpenFlag = 0;
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
 //       printf("%s\n",buff1);
 //       fflush(stdout);
        write(fd,buff1,128);
    });
//    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(MyGetItemText(int row, int column)));
//    char buff1[128] = "loadfile ../MySystemProject/song/";
//    QTableWidgetItem *username = new QTableWidgetItem;
//        username = ui->tableWidget->item(row,0);

//        QByteArray sr =  username->text().toLocal8Bit();
//        int len = sr.length();
//        char* buf1 = new char[len+2];
//        buf[len] = buf[len+1] = 0;
//        // QByteArray转char*
//        strcpy(buf,sr.data());
//    strcpy(buf,buf1);
//    strcat(buff1,buf);
//    write(fd,buff1,128);

       //当改变选值框的值时，同时进度条也改变位置
       void (QSpinBox::*mysignal)(int) = &QSpinBox::valueChanged;
       connect(ui->spinBox_huds,mysignal, ui->huds, &QSlider::setValue);

    bzero(buf,sizeof(buf));
    connect(ui->pushButton_pause,SIGNAL(clicked()),this,SLOT(MyClickedPlaying()));


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
       qDebug()<< ui->listWidget->currentItem()->text();
    });
    InitializeListFunction();
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

void MainWindow::MyClickedPlaying()
{


       write(fd,"pause\n",128);

}
void MainWindow::MyVolumeSet()
{
    int val = ui->huds->value();
    char buff1[128] ="volume ";
    char a[20];
    sprintf(a,"%d",val);
    strcat(buff1,a);
    write(fd,buff1,128);
}

void MainWindow::MyGetItemText(int row, int column)
{

}

