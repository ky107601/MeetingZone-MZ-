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
//합쳐진 영상 imshow
void display_all_client(map<string, queue<Mat>>& frameQueues) {
    while (!stop.load()) {
        Mat mergedFrame = mergeFrames(frameQueues);

        if (!mergedFrame.empty()) {
            imshow("Merged Frames", mergedFrame);
            waitKey(1);
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }
    destroyWindow("Merged Frames");
    
}



void videoallplay(Mat& frame, const string& ip) {
    static map<string, queue<Mat>> frameQueues; // 클라이언트별 프레임 큐
    static vector<thread> threads;
    static mutex localMutex;

    // 클라이언트별 프레임 큐 초기화
    if (frameQueues.find(ip) == frameQueues.end()) {
        lock_guard<mutex> lock(localMutex);
        frameQueues[ip] = queue<Mat>();
    }

    {
        lock_guard<mutex> lock(queueMutex);
        if (!frame.empty()) {
            frameQueues[ip].push(frame.clone());
        }
    }

    // 데이터 수신 스레드가 생성되지 않은 경우 한 번만 실행
    if (threads.empty()) {
        threads.emplace_back([&frameQueues]() {
            display_all_client(frameQueues);
        });
    }

    // 스레드 종료 관리 (stop 플래그가 true가 되면 join)
    for (auto& thread : threads) {
        if (thread.joinable() && stop.load()) {
            thread.join();
        }
    }
}


// 받은 데이터 합성(합치기)
Mat mergeFrames(map<string, queue<Mat>>& frameQueues) {
    int cols = 2;
    int rows = (frameQueues.size() + cols - 1) / cols;
    int width = 320, height = 240;

    Mat mergedFrame = Mat::zeros(rows * height, cols * width, CV_8UC3);

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

        if (!frame.empty()) {
            Mat resizedFrame;
            resize(frame, resizedFrame, Size(width, height));

            int x = (i % cols) * width;
            int y = (i / cols) * height;

            resizedFrame.copyTo(mergedFrame(Rect(x, y, width, height)));
        }
        ++i;
    }

    return mergedFrame;
}
