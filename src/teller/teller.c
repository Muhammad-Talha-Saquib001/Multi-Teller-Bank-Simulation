/**
 * @file teller.c
 * @brief Teller system implementation for bank simulation
 */

#define _POSIX_C_SOURCE 200112L  // For pthread and time functions

#include "../include/teller.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/ipc.h"

// ======================
// Shared Teller State
// ======================

typedef struct {
  int num_terminals;
  int num_tellers;
  int available_terminals;
  pthread_mutex_t terminal_mutex;
  bool shutdown_requested;
  pthread_t* teller_threads;
  TellerStatus* teller_status;
} TellerSystem;

static TellerSystem system_state;

// ======================
// Terminal Management
// ======================

TerminalStatus acquire_terminal(int teller_id) {
  pthread_mutex_lock(&system_state.terminal_mutex);

  if (system_state.available_terminals > 0) {
    system_state.available_terminals--;
    system_state.teller_status[teller_id].assigned_terminal =
        system_state.num_terminals - system_state.available_terminals;
    pthread_mutex_unlock(&system_state.terminal_mutex);
    return TERMINAL_ACQUIRED;
  }

  pthread_mutex_unlock(&system_state.terminal_mutex);
  return TERMINAL_BUSY;
}

void release_terminal(int teller_id) {
  pthread_mutex_lock(&system_state.terminal_mutex);
  system_state.available_terminals++;
  system_state.teller_status[teller_id].assigned_terminal = -1;
  pthread_mutex_unlock(&system_state.terminal_mutex);
}

// ======================
// Teller Thread Function
// ======================

static void* teller_worker(void* arg) {
  // In teller_worker() function:
  CustomerRequest req;
  if (receive_request(CENTRAL_PIPE, &req, 1000)) {
    printf("Received request from customer %d\n",
           req.customer_id);  // Add this line
                              // ... rest of handling ...
  }
  int teller_id = *(int*)arg;
  system_state.teller_status[teller_id].is_active = true;

  while (!system_state.shutdown_requested) {
    CustomerRequest req;
    if (receive_request(CENTRAL_PIPE, &req, 1000)) {
      if (req.is_termination) break;

      TerminalStatus status = acquire_terminal(teller_id);
      if (status == TERMINAL_ACQUIRED) {
        // Simulate transaction processing
        sleep(1 + rand() % 2);
        release_terminal(teller_id);

        // Send response (simplified)
        send_request(req.response_pipe, &req);
      }
    }
  }

  system_state.teller_status[teller_id].is_active = false;
  return NULL;
}

// ======================
// System Lifecycle
// ======================

bool teller_system_init(int num_tellers, int num_terminals) {
  // Initialize shared state
  system_state.num_terminals = num_terminals;
  system_state.num_tellers = num_tellers;
  system_state.available_terminals = num_terminals;
  system_state.shutdown_requested = false;

  // Create mutex
  if (pthread_mutex_init(&system_state.terminal_mutex, NULL) != 0) {
    return false;
  }

  // Allocate thread and status arrays
  system_state.teller_threads = malloc(num_tellers * sizeof(pthread_t));
  system_state.teller_status = malloc(num_tellers * sizeof(TellerStatus));

  if (!system_state.teller_threads || !system_state.teller_status) {
    return false;
  }

  // Initialize IPC
  if (!initialize_ipc_system()) {
    return false;
  }

  return true;
}

void teller_system_start() {
  // Create teller threads
  for (int i = 0; i < system_state.num_tellers; i++) {
    int* teller_id = malloc(sizeof(int));
    *teller_id = i;
    pthread_create(&system_state.teller_threads[i], NULL, teller_worker,
                   teller_id);
  }
}

void teller_system_shutdown() {
  // Signal shutdown
  system_state.shutdown_requested = true;

  // Send termination signals
  CustomerRequest term_req = {.is_termination = true};
  for (int i = 0; i < system_state.num_tellers; i++) {
    send_request(CENTRAL_PIPE, &term_req);
  }

  // Join threads
  for (int i = 0; i < system_state.num_tellers; i++) {
    pthread_join(system_state.teller_threads[i], NULL);
  }

  // Cleanup
  cleanup_ipc_system();
  pthread_mutex_destroy(&system_state.terminal_mutex);
  free(system_state.teller_threads);
  free(system_state.teller_status);
}

// ======================
// Status Reporting
// ======================

int get_teller_status(TellerStatus* status_array, int max_size) {
  int count =
      system_state.num_tellers;  // Use the stored count instead of sizeof
  if (count > max_size) count = max_size;

  for (int i = 0; i < count; i++) {
    status_array[i] = system_state.teller_status[i];
  }

  return count;
}