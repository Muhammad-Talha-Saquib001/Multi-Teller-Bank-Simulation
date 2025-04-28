#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "banking_ops.h"
#include "common.h"

// Simulated customer account data
typedef struct {
    double balance;
    bool is_active;
} CustomerAccount;

// Simulated exchange rates
static const double EXCHANGE_RATES[3][3] = {
    {1.0, 0.85, 110.0},  // USD to USD, EUR, JPY
    {1.18, 1.0, 130.0},  // EUR to USD, EUR, JPY
    {0.0091, 0.0077, 1.0} // JPY to USD, EUR, JPY
};

// Simulated billers
static const char* BILLERS[] = {
    "Electricity",
    "Water",
    "Internet",
    "Phone"
};

// Simulated customer accounts
static CustomerAccount customer_accounts[MAX_CUSTOMERS];

// Initialize customer accounts
static void init_customer_accounts() {
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        customer_accounts[i].balance = 1000.0; // Initial balance
        customer_accounts[i].is_active = true;
    }
}

// Simulate processing delay
static void simulate_processing_delay() {
    sleep(1); // 1 second delay
}

bool withdraw(int customer_id, double amount) {
    if (customer_id < 0 || customer_id >= MAX_CUSTOMERS || !customer_accounts[customer_id].is_active) {
        return false;
    }

    simulate_processing_delay();
    
    if (customer_accounts[customer_id].balance >= amount) {
        customer_accounts[customer_id].balance -= amount;
        return true;
    }
    return false;
}

bool deposit(int customer_id, double amount) {
    if (customer_id < 0 || customer_id >= MAX_CUSTOMERS || !customer_accounts[customer_id].is_active) {
        return false;
    }

    simulate_processing_delay();
    customer_accounts[customer_id].balance += amount;
    return true;
}

double check_balance(int customer_id) {
    if (customer_id < 0 || customer_id >= MAX_CUSTOMERS || !customer_accounts[customer_id].is_active) {
        return -1.0;
    }

    simulate_processing_delay();
    return customer_accounts[customer_id].balance;
}

double currency_conversion(int customer_id, const char* from_currency, 
                         const char* to_currency, double amount) {
    if (customer_id < 0 || customer_id >= MAX_CUSTOMERS || !customer_accounts[customer_id].is_active) {
        return -1.0;
    }

    simulate_processing_delay();
    
    // Simple currency conversion using predefined rates
    int from_idx = 0, to_idx = 0;
    
    if (strcmp(from_currency, "USD") == 0) from_idx = 0;
    else if (strcmp(from_currency, "EUR") == 0) from_idx = 1;
    else if (strcmp(from_currency, "JPY") == 0) from_idx = 2;
    
    if (strcmp(to_currency, "USD") == 0) to_idx = 0;
    else if (strcmp(to_currency, "EUR") == 0) to_idx = 1;
    else if (strcmp(to_currency, "JPY") == 0) to_idx = 2;
    
    return amount * EXCHANGE_RATES[from_idx][to_idx];
}

bool loan_request(int customer_id, double amount, int term_months) {
    if (customer_id < 0 || customer_id >= MAX_CUSTOMERS || !customer_accounts[customer_id].is_active) {
        return false;
    }

    simulate_processing_delay();
    
    // Simple loan approval logic
    if (amount <= customer_accounts[customer_id].balance * 2 && term_months <= 60) {
        customer_accounts[customer_id].balance += amount;
        return true;
    }
    return false;
}

bool bill_payment(int customer_id, int biller_id, double amount) {
    if (customer_id < 0 || customer_id >= MAX_CUSTOMERS || !customer_accounts[customer_id].is_active) {
        return false;
    }

    if (biller_id < 0 || biller_id >= sizeof(BILLERS)/sizeof(BILLERS[0])) {
        return false;
    }

    simulate_processing_delay();
    
    if (customer_accounts[customer_id].balance >= amount) {
        customer_accounts[customer_id].balance -= amount;
        return true;
    }
    return false;
}

void display_customer_menu(int customer_id) {
    printf("\n=== Banking Services Menu ===\n");
    printf("Customer ID: %d\n", customer_id);
    printf("1. Withdraw\n");
    printf("2. Deposit\n");
    printf("3. Check Balance\n");
    printf("4. Currency Conversion\n");
    printf("5. Loan Request\n");
    printf("6. Bill Payment\n");
    printf("0. Exit\n");
    printf("===========================\n");
    printf("Please select an option: ");
}

// Initialize the banking operations system
void banking_ops_init() {
    init_customer_accounts();
} 