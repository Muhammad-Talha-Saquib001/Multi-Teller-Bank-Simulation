#include "request_queue.h"

#include <stdio.h>
#include <stdlib.h>

void init_queue(RequestQueue* q) {
  q->front = 0;
  q->rear = -1;
  q->size = 0;
  pthread_mutex_init(&q->lock, NULL);
  pthread_cond_init(&q->not_empty, NULL);
}

void enqueue(RequestQueue* q, CustomerRequest req) {
  pthread_mutex_lock(&q->lock);

  if (q->size == MAX_QUEUE_SIZE) {
    printf("Queue full! Dropping request.\n");
    pthread_mutex_unlock(&q->lock);
    return;
  }

  q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
  q->requests[q->rear] = req;
  q->size++;

  pthread_cond_signal(&q->not_empty);
  pthread_mutex_unlock(&q->lock);
}

CustomerRequest dequeue(RequestQueue* q) {
  pthread_mutex_lock(&q->lock);

  while (q->size == 0) {
    pthread_cond_wait(&q->not_empty, &q->lock);
  }

  CustomerRequest req = q->requests[q->front];
  q->front = (q->front + 1) % MAX_QUEUE_SIZE;
  q->size--;

  pthread_mutex_unlock(&q->lock);
  return req;
}
