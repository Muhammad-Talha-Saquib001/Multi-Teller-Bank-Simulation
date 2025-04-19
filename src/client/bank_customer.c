#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <customer_id> <service_time_ms>\n", argv[0]);
    return 1;
  }

  int customer_id = atoi(argv[1]);
  int service_time_ms = atoi(argv[2]);

  printf("Customer %d requesting service for %d ms\n", customer_id,
         service_time_ms);
  // TODO: Connect to manager via IPC, send request
  return 0;
}
