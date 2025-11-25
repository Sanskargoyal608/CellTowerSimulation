#ifndef TOWER_4G_H
#define TOWER_4G_H

#include "CellTower.h"
#include "Channel.h"
#include "CapacityExceededException.h"

// 4G uses OFDM with 100 sub-channels (1 MHz / 10 kHz).
// Supports 30 users * 4 Antennas (MIMO) = 120 users per channel.
class Tower4G : public CellTower {
protected: // Protected so 5G can inherit this later
    // 100 Channels of 10kHz each
    Channel<UserDevice*, 120>* channels[100]; 

public:
    Tower4G(int id, CellularCore* c) : CellTower(id, c) {
        // Initialize 100 channels: 0.00, 0.01, 0.02 ... 0.99 MHz
        for(int i=0; i<100; i++) {
            channels[i] = new Channel<UserDevice*, 120>(i * 0.01);
        }
    }

    // Total Capacity = 100 channels * 120 users = 12,000 users
    int calculateMaxCapacity() override {
        return 100 * 120; 
    }
    
    Channel<UserDevice*, 120>* getFirstChannel() {
        return channels[0];
    }

    bool addUser(UserDevice* user) override {
        // 4G Requirement: 10 messages per user 
        int messagesPerUser = 10; 
        
        if (!core->addMessageLoad(messagesPerUser)) {
             throw CapacityExceededException("Core Overhead Limit Reached (4G)");
        }

        // Search through all 100 sub-channels
        for(int i=0; i<100; i++) {
            if (!channels[i]->isFull()) {
                channels[i]->add(user);
                return true; 
            }
        }

        throw CapacityExceededException("All 4G Sub-Channels Full");
    }

    // New Function for Requirement 
    // Calculate how many Cores are needed to support full 12,000 users
    int calculateRequiredCores(long singleCoreCapacity) {
        long totalMessages = (long)calculateMaxCapacity() * 10; // 120,000 msgs
        
        int cores = totalMessages / singleCoreCapacity;
        if (totalMessages % singleCoreCapacity != 0) cores++; // Round up
        
        return cores;
    }
};

#endif