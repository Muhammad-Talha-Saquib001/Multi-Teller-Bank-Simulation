# Project Configuration
PROJECT     := bank_simulation
CC          := gcc
CFLAGS      := -Wall -Wextra -I./include -std=c11
LDFLAGS     := -lpthread
BUILD_DIR   := ./bin
SRC_DIR     := ./src
INCLUDE_DIR := ./include

# Targets
TARGET_TELLER   := $(BUILD_DIR)/teller
TARGET_CUSTOMER := $(BUILD_DIR)/customer

# Sources
SRC_TELLER   := $(SRC_DIR)/teller/main.c $(SRC_DIR)/ipc/named_pipe.c
SRC_CUSTOMER := $(SRC_DIR)/customer/main.c $(SRC_DIR)/ipc/named_pipe.c

# Ensure build directory exists
$(shell mkdir -p $(BUILD_DIR))

# Default target
all: teller customer

# Teller executable
teller: $(SRC_TELLER) $(INCLUDE_DIR)/*.h
	@echo "Building teller system..."
	$(CC) $(CFLAGS) $(SRC_TELLER) -o $(TARGET_TELLER) $(LDFLAGS)

# Customer executable
customer: $(SRC_CUSTOMER) $(INCLUDE_DIR)/*.h
	@echo "Building customer process..."
	$(CC) $(CFLAGS) $(SRC_CUSTOMER) -o $(TARGET_CUSTOMER) $(LDFLAGS)

# Clean build artifacts
clean:
	@echo "Cleaning project..."
	rm -rf $(BUILD_DIR)/*

# Phony targets
.PHONY: all teller customer clean