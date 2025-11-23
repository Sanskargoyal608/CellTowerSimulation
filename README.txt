Cellular Network Simulator - Monsoon 2025
=========================================

Project Overview
----------------
This project simulates the evolution of cellular networks from 2G to 5G using Object-Oriented Programming (OOP) principles in C++. It manages user connections, message loads, and frequency channel allocations using custom memory management and direct system calls.

Architecture
------------
- **Framework**: Abstract Base Classes (CellTower, UserDevice, CellularCore).
- **OOP Features**: Inheritance, Polymorphism, Data Abstraction.
- **Templates**: Used for the 'Channel' class to manage frequency slots generic to the generation.
- **Exception Handling**: 'CapacityExceededException' handles network overloads.
- **IO**: Custom 'basicIO' library using 'syscall.S' (Assembly) for direct system interaction.

Build Instructions
------------------
The project includes a Makefile that generates two versions of the binary [Source: 32].

1. Open a terminal in the project root.
2. Run the make command:
   $ make

3. This will generate two executables:
   - ./simulator_debug  (Includes debugging symbols)
   - ./simulator_opt    (Optimized for execution speed)

Running the Simulation
----------------------
To run the simulation and view the 5G capacity results:

   $ ./simulator_opt

Input Format
------------
The simulation scenarios are driven by the main driver file (src/main.cpp). The logic instantiates specific generation towers (2G/3G/4G/5G) and simulates user loads to verify capacity and core requirements.

Credits
-------
IO Library: Provided BasicIO.h/cpp and syscall.S