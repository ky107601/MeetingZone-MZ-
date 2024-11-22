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
#include <condition_variable>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;
using namespace cv;


// 전역 변수
extern mutex queueMutex;                     // 큐 접근 보호를 위한 뮤텍스
extern condition_variable frameAvailable;    // 프레임 추가 알림
extern bool stopThreads;     

// 함수 선언
void handle_client(int client_sock);
void videoCaptureThread(const string& video, queue<Mat>& frameQueue);  // 동영상 캡처 스레드
void displayFrames(vector<queue<Mat>>& frameQueues, const vector<string>& windowNames); // 프레임 출력
void videoplay();
#endif // SERVER_H
