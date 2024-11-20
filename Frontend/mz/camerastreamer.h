#ifndef CAMERASTREAMER_H
#define CAMERASTREAMER_H

#include <QWidget>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <QDebug>

using namespace cv;

class CameraStreamer : public QWidget
{
    Q_OBJECT
public:
    CameraStreamer(QWidget *parent = nullptr);
    ~CameraStreamer();

    VideoCapture cap;  // OpenCV VideoCapture with GStreamer pipeline
    QTimer *captureTimer;  // Timer for frame capture

public slots:
    void startStreaming();
    void captureFrame();


};

#endif // CAMERASTREAMER_H
