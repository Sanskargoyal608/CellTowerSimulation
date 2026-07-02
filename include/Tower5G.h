#ifndef TOWER_5G_H
#define TOWER_5G_H

#include "Tower4G.h"
#include "Channel.h"
#include "CapacityExceededException.h"

// 5G inherits all 4G (OFDM) capabilities and adds High-Frequency Massive MIMO.
// 4G portion : 100 channels x 120 users = 12,000 users
// 5G portion : 10 channels x 480 users = 4,800 users
// Total max  : 16,800 users
class Tower5G : public Tower4G {
private:
    // New High-Frequency Band: 10 MHz total, split into 10 channels of 1 MHz.
    // Density: 30 users * 16 Massive-MIMO antennas = 480 users per channel.
    Channel<UserDevice*, 480>* highFreqChannels[10];

public:
    Tower5G(int id, CellularCore* c) : Tower4G(id, c) {
        // Initialize 10 High-Frequency channels starting at 1800 MHz
        for (int i = 0; i < 10; i++) {
            highFreqChannels[i] = new Channel<UserDevice*, 480>(1800.0 + i);
        }
    }

    virtual ~Tower5G() {
        for (int i = 0; i < 10; i++) delete highFreqChannels[i];
    }

    // Total capacity = inherited 4G capacity + 5G high-freq capacity
    int calculateMaxCapacity() override {
        int capacity4G = Tower4G::calculateMaxCapacity(); // 12,000
        int capacity5G = 10 * 480;                        // 4,800
        return capacity4G + capacity5G;                   // 16,800
    }

    // Try 4G first; if full, overflow into 5G high-frequency channels.
    bool addUser(UserDevice* user) override {
        // 1. Try the standard 4G sub-channels first
        try {
            Tower4G::addUser(user);
            return true;
        } catch (CapacityExceededException& e) {
            // 4G is full — fall through to 5G
        }

        // 2. 4G is full: check core message budget before using 5G
        if (!core->addMessageLoad(10)) {
            throw CapacityExceededException("Core Overhead Limit Reached (5G)");
        }

        // 3. Place user in first available 5G high-frequency channel
        for (int i = 0; i < 10; i++) {
            if (!highFreqChannels[i]->isFull()) {
                highFreqChannels[i]->add(user);
                return true;
            }
        }

        throw CapacityExceededException("All 4G and 5G Channels Full");
    }

    // Returns the first 5G high-frequency channel for reporting
    Channel<UserDevice*, 480>* getFirstHighFreqChannel() {
        return highFreqChannels[0];
    }

    // Calculate cores needed for full 5G capacity:
    // 16,800 users * 10 messages = 168,000 total messages
    int calculateRequiredCores(long singleCoreCapacity) {
        long totalMessages = (long)calculateMaxCapacity() * 10L;
        int cores = (int)(totalMessages / singleCoreCapacity);
        if (totalMessages % singleCoreCapacity != 0) cores++;
        return cores;
    }
};

#endif