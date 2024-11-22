#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include <opencv2/opencv.hpp>

using namespace cv;

class NetworkManager 
{
public:
    NetworkManager();
    ~NetworkManager();

    void sendMetadata(); //send data to server
    void startStreaming(); //start rtp/rtsp
private:

};

#endif // NETWORKMANAGER_H
