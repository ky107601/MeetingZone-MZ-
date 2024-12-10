QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 01_TcpClient_Commented
TEMPLATE = app
INCLUDEPATH +=  /usr/local/include/opencv4/  

LIBS += -L/usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++17

SOURCES += \
        main.cpp \
        widget.cpp

HEADERS += \
        widget.h

FORMS += \
        widget.ui

# Include OpenCV
INCLUDEPATH += /usr/include/opencv4
LIBS += -L/usr/lib \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui \
        -lopencv_videoio \
        -lopencv_imgcodecs

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG   += link_pkgconfig
PKGCONFIG += opencv4
