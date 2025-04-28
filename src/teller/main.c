#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/ipc.h"
#include "../include/teller.h"

int main(int argc, char* argv[]) {
  printf("=== STARTING TELLER SYSTEM ===\n");

  if (argc < 3) {
    printf("Usage: %s <tellers> <terminals>\n", argv[0]);
    return EXIT_FAILURE;
  }

  printf("Initializing...\n");
  if (!initialize_ipc_system()) {
    printf("IPC initialization failed!\n");
    return EXIT_FAILURE;
  }

  int tellers = atoi(argv[1]);
  int terminals = atoi(argv[2]);
  printf("Creating %d tellers with %d terminals...\n", tellers,
         terminals);  // DEBUG

  if (!teller_system_init(tellers, terminals)) {
    printf("Teller init failed!\n");
    cleanup_ipc_system();
    return EXIT_FAILURE;
  }

  printf("Starting teller threads...\n");
  teller_system_start();

  printf("Teller system running. Press Ctrl+C to exit.\n");
  while (1) {
    sleep(1);
  }
  // Unreachable
  teller_system_shutdown();
  return EXIT_SUCCESS;
}