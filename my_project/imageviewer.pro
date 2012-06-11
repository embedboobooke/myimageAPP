HEADERS = imageviewer.h \
    colordetector.h \
    histogram.h \
    colorhistogram.h \
    MatToQImage.h \
    watershedSegmentation.h
SOURCES = imageviewer.cpp \
    main.cpp \ # imageInverse.cpp
    imageConvert.cpp \
    imageProcess.cpp \
    colordetectorLab.cpp \
    colordetector.cpp \
    histogram.cpp \
    MatToQImage.cpp \
    video.cpp \
    watershed.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/imageviewer
sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    $$FORMS \
    imageviewer.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/imageviewer
INSTALLS += target \
    sources
symbian:include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
wince*::DEPLOYMENT_PLUGIN += qjpeg \
    qmng \
    qgif
INCLUDEPATH += /usr/local/include
LIBS += -L \
    /usr/local/lib \
    -lopencv_calib3d \
    -lopencv_contrib \
    -lopencv_core \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_gpu \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_legacy \
    -lopencv_ml \
    -lopencv_objdetect \
    -lopencv_video \
    #-lopencv_ffmpeg
