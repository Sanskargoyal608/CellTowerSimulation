#ifndef TOWER_2G_H
#define TOWER_2G_H

#include "CellTower.h"
#include "Channel.h"
#include "CapacityExceededException.h"

class Tower2G : public CellTower {
private:
    // 5 Channels of 200kHz make up the 1MHz total 
    // <UserDevice*, 16> sets the Template limit to 16 users 
    Channel<UserDevice*, 16>* channels[5]; 

public:
    Tower2G(int id, CellularCore* c) : CellTower(id, c) {
        // Initialize 5 channels starting at 0.0 MHz with 0.2 MHz steps
        for(int i=0; i<5; i++) {
            channels[i] = new Channel<UserDevice*, 16>(i * 0.2);
        }
    }

    // Total Capacity = 5 channels * 16 users = 80 users
    int calculateMaxCapacity() override {
        return 5 * 16; 
    }

    bool addUser(UserDevice* user) override {
        // 1. Check Core Message Overhead
        // 2G Requirement: 5 messages (data) + 15 messages (voice) = 20 total 
        int messagesPerUser = 20; 
        
        // If core cannot handle these messages, reject the user [cite: 9]
        if (!core->addMessageLoad(messagesPerUser)) {
             throw CapacityExceededException("Core Overhead Limit Reached (2G)");
        }

        // 2. Find an open frequency channel
        // We iterate through our 5 channels to find space
        for(int i=0; i<5; i++) {
            if (!channels[i]->isFull()) {
                channels[i]->add(user);
                return true; // Successfully added
            }
        }

        // 3. If we get here, all 5 channels are full
        throw CapacityExceededException("All 2G Frequency Channels Full");
    }

    // Requirement: Identify users occupying the first channel 
    Channel<UserDevice*, 16>* getFirstChannel() {
        return channels[0];
    }
};

#endif