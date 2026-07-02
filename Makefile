# Compiler
CXX := g++

# Project name
TARGET_NAME := game.exe

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin

# Find all .cpp files in src/
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# SFML and Local Include paths
SFML_INCLUDE := /mingw64/include
SFML_LIB := /mingw64/lib

# Compiler & Linker Flags (CRITICAL: Added -I$(INC_DIR) and -I$(SFML_INCLUDE) here)
CXXFLAGS := -std=c++17 -Wall -Wextra -I$(INC_DIR) -I$(SFML_INCLUDE)
LDFLAGS := -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system

# Target path
TARGET := $(BIN_DIR)/$(TARGET_NAME)

# Default target
all: $(TARGET)

# Link step: matches object files to binary
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile step: compiles individual .cpp files to .o files using CXXFLAGS
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories safely using Windows-compatible fallback
$(BUILD_DIR):
	@mkdir $(BUILD_DIR) 2>nul || mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir $(BIN_DIR) 2>nul || mkdir -p $(BIN_DIR)

# Clean
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR) 2>nul || rmdir /s /q $(BUILD_DIR) $(BIN_DIR) 2>nul || true

# Run
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
