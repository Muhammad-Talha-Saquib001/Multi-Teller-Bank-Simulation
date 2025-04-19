# =============================================
# Multi-Teller Bank Simulation (IPC Version)
# =============================================

# Compiler and Flags
CC       = gcc
CFLAGS   = -Wall -Wextra -pedantic -std=c11 -I./include
LDFLAGS  = -lpthread
DEBUG    = -g  # Debug symbols (remove for release)

# Directories
SRCDIR   = src
INCDIR   = include
BUILDDIR = build
BINDIR   = bin

# Targets
MANAGER_TARGET = $(BINDIR)/bank_manager
CUSTOMER_TARGET = $(BINDIR)/bank_customer

# Manager (Server) Sources
MANAGER_SRCS = $(SRCDIR)/manager/bank_manager.c \
               $(SRCDIR)/manager/terminal_handler.c \
               $(SRCDIR)/common/ipc_utils.c \
               $(SRCDIR)/common/logging.c

# Customer (Client) Sources
CUSTOMER_SRCS = $(SRCDIR)/client/bank_customer.c \
                $(SRCDIR)/common/ipc_utils.c \
                $(SRCDIR)/common/logging.c

# Object Files
MANAGER_OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(MANAGER_SRCS))
CUSTOMER_OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(CUSTOMER_SRCS))

# Default Target (build both)
all: directories $(MANAGER_TARGET) $(CUSTOMER_TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILDDIR)/manager $(BUILDDIR)/client $(BUILDDIR)/common $(BINDIR)

# Build Manager (Server)
$(MANAGER_TARGET): $(MANAGER_OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ $(LDFLAGS)
	@echo "[+] Built server executable: $(MANAGER_TARGET)"

# Build Customer (Client)
$(CUSTOMER_TARGET): $(CUSTOMER_OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@
	@echo "[+] Built client executable: $(CUSTOMER_TARGET)"

# Compile Manager Objects
$(BUILDDIR)/manager/%.o: $(SRCDIR)/manager/%.c
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

# Compile Client Objects
$(BUILDDIR)/client/%.o: $(SRCDIR)/client/%.c
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

# Compile Common Utilities
$(BUILDDIR)/common/%.o: $(SRCDIR)/common/%.c
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

# Clean build artifacts
clean:
	@rm -rf $(BUILDDIR) $(BINDIR)
	@echo "[+] Cleaned all build artifacts."

# Run Manager (Server)
run-server: $(MANAGER_TARGET)
	@./$(MANAGER_TARGET)

# Run Customer (Client) - Example
run-client: $(CUSTOMER_TARGET)
	@./$(CUSTOMER_TARGET) 1 2000  # Args: customer_id service_time_ms

.PHONY: all directories clean run-server run-client