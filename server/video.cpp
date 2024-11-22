#include "server.h"

mutex queueMutex;
condition_variable frameAvailable;
bool stopThreads = false;

void videoCaptureThread(const string& video, queue<Mat>& frameQueue) {
    VideoCapture capture(video);
    if (!capture.isOpened()) {
        cerr << "동영상을 열 수 없습니다: " << video << endl;
        return;
    }

    while (!stopThreads) {
        Mat frame;
        if (!capture.read(frame)) {
            cout << "동영상 재생이 끝났습니다: " << video << endl;
            break;
        }

        // 프레임 크기 조정
        Mat resizedFrame;
        resize(frame, resizedFrame, Size(320, 240), 0, 0, INTER_CUBIC);

        // 프레임을 큐에 추가
        {
            lock_guard<mutex> lock(queueMutex);
            frameQueue.push(resizedFrame);
        }
        frameAvailable.notify_all();
    }

    capture.release();
}

void displayFrames(vector<queue<Mat>>& frameQueues, const vector<string>& windowNames) {
    // 창 위치 계산
    int base_x = 100, base_y = 100, offset_x = 100;

    // 창 생성 및 위치 지정
    for (size_t i = 0; i < windowNames.size(); ++i) {
        namedWindow(windowNames[i], WINDOW_NORMAL);
        moveWindow(windowNames[i], base_x + i * offset_x, base_y);
    }

    while (!stopThreads) {
        vector<Mat> frames(windowNames.size());

        // 각 큐에서 프레임 가져오기
        for (size_t i = 0; i < frameQueues.size(); ++i) {
            unique_lock<mutex> lock(queueMutex);
            frameAvailable.wait(lock, [&frameQueues, i] { return !frameQueues[i].empty() || stopThreads; });

            if (!frameQueues[i].empty()) {
                frames[i] = frameQueues[i].front();
                frameQueues[i].pop();
            }
        }

        // 각 창에 프레임 출력
        for (size_t i = 0; i < frames.size(); ++i) {
            if (!frames[i].empty()) {
                imshow(windowNames[i], frames[i]);
            }
        }

        // 'q' 키 입력 시 종료
        if (waitKey(1) == 'q') {
            stopThreads = true;
            frameAvailable.notify_all();
            break;
        }
    }

    destroyAllWindows();
}

void videoplay()
{
    // 동영상 파일 이름 목록
    vector<string> videoFiles = {"test1.mp4", "test2.mp4"}; // 동적으로 추가 가능

    // 스레드와 큐 관리
    vector<thread> threads;
    vector<queue<Mat>> frameQueues(videoFiles.size()); // 동영상 파일마다 큐 생성

    // 동영상 파일마다 스레드 생성
    for (size_t i = 0; i < videoFiles.size(); ++i) {
        threads.emplace_back(videoCaptureThread, videoFiles[i], ref(frameQueues[i]));
    }

    // 메인 스레드에서 프레임 출력
    displayFrames(frameQueues, videoFiles);

    // 모든 스레드 종료 대기
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}