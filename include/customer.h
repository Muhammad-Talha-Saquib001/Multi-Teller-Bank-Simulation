/**
 * @file customer.h
 * @brief Customer process interface for bank simulation
 *
 * Handles customer request generation and response processing.
 */

#ifndef BANK_SIMULATION_CUSTOMER_H
#define BANK_SIMULATION_CUSTOMER_H

#include "common.h"

// ======================
// Customer Operations
// ======================

/**
 * @brief Initializes customer process resources
 * @param customer_id Unique customer identifier
 * @return true on success, false on initialization failure
 */
bool customer_init(int customer_id);

/**
 * @brief Sends a service request to the teller system
 * @param customer_id ID of the requesting customer
 * @return true if request was successfully sent
 */
bool request_service(int customer_id);

/**
 * @brief Waits for response from teller system
 * @param customer_id ID of the waiting customer
 * @param timeout_ms Maximum wait time in milliseconds
 * @return TerminalStatus indicating transaction result
 */
TerminalStatus wait_for_response(int customer_id, int timeout_ms);

/**
 * @brief Cleans up customer process resources
 * @param customer_id ID of the customer to cleanup
 */
void customer_cleanup(int customer_id);

// ======================
// Debug Utilities
// ======================

/**
 * @brief Gets the last error message for the customer
 * @return Null-terminated error description
 */
const char* customer_last_error();

#endif  // BANK_SIMULATION_CUSTOMER_H