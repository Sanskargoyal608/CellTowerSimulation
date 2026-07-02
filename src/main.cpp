#include <basicIO.h>
#include "CellularCore.h"
#include "UserDevice.h"
#include "Tower2G.h"
#include "Tower3G.h"
#include "Tower4G.h"
#include "Tower5G.h"
#include "CapacityExceededException.h"

// ============================================================
//  Helper: print a section divider
// ============================================================
static void printDivider(const char* title) {
    io.outputstring("\n");
    io.outputstring("================================================================\n");
    io.outputstring("  ");
    io.outputstring(title);
    io.outputstring("\n");
    io.outputstring("================================================================\n");
}

// ============================================================
//  Helper: print all users in a channel (generic via count/id)
// ============================================================
template<int LIMIT>
static void printChannelUsers(Channel<UserDevice*, LIMIT>* ch) {
    if (ch->getCount() == 0) {
        io.outputstring("  (none)\n");
        return;
    }
    for (int i = 0; i < ch->getCount(); i++) {
        UserDevice* u = ch->getUser(i);
        if (u) {
            io.outputstring("  User#");
            io.outputint(u->getId());
            io.outputstring("\n");
        }
    }
}

// ============================================================
//  SCENARIO 1 — 2G (TDMA)
//  5 channels x 16 users = 80 max users
// ============================================================
static void run2GScenario() {
    printDivider("SCENARIO 1 : 2G Tower (TDMA)");

    CellularCore* core2G = new CellularCore(10000); // generous overhead budget
    Tower2G* tower2G    = new Tower2G(101, core2G);

    int maxCap = tower2G->calculateMaxCapacity();
    io.outputstring("[INFO] Technology    : TDMA (Time-Division Multiple Access)\n");
    io.outputstring("[INFO] Configuration : 5 channels x 16 users/channel\n");
    io.outputstring("[INFO] Max Capacity  : ");
    io.outputint(maxCap);
    io.outputstring(" users\n\n");

    // --- Fill to max capacity ---
    io.outputstring("[SIM] Connecting users up to max capacity...\n");
    int connected = 0;
    for (int i = 0; i < maxCap; i++) {
        UserDevice* u = new UserDevice(i);
        try {
            tower2G->addUser(u);
            connected++;
        } catch (CapacityExceededException& e) {
            io.outputstring("[ERROR] Unexpected exception: ");
            io.errorstring(e.message);
            io.outputstring("\n");
            break;
        }
    }
    io.outputstring("[SIM] Users connected: ");
    io.outputint(connected);
    io.outputstring(" / ");
    io.outputint(maxCap);
    io.outputstring("\n");

    // --- Print users in first channel ---
    io.outputstring("\n[REPORT] Users in first 2G channel (0.0–0.2 MHz):\n");
    Channel<UserDevice*, 16>* ch2G = tower2G->getFirstChannel();
    io.outputstring("  Count : ");
    io.outputint(ch2G->getCount());
    io.outputstring(" users\n");
    printChannelUsers<16>(ch2G);

    // --- Demonstrate exception on overload ---
    io.outputstring("\n[TEST] Attempting to add one more user beyond capacity...\n");
    try {
        UserDevice* extra = new UserDevice(9999);
        tower2G->addUser(extra);
        io.outputstring("[FAIL] No exception thrown — unexpected!\n");
    } catch (CapacityExceededException& e) {
        io.outputstring("[OK] CapacityExceededException caught: ");
        io.errorstring(e.message);
        io.outputstring("\n");
    }

    delete tower2G;
    delete core2G;
}

// ============================================================
//  SCENARIO 2 — 3G (CDMA)
//  5 channels x 32 users = 160 max users
// ============================================================
static void run3GScenario() {
    printDivider("SCENARIO 2 : 3G Tower (CDMA)");

    CellularCore* core3G = new CellularCore(10000);
    Tower3G* tower3G    = new Tower3G(201, core3G);

    int maxCap = tower3G->calculateMaxCapacity();
    io.outputstring("[INFO] Technology    : CDMA (Code-Division Multiple Access)\n");
    io.outputstring("[INFO] Configuration : 5 channels x 32 users/channel\n");
    io.outputstring("[INFO] Max Capacity  : ");
    io.outputint(maxCap);
    io.outputstring(" users\n\n");

    // --- Fill to max capacity ---
    io.outputstring("[SIM] Connecting users up to max capacity...\n");
    int connected = 0;
    for (int i = 0; i < maxCap; i++) {
        UserDevice* u = new UserDevice(i);
        try {
            tower3G->addUser(u);
            connected++;
        } catch (CapacityExceededException& e) {
            io.outputstring("[ERROR] Unexpected exception: ");
            io.errorstring(e.message);
            io.outputstring("\n");
            break;
        }
    }
    io.outputstring("[SIM] Users connected: ");
    io.outputint(connected);
    io.outputstring(" / ");
    io.outputint(maxCap);
    io.outputstring("\n");

    // --- Print users in first channel ---
    io.outputstring("\n[REPORT] Users in first 3G channel (0.0–0.2 MHz):\n");
    Channel<UserDevice*, 32>* ch3G = tower3G->getFirstChannel();
    io.outputstring("  Count : ");
    io.outputint(ch3G->getCount());
    io.outputstring(" users\n");
    printChannelUsers<32>(ch3G);

    // --- Demonstrate exception on overload ---
    io.outputstring("\n[TEST] Attempting to add one more user beyond capacity...\n");
    try {
        UserDevice* extra = new UserDevice(9999);
        tower3G->addUser(extra);
        io.outputstring("[FAIL] No exception thrown — unexpected!\n");
    } catch (CapacityExceededException& e) {
        io.outputstring("[OK] CapacityExceededException caught: ");
        io.errorstring(e.message);
        io.outputstring("\n");
    }

    delete tower3G;
    delete core3G;
}

// ============================================================
//  SCENARIO 3 — 4G (OFDM + MIMO)
//  100 channels x 120 users = 12,000 max users
// ============================================================
static void run4GScenario() {
    printDivider("SCENARIO 3 : 4G Tower (OFDM + MIMO)");

    CellularCore* core4G = new CellularCore(200000);
    Tower4G* tower4G    = new Tower4G(301, core4G);

    int maxCap = tower4G->calculateMaxCapacity();
    io.outputstring("[INFO] Technology    : OFDM + MIMO (4 antennas)\n");
    io.outputstring("[INFO] Configuration : 100 channels x 120 users/channel\n");
    io.outputstring("[INFO] Max Capacity  : ");
    io.outputint(maxCap);
    io.outputstring(" users\n\n");

    // --- Fill to max capacity ---
    io.outputstring("[SIM] Connecting all ");
    io.outputint(maxCap);
    io.outputstring(" users (this may take a moment)...\n");
    int connected = 0;
    for (int i = 0; i < maxCap; i++) {
        UserDevice* u = new UserDevice(i);
        try {
            tower4G->addUser(u);
            connected++;
        } catch (CapacityExceededException& e) {
            io.outputstring("[ERROR] Unexpected exception at user ");
            io.outputint(i);
            io.outputstring(": ");
            io.errorstring(e.message);
            io.outputstring("\n");
            break;
        }
    }
    io.outputstring("[SIM] Users connected: ");
    io.outputint(connected);
    io.outputstring(" / ");
    io.outputint(maxCap);
    io.outputstring("\n");

    // --- Print users in first channel ---
    io.outputstring("\n[REPORT] Users in first 4G channel (0.00–0.01 MHz):\n");
    Channel<UserDevice*, 120>* ch4G = tower4G->getFirstChannel();
    io.outputstring("  Count : ");
    io.outputint(ch4G->getCount());
    io.outputstring(" users\n");
    printChannelUsers<120>(ch4G);

    // --- Computational core requirement ---
    int cores = tower4G->calculateRequiredCores(50000);
    io.outputstring("\n[REPORT] Computational Core Requirement:\n");
    io.outputstring("  Total messages at full load : ");
    io.outputint(maxCap * 10);
    io.outputstring("\n");
    io.outputstring("  Single-core capacity        : 50,000 msg/s\n");
    io.outputstring("  Cores needed for stability  : ");
    io.outputint(cores);
    io.outputstring("\n");

    delete tower4G;
    delete core4G;
}

// ============================================================
//  SCENARIO 4 — 5G Hybrid (4G OFDM + Massive MIMO, interactive)
//  4G: 100 ch x 120 = 12,000 | 5G: 10 ch x 480 = 4,800 | Total = 16,800
// ============================================================
static void run5GScenario() {
    printDivider("SCENARIO 4 : 5G Hybrid Tower (4G OFDM + 5G Massive MIMO)");

    CellularCore* core5G = new CellularCore(400000); // headroom for full 5G load
    Tower5G* tower5G    = new Tower5G(501, core5G);

    int maxCap = tower5G->calculateMaxCapacity();
    io.outputstring("[INFO] Technology    : 4G OFDM + 5G Massive MIMO (16 antennas)\n");
    io.outputstring("[INFO] 4G portion    : 100 channels x 120 users = 12,000\n");
    io.outputstring("[INFO] 5G HF portion : 10 channels  x 480 users =  4,800\n");
    io.outputstring("[INFO] Max Capacity  : ");
    io.outputint(maxCap);
    io.outputstring(" users\n\n");

    // --- Interactive user count input ---
    io.outputstring("Enter number of users to simulate (max ");
    io.outputint(maxCap);
    io.outputstring("): ");
    int usersToSimulate = io.inputint();
    io.outputstring("\n");

    io.outputstring("[SIMULATION START] Connecting users...\n");
    int connected = 0;
    for (int i = 0; i < usersToSimulate; i++) {
        UserDevice* u = new UserDevice(i);
        try {
            tower5G->addUser(u);
            connected++;

            if (i == 0)
                io.outputstring("[EVENT] First user connected.\n");

            if (i == 11999)
                io.outputstring("[WARN]  4G sub-channels approaching capacity...\n");

            if (i == 12000)
                io.outputstring("[EVENT] 4G FULL -> Switching to 5G High-Frequency bands.\n");

            if (i > 0 && i % 4000 == 0) {
                io.outputstring("[STATUS] Active connections: ");
                io.outputint(connected);
                io.outputstring("\n");
            }

        } catch (CapacityExceededException& e) {
            io.outputstring("\n[CRITICAL] Network overload at user ID ");
            io.outputint(i);
            io.outputstring("\n[ERROR]    Reason: ");
            io.errorstring(e.message);
            io.outputstring("\n");
            break;
        }
    }

    io.outputstring("\n[SIMULATION END]\n");
    io.outputstring("  Total users connected : ");
    io.outputint(connected);
    io.outputstring("\n");

    // --- Users in first 5G high-frequency channel ---
    io.outputstring("\n[REPORT] Users in first 5G High-Frequency channel (1800 MHz):\n");
    Channel<UserDevice*, 480>* ch5G = tower5G->getFirstHighFreqChannel();
    io.outputstring("  Count : ");
    io.outputint(ch5G->getCount());
    io.outputstring(" users\n");
    if (ch5G->getCount() == 0) {
        io.outputstring("  (no 5G spillover — all users fit in 4G)\n");
    } else {
        for (int i = 0; i < ch5G->getCount(); i++) {
            UserDevice* u = ch5G->getUser(i);
            if (u) {
                io.outputstring("  User#");
                io.outputint(u->getId());
                io.outputstring("\n");
            }
        }
    }

    // --- Computational core requirement ---
    int cores = tower5G->calculateRequiredCores(50000);
    io.outputstring("\n[REPORT] Computational Core Requirement:\n");
    io.outputstring("  Total messages at full load : ");
    io.outputint(maxCap * 10);
    io.outputstring("\n");
    io.outputstring("  Single-core capacity        : 50,000 msg/s\n");
    io.outputstring("  Cores needed for stability  : ");
    io.outputint(cores);
    io.outputstring("\n");

    delete tower5G;
    delete core5G;
}

// ============================================================
//  MAIN DRIVER
// ============================================================
int main() {
    io.outputstring("##############################################################\n");
    io.outputstring("#       CELLULAR NETWORK SIMULATOR  —  OOPD 2025            #\n");
    io.outputstring("#  2G (TDMA)  |  3G (CDMA)  |  4G (OFDM)  |  5G (MMIMO)   #\n");
    io.outputstring("##############################################################\n");

    run2GScenario();
    run3GScenario();
    run4GScenario();
    run5GScenario();

    io.outputstring("\n##############################################################\n");
    io.outputstring("#                  SIMULATION COMPLETE                      #\n");
    io.outputstring("##############################################################\n");
    return 0;
}