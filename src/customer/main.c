#include <stdio.h>
#include <stdlib.h>

#include "../include/customer.h"

int main(int argc, char* argv[]) {
  (void)argc;
  printf("Customer %s starting...\n", argv[1]);  // Add this

  if (!customer_init(atoi(argv[1]))) {
    perror("Init failed");  // Change from fprintf to perror
    exit(EXIT_FAILURE);
  }

  printf("Sending request...\n");  // Add this
  if (!request_service(atoi(argv[1]))) {
    perror("Request failed");
    customer_cleanup(atoi(argv[1]));
    exit(EXIT_FAILURE);
  }

  printf("Waiting for response...\n");  // Add this
  TerminalStatus status = wait_for_response(atoi(argv[1]), 5000);

  printf("Result: %s\n",
         status == TERMINAL_ACQUIRED ? "SUCCESS" : "FAILED");  // Add this
  // ... rest of code ...
}