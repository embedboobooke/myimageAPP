#include <QtGui>
#include "imageviewer.h"
//#include "MatToQImage.h"

void ImageViewer::colorReduce(cv::Mat &image, int div)
{
    int i, j;
    int nl = image.rows;        // 图像行数
    int nc = image.cols * image.channels();      // 每行图像的元素总数

    for (j = 0; j < nl; j++) {
        unsigned char *data = image.ptr<uchar>(j);      // get the address of row j
        for (i = 0; i < nc; i++) {
            data[i] = data[i]/div*div + div/2;          // end of pixel processing
        }//end of line
    }
}

void ImageViewer::colorReduceClick()
{
    colorReduce(image, 64);
/*
    if (imageType)
        img= QImage((const unsigned char *)(image.data),  // Qt image structure
                image.cols, image.rows, QImage::Format_Indexed8);
    else
        img= QImage((const unsigned char *)(image.data),  // Qt image structure
                image.cols, image.rows, QImage::Format_RGB888);
*/

    imageDisplay(image);
}

void ImageViewer::sharpen(const cv::Mat image, cv::Mat result)
{
    int i, j;

    // result.create(image.size(), image.type());
    for (j = 1; j < image.rows - 1; j++) {    // for all row,except first and last
        const uchar *previous = image.ptr<const uchar>(j-1);    // previous row
        const uchar *current = image.ptr<const uchar>(j);       // current row
        const uchar *next = image.ptr<const uchar>(j+1);        // next row

        uchar *output = result.ptr<uchar>(j);              // output row
        for (i = 1; i < image.cols - 1; i++) {
            *output++ = cv::saturate_cast<uchar>(5*current[i] - current[i-1]
                                                 - current[i+1] - previous[i] - next[i]);
        }
    }

    /*
     * set the unprocess pixels to 0
     */
    result.row(0).setTo(cv::Scalar(0));
    result.row(result.rows-1).setTo(cv::Scalar(0));
    result.col(0).setTo(cv::Scalar(0));
    result.col(result.cols-1).setTo(cv::Scalar(0));
}

void ImageViewer::sharpenClick()
{
    cv::Mat result;

    switch(image.channels())
    {
        case 1:             // 锐化只支持灰度图
            result.create(image.size(),image.type());
            sharpen(image, image);
/*
            if (imageType)
                img= QImage((const unsigned char *)(result.data),  // Qt image structure
                        result.cols, result.rows, QImage::Format_Indexed8);
            else
                img= QImage((const unsigned char *)(result.data),  // Qt image structure
                        result.cols, result.rows, QImage::Format_RGB888);
*/

            imageDisplay(image);
            break;

        case 3:             // 不支持真彩图
            QMessageBox::information(this, tr("warning"),
                                     tr("Cannot support color image").arg(fileName));
            break;

        default:
            break;
    }
}
