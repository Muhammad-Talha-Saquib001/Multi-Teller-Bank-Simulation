/**
 * @file common.h
 * @brief Shared definitions for the bank simulation project
 *
 * Contains constants, structures, and utilities used by both
 * teller and customer components.
 */

#ifndef BANK_SIMULATION_COMMON_H
#define BANK_SIMULATION_COMMON_H

#include <stdbool.h>
#include <stddef.h>

// ======================
// Project Configuration
// ======================

#define MAX_CUSTOMERS 100     ///< Maximum concurrent customers
#define MAX_TELLERS 10        ///< Maximum teller threads
#define MAX_TERMINALS 5       ///< Shared terminals available
#define PIPENAME_MAX 256      ///< Maximum pipe path length
#define MAX_QUEUE_LENGTH 100  ///< Maximum queue capacity

// ======================
// IPC Communication
// ======================

#define CENTRAL_PIPE "/tmp/bank_central_pipe"    ///< Main command pipe
#define TERMINAL_PIPE "/tmp/bank_terminal_pipe"  ///< Terminal management

/**
 * @brief Customer request structure
 */
typedef struct {
  int customer_id;                   ///< Unique customer identifier
  char response_pipe[PIPENAME_MAX];  ///< Pipe for responses
  bool is_termination;               ///< Special shutdown signal
} CustomerRequest;

/**
 * @brief Terminal acquisition result
 */
typedef enum {
  TERMINAL_ACQUIRED,  ///< Successfully acquired terminal
  TERMINAL_BUSY,      ///< All terminals occupied
  TERMINAL_ERROR      ///< System error occurred
} TerminalStatus;

// ======================
// Utility Functions
// ======================

/**
 * @brief Creates a unique response pipe name
 * @param customer_id ID of the customer
 * @param buffer Buffer to store the pipe name
 * @param size Size of the buffer
 */
void make_response_pipe_name(int customer_id, char* buffer, size_t size);

#endif