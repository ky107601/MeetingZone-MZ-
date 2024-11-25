#include "server.h"


int main() {
    int server_sock, client_sock, c;
    struct sockaddr_in server, client;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        cerr << "소켓 생성 실패" << endl;
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cerr << "Bind 실패" << endl;
        return 1;
    }

    listen(server_sock, 3);
    cout << "연결을 기다리고 있습니다..." << endl;
    
    c = sizeof(struct sockaddr_in);
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t*)&c))) {
        thread client_thread(handle_client, client_sock);
        client_thread.detach();  // 클라이언트 요청을 새로운 스레드에서 처리
    }

    if (client_sock < 0) {
        cerr << "연결 실패" << endl;
        return 1;
    }

    return 0;
}
