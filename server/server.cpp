#include "server.h"


// 클라이언트 처리 함수
void handle_client(int client_sock) {
    char buffer[1024];
    string response;
    // vector<uchar> videoBuffer;
    
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int read_size = recv(client_sock, buffer, sizeof(buffer) - 1, 0); 
        if (read_size <= 0) {
            cout << "클라이언트 연결해제" << endl;
            break;
        }

        buffer[read_size] = '\0';
        string data(buffer);
    
        cout << "클라이언트에서 보낸 데이터: " << data << endl;  // 수신한 데이터 출력

        try {
            json received_data = json::parse(data);  // JSON 데이터 파싱

            if (received_data["type"] == "video_metadata") {
                cout << "영상 메타 데이터 수신:" << endl;
                cout << "  - 영상 이름: " << received_data["video_name"] << endl;
                cout << "  - 해상도: " << received_data["resolution"] << endl;
                cout << "  - 길이: " << received_data["duration"] << "초" << endl;
                cout << "  - 크기: " << received_data["size"] << "바이트" << endl;

                string ack = "{\"status\":\"ok\",\"message\":\"Metadata received\"}"; // 클라이언트로 ack 전송
                send(client_sock, ack.c_str(), ack.size(), 0);

                cout << "영상 데이터 수신 중..." << endl;

                // // 영상 데이터 수신
                // int videoSize = received_data["size"];
                // vector<uchar> frameData(videoSize);
                // int receivedBytes = 0;
                // while (receivedBytes < videoSize) {
                //     int chunkSize = recv(client_sock, frameData.data() + receivedBytes, videoSize - receivedBytes, 0);
                //     if (chunkSize <= 0) {
                //         cerr << "영상 데이터 수신 중단" << endl;
                //         break;
                //     }
                //     receivedBytes += chunkSize;
                // }

                // // OpenCV로 디코딩하여 출력
                // Mat frame = imdecode(frameData, IMREAD_COLOR);
                // if (!frame.empty()) {
                //     imshow("Received Video", frame);
                //     waitKey(1);
                // }

                // // 클라이언트로 다시 영상 전송
                // cout << "영상 데이터를 클라이언트로 전송 중..." << endl;
                // vector<uchar> encodedFrame;
                // imencode(".jpg", frame, encodedFrame);
                // json videoResponse = {
                //     {"type", "video_response"},
                //     {"size", static_cast<int>(encodedFrame.size())}
                // };

                // // JSON 전송
                // string response = videoResponse.dump();
                // send(client_sock, response.c_str(), response.size(), 0);

                // // 영상 데이터 전송
                // send(client_sock, reinterpret_cast<const char*>(encodedFrame.data()), encodedFrame.size(), 0);
                // cout << "영상 데이터 전송 완료" << endl;
            }

        } catch (const std::exception &e) {
            cerr << "JSON 파싱 에러: " << e.what() << endl;
        }
    }

    close(client_sock);
}

