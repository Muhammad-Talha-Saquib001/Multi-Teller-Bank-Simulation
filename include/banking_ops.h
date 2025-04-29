#ifndef BANKING_OPS_H
#define BANKING_OPS_H

#include "../include/common.h"  // For CustomerRequest

// Menu display (runs in customer process)
void display_customer_menu(int customer_id);

// Banking operations (run in teller process)
void handle_withdraw(int customer_id, double amount);
void handle_deposit(int customer_id, double amount);
void handle_balance_check(int customer_id);
void handle_currency_conversion(int customer_id, const char* target_currency);
void handle_bill_payment(int customer_id, const char* biller, double amount);
void handle_loan_request(int customer_id, double amount);

#endif