#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QIcon>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QEvent>
#include <QHBoxLayout>
#include "camerastreamer.h"
#include "camviewer.h"

using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool micFlag = false; //mic status
    QIcon micOffIcon; //mic icon
    QIcon micOnIcon;

    bool videoFlag = false; //video status
    QIcon videoOffIcon; //video icon
    QIcon videoOnIcon;

    VideoCapture cap;
    QTimer *captureTimer;

    camViewer *cam; //QLabel
    string width, height;

public slots:
    void changeIcon();
    void updateFrame();

private:
    Ui::Widget *ui;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};
#endif // WIDGET_H
