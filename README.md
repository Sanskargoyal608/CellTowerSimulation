# Cellular Network Simulator
### OOPD Project — Monsoon 2025 | Group 23

> A C++ simulation of the evolution of cellular networks from **2G to 5G**, demonstrating core Object-Oriented Programming principles: inheritance, polymorphism, templates, and custom exception handling — all powered by a custom I/O library built on bare-metal assembly syscalls.

---

## Table of Contents
- [Overview](#overview)
- [OOP Concepts Demonstrated](#oop-concepts-demonstrated)
- [Network Architecture](#network-architecture)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Build Instructions](#build-instructions)
- [Running the Simulation](#running-the-simulation)
- [Sample Output](#sample-output)
- [Credits](#credits)

---

## Overview

The simulator models four generations of cellular technology, each with increasing capacity and complexity:

| Generation | Technology | Channels | Users/Channel | Max Users |
|:---:|:---:|:---:|:---:|:---:|
| **2G** | TDMA | 5 × 200 kHz | 16 | **80** |
| **3G** | CDMA | 5 × 200 kHz | 32 | **160** |
| **4G** | OFDM + MIMO (×4) | 100 × 10 kHz | 120 | **12,000** |
| **5G** | 4G OFDM + Massive MIMO (×16) | 100 + 10 channels | 120 / 480 | **16,800** |

Each scenario demonstrates:
- Connecting users up to maximum capacity
- Identifying users in the **first frequency channel**
- Throwing and catching `CapacityExceededException` on overload
- Computing the number of **computational cores** needed (4G and 5G)

---

## OOP Concepts Demonstrated

| Concept | Where |
|---|---|
| **Abstract Base Class** | `CellTower` — pure virtual `addUser()` and `calculateMaxCapacity()` |
| **Inheritance** | `Tower2G`, `Tower3G`, `Tower4G` all extend `CellTower`; `Tower5G` extends `Tower4G` |
| **Polymorphism** | Each tower overrides `addUser()` with generation-specific channel logic |
| **Templates** | `Channel<T, LIMIT>` — a generic, compile-time-bounded frequency channel |
| **Exception Handling** | `CapacityExceededException` thrown when channel or core limits are exceeded |
| **Data Abstraction** | `CellularCore` hides message-load tracking behind a simple `addMessageLoad()` interface |
| **Custom Memory** | No `<iostream>` or standard I/O — all output via assembly `syscall` |

---

## Network Architecture

```
                    ┌─────────────────────────────────────────┐
                    │              CellTower (ABC)             │
                    │  + towerId : int                         │
                    │  + core    : CellularCore*               │
                    │  # addUser(UserDevice*) = 0 (virtual)   │
                    │  # calculateMaxCapacity() = 0 (virtual) │
                    └────────────┬────────────────────────────┘
                                 │ inherits
          ┌──────────────────────┼────────────────────────┐
          │                      │                        │
    ┌─────▼──────┐         ┌─────▼──────┐         ┌──────▼──────┐
    │  Tower2G   │         │  Tower3G   │         │   Tower4G   │
    │  TDMA      │         │  CDMA      │         │   OFDM+MIMO │
    │  80 users  │         │  160 users │         │  12,000 usr │
    └────────────┘         └────────────┘         └──────┬──────┘
                                                         │ inherits
                                                   ┌─────▼──────┐
                                                   │   Tower5G  │
                                                   │ 4G + MMIMO │
                                                   │ 16,800 usr │
                                                   └────────────┘

  Each tower manages an array of:  Channel<UserDevice*, LIMIT>
  CellularCore tracks total message load across all active users.
```

---

## Project Structure

```
Group-23-project/
├── include/
│   ├── CellTower.h                 # Abstract base class (contract)
│   ├── CellularCore.h              # Core message-load manager
│   ├── UserDevice.h                # Device model
│   ├── Channel.h                   # Template frequency channel
│   ├── CapacityExceededException.h # Custom exception
│   ├── Tower2G.h                   # TDMA  — 80  users max
│   ├── Tower3G.h                   # CDMA  — 160 users max
│   ├── Tower4G.h                   # OFDM+MIMO — 12,000 users max
│   ├── Tower5G.h                   # Massive MIMO — 16,800 users max
│   └── basicIO.h                   # Custom I/O library header
├── src/
│   ├── basicIO.cpp                 # I/O implementation (uses syscalls)
│   └── main.cpp                    # Simulation driver (all 4 scenarios)
├── syscall.S                       # ARM64 assembly syscall wrappers
├── Makefile                        # Builds debug + optimised binaries
└── README.md
```

---

## Prerequisites

| Tool | Purpose | Install |
|---|---|---|
| `g++` / `clang++` | C++17 compiler | Xcode Command Line Tools (`xcode-select --install`) |
| `as` | Apple clang assembler | Included with Xcode CLT |
| `make` | Build system | Included with Xcode CLT |

> **Platform note:** This project targets **macOS (arm64 / Apple Silicon)**. The assembly file (`syscall.S`) uses the macOS BSD syscall ABI and must be assembled with Apple's `as`. It will not work out-of-the-box on Linux x86_64.

---

## Build Instructions

```bash
# 1. Clone the repository
git clone https://github.com/Sanskargoyal608/CellTowerSimulation.git
cd CellTowerSimulation

# 2. Build both debug and optimised binaries
make

# Generated binaries:
#   ./simulator_debug   — with full debugging symbols (-g)
#   ./simulator_opt     — speed-optimised build (-O2)

# 3. Clean build artefacts
make clean
```

---

## Running the Simulation

```bash
./simulator_opt
```

The program runs four sequential scenarios automatically. When it reaches the **5G interactive scenario**, it will prompt:

```
Enter number of users to simulate (max 16800):
```

Enter any number (e.g. `15000`) and press Enter.

**Quick non-interactive run:**
```bash
echo "15000" | ./simulator_opt
```

---

## Sample Output

```
##############################################################
#       CELLULAR NETWORK SIMULATOR  —  OOPD 2025            #
#  2G (TDMA)  |  3G (CDMA)  |  4G (OFDM)  |  5G (MMIMO)   #
##############################################################

================================================================
  SCENARIO 1 : 2G Tower (TDMA)
================================================================
[INFO] Technology    : TDMA (Time-Division Multiple Access)
[INFO] Configuration : 5 channels x 16 users/channel
[INFO] Max Capacity  : 80 users

[SIM] Connecting users up to max capacity...
[SIM] Users connected: 80 / 80

[REPORT] Users in first 2G channel (0.0–0.2 MHz):
  Count : 16 users
  User#0  User#1  ... User#15

[TEST] Attempting to add one more user beyond capacity...
[OK] CapacityExceededException caught: All 2G Frequency Channels Full

================================================================
  SCENARIO 3 : 4G Tower (OFDM + MIMO)
================================================================
[INFO] Max Capacity  : 12000 users

[REPORT] Computational Core Requirement:
  Total messages at full load : 120000
  Single-core capacity        : 50,000 msg/s
  Cores needed for stability  : 3

================================================================
  SCENARIO 4 : 5G Hybrid Tower (4G OFDM + 5G Massive MIMO)
================================================================
[INFO] Max Capacity  : 16800 users

[EVENT] First user connected.
[STATUS] Active connections: 4000
...
[WARN]  4G sub-channels approaching capacity...
[EVENT] 4G FULL -> Switching to 5G High-Frequency bands.
...
[SIMULATION END]
  Total users connected : 15000

[REPORT] Users in first 5G High-Frequency channel (1800 MHz):
  Count : 480 users

[REPORT] Computational Core Requirement:
  Cores needed for stability  : 4

##############################################################
#                  SIMULATION COMPLETE                      #
##############################################################
```

---

## Credits

- **Custom I/O Library** (`basicIO.h`, `basicIO.cpp`, `syscall.S`): Provided as part of the OOPD 2025 course infrastructure.
- **Project Design & Implementation**: Group 23, OOPD Monsoon 2025.