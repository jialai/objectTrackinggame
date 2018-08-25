
#ifndef TRACKING_H
#define TRACKING_H

/*TO run this recognization function, The following should be added to mainwindow.cpp
 *

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        timer   = new QTimer(this);


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
        //Display time control
        timer->start(33);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::readFrame()
{

         cam.readFrameFromCam();
       ui->label->resize(cam.cameraFeed.cols,cam.cameraFeed.rows);
       ui->label->setPixmap(QPixmap::fromImage(cam.img));
}
*/
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************

/*The following should be added to mainwindow.h
 *
  private slots:

    void readFrame(); // to readFrame from camera

   private:
    QTimer  *timer; // to set up the time for readFrame
    Track cam; // the object of class Track
*/




//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
#include<QImage>
#include <QObject>
#include <sstream>
#include <string.h>
#include <iostream>
#include<stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include<vector>
#include <QDebug>
#include <QTime>
using namespace std;
using namespace cv;

class Track : public QObject
{
     Q_OBJECT
public:

        //initial min and max HSV filter values.
        //these will be changed using trackbars
        //
        int H_MIN;
        int H_MAX;
        int S_MIN;
        int S_MAX;
        int V_MIN;
        int V_MAX;
        int xpre[10000];
        int ypre[10000];
        //xnow and ynow values for the location of the object
        int xnow;
        int ynow;
        int i;
        bool flag;
        //default capture width and height
        const int FRAME_WIDTH;
        const int FRAME_HEIGHT;
        //max number of objects to be detected in frame
        const int MAX_NUM_OBJECTS;
        //minimum and maximum object area
        const int MIN_OBJECT_AREA;
        const int MAX_OBJECT_AREA;
        //names that will appear at the top of each window
        static String windowName;
        static String windowName1;
        static String windowName2;
        static String windowName3;
        static String trackbarWindowName;
        //Matrix to store each frame of the webcam feed
        //Mat cameraFeed;
        //matrix storage for HSV image
        //Mat HSV;
        //matrix storage for binary threshold image
        //Mat threshold;
        QImage    img;
        QImage    calimg;
        VideoCapture camera;
        Mat frame;
        //Matrix to store each frame of the webcam feed
        Mat cameraFeed;
        Mat dst;

        Track();
        ~Track();
        static void on_trackbar( int, void* );
        void createTrackbars();
        void drawObject(int x, int y,Mat &frame);
        void drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd, int len, int alpha,
                       cv::Scalar& color, int thickness, int lineType);
        double calAngle();
        void velocityVector( Mat &frame);
        void morphOps(Mat &thresh);
        void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);
        void readFrameFromCam();

        QImage cvMat2QImage(const cv::Mat& mat);
};

#endif // TRACKING_H


