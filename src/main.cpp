#include <basicIO.h>
#include "CellularCore.h"
#include "UserDevice.h"
#include "Tower5G.h"
#include "CapacityExceededException.h"

extern "C" int main() {
    io.outputstring("==============================================\n");
    io.outputstring("   CELLULAR NETWORK SIMULATOR (5G HYBRID)     \n");
    io.outputstring("==============================================\n");

    // 1. Initialize Infrastructure
    io.outputstring("[INIT] Initializing Cellular Core (Limit: 200,000 msgs)...\n");
    // New (Enough overhead to handle the switching logic)
    CellularCore* core = new CellularCore(300000); 

    io.outputstring("[INIT] Erecting 5G Tower (ID: 501)...\n");
    io.outputstring("[INFO] Tower inherits 4G OFDM + 5G Massive MIMO capabilities.\n");
    Tower5G* tower = new Tower5G(501, core);
    
    int maxCap = tower->calculateMaxCapacity();
    io.outputstring("[INFO] Theoretical Max Capacity: ");
    io.outputint(maxCap);
    io.outputstring(" users.\n\n");

    // 2. Start Simulation Loop
    io.outputstring("[SIMULATION START] Connecting users...\n");
    
    int connectedCount = 0;
    io.outputstring("Enter number of users to simulate: ");
    int usersToSimulate = io.inputint(); 
    io.outputstring("\n");    
    for (int i = 0; i < usersToSimulate; i++) {
        UserDevice* u = new UserDevice(i);
        
        try {
            tower->addUser(u);
            connectedCount++;

            // REPORTING LOGIC: Don't print every user, just milestones
            
            // Milestone 1: First user
            if (i == 0) io.outputstring("[EVENT] First User Connected.\n");

            // Milestone 2: 4G Network Full (Around 12,000)
            if (i == 11999) {
                io.outputstring("[WARNING] 4G Sub-channels approaching limit...\n");
            }
            if (i == 12000) {
                 io.outputstring("[EVENT] 4G CAPACITY REACHED. Switching to 5G High-Freq Bands.\n");
            }

            // Milestone 3: Reporting progress every 4000 users
            if (i > 0 && i % 4000 == 0) {
                io.outputstring("[STATUS] Active Connections: ");
                io.outputint(connectedCount);
                io.outputstring("...\n");
            }

        } catch (CapacityExceededException& e) {
            // Milestone 4: Total System Failure
            io.outputstring("\n[CRITICAL] Network Overload at User ID: ");
            io.outputint(i);
            io.outputstring("\n[ERROR] Reason: ");
            io.errorstring(e.message);
            io.outputstring("\n");
            break; // Stop simulation
        }
    }

    io.outputstring("\n[SIMULATION END] Final Report:\n");
    io.outputstring("------------------------------\n");
    io.outputstring("Total Users Connected: ");
    io.outputint(connectedCount);
    io.outputstring("\n");
    
    // 3. Core Analysis Requirement
    int needed = tower->calculateRequiredCores(50000);
    io.outputstring("Computational Requirement: ");
    io.outputint(needed);
    io.outputstring(" Cores needed for stability.\n");
    io.outputstring("\n[REPORT] Users in First Channel (0-10 kHz):\n");
    auto* firstChannel = tower->getFirstChannel(); 

    if (firstChannel->getCount() == 0) {
        io.outputstring("None\n");
    } else {
        for (int i = 0; i < firstChannel->getCount(); i++) {
            UserDevice* u = firstChannel->getUser(i);
            if (u) {
                io.outputstring("User");
                io.outputint(u->getId());
                io.outputstring(" ");
            }
        }
        io.outputstring("\n");
    }
    return 0;
}