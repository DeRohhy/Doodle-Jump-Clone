# AI Generated

CXX = g++ # The compiler we use

# Detect OS
UNAME_S := $(shell uname -s)

# Flags: C++20 standard, show warnings, look in include/ for headers, and auto-generate .d dependency files
CXXFLAGS = -std=c++20 -Wall -Iinclude -MMD -MP 

# Find every .cpp under src/, automatically
SRCS = $(shell find src -name '*.cpp')

# Turn each src/foo.cpp into build/foo.o
OBJS = $(SRCS:src/%.cpp=build/%.o)

# The .d (dependency) files Make will auto-generate
DEPS = $(OBJS:.o=.d)

# --- SFML Libraries ---
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# --- Platform-specific settings ---
ifeq ($(UNAME_S),Darwin)
    # macOS (Homebrew paths)
    # Check for Apple Silicon vs Intel
    ifeq ($(shell uname -m),arm64)
        SFML_PREFIX = /opt/homebrew
    else
        SFML_PREFIX = /usr/local
    endif
    CXXFLAGS += -I$(SFML_PREFIX)/include
    LIBS := -L$(SFML_PREFIX)/lib $(LIBS)
    TARGET = DoodleJumpClone
else ifeq ($(OS),Windows_NT)
    # Windows (MinGW)
    TARGET = DoodleJumpClone.exe
else
    # Linux or other Unix
    TARGET = DoodleJumpClone
endif

# --- Link phase: all .o files into the program ---
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS)

# --- Compile phase: one pattern rule for ALL files ---
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pull in the auto-generated header dependencies
-include $(DEPS)

clean:
ifeq ($(OS),Windows_NT)
	if exist build rmdir /s /q build
	if exist $(TARGET) del $(TARGET)
else
	rm -rf build $(TARGET)
endif

.PHONY: clean
