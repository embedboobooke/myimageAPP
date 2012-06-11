#include "colordetector.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>

cv::Mat ColorDetector::process(const cv::Mat &image) {

          cv::Mat converted;

          /*
           * 数据大小同image，但是不拷贝原图像的机构体
           * 且图像的格式设置为单通道的灰度图像，与QT的显示密切相关
           */
          result.create(image.rows,image.cols,CV_8U);

          /*
           * 数据大小同image，但是不拷贝原图像的机构体，
           * 格式同原图像的格式,=image.type()
           */
          converted.create(image.rows,image.cols,image.type());
          qDebug()<<image.type();

          // 改变颜色至Lab color空间
          cv::cvtColor(image, converted, CV_BGR2Lab);

          qDebug()<<"the convert type is "<<converted.type();

          // 获取迭代
          cv::Mat_<cv::Vec3b>::iterator it= converted.begin<cv::Vec3b>();
          cv::Mat_<cv::Vec3b>::iterator itend= converted.end<cv::Vec3b>();
          cv::Mat_<uchar>::iterator itout= result.begin<uchar>();

          for ( ; it!= itend; ++it, ++itout) {

                // process each pixel ---------------------
                  // 根据目标图像计算颜色值距离
                  if (getDistance(*it)<minDist) {

                          *itout= 255;

                  } else {

                          *itout= 0;
                  }

        //end
          }

          return result;
}
