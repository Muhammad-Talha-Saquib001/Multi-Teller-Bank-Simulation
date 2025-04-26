/**
 * @file named_pipe.c
 * @brief Named pipe implementation for bank simulation
 */

#define _POSIX_C_SOURCE 200112L  // Enables POSIX.1-2001 features

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/ipc.h"

// Internal helper to log errors
static void _log_error(const char* msg) {
  fprintf(stderr, "[IPC ERROR] %s: %s\n", msg, strerror(errno));
}

// --- Pipe Management ---
bool create_pipe(const char* pipe_path) {
  if (mkfifo(pipe_path, 0666) == -1) {
    if (errno != EEXIST) {  // Ignore if pipe already exists
      _log_error("Failed to create pipe");
      return false;
    }
  }
  return true;
}

void remove_pipe(const char* pipe_path) {
  unlink(pipe_path);  // Silent failure is acceptable
}

// --- Message Transmission ---
bool send_request(const char* pipe_path, const CustomerRequest* req) {
  int fd = open(pipe_path, O_WRONLY | O_NONBLOCK);
  if (fd == -1) {
    _log_error("Failed to open pipe for writing");
    return false;
  }

  ssize_t bytes = write(fd, req, sizeof(CustomerRequest));
  close(fd);

  if (bytes != sizeof(CustomerRequest)) {
    _log_error("Incomplete write to pipe");
    return false;
  }
  return true;
}

bool receive_request(const char* pipe_path, CustomerRequest* req,
                     int timeout_ms) {
  int fd = open(pipe_path, O_RDONLY | O_NONBLOCK);
  if (fd == -1) {
    _log_error("Failed to open pipe for reading");
    return false;
  }

  struct timespec start, now;
  if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
    _log_error("Failed to get start time");
    close(fd);
    return false;
  }

  // Define sleep interval (10ms)
  const struct timespec sleep_time = {
      .tv_sec = 0,
      .tv_nsec = 10000000  // 10ms in nanoseconds
  };

  while (true) {
    ssize_t bytes = read(fd, req, sizeof(CustomerRequest));
    if (bytes > 0) {
      close(fd);
      return (bytes == sizeof(CustomerRequest));
    }

    // Timeout check
    if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
      _log_error("Failed to get current time");
      close(fd);
      return false;
    }

    long elapsed_ms = (now.tv_sec - start.tv_sec) * 1000 +
                      (now.tv_nsec - start.tv_nsec) / 1000000;
    if (elapsed_ms >= timeout_ms && timeout_ms > 0) {
      close(fd);
      return false;
    }

    // Use nanosleep instead of usleep
    nanosleep(&sleep_time, NULL);
  }
}

// --- System Lifecycle ---
bool initialize_ipc_system() {
  // Create central pipe with correct permissions
  if (mkfifo(CENTRAL_PIPE, 0666) == -1 && errno != EEXIST) {
    perror("mkfifo(CENTRAL_PIPE) failed");
    return false;
  }

  // Create terminal pipe
  if (mkfifo(TERMINAL_PIPE, 0666) == -1 && errno != EEXIST) {
    perror("mkfifo(TERMINAL_PIPE) failed");
    unlink(CENTRAL_PIPE);
    return false;
  }

  return true;
}

void cleanup_ipc_system() {
  remove_pipe(CENTRAL_PIPE);
  remove_pipe(TERMINAL_PIPE);
}