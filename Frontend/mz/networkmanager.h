#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QLineEdit>
#include <QTimer>
#include <opencv2/opencv.hpp>

using namespace cv;

class NetworkManager : public QWidget
{
    Q_OBJECT
public:
    NetworkManager(QWidget *parent = nullptr);
    ~NetworkManager();

    void sendMetadata(); //send data to server
    void startStreaming(); //start rtp/rtsp
public slots:

private:

};

#endif // NETWORKMANAGER_H
