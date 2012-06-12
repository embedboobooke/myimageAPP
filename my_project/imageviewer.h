/****************************************************************************
**  声明了共有继承类
**  项目来源江西省教育厅
**  本软件采用中英注释，个别地方中文注释实在有点晦涩，所以参考Qt用户手册的英语注释
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
    int  cameraCaptureClick();
    void watershedClick();
    void fourTraceClick();
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
    void FourAreaTrackPlasm(int minvalue, int maxvalue);

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
    int imageType;      //图停8位式通式
    int logoType;       //水印停=0:透谋冢=1mask式示影=2叶式示logo


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
    QAction *fourTraceAct;

    /*
     * video Act
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
