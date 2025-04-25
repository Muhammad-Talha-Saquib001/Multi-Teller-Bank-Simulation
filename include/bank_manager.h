
#ifndef BANK_MANAGER_H
#define BANK_MANAGER_H

#include "bank_common.h"  // For CustomerMessage, IPC constants

// ======================
// Server Control
// ======================

/*
 * Starts the bank server with a given number of terminals.
 * Returns 0 on success, or an IPCReturnCode error.
 */
int start_server(int num_terminals);

/*
 * Gracefully shuts down the server, closing all connections.
 */
void shutdown_server();

// ======================
// Terminal Management
// ======================

/*
 * Allocates a terminal to a customer (blocking if none available).
 * Returns terminal_id (>=0) or -1 on error.
 */
int allocate_terminal(int customer_id);

/*
 * Releases a terminal for reuse.
 */
void release_terminal(int terminal_id);

// ======================
// Thread-Safe Logging
// ======================

/*
 * Logs server events (thread-safe).
 */
void log_server_event(const char* message);

#endif  // BANK_MANAGER_H