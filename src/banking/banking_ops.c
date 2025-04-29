#include "../include/banking_ops.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Shared simple "account balance" simulation
static double customer_balance = 1000.0;  // Starting balance

void display_customer_menu(int customer_id) {
  printf("\n=== Customer %d Menu ===\n", customer_id);
  printf("1. Withdraw\n");
  printf("2. Deposit\n");
  printf("3. Check Balance\n");
  printf("4. Currency Conversion\n");
  printf("5. Bill Payment\n");
  printf("6. Loan Request\n");
  printf("0. Exit\n");
  printf("Select option: ");
}

void handle_withdraw(int customer_id, double amount) {
  printf("Teller: Withdrawing %.2f for customer %d\n", amount, customer_id);
  if (amount <= customer_balance) {
    customer_balance -= amount;
    printf("Success! New balance: %.2f\n", customer_balance);
  } else {
    printf("Error: Insufficient funds\n");
  }
}

void handle_deposit(int customer_id, double amount) {
  printf("Teller: Depositing %.2f for customer %d\n", amount, customer_id);
  customer_balance += amount;
  printf("Success! New balance: %.2f\n", customer_balance);
}

void handle_balance_check(int customer_id) {
  printf("Teller: Balance check for customer %d\n", customer_id);
  printf("Current balance: %.2f\n", customer_balance);
}

void handle_currency_conversion(int customer_id, const char* target_currency) {
  printf("Teller: Converting currency to %s for customer %d\n", target_currency,
         customer_id);
  // Simulated conversion rates
  double rate = 0.0;
  if (strcmp(target_currency, "EUR") == 0)
    rate = 0.85;
  else if (strcmp(target_currency, "GBP") == 0)
    rate = 0.73;

  if (rate > 0) {
    printf("Converted amount: %.2f %s\n", customer_balance * rate,
           target_currency);
  } else {
    printf("Error: Unsupported currency\n");
  }
}

void handle_bill_payment(int customer_id, const char* biller, double amount) {
  printf("Teller: Paying %.2f to %s for customer %d\n", amount, biller,
         customer_id);
  if (amount <= customer_balance) {
    customer_balance -= amount;
    printf("Payment successful! New balance: %.2f\n", customer_balance);
  } else {
    printf("Error: Insufficient funds\n");
  }
}

void handle_loan_request(int customer_id, double amount) {
  printf("Teller: Processing loan request of %.2f for customer %d\n", amount,
         customer_id);
  // Simple approval logic
  if (amount <= 5000) {
    printf("Loan approved! Amount will be deposited.\n");
    customer_balance += amount;
  } else {
    printf("Loan denied: Amount too large\n");
  }
}