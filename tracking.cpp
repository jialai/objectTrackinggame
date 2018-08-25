#include "tracking.h"、
//names that will appear at the top of each window
String Track::windowName = "Original Image";
String Track::windowName1= "HSV Image";
String Track::windowName2= "Thresholded Image";
String Track::windowName3= "After Morphological Operations";
String Track::trackbarWindowName= "Trackbars";
Track::Track():FRAME_WIDTH(640),FRAME_HEIGHT(480),MAX_NUM_OBJECTS(50),MIN_OBJECT_AREA(20*20),MAX_OBJECT_AREA(FRAME_HEIGHT*FRAME_WIDTH/1.5)
{
    H_MIN = 0;
    H_MAX = 256;
    S_MIN = 0;
    S_MAX = 256;
    V_MIN = 0;
    V_MAX = 256;
    xnow=0;
    ynow=0;
    i=0;
    flag=0;


}
 Track::~Track(){

}

void Track::on_trackbar( int, void* )
{//This function gets called whenever a
    // trackbar position is changed
}

void Track::createTrackbars(){
    //create window for trackbars
    namedWindow(trackbarWindowName,WINDOW_OPENGL);
    //create memory to store trackbar name on window
    char TrackbarName[50];
    sprintf( TrackbarName, "H_MIN", H_MIN);
    sprintf( TrackbarName, "H_MAX", H_MAX);
    sprintf( TrackbarName, "S_MIN", S_MIN);
    sprintf( TrackbarName, "S_MAX", S_MAX);
    sprintf( TrackbarName, "V_MIN", V_MIN);
    sprintf( TrackbarName, "V_MAX", V_MAX);
    //create trackbars and insert them into window
    //3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
    //the max value the trackbar can move (eg. H_HIGH),
    //and the function that is called whenever the trackbar is moved(eg. on_trackbar)
    //
     //                         ---->    ---->     ---->
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}
/*
//show time
double Track::timeCount(){
     double time1 = static_cast<double>(getTickCount());
     time1=((double)getTickCount()-time1)/getTickFrequency();
}*/

void Track::drawObject(int x, int y,Mat &frame){

    //use some of the openCV drawing functions to draw crosshairs
    //on your tracked image!

    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

    circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
        line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
        line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
        line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
        line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);
    std::stringstream ssx;
    ssx << x;
    String Sx=ssx.str();
    std::stringstream ssy;
    ssy<<y;
    String Sy=ssy.str();

   putText(frame, Sx+","+ Sy,Point(x,y+30),1,1,Scalar(0,255,0),2);
}

void Track::drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd, int len, int alpha,
               cv::Scalar& color, int thickness, int lineType)
{
    const double PI = 3.1415926;
    Point arrow;
    //Calculation of radian
    double angle = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));
    line(img, pStart, pEnd, color, thickness, lineType);
    //Calculate the endpoint position at the other end of the arrow's corner
    //(top or bottom depending on the arrow's direction, that is, the position of pStart and pEnd)
    arrow.x = pEnd.x + len * cos(angle + PI * alpha / 180);
    arrow.y = pEnd.y + len * sin(angle + PI * alpha / 180);
    line(img, pEnd, arrow, color, thickness, lineType);
    arrow.x = pEnd.x + len * cos(angle - PI * alpha / 180);
    arrow.y = pEnd.y + len * sin(angle - PI * alpha / 180);
    line(img, pEnd, arrow, color, thickness, lineType);
}

//Calculation of radian
double Track::calAngle()
{
    //Calculate the motion Angle between 10 frame, this number can be changed for more accurate
    double angle = atan2((double)(ypre[i-10]-ynow), (double)(xpre[i-10] - xnow));
    return angle;
}
//draw the velocity
void Track::velocityVector( Mat &frame){
    //Calculation of radian to represent direction of motion
    double a;
    a=calAngle();

    //end point position of arrow
    int xEnd=xnow-50*cos(a);
    int yEnd=ynow-50*sin(a);
    Point pStart(xnow, ynow), pEnd(xEnd, yEnd);
    Scalar lineColor(0,255,0);
    drawArrow(frame, pStart, pEnd, 10, 45, lineColor,2, CV_AA);
}

void Track::morphOps(Mat &thresh){

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

    erode(thresh,thresh,erodeElement);
    erode(thresh,thresh,erodeElement);


    dilate(thresh,thresh,dilateElement);
    dilate(thresh,thresh,dilateElement);
}

void Track::trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){

    Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
                    x = moment.m10/area;
                    y = moment.m01/area;
                    objectFound = true;
                    refArea = area;
                }else objectFound = false;


            }
            //let user know you found an object
            if(objectFound ==true){
                putText(cameraFeed,"Tracking Object",Point(0,30),2,1,Scalar(0,255,0),2);
                //putText(cameraFeed,"Time Count:", Point(0,100),2,1, Scalar(0,255,0),2);
                //draw object location on screen
                drawObject(x,y,cameraFeed);
                //i is used to be a counter of frame
                if(i>5)
                //draw the velocity
                velocityVector(cameraFeed);
                //update i
                if(i>1000)
                i=0;
            }

        }else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
    }
}
QImage Track::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate color indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        //qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        //qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
void Track::readFrameFromCam()
{

    xpre[i]=xnow;
    ypre[i]=ynow;
    i+=1;

    bool trackObjects = true;
    bool useMorphOps = true;
    //Matrix to store each frame of the webcam feed
    //Mat cameraFeed;
    //matrix storage for HSV image
    Mat HSV;
    //matrix storage for binary threshold image
    Mat threshold;

    //create slider bars for HSV filtering
   createTrackbars();

    //start an infinite loop where webcam feed is copied to cameraFeed matrix
    //all of our operations will be performed within this loop
    camera.read(cameraFeed);
    //flip
    cv::flip(cameraFeed, dst, 1);
    //convert frame from BGR to HSV colorspace
    cvtColor(dst,HSV,COLOR_BGR2HSV);
    //filter HSV image between values and store filtered image to
    //threshold matrix
    inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
    //perform morphological operations on thresholded image to eliminate noise
    //and emphasize the filtered object(s)
    if(useMorphOps)
    morphOps(threshold);
    //pass in thresholded frame to our object tracking function
    //this function will return the x and y coordinates of the
    //filtered object
    if(trackObjects)
         trackFilteredObject(xnow,ynow,threshold,dst);

    //show frames, the following are soem windows to help you adjust parameter
    putText(dst,"Start", Point(50,50),2,1, Scalar(0,255,0),2);
    putText(dst,"Finish", Point(500,450),2,1, Scalar(0,255,0),2);




     imshow(windowName2,threshold);
     imshow(windowName,dst);

    //if(flag){
            //createTrackbars();
            //imshow(trackbarWindowName,HSV);
            //}

            img = cvMat2QImage(dst);
            calimg = cvMat2QImage(threshold);

}

