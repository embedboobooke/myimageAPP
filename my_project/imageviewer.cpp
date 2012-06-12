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

#include <QtGui>

#include "imageviewer.h"
#include "histogram.h"
#include "MatToQImage.h"
#include <QDebug>

//! [0]
ImageViewer::ImageViewer()
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);// 设置Label尽可能大(The widget will get as much space as possible.)
    imageLabel->setScaledContents(true); // if value is true,it will scale the pixmap to fill the available space

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel); // The imageLabel becomes a child of the scroll area,
                                       // and will be destroyed when the scroll area is deleted or when a new widget is set.
                                       // Sets scrollArea to be the main window's central widget
    setCentralWidget(scrollArea);

    createActions();
    createMenus();

    init();

    setWindowTitle(tr("Image Process")); // 窗口名要用中文，待解决中文乱码的问题
    resize(500, 400);
}
//! [0]

//! [1]

void ImageViewer::init()
{
    isUntitled = true;
    logoType = 0;          // 打水印的类型
}

void ImageViewer::open()
//! [1] //! [2]
{
    fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QImage qimage(fileName);     // C++的高级用法？经测试可以改为：QImage image = QImage(fileName)
        setCurrentFile(fileName);

        if (qimage.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
//! [2] //! [3]

        image = cv::imread(fileName.toAscii().data());    // imread()第二个参数Flags(>0:3 channels;<0:Return the loaded image as is
                                                          // ;=0:Return a grayscale image,函数第二个参数不写时，默认为<0)

        qDebug()<<"the channels is:"<<image.channels();
        qDebug()<<"the depth is:"<<image.depth();
        qDebug()<<"the image type is:"<<image.type();
/*
        if (image.channels() > 1)
            cv::cvtColor(image, image, CV_BGR2RGB);       // 只适用于原始image类型为多通道的,单通道的不行

        imageType = 0;
        img = QImage((const unsigned char*)(image.data),  // Qt image structure
                    image.cols, image.rows, QImage::Format_RGB888);
*/

        imageDisplay(image);
        dst = image.clone();
        //updateActions();

#if 0
        cv::cvtColor(image,image,CV_BGR2RGB);  // change color channel ordering
        QImage img= QImage((const unsigned char*)(image.data),  // Qt image structure
                image.cols,image.rows,QImage::Format_RGB888);

        /*
         * QPixmap::fromImage(image),Converts the given image to a pixmap using the specified
         * flags to control the conversion,In case of monochrome and 8-bit images, the image is
         * first converted to a 32-bit pixmap and then filled with the colors in the color table.
         */
        imageLabel->setPixmap(QPixmap::fromImage(img));   // Setting the pixmap clears any previous content.
                                                          // The buddy shortcut, if any, is disabled.

//! [3] //! [4]
        scaleFactor = 1.0;

        printAct->setEnabled(true);         // 激活打印菜单
        fitToWindowAct->setEnabled(true);   // 激活fitToWindow菜单
        updateActions();                    // 激活相关的子菜单项

        if (!fitToWindowAct->isChecked())   // fitToWindow菜单是否按下？
            imageLabel->adjustSize();       // Adjusts the size of the widget to fit the contents(调整是imageLabel的大小)
#endif
    }
}

bool ImageViewer::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool ImageViewer::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void ImageViewer::imageDisplay(cv::Mat &image)
{
    //cv::cvtColor(image,image,CV_BGR2RGB);  // change color channel ordering
    //QImage img= QImage((const unsigned char*)(image.data),  // Qt image structure
            //image.cols,image.rows,QImage::Format_RGB888);

    //img= QImage((const unsigned char*)(image.data),  // Qt image structure
            //image.cols,image.rows,QImage::Format_RGB888);

    /*
     * QPixmap::fromImage(image),Converts the given image to a pixmap using the specified
     * flags to control the conversion,In case of monochrome and 8-bit images, the image is
     * first converted to a 32-bit pixmap and then filled with the colors in the color table.
     */
    imageLabel->setPixmap(QPixmap::fromImage(MatToQImage(image), Qt::AutoColor)); //Setting the pixmap clears any previous content.
                                                      //The buddy shortcut, if any, is disabled.

//! [3] //! [4]
    scaleFactor = 1.0;

    printAct->setEnabled(true);         // 激活打印菜单
    fitToWindowAct->setEnabled(true);   // 激活fitToWindow菜单
    updateActions();                    // 激活相关的子菜单项

    if (!fitToWindowAct->isChecked())   // fitToWindow菜单是否按下？
        imageLabel->adjustSize();       // Adjusts the size of the widget to fit the contents(调整是imageLabel的大小)
}

//! [4]

//! [5]
void ImageViewer::print()
//! [5] //! [6]
{
    Q_ASSERT(imageLabel->pixmap());
#ifndef QT_NO_PRINTER
//! [6] //! [7]
    QPrintDialog dialog(&printer, this);
//! [7] //! [8]
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}
//! [8]

//! [9]
void ImageViewer::zoomIn()
//! [9] //! [10]
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

//! [10] //! [11]
void ImageViewer::normalSize()
//! [11] //! [12]
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}
//! [12]

//! [13]
void ImageViewer::fitToWindow()
//! [13] //! [14]
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}
//! [14]


//! [15]

void ImageViewer::addSaltClick()
{
    salt(image, 30000);

    if (!imageType)
        img= QImage((const unsigned char*)(image.data),  // Qt image structure
                image.cols,image.rows,QImage::Format_RGB888);
    else
        img= QImage((const unsigned char*)(image.data),  // Qt image structure
                image.cols,image.rows,QImage::Format_Indexed8);

    imageDisplay(image);
    dst = image.clone();
}

#if 0
void ImageViewer::colorToGray()
{
    cv::cvtColor(image, image, CV_BGR2GRAY);

    img= QImage((const unsigned char*)(image.data),  // Qt image structure
                image.cols,image.rows,QImage::Format_Indexed8);

    imageDisplay();
    // cv::namedWindow("Image 2");
    // cv::imshow("Image 2",image);
}
#endif

void ImageViewer::colorToGrayClick()
{
    gray(image);

    if(image.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)image.data;
        // Create QImage with same dimensions as input Mat
        img = QImage(qImageBuffer, image.cols, image.rows, image.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(image.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)image.data;
        // Create QImage with same dimensions as input Mat
        img = QImage(qImageBuffer, image.cols, image.rows, image.step, QImage::Format_RGB888);
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
    }

#if 0
    qDebug()<<"the channels of this gray image is:"<<image.channels();
    qDebug()<<"the depth of this gray image is:"<<image.depth();
    qDebug()<<"the type of this gray image is:"<<image.type();

    //cv::imshow("grayImage", imggray);
    //cv::waitKey();

    // imageType = QImage::Format_Indexed8;
    img= QImage((const unsigned char*)(image.data),  // Qt image structure
                image.cols,image.rows,QImage::Format_Indexed8);

    imageType = 1;
#endif

    imageDisplay(image);
    qDebug() <<"the step of image is:" <<*(image.data);

    dst = image.clone();

    //cv::namedWindow("Image 2");
    //cv::imshow("Image 2",image);
}

void ImageViewer::gray(cv::Mat &image)
{
    //imggray.create(image.rows, image.cols, CV_8U);
    cvtColor(image, image, CV_RGB2GRAY);
}

void ImageViewer::colorToHSVClick()
{
    cv::cvtColor(image, image, CV_BGR2HSV);
    imageDisplay(image);
}

void ImageViewer::salt(cv::Mat &image, int n)
{
    int i,j;

    srand(cv::getTickCount());

    for (int k=0; k<n; k++) {
        // rand() is the MFC random number generator
        i= rand()%image.cols;
        j= rand()%image.rows;

        if (image.channels() == 1) { // gray-level image
            image.at<uchar>(j,i)= 255;
        } else if (image.channels() == 3) { // color image

            image.at<cv::Vec3b>(j,i)[0]= 255;
            image.at<cv::Vec3b>(j,i)[1]= 255;
            image.at<cv::Vec3b>(j,i)[2]= 255;
        }
    }
}

/* saveAS时不能保存灰度后的图像
 * 但是加椒盐处理后的图像是可以保存的，程序存在Bug,有待修改
 */
bool ImageViewer::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Save"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    img.save(fileName, "bmp");

    /************************************************
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    ************************************************/
    setCurrentFile(fileName);
    //statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void ImageViewer::setCurrentFile(const QString &fileName)
{
    static int sequenceNumber = 1;

    isUntitled = fileName.isEmpty();
    if (isUntitled) {
        curFile = tr("test.bmp").arg(sequenceNumber++);
    } else {
        curFile = QFileInfo(fileName).canonicalFilePath();
    }
}

void ImageViewer::about()
//! [15] //! [16]
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
}
//! [16]

//! [17]
void ImageViewer::createActions()
//! [17] //! [18]
{
    openAct = new QAction(tr("&打开..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save..."), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("&Save As..."), this);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setEnabled(false);
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    addSaltAct = new QAction(tr("&add Salt"), this);
    addSaltAct->setEnabled(false);
    addSaltAct->setCheckable(true);
    addSaltAct->setShortcut(tr("++"));
    connect(addSaltAct, SIGNAL(triggered()), this, SLOT(addSaltClick()));

    colorToGrayAct = new QAction(tr("&colorToGray"), this);
    colorToGrayAct->setEnabled(false);
    colorToGrayAct->setCheckable(true);
    colorToGrayAct->setShortcut(tr("Ctrl+P"));
    connect(colorToGrayAct, SIGNAL(triggered()), this, SLOT(colorToGrayClick()));

    colorToHSVAct = new QAction(tr("&HSV"), this);
    colorToHSVAct->setEnabled(false);
    colorToHSVAct->setCheckable(true);
    connect(colorToHSVAct, SIGNAL(triggered()), this, SLOT(colorToHSVClick()));

    rotationAct = new QAction(tr("&Rotation"), this);
    rotationAct->setEnabled(false);
    rotationAct->setCheckable(true);
    //rotationAct->setShortcut(tr("Ctrl+P"));
    connect(rotationAct, SIGNAL(triggered()), this, SLOT(imageRotationClick()));

    horizontalrAct = new QAction(tr("&Hmirror"), this);
    horizontalrAct->setEnabled(false);
    horizontalrAct->setCheckable(true);
    //rotationAct->setShortcut(tr("Ctrl+P"));
    connect(horizontalrAct, SIGNAL(triggered()), this, SLOT(horizontalMirrorClick()));

    verticalAct = new QAction(tr("&Vmirror"), this);
    verticalAct->setEnabled(false);
    verticalAct->setCheckable(true);
    //rotationAct->setShortcut(tr("Ctrl+P"));
    connect(verticalAct, SIGNAL(triggered()), this, SLOT(verticalMirrorClick()));

    addImageAct = new QAction(tr("&Add Image"), this);
    addImageAct->setEnabled(false);
    addImageAct->setCheckable(true);
    //rotationAct->setShortcut(tr("Ctrl+P"));
    connect(addImageAct, SIGNAL(triggered()), this, SLOT(addImageClick()));

    colorReduceAct = new QAction(tr("&Color Reduce"), this);
    colorReduceAct->setEnabled(false);
    colorReduceAct->setCheckable(true);
    connect(colorReduceAct, SIGNAL(triggered()), this, SLOT(colorReduceClick()));

    sharpenAct = new QAction(tr("&Image Sharpen"), this);
    sharpenAct->setEnabled(false);
    sharpenAct->setCheckable(true);
    connect(sharpenAct, SIGNAL(triggered()), this, SLOT(sharpenClick()));

    colordetectorAct = new QAction(tr("&Image Detect"), this);
    colordetectorAct->setEnabled(false);
    colordetectorAct->setCheckable(true);
    connect(colordetectorAct, SIGNAL(triggered()), this, SLOT(colordetectorClick()));

    histogramAct = new QAction(tr("&Histogram"), this);
    histogramAct->setEnabled(false);
    histogramAct->setCheckable(true);
    connect(histogramAct, SIGNAL(triggered()), this, SLOT(histogramClick()));

    thresholdAct = new QAction(tr("&Threshold"), this);
    thresholdAct->setEnabled(false);
    thresholdAct->setCheckable(true);
    connect(thresholdAct, SIGNAL(triggered()), this, SLOT(thresholdClick()));

    equalizeAct = new QAction(tr("&Equalize"), this);
    equalizeAct->setEnabled(false);
    equalizeAct->setCheckable(true);
    connect(equalizeAct, SIGNAL(triggered()), this, SLOT(equalizeClick()));

    equalizeHistogramAct = new QAction(tr("&EqualizeHistogram"), this);
    equalizeHistogramAct->setEnabled(false);
    equalizeHistogramAct->setCheckable(true);
    connect(equalizeHistogramAct, SIGNAL(triggered()), this, SLOT(equalizeHistogramClick()));

    stretchAct = new QAction(tr("Stretch"), this);
    stretchAct->setEnabled(false);
    stretchAct->setCheckable(true);
    connect(stretchAct, SIGNAL(triggered()), this, SLOT(stretchClick()));

    stretchHistogramAct = new QAction(tr("StretchHistogram"), this);
    stretchHistogramAct->setEnabled(false);
    stretchHistogramAct->setCheckable(true);
    connect(stretchHistogramAct, SIGNAL(triggered()), this, SLOT(stretchHistogramClick()));

    fourTraceAct = new QAction(tr("FourTrace"), this);
    fourTraceAct->setEnabled(false);
    fourTraceAct->setCheckable(true);
    connect(fourTraceAct, SIGNAL(triggered()), this, SLOT(fourTraceClick()));

    cameraCaptureAct = new QAction(tr("Camera"), this);
    //cameraCaptureAct->setEnabled(false);
    //cameraCaptureAct->setCheckable(true);
    connect(cameraCaptureAct, SIGNAL(triggered()), this, SLOT(cameraCaptureClick()));

    watershedAct = new QAction(tr("Watershed"), this);
    connect(watershedAct, SIGNAL(triggered()), this, SLOT(watershedClick()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}
//! [18]

//! [19]
void ImageViewer::createMenus()
//! [19] //! [20]
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    imageMenu = new QMenu(tr("&Image"), this);
    imageMenu->addAction(addSaltAct);
    imageMenu->addAction(colorToGrayAct);
    imageMenu->addAction(colorToHSVAct);
    imageMenu->addAction(rotationAct);
    imageMenu->addSeparator();
    mirrorMenu = imageMenu->addMenu(tr("&Mirror"));
    mirrorMenu->addAction(horizontalrAct);
    mirrorMenu->addAction(verticalAct);
    imageMenu->addAction(addImageAct);

    imageProcessMenu = new QMenu(tr("&ImageProcess"), this);
    imageProcessMenu->addAction(colorReduceAct);
    imageProcessMenu->addAction(sharpenAct);
    imageProcessMenu->addAction(colordetectorAct);
    imageProcessMenu->addAction(histogramAct);
    imageProcessMenu->addAction(thresholdAct);
    imageProcessMenu->addAction(equalizeAct);
    imageProcessMenu->addAction(equalizeHistogramAct);
    imageProcessMenu->addAction(stretchAct);
    imageProcessMenu->addAction(stretchHistogramAct);
    imageProcessMenu->addAction(fourTraceAct);

    videoMenu = new QMenu(tr("&Video"), this);
    videoMenu->addAction(cameraCaptureAct);
    videoMenu->addAction(watershedAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(imageMenu);
    menuBar()->addMenu(imageProcessMenu);
    menuBar()->addMenu(videoMenu);
    menuBar()->addMenu(helpMenu);
}
//! [20]

//! [21]
void ImageViewer::updateActions()
//! [21] //! [22]
{
    saveAct->setEnabled(!fitToWindowAct->isChecked());
    saveAsAct->setEnabled(!fitToWindowAct->isChecked());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
    addSaltAct->setEnabled(!fitToWindowAct->isChecked());
    colorToGrayAct->setEnabled(!fitToWindowAct->isChecked());
    colorToHSVAct->setEnabled(!fitToWindowAct->isChecked());
    rotationAct->setEnabled(!fitToWindowAct->isChecked());
    horizontalrAct->setEnabled(!fitToWindowAct->isChecked());
    verticalAct->setEnabled(!fitToWindowAct->isChecked());
    addImageAct->setEnabled(!fitToWindowAct->isChecked());

    colorReduceAct->setEnabled(!fitToWindowAct->isChecked());
    sharpenAct->setEnabled(!fitToWindowAct->isChecked());
    colordetectorAct->setEnabled(!fitToWindowAct->isChecked());
    histogramAct->setEnabled(!fitToWindowAct->isChecked());
    thresholdAct->setEnabled(!fitToWindowAct->isChecked());
    equalizeAct->setEnabled(!fitToWindowAct->isChecked());
    equalizeHistogramAct->setEnabled(!fitToWindowAct->isChecked());
    stretchAct->setEnabled(!fitToWindowAct->isChecked());
    stretchHistogramAct->setEnabled(!fitToWindowAct->isChecked());
    fourTraceAct->setEnabled(!fitToWindowAct->isChecked());

    cameraCaptureAct->setEnabled(!fitToWindowAct->isChecked());
}
//! [22]

//! [23]
void ImageViewer::scaleImage(double factor)
//! [23] //! [24]
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}
//! [24]

//! [25]
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
//! [26]
