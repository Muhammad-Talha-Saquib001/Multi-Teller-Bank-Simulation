# Multi-Teller-Bank-Simulation
A C-based multi-threaded bank counter simulation combining Producer-Consumer and Dining Philosophers problems. Uses pthreads, mutexes, and semaphores for synchronization, demonstrating OS concurrency and deadlock prevention.

Usage:

make or make all: Builds both executables

make teller: Builds only the teller system

make customer: Builds only the customer process

make clean: Removes all build artifacts