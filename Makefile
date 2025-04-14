# =============================================
# Multi-Teller Bank Simulation - Makefile
# =============================================

# Compiler and Flags
CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11 -I./include
LDFLAGS = -lpthread

# Directories
SRCDIR  = src
INCDIR  = include
BUILDDIR= build
BINDIR  = bin

# Targets
TARGET  = $(BINDIR)/bank_simulation

# Source and Object Files
SRCS    = $(wildcard $(SRCDIR)/*.c)
OBJS    = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

# Default Target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILDDIR) $(BINDIR)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Build successful! Executable: $(TARGET)"

# Compile each .c file to .o
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@rm -rf $(BUILDDIR) $(BINDIR)
	@echo "Cleaned build artifacts."

# Run the program (after building)
run: all
	@./$(TARGET)

# Phony targets (not actual files)
.PHONY: all clean run directories