//
// Created by Moxiao Li on 4/6/26.
//

#ifndef ADVANCED_COMPUTING_FOR_FINANCE_HFT_CLIENT_H
#define ADVANCED_COMPUTING_FOR_FINANCE_HFT_CLIENT_H
#include <iostream>
#include <deque>
#include <string>


std::deque<float> priceHistory;

int totalPricesReceived = 0;
int upSignals = 0;
int downSignals = 0;
int hitAttempts = 0;
int ignoredUpdates = 0;
int sendFailures = 0;

bool parsePriceMessage(const std::string& data, int& priceId, float& price);

void updatePriceHistory(std::deque<float>& priceHistory, float price);

void detect_momentum(const std::deque<float>& priceHistory, const int priceId, const int socketFd);

void printClientStats();

void receiveAndRespond(int socketFd, const std::string& name);


#endif //ADVANCED_COMPUTING_FOR_FINANCE_HFT_CLIENT_H