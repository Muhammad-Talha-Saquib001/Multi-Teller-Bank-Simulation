# Multi-Teller-Bank-Simulation
A C-based multi-threaded bank counter simulation combining Producer-Consumer and Dining Philosophers problems. Uses pthreads, mutexes, and semaphores for synchronization, demonstrating OS concurrency and deadlock prevention.

Build Everything:
  make all

Run the Server:
  make run-server

Run a Customer (in a New Terminal):
  make run-client  # Or manually: ./bin/bank_customer 1 2000

Clean and Rebuild:
  make clean && make