/**
 * @file customer.c
 * @brief Customer process implementation for bank simulation
 */

#define _POSIX_C_SOURCE 200112L
#include "../include/customer.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/ipc.h"

// ======================
// Customer State
// ======================

static struct {
  int customer_id;
  char response_pipe[PIPENAME_MAX];
  bool initialized;
} customer_state;

// ======================
// Core Functions
// ======================

bool customer_init(int customer_id) {
  customer_state.customer_id = customer_id;
  make_response_pipe_name(customer_id, customer_state.response_pipe,
                          PIPENAME_MAX);

  if (!create_pipe(customer_state.response_pipe)) {
    return false;
  }

  customer_state.initialized = true;
  return true;
}

bool request_service(int customer_id) {
  if (!customer_state.initialized) return false;

  CustomerRequest req = {.customer_id = customer_id, .is_termination = false};
  snprintf(req.response_pipe, PIPENAME_MAX, "%s", customer_state.response_pipe);

  return send_request(CENTRAL_PIPE, &req);
}

TerminalStatus wait_for_response(int customer_id, int timeout_ms) {
  assert(customer_id == customer_state.customer_id);  // Verify match
  if (!customer_state.initialized) return TERMINAL_ERROR;

  CustomerRequest response;
  if (!receive_request(customer_state.response_pipe, &response, timeout_ms)) {
    return TERMINAL_BUSY;  // Timeout treated as "terminal busy"
  }
  return TERMINAL_ACQUIRED;
}

void customer_cleanup(int customer_id) {
  (void)customer_id;  // Explicitly unused
  if (customer_state.initialized) {
    remove_pipe(customer_state.response_pipe);
    customer_state.initialized = false;
  }
}

// ======================
// Utility Functions
// ======================

const char* customer_last_error() { return strerror(errno); }

void make_response_pipe_name(int customer_id, char* buffer, size_t size) {
  snprintf(buffer, size, "/tmp/bank_customer_%d_pipe", customer_id);
}