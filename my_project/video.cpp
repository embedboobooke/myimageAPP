#include "imageviewer.h"
#include <opencv2/opencv.hpp>

int ImageViewer::cameraCaptureClick()
{
    Mat edges;
    VideoCapture capture(0);

    if (!capture.isOpened())
        return -1;

    for (;;) {
        Mat frame;
        capture >> frame;
        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        //imshow("edges", edges);
        imageDisplay(edges);
        //imageDisplay(frame);
        //imshow("video", frame);
        if (waitKey(30) >= 0) break;
    }
}
