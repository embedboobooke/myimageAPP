/************************************************************************
** 将opencv图像封装格式转化为Qt图像封装格式
** 将opencv格式的图像显示在Qt容器中，必须把其
** 格式转换为Qt支持的图像数据类型----QImage
************************************************************************/

#include "MatToQImage.h"

QImage MatToQImage(const Mat& image)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(image.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)image.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, image.cols, image.rows, image.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(image.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)image.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, image.cols, image.rows, image.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
} // MatToQImage()
