#include "../include/banking_ops.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

// // Shared simple "account balance" simulation
// static double customer_balance = 1000.0;  // Starting balance

void strip_newline(char* str) {
  size_t len = strlen(str);
  if (len > 0 && str[len - 1] == '\n') {
    str[len - 1] = '\0';
  }
}

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

void handle_withdraw(Customer* customer, double amount) {
  printf("Teller: Withdrawing %.2f for customer %d\n", amount,
         customer->customer_id);
  if (amount <= customer->balance) {
    customer->balance -= amount;
    printf("Success! New balance: %.2f\n", customer->balance);
  } else {
    printf("Error: Insufficient funds\n");
  }
}

void handle_deposit(Customer* customer, double amount) {
  printf("Teller: Depositing %.2f for customer %d\n", amount,
         customer->customer_id);
  customer->balance += amount;
  printf("Success! New balance: %.2f\n", customer->balance);
}

void handle_balance_check(Customer* customer) {
  printf("Teller: Balance check for customer %d\n", customer->customer_id);
  printf("Current balance: %.2f\n", customer->balance);
}

void handle_currency_conversion(CustomerRequest customer,
                                char* target_currency) {
  printf("Teller: Converting currency to %s for customer %d\n", target_currency,
         customer.customer_id);
  double rate = 0.0;
  if (strcmp(target_currency, "EUR") == 0)
    rate = 0.85;
  else if (strcmp(target_currency, "GBP") == 0)
    rate = 0.73;

  if (rate > 0) {
    printf("Converted amount: %.2f %s\n", customer.amount * rate,
           target_currency);
  } else {
    printf("Error: Unsupported currency\n");
  }
}

void handle_bill_payment(Customer* customer, const char* biller,
                         double amount) {
  printf("Teller: Paying %.2f to %s for customer %d\n", amount, biller,
         customer->customer_id);
  if (amount <= customer->balance) {
    customer->balance -= amount;
    printf("Payment successful! New balance: %.2f\n", customer->balance);
  } else {
    printf("Error: Insufficient funds\n");
  }
}

void handle_loan_request(Customer* customer, double amount) {
  printf("Teller: Processing loan request of %.2f for customer %d\n", amount,
         customer->customer_id);
  if (amount <= 5000) {
    printf("Loan approved! Amount will be deposited.\n");
    customer->balance += amount;
  } else {
    printf("Loan denied: Amount too large\n");
  }
}
