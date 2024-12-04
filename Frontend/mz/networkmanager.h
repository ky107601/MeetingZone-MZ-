#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <iostream>
#include <thread>
#include <cstdlib>
#include <csignal>
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>

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
#endif // NETWORKMANAGER_H
