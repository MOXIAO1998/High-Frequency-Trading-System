//
// Created by Moxiao Li on 4/6/26.
//

#ifndef ADVANCED_COMPUTING_FOR_FINANCE_HTF_SERVER_H
#define ADVANCED_COMPUTING_FOR_FINANCE_HTF_SERVER_H
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <atomic>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <utility>

using namespace std;
using namespace std::chrono;
#define PORT 12345
#define BUFFER_SIZE 1024

struct ClientInfo {
    int socket;
    string name;
    thread clientThread;
};

vector<unique_ptr<ClientInfo>> clients;
mutex clientsMutex;

unordered_map<int, steady_clock::time_point> priceTimestamps;
unordered_set<int> priceAlreadyHit;
mutex priceMutex;

atomic<int> priceId{0};

void broadcastPrices();
void handleClient(ClientInfo* client);
void startServer();
#endif //ADVANCED_COMPUTING_FOR_FINANCE_HTF_SERVER_H