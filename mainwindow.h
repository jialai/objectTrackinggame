#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dialog.h"
#include<QMainWindow>
#include<opencv2/opencv.hpp>
#include<QTimer>
#include<QImage>
#include"tracking.h"
#include <QDateTime>
#include <QTime>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>

using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();







private slots:

    void readFrame();
    void updateDisplay();
    //void on_btn_start_clicked();
    //void on_btn_stop_clicked();
   // void on_btn_pause_clicked();
    void on_btn_point_clicked();


    void on_btn_clear_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTime timeToSave;
    Track cam;
    QTimer *pTimer;
    QTime  baseTime;
     //show time
    QString timeStr;
    //QImage    img;
    //QImage    calimg;
    //VideoCapture camera;
    //Mat frame;

    Dialog *my;
    //used to control when the start function will be triggered
    //if the object in start position, the value is false, otherwise, the value is true
    bool position;





};

#endif // MAINWINDOW_H
