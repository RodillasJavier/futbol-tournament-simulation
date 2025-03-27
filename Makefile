# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Directory variables
SRC_DIR = src
MODULES_DIR = $(SRC_DIR)/modules
UTILS_DIR = $(SRC_DIR)/utils
TEST_DIR = tests
BUILD_DIR = build
BIN_DIR = bin

# Include directories -- 	Set include flags to look in our source and module
# 							directories for any header files
INCLUDES = -I$(SRC_DIR) -I$(MODULES_DIR) -I$(UTILS_DIR)



# Source files --	Finds all c files in each directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
MODULE_FILES = $(wildcard $(MODULES_DIR)/*.c)
UTILS_FILES = $(wildcard $(UTILS_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

# Object files --	Convert file paths from dir/foo.c to build/foo.o
SRC_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
MODULE_OBJS = $(patsubst $(MODULES_DIR)/%.c,$(BUILD_DIR)/%.o,$(MODULE_FILES))
UTILS_OBJS = $(patsubst $(UTILS_DIR)/%.c,$(BUILD_DIR)/%.o,$(UTILS_FILES))

# Test executables --	file paths for our executables that we generate, stored
#						in our bin directory
TEST_PLAYER = $(BIN_DIR)/test_player
TEST_TEAM = $(BIN_DIR)/test_team
TEST_MATCH = $(BIN_DIR)/test_match
TEST_LEAGUE = $(BIN_DIR)/test_league
TEST_TOURNAMENT = $(BIN_DIR)/test_tournament

# Main executable
MAIN = $(BIN_DIR)/main



# Phony targets --	Commands for our make file
.PHONY: all clean dirs test_player test_team test_match test_league test_tournament tests main

# Default target --	when running 'make' from command line, just build all
all: dirs $(TEST_PLAYER) $(TEST_TEAM) $(TEST_MATCH) $(TEST_LEAGUE) $(TEST_TOURNAMENT) $(MAIN)



# Create our 'build' and 'bin' directories
dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)



# Compilation --	Compile any .c files in 'source' or 'module' directories into
# 					.o files in build. '-c $< -o $@' ~ "-c input to -o output"
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD_DIR)/%.o: $(MODULES_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD_DIR)/%.o: $(UTILS_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@



# Linking test files --	Link test and class object files together to make test
# 						executables. '$^ -o $@' ~ link all dependency objects 
# 						to output an executable.
$(TEST_PLAYER): $(BUILD_DIR)/test_player.o $(BUILD_DIR)/player.o
	$(CC) $(CFLAGS) $^ -o $@
$(TEST_TEAM): $(BUILD_DIR)/test_team.o $(BUILD_DIR)/player.o $(BUILD_DIR)/team.o
	$(CC) $(CFLAGS) $^ -o $@
$(TEST_MATCH): $(BUILD_DIR)/test_match.o $(BUILD_DIR)/player.o $(BUILD_DIR)/team.o $(BUILD_DIR)/match.o $(BUILD_DIR)/random_utils.o $(BUILD_DIR)/match_simulation.o
	$(CC) $(CFLAGS) $^ -o $@
$(TEST_LEAGUE): $(BUILD_DIR)/test_league.o $(BUILD_DIR)/player.o $(BUILD_DIR)/team.o $(BUILD_DIR)/match.o $(BUILD_DIR)/league.o $(BUILD_DIR)/random_utils.o $(BUILD_DIR)/match_simulation.o
	$(CC) $(CFLAGS) $^ -o $@
$(TEST_TOURNAMENT): $(BUILD_DIR)/test_tournament.o $(BUILD_DIR)/player.o $(BUILD_DIR)/team.o $(BUILD_DIR)/match.o $(BUILD_DIR)/league.o $(BUILD_DIR)/tournament.o $(BUILD_DIR)/random_utils.o $(BUILD_DIR)/match_simulation.o
	$(CC) $(CFLAGS) -lm $^ -o $@

# Linking main file
$(MAIN): $(BUILD_DIR)/main.o $(BUILD_DIR)/player.o $(BUILD_DIR)/team.o $(BUILD_DIR)/match.o $(BUILD_DIR)/league.o $(BUILD_DIR)/tournament.o $(BUILD_DIR)/random_utils.o $(BUILD_DIR)/match_simulation.o
	$(CC) $(CFLAGS) -lm $^ -o $@



# Compile test files --	Compile test c files into test object files: 
# 						'test/test.c => build/test.o'
$(BUILD_DIR)/test_player.o: $(TEST_DIR)/test_player.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD_DIR)/test_team.o: $(TEST_DIR)/test_team.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD_DIR)/test_match.o: $(TEST_DIR)/test_match.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD_DIR)/test_league.o: $(TEST_DIR)/test_league.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(BUILD_DIR)/test_tournament.o: $(TEST_DIR)/test_tournament.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile main file
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
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

test_league: $(TEST_LEAGUE)
	@echo "Running League Test..."
	@./$(TEST_LEAGUE)

test_tournament: $(TEST_TOURNAMENT)
	@echo "Running Tournament Test..."
	@./$(TEST_TOURNAMENT)

# Run all tests
tests: test_player test_team test_match test_league test_tournament

# Main target
main: $(MAIN)
	@echo "Running Main Program..."
	@./$(MAIN)

# Clean build files
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned build directories"