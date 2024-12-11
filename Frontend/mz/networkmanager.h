#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <iostream>
#include <thread>
#include <cstdlib>
#include <csignal>
#include <chrono>
#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/core/mat.hpp>

extern "C" {
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
#include <libavutil/time.h>
}

class NetworkManager {
private:
    // Singleton
    NetworkManager() = default;
    ~NetworkManager() = default;
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    // Inner Field
    std::string ip_address = "";   // Server IP 주소 문자열

    int width = 640;
    int height = 480;
    int frameRate = 10;

    AVFormatContext* output_ctx = nullptr;
    AVStream* video_stream = nullptr;
    AVCodecContext* codec_ctx = nullptr;
    AVFrame* frame = nullptr;
    uint8_t* buffer = nullptr;
    SwsContext* sws_ctx = nullptr;

    cv::VideoCapture cap;

    // Inner Functions
    void configCodecParam();
    void setFrame();
    void freeAllAV();
    void openCamera();
    void sendImages();
    void updateImage(cv::Mat& image, int imageCounter);
    //void sendXY(QByteArray data);

public:
    // Singleton GetInstance()
    static NetworkManager& getInstance() {
        static NetworkManager instance;
        return instance;
    }

    void set_ip_addr(std::string new_addr);

    // Funciton to get IP Address
    std::string get_ip_addr();

    // Function to start/stop MediaMTX server
    void startMediaMTX();
    void stopMediaMTX();

    // Function for RTSP Streaming
    void startRTSP(const std::string& rtsp_url);
    void stopRTSP();
};

#endif // NETWORKMANAGER_H
