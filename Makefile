# Compiler
CXX := g++

# Detect OS
ifeq ($(OS),Windows_NT)
    PLATFORM := Windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        PLATFORM := macOS
    endif
endif

# Project name
TARGET := game

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Compiler flags
CXXFLAGS := -std=c++17 -Wall -Wextra

# Platform-specific settings
ifeq ($(PLATFORM),Windows)
    SFML_INCLUDE := /mingw64/include
    SFML_LIB := /mingw64/lib
    LDFLAGS := -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system
    TARGET := $(BIN_DIR)/$(TARGET).exe
else ifeq ($(PLATFORM),macOS)
    SFML_INCLUDE := /opt/homebrew/include
    SFML_LIB := /opt/homebrew/lib
    LDFLAGS := -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system
    TARGET := $(BIN_DIR)/$(TARGET)
endif

CXXFLAGS += -I$(SFML_INCLUDE)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
