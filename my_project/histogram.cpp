#include "histogram.h"
#include "imageviewer.h"
#include <QDebug>
#include <QtGui>

cv::Mat thresholded;
cv::Mat equalize;
cv::Mat stretch;
Histogram1D h;

void ImageViewer::histogramClick()
{
    //cv::Mat thresholded;

    //cv::Mat eq= h.equalize(image);
    thresholded = image;

    if (image.channels() > 1) {

        QMessageBox::information(this, tr("warning"),
                     tr("Cannot support color image").arg(fileName));
        return;
    }

    cv::MatND histo= h.getHistogram(image);

    for (int i = 0; i < 256; i++)
        qDebug() << "Value " << i << " = " << histo.at<float>(i) << endl;

    image = h.getHistogramImage(image);

    imageDisplay(image);

#if 0

    //thresholded.create(image.rows,image.cols,image.type());
    //image = cv::imread("test.bmp");
    cv::threshold(image,image,60,255,cv::THRESH_BINARY);
/*
    if (imageType)
        img = QImage((const unsigned char *)(image.data),  // Qt image structure
                image.cols, image.rows, QImage::Format_Indexed8);
    else
        img = QImage((const unsigned char *)(thresholded.data),  // Qt image structure
                thresholded.cols, thresholded.rows, QImage::Format_RGB888);
*/
    imageDisplay();

#endif
}

void ImageViewer::thresholdClick()
{
    cv::threshold(image, thresholded, 60,255, cv::THRESH_BINARY);
    imageDisplay(thresholded);
}

void ImageViewer::equalizeClick()
{

    equalize = h.equalize(image);
    imageDisplay(equalize);
}

void ImageViewer::equalizeHistogramClick()
{
    cv::Mat eq = h.getHistogramImage(equalize);
    imageDisplay(eq);
}

void ImageViewer::stretchClick()
{
    // Stretch the image ignoring bins with less than 5 pixels
    stretch = h.stretch(image, 5);
    imageDisplay(stretch);
}

void ImageViewer::stretchHistogramClick()
{
    cv::Mat str = h.getHistogramImage(stretch);
    imageDisplay(str);
}
