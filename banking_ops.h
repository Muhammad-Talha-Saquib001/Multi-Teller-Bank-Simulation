/**
 * @file banking_ops.h
 * @brief Banking operation functions for teller system
 */

#ifndef BANKING_OPS_H
#define BANKING_OPS_H

#include <stdbool.h>
#include "common.h"

// ======================
// Banking Operations
// ======================

/**
 * @brief Withdraws specified amount from customer's account
 * @param customer_id ID of the customer
 * @param amount Amount to withdraw
 * @return true if successful, false otherwise
 */
bool withdraw(int customer_id, double amount);

/**
 * @brief Deposits specified amount to customer's account
 * @param customer_id ID of the customer
 * @param amount Amount to deposit
 * @return true if successful, false otherwise
 */
bool deposit(int customer_id, double amount);

/**
 * @brief Checks current balance of customer's account
 * @param customer_id ID of the customer
 * @return Current balance
 */
double check_balance(int customer_id);

/**
 * @brief Converts amount between currencies
 * @param customer_id ID of the customer
 * @param from_currency Source currency code
 * @param to_currency Target currency code
 * @param amount Amount to convert
 * @return Converted amount
 */
double currency_conversion(int customer_id, const char* from_currency, 
                         const char* to_currency, double amount);

/**
 * @brief Processes loan application
 * @param customer_id ID of the customer
 * @param amount Loan amount requested
 * @param term_months Loan term in months
 * @return true if approved, false otherwise
 */
bool loan_request(int customer_id, double amount, int term_months);

/**
 * @brief Processes bill payment
 * @param customer_id ID of the customer
 * @param biller_id ID of the biller
 * @param amount Amount to pay
 * @return true if successful, false otherwise
 */
bool bill_payment(int customer_id, int biller_id, double amount);

/**
 * @brief Displays the customer service menu
 * @param customer_id ID of the customer
 */
void display_customer_menu(int customer_id);

#endif 