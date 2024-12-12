#include "server.h"
#include "networkmanager.h"

int main() {
    int server_sock, client_sock,c;
    struct sockaddr_in server, client;

    // socket 생성
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        cerr << "소켓 생성 실패" << endl;
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    int opt = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        //std::cerr << "setsockopt 실패" << std::endl;
        close(server_sock);
        return -1;
    }

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cerr << "Bind 실패" << endl;
        return 1;
    }
    
    NetworkManager& networkManager = NetworkManager::getInstance();

    networkManager.startMediaMTX();
    
    listen(server_sock, 3); // 수신대기
    cout << "연결을 기다리고 있습니다..." << endl;

    char ip[INET_ADDRSTRLEN];
    c = sizeof(struct sockaddr_in); // 구조체 크기 저장

    thread send_thread(send_merged_frames, client_sock);
    send_thread.detach();

    while ((client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t*)&c))) {
        inet_ntop(AF_INET, &client.sin_addr, ip, INET_ADDRSTRLEN);
        thread client_thread(receive_frames, client_sock,ip);
        //cout << "클라이언트 연결: " << ip << ":" << ntohs(client.sin_port) << endl;
        client_thread.detach();  // 클라이언트 요청을 새로운 스레드에서 처리
    }

    if (client_sock < 0) {
        cerr << "연결 실패" << endl;
        networkManager.stopRTSP();
        networkManager.stopMediaMTX();
        return 1;
    }

    networkManager.stopRTSP();
    networkManager.stopMediaMTX();

    return 0;
}
