#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <ctime>
#include <arpa/inet.h>
#include "../includes/hft_client.h"
#include <deque>
#include <sys/socket.h>
using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

bool parsePriceMessage(const std::string& data, int& priceId, float& price) {
    size_t commaPos = data.find(',');
    if (commaPos == string::npos) {
        return false;
    }

    try {
        priceId = stoi(data.substr(0, commaPos));
        price = stof(data.substr(commaPos + 1));
    } catch (const exception& e) {
        cerr << "Failed to parse message: " << data
             << " | Error: " << e.what() << endl;
        return false;
    }
    return true;
}
void updatePriceHistory(std::deque<float>& priceHistory, float price) {
    if (priceHistory.size() >= 3) {
        priceHistory.pop_front();
    }
    priceHistory.push_back(price);
}


void detect_momentum(const std::deque<float>& priceHistory, const int priceId, const int socketFd){
    float a = priceHistory[0];
    float b = priceHistory[1];
    float c = priceHistory[2];

    bool up = (a < b) && (b < c);
    bool down = (a > b) && (b > c);

    if (up || down) {
        // Send order (price ID)
        if (up) {
            cout << "Momentum up! Sending order for price ID " << priceId << endl;
            upSignals++;
        }else {
            cout << "Momentum down! Sending order for price ID " << priceId << endl;
            downSignals++;
        }
        this_thread::sleep_for(chrono::milliseconds(10 + rand() % 50));

        string order = to_string(priceId);
        int bytesSent = send(socketFd, order.c_str(), order.length(), 0);
        if (bytesSent < 0) {
            sendFailures++;
            cout << "Order send failed." << endl;
            return;
        }

        hitAttempts++;
        cout << "Sent order attempt." << endl;
    }else {
        ignoredUpdates++;
        cout << "No momentum. Ignoring price ID " << priceId << endl;
    }
}

void printClientStats() {
    cout << "\n===== Client Stats =====" << endl;
    cout << "Total prices received: " << totalPricesReceived << endl;
    cout << "Up momentum signals: " << upSignals << endl;
    cout << "Down momentum signals: " << downSignals << endl;
    cout << "Total momentum signals: " << (upSignals + downSignals) << endl;
    cout << "Hit attempts (orders sent): " << hitAttempts << endl;
    cout << "Ignored updates: " << ignoredUpdates << endl;
    cout << "Send failures: " << sendFailures << endl;

    double signalRate = totalPricesReceived > 0
        ? static_cast<double>(upSignals + downSignals) / (totalPricesReceived-2.0)
        : 0.0;

    double attemptRate = totalPricesReceived > 0
        ? static_cast<double>(hitAttempts) / (totalPricesReceived-2.0)
        : 0.0;

    cout << "Momentum signal rate: " << signalRate * 100 << "%" << endl;
    cout << "Hit attempt rate: " << attemptRate * 100 << "%" << endl;
    cout << "Note: actual hit success cannot be confirmed without server acknowledgment."
         << endl;
}

void receiveAndRespond(int socketFd, const string& name) {
    char buffer[BUFFER_SIZE];

    // Send client name
    send(socketFd, name.c_str(), name.size(), 0);

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(socketFd, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            cerr << "Server closed connection or error occurred." << endl;
            break;
        }

        string data(buffer);
        // find the split comma:  ID,Price
        int priceId;
        float price;
        if (parsePriceMessage(data, priceId, price) == false) {
            continue;
        }

        cout << "📥 Received price ID: " << priceId << ", Value: " << price << endl;

        // update price history data
        updatePriceHistory(priceHistory, price);
        totalPricesReceived++;

        if (priceHistory.size() == 3) {
            detect_momentum(priceHistory, priceId, socketFd);
        }
        // Simulate reaction delay
        // this_thread::sleep_for(chrono::milliseconds(100 + rand() % 300));
    }
    printClientStats();
    close(socketFd);
}

//
// int main() {
//     srand(time(nullptr));
//
//     string name;
//     cout << "Enter your client name: ";
//     getline(cin, name);
//
//     int sock = socket(AF_INET, SOCK_STREAM, 0);
//     if (sock < 0) {
//         cerr << "Socket creation failed!" << endl;
//         return 1;
//     }
//
//     sockaddr_in serverAddr{};
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(SERVER_PORT);
//     inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
//
//     if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
//         cerr << "Connection to server failed!" << endl;
//         return 1;
//     }
//
//     cout << "✅ Connected to server at " << SERVER_IP << ":" << SERVER_PORT << endl;
//     receiveAndRespond(sock, name);
//     return 0;
// }
