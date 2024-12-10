#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <iostream>
#include <thread>
#include <cstdlib>
#include <csignal>
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>
#include <QDebug>

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

    // Inner Functions
    void configCodecParam(AVCodecContext* codec_ctx);
    void setFrame(AVFrame* frame, AVCodecContext* codec_ctx);
    void freeAllAV(AVFormatContext *output_ctx, AVFrame* frame, 
        uint8_t *buffer, AVCodecContext *codec_ctx);

public:
    // Singleton GetInstance()
    static NetworkManager& getInstance() {
        static NetworkManager instance;
        return instance;
    }

    void set_ip_addr(std::string new_addr);

    // Funciton to get IP Address
    std::string get_ip_addr();

    // Function to start MediaMTX server
    void startMediaMTX();

    // Function to stop MediaMTX server
    void stopMediaMTX();

    // Function for RTSP Streaming
    void rtsp_streaming(const std::string& rtsp_url);

};

// original main code
int test_main();

#endif // NETWORKMANAGER_H
