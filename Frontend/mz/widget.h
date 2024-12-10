#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QIcon>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QEvent>
#include <QString>
#include <QByteArray>
#include <QHBoxLayout>
#include "camviewer.h"
#include "backgroundpicture.h"
#include <libavcodec/ac3_parser.h>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QFileDialog>
#include <QAbstractSocket>
#include <QtConcurrent>
<<<<<<< HEAD

#include "networkmanager.h"
=======
>>>>>>> 05136ac (clframe write & read)

#include "networkmanager.h"

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

    backgroundPicture picture;
    QLabel *chatBox;
<<<<<<< HEAD

    Mat recFrame;
=======
>>>>>>> 05136ac (clframe write & read)

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void connectServer();
    void changeIcon();
    void updateFrame();
    void selectPicture();

private:
    Ui::Widget *ui;
    NetworkManager& networkManager = NetworkManager::getInstance();
    const std::string rtsp_url;

    QLabel *videoWindow; //비디오 출력 창
    QTcpSocket *tcpSocket; //클라이언트 소켓

    // Widget()
    void startStreaming();
    void setUI();
    string& setPipeline();
    void openCamera(string& pipeline);
    void setTimerForFrame();
    void setToggleVideo();
    void setBackground();

    // updateFrame()
    Mat& captureNewFrame();
    void getVideo();
    void sendVideo();
};
#endif // WIDGET_H
