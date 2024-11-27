#include "server.h"

mutex queueMutex;
condition_variable frameable;
atomic<bool> stop = false;

// 각 클라이언트의 프레임 데이터를 수신하여 큐에 추가(데이터 독립 관리)
void videothread(Mat& frame, queue<Mat>& frameQueue) {
    while (!stop.load()) {
        if (!frame.empty()) {
            Mat resizedFrame;
            resize(frame, resizedFrame, Size(320, 240), 0, 0, INTER_CUBIC); // 크기 조정     
            lock_guard<mutex> lock(queueMutex);         
            frameQueue.push(resizedFrame);
            frameable.notify_one(); // queue에서 데이터 처리
        }

        this_thread::sleep_for(chrono::milliseconds(50)); // 프레임 수신 간격
    }
}

void display_all_client(map<string, queue<Mat>>& frameQueues) {
    // 윈도우 생성
    for (const auto& [ip, frameQueue] : frameQueues) {
        namedWindow(ip, WINDOW_NORMAL);
    }

    while (!stop.load()) {
        for (auto& [ip, frameQueue] : frameQueues) {
            Mat frame;
            // 각 클라이언트의 프레임 큐에서 데이터 가져오기
            {
                lock_guard<mutex> lock(queueMutex);
                if (!frameQueue.empty()) {
                    frame = frameQueue.front();
                    frameQueue.pop();
                }
            }
            
            // 가져온 프레임 출력
            if (!frame.empty()) {
                imshow(ip, frame); // imshow로 출력
            }
        }

        // 'q' 키 입력 시 종료
        if (waitKey(1) == 'q') {
            stop.store(true);
            frameable.notify_all();
            break;
        }
    }

    // 윈도우 종료
    for (const auto& [ip, frameQueue] : frameQueues) {
        destroyWindow(ip);
    }
}

// 모든 클라이언트의 영상을 수신 및 출력
void videoallplay(Mat& frame, const string& ip) {
    // 클라이언트별 프레임 큐 및 스레드 관리
    static map<string, queue<Mat>> frameQueues; // IP별 프레임 큐
    static vector<thread> threads;

    // 클라이언트의 프레임 큐가 없으면 추가
    if (frameQueues.find(ip) == frameQueues.end()) {
        frameQueues[ip] = queue<Mat>(); // 새로운 큐 생성

        // 데이터 수신 스레드 시작
        threads.emplace_back(videothread, ref(frame), ref(frameQueues[ip]));
    }

    // 메인 스레드에서 디스플레이 처리
    display_all_client(frameQueues);

    // 스레드 종료 대기
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}