# ==========================================
# Platform Detection
# ==========================================
ifeq ($(OS),Windows_NT)
    # Windows (MSYS2 / MinGW)
    TARGET    := DoodleJumpClone.out
    SFML_INC  := 
    SFML_LIB  := 
else
    # macOS
    TARGET    := DoodleJumpClone.out
    # Homebrew installs SFML here on Apple Silicon Macs (M1/M2/M3/M4)
    SFML_INC  := -I/opt/homebrew/include
    SFML_LIB  := -L/opt/homebrew/lib
endif

# ==========================================
# Compiler & Flags
# ==========================================
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude $(SFML_INC) -MMD -MP
LDFLAGS  := $(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

# ==========================================
# Output & Directories
# ==========================================
SRC_DIR   := src
BUILD_DIR := build

# ==========================================
# File Discovery (Recursive)
# ==========================================
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES   := $(call rwildcard,$(SRC_DIR),*.cpp)
OBJECTS   := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS      := $(OBJECTS:.o=.d)

# ==========================================
# Build Rules
# ==========================================
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking binary: $@"
	@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build successful!"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	@echo "Cleaning build directory and target..."
	@rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
