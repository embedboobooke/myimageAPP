#include "watershedSegmentation.h"
#include "imageviewer.h"

void ImageViewer::watershedClick()
{
    cv::Mat result;
    cv::Mat bgModel, fgModel;
    //cv::Rect rectangle(50, 70, image.cols-150, image.rows-180);
    cv::Rect rectangle2(10,100,380,180);

    cv::grabCut(image,    // input image
                    result,   // segmentation result
                    rectangle2,// rectangle containing foreground
                    bgModel,fgModel, // models
                    5,        // number of iterations
                    cv::GC_INIT_WITH_RECT); // use rectangle

    // Get the pixels marked as likely foreground
    cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
    // Generate output image
    cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
    //image.copyTo(foreground, result); // bg pixels not copied

    // draw rectangle on original image
    //cv::rectangle(image, rectangle, cv::Scalar(255,255,255), 1);

    //foreground.create(image.size(),CV_8UC3);
        //foreground.setTo(cv::Scalar(255,255,255));
        result = result & 1;
        image.copyTo(foreground, result); // bg pixels not copied
        //result = result & 1;

    imageDisplay(foreground);
}
