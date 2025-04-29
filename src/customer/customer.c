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

bool request_service(CustomerRequest* req) {  // Changed from int customer_id
  if (!customer_state.initialized) return false;
  return send_request(CENTRAL_PIPE, req);
}

TerminalStatus wait_for_response(int customer_id, int timeout_ms) {
  assert(customer_id == customer_state.customer_id);
  if (!customer_state.initialized) return TERMINAL_ERROR;

  CustomerRequest response;
  if (!receive_request(customer_state.response_pipe, &response, timeout_ms)) {
    return TERMINAL_BUSY;
  }
  return response
      .operation_status;  // You'll need to add this field to CustomerRequest
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

// ======================
// Interactive Menu
// ======================

void run_customer_interactive(int customer_id) {
  if (!customer_init(customer_id)) {
    fprintf(stderr, "Initialization failed: %s\n", customer_last_error());
    return;
  }

  CustomerRequest req = {.customer_id = customer_id,
                         .is_termination = false,
                         .amount = 0.0,
                         .target = ""};
  make_response_pipe_name(customer_id, req.response_pipe, PIPENAME_MAX);

  while (1) {
    printf("\n=== Customer %d Menu ===\n", customer_id);
    printf(
        "1. Withdraw\n2. Deposit\n3. Balance\n4. Currency\n5. Bill Pay\n6. "
        "Loan\n0. Exit\n");
    printf("Select: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 0) break;

    switch (choice) {
      case 1:
        req.operation = WITHDRAW;
        printf("Amount: ");
        scanf("%lf", &req.amount);
        break;
      case 2:
        req.operation = DEPOSIT;
        printf("Amount: ");
        scanf("%lf", &req.amount);
        break;
      // ... Add other cases ...
      default:
        printf("Invalid choice\n");
        continue;
    }

    if (!request_service(&req)) {
      fprintf(stderr, "Service request failed\n");
      break;
    }

    TerminalStatus status = wait_for_response(customer_id, 5000);
    printf("Operation %s\n",
           status == TERMINAL_ACQUIRED ? "succeeded" : "failed");
  }

  customer_cleanup(customer_id);
}