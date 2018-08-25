#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include"tracking.h"
#include "ui_dialog.h"
#include"dialog.h"
#include <QString>
#include<QTAlgorithms>
#include<vector>
typedef struct record{
        QTime time;   //record time
        QString name;  //user name

   }rec;

vector<rec> vec;  //vector to save name and time
rec score; //name and time
bool cmp(rec a, rec b){
    return (a.time<b.time);

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        position=false;

        ui->setupUi(this);
        timer   = new QTimer(this);
        this->init();
        connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));



        //cam is the object of class Track
        cam.camera.open(0);
        if (!cam.camera.isOpened())
        {
            std::cerr <<"Could not access the camera or video!"<<
            std::endl;
            exit(1);
        }
        cam.camera.set(CV_CAP_PROP_FRAME_WIDTH ,640);
        //Set resolution
        cam.camera.set(CV_CAP_PROP_FRAME_HEIGHT , 480);
        //Display time control*/
        timer->start(33);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->pTimer;
}


void MainWindow::readFrame()
{

       cam.readFrameFromCam();
       ui->label->resize(cam.dst.cols,cam.dst.rows);
       ui->label->setPixmap(QPixmap::fromImage(cam.img));
       //if object in start area, start counting

       if(cam.xnow<100&&cam.ynow<100&&position){
          QMessageBox::information(this, QString::fromLocal8Bit("warn"),QString::fromLocal8Bit("Are You Ready!"));
          //get current time
          this->baseTime = this->baseTime.currentTime();
          this->pTimer->start(1);
          position=false;
       }

       if(cam.xnow>100&&cam.ynow>100){
           position=true;
       }



}

//initial time count
void MainWindow::init()
{
    //default set
    this->ui->lcd_play->setText("00:00:00:000");

    this->pTimer = new QTimer;
    //connect to signal
    connect(this->pTimer,SIGNAL(timeout()),this,SLOT(updateDisplay()));

}

/*
//start
void MainWindow::on_btn_start_clicked()
{   //if object is not in start area,show warning message

    if(cam.xnow<200&&cam.ynow<200){
       QMessageBox::information(this, QString::fromLocal8Bit("warn"),QString::fromLocal8Bit("Are You Ready!"));
       //get current time
       this->baseTime = this->baseTime.currentTime();
       this->pTimer->start(1);
    }

    else{
        //get current time
        this->baseTime = this->baseTime.currentTime();
        this->pTimer->start(1);

        //reset status
        if(this->ui->btn_stop->text() != "stop")
        {
            this->ui->btn_stop->setText("stop");
        }
   }
}
*/
/*
//end
void MainWindow::on_btn_stop_clicked()
{
    if(this->ui->btn_stop->text() == "stop")
    {
        this->ui->btn_stop->setText("clear");
        this->pTimer->stop();
    }
    else if(this->ui->btn_stop->text() == "clear")
    {
        this->ui->btn_stop->setText("stop");
        this->ui->tb_display->clear();
        this->ui->lcd_play->setText("00:00:00:000");
    }

}*/

/*//pause
void MainWindow::on_btn_pause_clicked()
{
    static QTime pauseTime;
    if(this->ui->btn_pause->text() == "pause")
    {
        pauseTime = QTime::currentTime();
        this->ui->btn_pause->setText("continue");
        this->pTimer->stop();
    }
    else if(this->ui->btn_pause->text() == "continue")
    {
        //compute different
        QTime cut = QTime::currentTime();
        int t = pauseTime.msecsTo(cut);
        this->baseTime = this->baseTime.addMSecs(t);
        this->ui->btn_pause->setText("pause");
        this->pTimer->start();
    }
}
*/
//record time
void MainWindow::on_btn_point_clicked()
{
    QString timeShow;
    string stimeShow;
    timeShow=this->ui->lcd_play->text();
    stimeShow=timeShow.toStdString();
    if(stimeShow=="00:00:00:000"){
        QMessageBox::information(this, QString::fromLocal8Bit("warn"),QString::fromLocal8Bit("There is no time to record!"));
    }
    else if(cam.xnow<400&&cam.ynow<400){
        QMessageBox::information(this, QString::fromLocal8Bit("warn"),QString::fromLocal8Bit("There is no time to record!"));
    }
    else{
    this->pTimer->stop();
    this->ui->lcd_play->setText(timeStr);
    my = new Dialog(this);
    my->exec();

    //rank
    score.name = my->applySuccessful();
    score.time = timeToSave;
    vec.push_back(score);
    qSort(vec.begin(), vec.end(),cmp);
    //clear orginal record
    this->ui->tb_display->clear();


    //add timeStr to list
    this->ui->tb_display->setFontPointSize(48);

    //show rank result
    for(int i=0;i<vec.size();i++){
    //this->ui->tb_display->append(my->applySuccessful()+":         "+this->timeStr);
      this->ui->tb_display->append(vec[i].name+":    "+ vec[i].time.toString("hh:mm:ss:zzz"));
    }
    this->ui->lcd_play->setText("00:00:00:000");
    }
}

//updat time
void MainWindow::updateDisplay()
{
    /*
     * 1.press start button to get current time and assign this time to base time
     * 2.start time counting, 1s
     * 3.slot get currTime again
     * 4.calculate difference value between two time
     * 5.declare a showTime to object and add t to it
     * 6.The Settings are displayed after formatting
     */
    QTime currTime = QTime::currentTime();
    int t = this->baseTime.msecsTo(currTime);
    QTime showTime(0,0,0,0);
    showTime = showTime.addMSecs(t);
    timeToSave=showTime;
    this->timeStr = showTime.toString("hh:mm:ss:zzz");
    this->ui->lcd_play->setText(timeStr);

     //if object in stop area, stop counting
    if(cam.xnow>400&&cam.ynow>400)
    {

       this->pTimer->stop();


    }

}

/*void MainWindow::on_pushButton_clicked()
{
    createTrackbars();

    dialog = new Dialog(this);
    //dialog->setCaption("A QDialog Window");
    dialog->setMinimumSize(200,80);
    dialog->exec();


  delete dialog;
}*/


//clear record
void MainWindow::on_btn_clear_clicked()
{
    this->ui->tb_display->clear();
    this->ui->lcd_play->setText("00:00:00:000");
}

