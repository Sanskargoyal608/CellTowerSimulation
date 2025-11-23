# Compiler and Assembler
CXX = g++
ASM = nasm

# Common Flags
# -fexceptions: Required for try/catch [cite: 30]
# -I./include: Tells compiler where header files are
COMMON_FLAGS = -I./include -fexceptions

# Debug Flags: -g for symbols
CXXFLAGS_DEBUG = -g $(COMMON_FLAGS)

# Optimized Flags: -O2 for speed 
CXXFLAGS_OPT = -O2 $(COMMON_FLAGS)

# Source Files
SRCS_CPP = src/basicIO.cpp src/main.cpp
SRCS_ASM = syscall.S

# Object Files (We need separate objects for debug vs opt to avoid conflicts)
OBJS_DEBUG = $(SRCS_CPP:.cpp=.o) $(SRCS_ASM:.S=.o)
OBJS_OPT = $(SRCS_CPP:.cpp=.opt.o) $(SRCS_ASM:.S=.opt.o)

# Binary Names
TARGET_DEBUG = simulator_debug
TARGET_OPT = simulator_opt

# Default Target: Build Both
all: $(TARGET_DEBUG) $(TARGET_OPT)

# --- DEBUG BUILD ---
$(TARGET_DEBUG): $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) -o $(TARGET_DEBUG) $(OBJS_DEBUG)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@

%.o: %.S
	$(ASM) -f elf64 $< -o $@

# --- OPTIMIZED BUILD ---
$(TARGET_OPT): $(OBJS_OPT)
	$(CXX) $(CXXFLAGS_OPT) -o $(TARGET_OPT) $(OBJS_OPT)

src/%.opt.o: src/%.cpp
	$(CXX) $(CXXFLAGS_OPT) -c $< -o $@

%.opt.o: %.S
	$(ASM) -f elf64 $< -o $@

# Cleanup
clean:
	rm -f src/*.o src/*.opt.o *.o *.opt.o $(TARGET_DEBUG) $(TARGET_OPT)