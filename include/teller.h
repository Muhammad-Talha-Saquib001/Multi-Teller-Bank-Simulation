/**
 * @file teller.h
 * @brief Teller system interface for bank simulation
 *
 * Manages teller threads, terminal allocation, and customer request processing.
 */

#ifndef TELLER_H
#define TELLER_H

#include <pthread.h>  // For pthread_mutex_t
#include <stdbool.h>

#include "banking_ops.h"  // For Customer
#include "common.h"       // For MAX_CUSTOMERS


// ======================
// Teller Configuration
// ======================

typedef struct {
  int teller_id;          ///< Unique teller identifier
  bool is_active;         ///< Teller thread status flag
  int assigned_terminal;  ///< Currently occupied terminal (-1 if none)
} TellerStatus;

// ======================
// Core System Functions
// ======================

/**
 * @brief Initializes the teller system
 * @param num_tellers Number of tellers to create
 * @param num_terminals Available terminals
 * @return true on success, false on initialization failure
 */
bool teller_system_init(int num_tellers, int num_terminals);

/**
 * @brief Starts the teller service threads
 */
void teller_system_start();

/**
 * @brief Gracefully shuts down the teller system
 */
void teller_system_shutdown();

// ======================
// Terminal Management
// ======================

/**
 * @brief Attempts to acquire a terminal (thread-safe)
 * @param teller_id ID of requesting teller
 * @return TerminalStatus indicating result
 */
TerminalStatus acquire_terminal(int teller_id);

/**
 * @brief Releases a terminal (thread-safe)
 * @param teller_id ID of teller releasing terminal
 */
void release_terminal(int teller_id);

// ======================
// Utility Functions
// ======================

/**
 * @brief Gets current status of all tellers
 * @param status_array Buffer to store statuses
 * @param max_size Maximum entries in buffer
 * @return Number of tellers populated
 */
int get_teller_status(TellerStatus* status_array, int max_size);

#endif