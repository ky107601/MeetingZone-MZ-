#include "server.h"


void handle_client(int client_sock, const string& ip) {
    const int chunkSize = 4096;  // 데이터 청크 크기
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

