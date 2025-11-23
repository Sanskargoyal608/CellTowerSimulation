#ifndef TOWER_3G_H
#define TOWER_3G_H

#include "CellTower.h"
#include "Channel.h"
#include "CapacityExceededException.h"

// 3G uses CDMA. Supports 32 users per 200 kHz channel.
class Tower3G : public CellTower {
private:
    // 5 Channels of 200kHz make up the 1MHz total
    // Template <UserDevice*, 32> changes the limit to 32 users
    Channel<UserDevice*, 32>* channels[5]; 

public:
    Tower3G(int id, CellularCore* c) : CellTower(id, c) {
        // Initialize 5 channels
        for(int i=0; i<5; i++) {
            channels[i] = new Channel<UserDevice*, 32>(i * 0.2);
        }
    }

    // Total Capacity = 5 channels * 32 users = 160 users 
    int calculateMaxCapacity() override {
        return 5 * 32; 
    }

    bool addUser(UserDevice* user) override {
        // 3G Requirement: 10 messages per user 
        int messagesPerUser = 10; 
        
        if (!core->addMessageLoad(messagesPerUser)) {
             throw CapacityExceededException("Core Overhead Limit Reached (3G)");
        }

        // Find an open frequency channel
        for(int i=0; i<5; i++) {
            if (!channels[i]->isFull()) {
                channels[i]->add(user);
                return true; 
            }
        }

        throw CapacityExceededException("All 3G Frequency Channels Full");
    }

    // Requirement: Identify users occupying the first channel [cite: 20]
    Channel<UserDevice*, 32>* getFirstChannel() {
        return channels[0];
    }
};

#endif