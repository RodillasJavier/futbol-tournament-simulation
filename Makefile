# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Directories
SRC_DIR = src
MODULES_DIR = modules
TEST_DIR = tests
BUILD_DIR = build
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
MODULE_FILES = $(wildcard $(MODULES_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

# Object files
SRC_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
MODULE_OBJS = $(patsubst $(MODULES_DIR)/%.c,$(BUILD_DIR)/%.o,$(MODULE_FILES))

# Test executables
TEST_PLAYER = $(BIN_DIR)/test_player
TEST_TEAM = $(BIN_DIR)/test_team
TEST_MATCH = $(BIN_DIR)/test_match

# Include directories
INCLUDES = -I$(SRC_DIR) -I$(MODULES_DIR)

# Phony targets
.PHONY: all clean dirs test_player test_team test_match tests

# Default target
all: dirs $(TEST_PLAYER) $(TEST_TEAM) $(TEST_MATCH)

# Create build directories
dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile module files
$(BUILD_DIR)/%.o: $(MODULES_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link test_player
$(TEST_PLAYER): $(BUILD_DIR)/test_player.o $(BUILD_DIR)/player.o
	$(CC) $(CFLAGS) $^ -o $@

# Link test_team
$(TEST_TEAM): $(BUILD_DIR)/test_team.o $(BUILD_DIR)/player.o $(BUILD_DIR)/team.o
	$(CC) $(CFLAGS) $^ -o $@

# Link test_match
$(TEST_MATCH): $(BUILD_DIR)/test_match.o $(BUILD_DIR)/player.o $(BUILD_DIR)/team.o $(BUILD_DIR)/match.o $(BUILD_DIR)/match_simulation.o
	$(CC) $(CFLAGS) $^ -o $@

# Compile test files
$(BUILD_DIR)/test_player.o: $(TEST_DIR)/test_player.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/test_team.o: $(TEST_DIR)/test_team.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/test_match.o: $(TEST_DIR)/test_match.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run tests
test_player: $(TEST_PLAYER)
	@echo "Running Player Test..."
	@./$(TEST_PLAYER)

test_team: $(TEST_TEAM)
	@echo "Running Team Test..."
	@./$(TEST_TEAM)

test_match: $(TEST_MATCH)
	@echo "Running Match Test..."
	@./$(TEST_MATCH)

# Run all tests
tests: test_player test_team test_match

# Clean build files
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned build directories"