#ifndef TOWER_5G_H
#define TOWER_5G_H

#include "Tower4G.h"
#include "Channel.h"
#include "CapacityExceededException.h"

// 5G inherits 4G capabilities and adds High-Frequency Massive MIMO channels.
class Tower5G : public Tower4G {
private:
    // New High Frequency Band: 10 MHz total.
    // We split this into 10 channels of 1 MHz each.
    // Density: 30 users * 16 Antennas (Massive MIMO) = 480 users per channel.
    Channel<UserDevice*, 480>* highFreqChannels[10];

public:
    Tower5G(int id, CellularCore* c) : Tower4G(id, c) {
        // Initialize the 10 new High-Freq channels
        for(int i=0; i<10; i++) {
            highFreqChannels[i] = new Channel<UserDevice*, 480>(1800.0 + i); 
        }
    }

    // Capacity = 4G Capacity + 5G Add-on
    int calculateMaxCapacity() override {
        int capacity4G = Tower4G::calculateMaxCapacity(); // 12,000
        int capacity5G = 10 * 480; // 10 channels * 480 users = 4,800
        return capacity4G + capacity5G;
    }

    bool addUser(UserDevice* user) override {
        // 1. Try to add to the standard 4G network first
        try {
            Tower4G::addUser(user);
            return true;
        } catch (CapacityExceededException& e) {
            // 4G is full, so we try the 5G High-Frequency bands
        }

        // 2. Try High-Frequency Channels (Massive MIMO)
        // Check Core Limits first (same message cost assumed)
        if (!core->addMessageLoad(10)) {
             throw CapacityExceededException("Core Overhead Limit Reached (5G)");
        }

        for(int i=0; i<10; i++) {
            if (!highFreqChannels[i]->isFull()) {
                highFreqChannels[i]->add(user);
                return true;
            }
        }

        throw CapacityExceededException("All 4G and 5G Channels Full");
    }
};

#endif