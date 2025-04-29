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

#include "../banking/banking_ops.c"
#include "../include/banking_ops.h"
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
  int teller_id = *(int*)arg;

  while (!system_state.shutdown_requested) {
    CustomerRequest req;
    if (receive_request(CENTRAL_PIPE, &req, 1000)) {
      printf("Teller %d received request from customer %d\n", teller_id,
             req.customer_id);

      // Handle banking operations
      switch (req.operation) {
        case WITHDRAW:
          handle_withdraw(req.customer_id, req.amount);
          break;
        case DEPOSIT:
          handle_deposit(req.customer_id, req.amount);
          break;
        case BALANCE_CHECK:
          handle_balance_check(req.customer_id);
          break;
        case CURRENCY_CONV:
          handle_currency_conversion(req.customer_id, req.target);
          break;
        case BILL_PAYMENT:
          handle_bill_payment(req.customer_id, req.target, req.amount);
          break;
        case LOAN_REQUEST:
          handle_loan_request(req.customer_id, req.amount);
          break;
        default:
          printf("Invalid operation requested\n");
      }

      // Send response (simplified)
      send_request(req.response_pipe, &req);
    }
  }
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
  int count = system_state.num_tellers;
  if (count > max_size) count = max_size;

  for (int i = 0; i < count; i++) {
    status_array[i] = system_state.teller_status[i];
  }

  return count;
}