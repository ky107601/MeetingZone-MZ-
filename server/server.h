#ifndef SERVER_H
#define SERVER_H

#include <string>
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
#include "nlohmann/json.hpp"



using json = nlohmann::json;
using namespace std;


// 클라이언트 처리 함수 선언
void handle_client(int client_sock);

#endif // SERVER_H
