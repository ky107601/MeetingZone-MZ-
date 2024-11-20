#include "camerastreamer.h"

CameraStreamer::CameraStreamer(QWidget *parent) : QWidget(parent), cap()
{
    // GStreamer 파이프라인 초기화
    std::string pipeline =
        "libcamerasrc camera-name=/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36 "
        "! video/x-raw,width=640,height=480,framerate=10/1,format=RGBx "
        "! videoconvert ! videoscale ! appsink";

    if (!cap.open(pipeline, cv::CAP_GSTREAMER)) {
        throw std::runtime_error("Failed to open the camera with GStreamer pipeline.");
    }
}

CameraStreamer::~CameraStreamer() {
    if (cap.isOpened()) {
        cap.release();
    }
}

void CameraStreamer::startStreaming() {
    if (!captureTimer->isActive()) {
        captureTimer->start(100); // 약 10fps
    }
}

void CameraStreamer::captureFrame()
{
    Mat frame;
    cap >> frame;

    if (!frame.empty()) {
        // OpenCV에서 프레임 변환
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

        // QLabel에 프레임 표시
       // videoLabel->setPixmap(QPixmap::fromImage(image).scaled(videoLabel->size(), Qt::KeepAspectRatio));
    } else {
        qDebug() << "Empty frame captured!";
    }
}
