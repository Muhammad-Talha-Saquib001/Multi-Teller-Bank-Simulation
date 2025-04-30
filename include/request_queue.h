// request_queue.h
#ifndef REQUEST_QUEUE_H
#define REQUEST_QUEUE_H

#include <pthread.h>

#include "common.h"  // for CustomerRequest

#define MAX_QUEUE_SIZE 100

typedef struct {
  CustomerRequest requests[MAX_QUEUE_SIZE];
  int front;
  int rear;
  int size;

  pthread_mutex_t lock;
  pthread_cond_t not_empty;
} RequestQueue;

void init_queue(RequestQueue* q);
void enqueue(RequestQueue* q, CustomerRequest req);
CustomerRequest dequeue(RequestQueue* q);

#endif
