#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/customer.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <customer_id>\n", argv[0]);
    return EXIT_FAILURE;
  }

  run_customer_interactive(atoi(argv[1]));
  return EXIT_SUCCESS;
}