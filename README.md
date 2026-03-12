# Multi-Teller Bank Simulation

![C](https://img.shields.io/badge/language-C-blue.svg)
![OS](https://img.shields.io/badge/os-Linux%20%2F%20POSIX-orange.svg)
![Concurrency](https://img.shields.io/badge/topic-Concurrency%20%26%20IPC-green.svg)

A robust, multi-threaded bank counter simulation developed in C (C11). This project demonstrates advanced Operating System concepts by modeling a real-world banking environment where multiple customers concurrently interact with several tellers and shared terminals.

It effectively combines classic OS synchronization problems, specifically the **Producer-Consumer** and **Dining Philosophers** problems, utilizing POSIX threads (pthreads), mutexes, semaphores, and Inter-Process Communication (IPC) pipes to ensure data integrity and prevent race conditions or deadlocks.

---

## 📋 Features

- **Concurrent Customer Processing**: Supports up to 100 concurrent customers and 10 teller threads.
- **Shared Resources Management**: Manages limited shared terminals (up to 5) among active tellers safely.
- **Inter-Process Communication (IPC)**: Utilizes central and terminal pipes for seamless command routing and state management between customer and teller processes.
- **Robust Synchronization**: Employs pthreads, mutexes, and semaphores to handle race conditions and deadlock prevention.
- **Diverse Banking Operations**:
  - Deposit & Withdrawal
  - Balance Inquiry
  - Currency Conversion (e.g., EUR, USD)
  - Bill Payments
  - Loan Requests (with term processing)

## 🏗️ Project Structure

The project is modularly structured, separating concerns across different directories:

```text
Multi-Teller-Bank-Simulation/
├── include/            # Shared header files (common definitions, IPC structures)
├── src/
│   ├── banking/        # Core banking logic
│   ├── customer/       # Customer process implementation
│   ├── ipc/            # Inter-process communication utilities
│   └── teller/         # Teller thread implementation and management
├── Makefile            # Build configuration
└── README.md           # Project documentation
```

## ⚙️ Prerequisites

To build and run this simulation, your system needs:

- **OS**: Linux or a POSIX-compliant system (WSL on Windows is supported).
- **Compiler**: `gcc` (with C11 standard support).
- **Libraries**: `pthread` and `rt` (POSIX threads and real-time extensions).
- **Tools**: `make`.

## 🚀 Installation & Build

A `Makefile` is provided to simplify the compilation process. The build artifacts will be placed inside a `bin/` directory.

1. **Clone the repository:**
   ```bash
   git clone <your-repo-url>
   cd Multi-Teller-Bank-Simulation
   ```

2. **Build the entire project:**
   ```bash
   make all
   # Or simply: make
   ```

3. **Build individual components (optional):**
   ```bash
   make teller     # Builds only the teller system executable
   make customer   # Builds only the customer process executable
   ```

4. **Clean build artifacts:**
   ```bash
   make clean
   ```

## 💻 Usage

After a successful build, you will find the executables in the newly created `bin` directory (`bin/teller` and `bin/customer`).

Since this is an IPC-based simulation, you generally need to start the teller process first to initialize the bank state and pipes, followed by the customer processes to simulate requests.

```bash
# Terminal 1: Start the Teller System
./bin/teller

# Terminal 2: Spawn Customers
./bin/customer
```

*(Note: Exact runtime arguments may vary based on your environment. Please refer to typical test scripts or run the binaries directly to see specific console outputs.)*

## 📚 Concepts Applied

This project was built as part of an Operating Systems coursework to solidify understanding of:
- Mutex Mutually Exclusive Locks.
- Reader-Writer & Producer-Consumer Models.
- Semaphore-based signaling.
- Deadlock detection/prevention.
- Memory and process isolation.

## 🤝 Contributors

Developed as part of the Operating Systems Theory course (4th Semester) at FAST University.

- **Team Members:**
  - Muhammad Talha Saquib
  - Ibrahim Kazmi
  - Burhan Farooq
  - Sahil Latif

---
*If you find this project helpful or interesting, feel free to give it a ⭐️!*