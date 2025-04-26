/**
 * @file ipc.h
 * @brief Inter-process communication utilities for bank simulation
 *
 * Provides pipe-based communication functions for teller-customer interaction.
 */

#ifndef BANK_SIMULATION_IPC_H
#define BANK_SIMULATION_IPC_H

#include "common.h"

// ======================
// Pipe Management
// ======================

/**
 * @brief Creates a named pipe with error handling
 * @param pipe_path Absolute path to the pipe
 * @return true on success, false on failure
 */
bool create_pipe(const char* pipe_path);

/**
 * @brief Safely removes a named pipe
 * @param pipe_path Absolute path to the pipe
 */
void remove_pipe(const char* pipe_path);

// ======================
// Message Transmission
// ======================

/**
 * @brief Writes a customer request to a pipe
 * @param pipe_path Destination pipe path
 * @param request Pointer to the request structure
 * @return true on successful write, false on failure
 */
bool send_request(const char* pipe_path, const CustomerRequest* request);

/**
 * @brief Reads a customer request from a pipe
 * @param pipe_path Source pipe path
 * @param request Pointer to store the received request
 * @param timeout_ms Maximum wait time in milliseconds (0 for infinite)
 * @return true on successful read, false on timeout or error
 */
bool receive_request(const char* pipe_path, CustomerRequest* request,
                     int timeout_ms);

// ======================
// System Initialization
// ======================

/**
 * @brief Sets up all required IPC channels
 * @return true on success, false on initialization failure
 */
bool initialize_ipc_system();

/**
 * @brief Cleans up all IPC resources
 */
void cleanup_ipc_system();

#endif