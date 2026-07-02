# ================================================================
#  Makefile — Cellular Network Simulator (OOPD 2025)
#  Targets: simulator_debug (with symbols) | simulator_opt (O2)
#
#  Platform: macOS arm64 (Apple Silicon)
#  Assembler: Apple clang 'as' (arm64, Mach-O)
#  Compiler : g++ / clang++
# ================================================================

CXX    = g++
ASM    = as

# -fexceptions: required for try/catch
# -I./include: header search path
COMMON_FLAGS   = -I./include -fexceptions

CXXFLAGS_DEBUG = -g   $(COMMON_FLAGS)
CXXFLAGS_OPT   = -O2  $(COMMON_FLAGS)

# macOS arm64 assembler flags
ASMFLAGS = 

# Source files
SRCS_CPP = src/basicIO.cpp src/main.cpp
SRCS_ASM = syscall.S

# Object files — separate sets to avoid debug/opt conflicts
OBJS_DEBUG = $(SRCS_CPP:.cpp=.o)  syscall.o
OBJS_OPT   = $(SRCS_CPP:.cpp=.opt.o) syscall.opt.o

TARGET_DEBUG = simulator_debug
TARGET_OPT   = simulator_opt

# ----------------------------------------------------------------
# Default: build both
# ----------------------------------------------------------------
all: $(TARGET_DEBUG) $(TARGET_OPT)

# ----------------------------------------------------------------
# Debug build
# ----------------------------------------------------------------
$(TARGET_DEBUG): $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) -o $@ $(OBJS_DEBUG)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@

syscall.o: syscall.S
	$(ASM) $(ASMFLAGS) -o $@ $<

# ----------------------------------------------------------------
# Optimised build
# ----------------------------------------------------------------
$(TARGET_OPT): $(OBJS_OPT)
	$(CXX) $(CXXFLAGS_OPT) -o $@ $(OBJS_OPT)

src/%.opt.o: src/%.cpp
	$(CXX) $(CXXFLAGS_OPT) -c $< -o $@

syscall.opt.o: syscall.S
	$(ASM) $(ASMFLAGS) -o $@ $<

# ----------------------------------------------------------------
# Cleanup
# ----------------------------------------------------------------
clean:
	rm -f src/*.o src/*.opt.o *.o *.opt.o $(TARGET_DEBUG) $(TARGET_OPT)