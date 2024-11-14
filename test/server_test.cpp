#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_set>
#include <unordered_map>
#include <memory>


using namespace std;
mutex mtx;

// 클라이언트 처리 함수
void handle_client(int client_sock) {
    char buffer[1024];
    string response;
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

        /*
        클라이언트에서 화면 들어오면 처리
        */
    }
    close(client_sock);
}

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
