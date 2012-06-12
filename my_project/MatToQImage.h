/************************************************************************
** 将opencv图像封装格式转化为Qt图像封装格式
************************************************************************/

#ifndef MATTOQIMAGE_H
#define MATTOQIMAGE_H

#include "imageviewer.h"
// Qt header files
#include <QtGui>
#include <QDebug>
// OpenCV header files
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;

QImage MatToQImage(const Mat&);

#endif // MATTOQIMAGE_H
