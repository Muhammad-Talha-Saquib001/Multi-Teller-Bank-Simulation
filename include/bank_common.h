#ifndef BANK_COMMON_H
#define BANK_COMMON_H

#include <stdint.h>  // For fixed-width types (e.g., uint32_t)

// ======================
// IPC Configuration
// ======================
#define SOCKET_PATH "/tmp/bank_socket"  // Unix domain socket path
#define MAX_MSG_SIZE 256                // Max bytes for IPC messages
#define MAX_TERMINALS 5                 // Max shared terminals

// ======================
// Message Types (IPC)
// ======================
typedef enum {
  MSG_REQUEST_TERMINAL,  // Customer -> Manager: Request terminal access
  MSG_TERMINAL_GRANTED,  // Manager -> Customer: Terminal allocated
  MSG_RELEASE_TERMINAL,  // Customer -> Manager: Terminal release
  MSG_SHUTDOWN           // Manager -> All: Graceful shutdown
} MessageType;

// ======================
// Message Struct (IPC)
// ======================
typedef struct {
  MessageType type;       // Message type (see enum above)
  uint32_t customer_id;   // Unique customer identifier
  uint32_t service_time;  // Transaction time in milliseconds
} CustomerMessage;

// ======================
// Error Codes
// ======================
typedef enum {
  IPC_SUCCESS = 0,
  IPC_SOCKET_ERROR,
  IPC_BIND_ERROR,
  IPC_MSG_INVALID
} IPCReturnCode;

#endif