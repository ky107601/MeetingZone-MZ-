#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <opencv2/opencv.hpp>
#include <queue>
#include <atomic>
#include <condition_variable>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;
using namespace cv;


// 전역 변수
extern mutex queueMutex;                     // 큐 접근 보호를 위한 뮤텍스
extern condition_variable frameable;    // 프레임 추가 알림
extern atomic<bool> stop;

// 함수 선언
void handle_client(int client_sock, const std::string& ip);
void videothread(Mat& frame, queue<Mat>& frameQueue);
void display_all_client(map<string, queue<Mat>>& frameQueues);
void videoallplay(Mat& frame, const string& ip) ;
#endif // SERVER_H
