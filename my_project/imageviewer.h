/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QPrinter>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "colordetector.h"

using namespace cv;

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

//! [0]
class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    //cv::Mat image;
    ImageViewer();
    //void imageDisplay(cv::Mat& tempImage);

private slots:
    void open();
    bool save();
    bool saveAs();
    void print();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void addSaltClick();
    void colorToGrayClick();
    void colorToHSVClick();
    void imageRotationClick();
    void horizontalMirrorClick();
    void verticalMirrorClick();
    void addImageClick();
    void colorReduceClick();
    void sharpenClick();
    void colordetectorClick();
    void histogramClick();
    void thresholdClick();
    void equalizeClick();
    void equalizeHistogramClick();
    void stretchClick();
    void stretchHistogramClick();
    int cameraCaptureClick();
    void watershedClick();
    void about();

private:
    void init();
    void imageDisplay(cv::Mat &image);
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void salt(cv::Mat &image, int n);
    void gray(cv::Mat &image1);
#if 0
    void rotation(cv::Mat &image, double angle);
#endif
    void colorReduce(cv::Mat &image, int div);
    void sharpen(cv::Mat image, cv::Mat result);

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    cv::Mat image;
    QString fileName;
    QImage img;
    double scaleFactor;
    bool isUntitled;
    cv::Mat dst;
    //cv::Mat imggray;
    ColorDetector cdetect;
    QString curFile;
    int imageType;      //ͼͣ8λʽͨʽ
    int logoType;       //ˮӡͣ=0:͸ıڣ=1maskʽʾӰ=2Ҷʽʾlogo


#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif
    /*
     * file Act
     */
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *printAct;
    QAction *exitAct;

    /*
     * view Act
     */
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;

    /*
     * image Process Act
     */
    QAction *addSaltAct;
    QAction *colorToGrayAct;
    QAction *colorToHSVAct;
    QAction *rotationAct;
    QAction *horizontalrAct;
    QAction *verticalAct;
    QAction *addImageAct;
    QAction *colorReduceAct;
    QAction *sharpenAct;
    QAction *colordetectorAct;
    QAction *histogramAct;
    QAction *thresholdAct;
    QAction *equalizeAct;
    QAction *equalizeHistogramAct;
    QAction *stretchAct;
    QAction *stretchHistogramAct;

    /*
     * vieo Act
     */
    QAction *cameraCaptureAct;
    QAction *watershedAct;

    /*
     * help Act
     */
    QAction *aboutAct;
    QAction *aboutQtAct;

    /*
     * mianWindow Menu
     */
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *imageMenu;
    QMenu *imageProcessMenu;
    QMenu *mirrorMenu;
    QMenu *videoMenu;
    QMenu *helpMenu;

};
//! [0]

#endif
