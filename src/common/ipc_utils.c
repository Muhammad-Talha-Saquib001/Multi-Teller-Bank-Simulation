#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "../include/bank_common.h"

// =============================================
// Socket Creation & Cleanup
// =============================================

/*
 * Creates a Unix domain socket (server or client).
 * Returns socket file descriptor or -1 on error.
 */
int create_socket() {
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    perror("socket() failed");
    return -1;
  }
  return sock_fd;
}

/*
 * Sets up the server socket and binds it to SOCKET_PATH.
 * Returns socket fd or -1 on error.
 */
int setup_server_socket() {
  int sock_fd = create_socket();
  if (sock_fd < 0) return -1;

  struct sockaddr_un addr = {
      .sun_family = AF_UNIX,
  };
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  // Ensure socket path is unlinked before binding
  unlink(SOCKET_PATH);

  if (bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("bind() failed");
    close(sock_fd);
    return -1;
  }

  if (listen(sock_fd, 5) < 0) {
    perror("listen() failed");
    close(sock_fd);
    return -1;
  }

  return sock_fd;
}

/*
 * Connects a client socket to the server.
 * Returns socket fd or -1 on error.
 */
int connect_to_server() {
  int sock_fd = create_socket();
  if (sock_fd < 0) return -1;

  struct sockaddr_un addr = {
      .sun_family = AF_UNIX,
  };
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  if (connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("connect() failed");
    close(sock_fd);
    return -1;
  }

  return sock_fd;
}

// =============================================
// Message Handling
// =============================================

/*
 * Sends a CustomerMessage over a socket.
 * Returns IPC_SUCCESS or IPC_SOCKET_ERROR.
 */
IPCReturnCode send_message(int sock_fd, const CustomerMessage* msg) {
  if (!msg) return IPC_MSG_INVALID;

  ssize_t bytes_sent = write(sock_fd, msg, sizeof(CustomerMessage));
  if (bytes_sent != sizeof(CustomerMessage)) {
    perror("write() failed");
    return IPC_SOCKET_ERROR;
  }
  return IPC_SUCCESS;
}

/*
 * Receives a CustomerMessage from a socket.
 * Returns IPC_SUCCESS or IPC_SOCKET_ERROR.
 */
IPCReturnCode receive_message(int sock_fd, CustomerMessage* msg) {
  if (!msg) return IPC_MSG_INVALID;

  ssize_t bytes_recv = read(sock_fd, msg, sizeof(CustomerMessage));
  if (bytes_recv != sizeof(CustomerMessage)) {
    perror("read() failed");
    return IPC_SOCKET_ERROR;
  }
  return IPC_SUCCESS;
}

// =============================================
// Cleanup
// =============================================

void close_socket(int sock_fd) {
  if (sock_fd >= 0) {
    close(sock_fd);
  }
}