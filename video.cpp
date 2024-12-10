#include "server.h"

mutex queueMutex;
condition_variable frameable;
atomic<bool> stop = false;

// 클라이언트 프레임 데이터를 처리하여 큐에 추가
void videothread(Mat& frame, queue<Mat>& frameQueue) {
    while (!stop.load()) {
        if (!frame.empty()) {
            Mat resizedFrame;
            resize(frame, resizedFrame, Size(320, 240), 0, 0, INTER_CUBIC); // 크기 조정
            {
                lock_guard<mutex> lock(queueMutex);
                frameQueue.push(resizedFrame);
                frameable.notify_one(); // 큐에서 처리 알림
            }
        }
        this_thread::sleep_for(chrono::milliseconds(60)); // 데이터 수신 간격
    }
}

// 병합된 프레임을 디스플레이
void display_all_client(map<string, queue<Mat>>& frameQueues) {
    while (!stop.load()) {
        Mat mergedFrame = mergeFrames(frameQueues);
        if (!mergedFrame.empty()) {
            imshow("Merged Frames", mergedFrame);
            waitKey(1);
        }
        this_thread::sleep_for(chrono::milliseconds(60)); // 30 FPS
    }
    destroyWindow("Merged Frames");
}

// 병합된 프레임을 모든 클라이언트에 전송
void servtocli(map<string, int>& client, map<string, queue<Mat>>& frameQueues) {
    Mat mergedFrame = mergeFrames(frameQueues);
    if (!mergedFrame.empty()) {
        vector<uchar> buffer;
        imencode(".jpg", mergedFrame, buffer);

        int64_t frameSize = buffer.size();

        for (auto& [ip, socket] : client) {
            if (socket > 0) { // 유효한 소켓 디스크립터인지 확인
                // 프레임 크기 전송
                if (write(socket, reinterpret_cast<const char*>(&frameSize), sizeof(frameSize)) == -1) {
                    cerr << "[ERROR] 프레임 크기 전송 실패 (IP: " << ip << ")" << endl;
                    continue;
                }

                // 프레임 데이터 전송
                if (write(socket, reinterpret_cast<const char*>(buffer.data()), buffer.size()) == -1) {
                    cerr << "[ERROR] 프레임 데이터 전송 실패 (IP: " << ip << ")" << endl;
                } else {
                    cout << "[INFO] 병합된 프레임 전송 성공 (IP: " << ip << ", 크기: " << frameSize << ")" << endl;
                }
            }
        }
    }
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

    // 병합된 프레임을 클라이언트로 전송
    static map<string, int> client; // 클라이언트별 소켓 디스크립터 관리
    servtocli(client, frameQueues);

    // 스레드 종료 관리
    for (auto& thread : threads) {
        if (thread.joinable() && stop.load()) {
            thread.join();
        }
    }
}

// 프레임 병합 함수
Mat mergeFrames(map<string, queue<Mat>>& frameQueues) {
    int cols = 2;
    int rows = (frameQueues.size() + cols - 1) / cols;
    int width = 320, height = 240;

    Mat mergedFrame = Mat::zeros(rows * height, cols * width, CV_8UC3);
    mergedFrame.setTo(Scalar(0, 0, 0));

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
