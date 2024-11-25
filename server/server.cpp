#include "server.h"


void handle_client(int client_sock) {
    const int chunkSize = 4096;  // 데이터 청크 크기
    char buffer[chunkSize];
    std::vector<uchar> frameBuffer;
    int64 frameSize = 0;

    while (true) {
        // 프레임 크기 수신
        if (recv(client_sock, reinterpret_cast<char*>(&frameSize), sizeof(frameSize), 0) <= 0) {
            std::cout << "클라이언트 연결 종료" << std::endl;
            break;
        }
        frameBuffer.clear();
        frameBuffer.resize(frameSize);

        // 프레임 데이터 수신
        int64 received = 0;
        while (received < frameSize) {
            int bytesRead = recv(client_sock, buffer, std::min(chunkSize, static_cast<int>(frameSize - received)), 0);
            if (bytesRead <= 0) {
                std::cerr << "데이터 수신 실패 또는 연결 종료" << std::endl;
                return;
            }
            std::copy(buffer, buffer + bytesRead, frameBuffer.begin() + received);
            received += bytesRead;
        }

        // 프레임 디코딩
        cv::Mat frame = cv::imdecode(frameBuffer, cv::IMREAD_COLOR);
        if (!frame.empty()) {
            // 프레임 출력
            videoplay(frame);
        }
    }

    close(client_sock);
}

