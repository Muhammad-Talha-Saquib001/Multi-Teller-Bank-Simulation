#ifndef BANKING_OPS_H
#define BANKING_OPS_H

#include "../include/common.h"  // For CustomerRequest

// Menu display (runs in customer process)
void display_customer_menu(int customer_id);

// Banking operations (run in teller process)
void handle_withdraw(Customer* customer, double amount);
void handle_deposit(Customer* customer, double amount);
void handle_balance_check(Customer* customer);
void handle_currency_conversion(CustomerRequest req, char* target_currency);
void handle_bill_payment(Customer* customer, const char* biller, double amount);
void handle_loan_request(Customer* customer, double amount);

#endif