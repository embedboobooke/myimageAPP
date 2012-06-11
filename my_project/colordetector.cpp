#include "imageviewer.h"
#include <QtGui>

void ImageViewer::colordetectorClick()
{
    //cv::Mat detectorM;

    cdetect.setTargetColor(130, 190, 230); // 参数体现蓝色天空的颜色

    if (image.channels() < 3) {

        QMessageBox::information(this, tr("warning"),
                                 tr("Cannot support grayscale image!!!").arg(fileName));
        return;
    }
    image = cdetect.process(image);

    // cv::namedWindow("detect");
    // cv::imshow("result", detectorM);
    // cv::waitKey();

    /*
     * 在colordetectorLab中已经设置图像的通道格式为灰度(单通道格式)
     * 所以在QT中显示的时候设置为：Format_Indexed8，方可正常显示
     */
    //img= QImage((const unsigned char *)(detectorM.data),       // Qt image structure
                //detectorM.cols, detectorM.rows, QImage::Format_Indexed8);
    // else
        //img= QImage((const unsigned char *)(detectorM.data),  // Qt image structure
                //detectorM.cols, detectorM.rows, QImage::Format_RGB888);
    imageDisplay(image);
}
