#include "server.h"
extern Mat mergedFrame;

void receive_frames(int client_sock, const string& ip)
{
    const int chunkSize = 64000;  // 데이터 청크 크기
    char buffer[chunkSize];
    vector<uchar> frameBuffer;
    int64 frameSize = 0;

    while (true) {
        // 프레임 크기 수신
        if (recv(client_sock, reinterpret_cast<char*>(&frameSize), sizeof(frameSize), 0) <= 0) {
            cout << "클라이언트 연결 종료 (" << ip << ")" << endl;
            break;
        }
        frameBuffer.clear();
        frameBuffer.resize(frameSize);

        // 프레임 데이터 수신
        int64 received = 0;
        while (received < frameSize) {
            int bytesRead = recv(client_sock, buffer, min(chunkSize, static_cast<int>(frameSize - received)), 0);
            if (bytesRead <= 0) {
                cerr << "데이터 수신 실패 또는 연결 종료 (" << ip << ")" << endl;
                return;
            }
            copy(buffer, buffer + bytesRead, frameBuffer.begin() + received);
            received += bytesRead;
        }

        // opencv로 받은 프레임 디코딩
        Mat frame = imdecode(frameBuffer, IMREAD_COLOR);
        if (!frame.empty()) {
            // 프레임 출력
            videoallplay(frame, ip);
        }
    }

    close(client_sock);
}

void send_merged_frames(int client_sock, const string& ip) {
    while(true){
        unique_lock<mutex> lock(queueMutex);
        if (mergedFrame.empty()) {
            lock.unlock();
            this_thread::sleep_for(chrono::milliseconds(100)); // 30 FPS 대기
            continue;
        }

        // 병합된 프레임을 JPEG로 인코딩
        vector<uchar> buffer;
        if (!imencode(".jpg", mergedFrame, buffer)) {
            cerr << "[ERROR] 병합된 프레임 인코딩 실패." << endl;
            continue;
        }

        int64_t frameSize = buffer.size();

        // 프레임 크기 전송
        if (write(client_sock, reinterpret_cast<const char*>(&frameSize), sizeof(frameSize)) == -1) {
            cerr << "[ERROR] 병합된 프레임 크기 전송 실패 (" << ip << ")" << endl;
            break;
        }

        // 프레임 데이터 전송
        if (write(client_sock, reinterpret_cast<const char*>(buffer.data()), buffer.size()) == -1) {
            cerr << "[ERROR] 병합된 프레임 데이터 전송 실패 (" << ip << ")" << endl;
            break;
        }

        cout << "[INFO] 병합된 프레임 전송 성공 (IP: " << ip << ", 크기: " << frameSize << ")" << endl;

        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(100)); // 30 FPS 대기
    }

    close(client_sock);
}
