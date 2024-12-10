#include "server.h"

mutex queueMutex;
condition_variable frameable;

Mat mergedFrame;

// 클라이언트 프레임 데이터를 처리하여 큐에 추가
void videothread(Mat& frame, queue<Mat>& frameQueue) {
    while(true){
        if (!frame.empty()) {
            Mat resizedFrame;
            resize(frame, resizedFrame, Size(320, 240), 0, 0, INTER_CUBIC); // 크기 조정
            {
                lock_guard<mutex> lock(queueMutex);
                frameQueue.push(resizedFrame);
                frameable.notify_one(); // 큐에서 처리 알림
            }
        }
        this_thread::sleep_for(chrono::milliseconds(100)); // 데이터 수신 간격
    }
}

// 병합된 프레임을 디스플레이
void display_all_client(map<string, queue<Mat>>& frameQueues) {
    while(true){
        Mat mergedFrame = mergeFrames(frameQueues);
        if (!mergedFrame.empty()) {
            imshow("Merged Frames", mergedFrame);
            waitKey(1);
        }
        this_thread::sleep_for(chrono::milliseconds(100)); // 30 FPS
    }
    destroyWindow("Merged Frames");
}


// 클라이언트별 프레임을 처리하고 병합된 프레임 디스플레이
void videoallplay(Mat& frame, const string& ip) {
    static map<string, queue<Mat>> frameQueues; // 클라이언트별 프레임 큐
    static vector<thread> threads;
    static mutex localMutex;

    // 클라이언트별 프레임 큐 초기화
    if (frameQueues.find(ip) == frameQueues.end()) {
        lock_guard<mutex> lock(localMutex);
        frameQueues[ip] = queue<Mat>();
    }

    // 클라이언트 프레임 추가
    {
        lock_guard<mutex> lock(queueMutex);
        if (!frame.empty()) {
            frameQueues[ip].push(frame.clone());
        }
    }

    // 병합된 프레임을 디스플레이
    if (threads.empty()) {
        threads.emplace_back([&frameQueues]() {
            display_all_client(frameQueues);
        });
    }


}

// 프레임 병합 함수
Mat mergeFrames(map<string, queue<Mat>>& frameQueues) {
    int cols = 2;
    int rows = 2;
    int width = 320, height = 240;

    mergedFrame = Mat::zeros(1280,960, CV_8UC3);

    int i = 0;
    for (auto& [ip, frameQueue] : frameQueues) {
        Mat frame;
        {
            lock_guard<mutex> lock(queueMutex);
            if (!frameQueue.empty()) {
                frame = frameQueue.front();
                frameQueue.pop();
            }
        }

        if (frame.empty()) {
            frame = Mat::zeros(height, width, CV_8UC3);
        }

        Mat resizedFrame;
        resize(frame, resizedFrame, Size(width, height));

        int x = (i % cols) * width;
        int y = (i / cols) * height;

        resizedFrame.copyTo(mergedFrame(Rect(x, y, width, height)));
        ++i;
    }

    return mergedFrame;
}
