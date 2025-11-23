#ifndef CHANNEL_H
#define CHANNEL_H

#include "CapacityExceededException.h"

template <typename T, int LIMIT>
class Channel {
private:
    T connectedUsers[LIMIT]; // FIXED: Removed the '*' here
    int userCount;
    double frequency;

public:
    Channel(double freq) : userCount(0), frequency(freq) {
        for(int i=0; i<LIMIT; i++) connectedUsers[i] = nullptr;
    }

    bool isFull() const {
        return userCount >= LIMIT;
    }

    void add(T user) {
        if (isFull()) {
            return;
        }
        connectedUsers[userCount++] = user;
    }

    int getCount() const { return userCount; }
    double getFrequency() const { return frequency; }
    
    T getUser(int index) {
        if(index >= 0 && index < userCount) return connectedUsers[index];
        return nullptr;
    }
};

#endif