#include "queue.h"
#include <stddef.h>
#include <stdlib.h>



Queue* queue_init(void) {
  Queue* q = malloc(sizeof(struct Queue));
  
  if (q == NULL)
    return NULL;

  if (pthread_mutex_init(&q->mutex, NULL)) {
    free(q);
    return NULL;
  }

  q->head = NULL;
  q->tail = NULL;
  q->lenght = 0;
  return q;
}

void queue_free(Queue* q, void (*func)(void *)) {
  while (q->head) {
    QueueNode* node = queue_get(q);
    if (func)
      func(node->data);
    free(node);
  }
  pthread_mutex_destroy(&q->mutex);
  free(q);
}

QueueNode* queue_get(Queue *q) {
  QueueNode* node;

  pthread_mutex_lock(&q->mutex);
  node = q->head;
  if (node != NULL) {
    q->head = node->next;
    if (q->tail == node)
      q->tail = NULL;
    q->lenght--;
  }
  pthread_mutex_unlock(&q->mutex);
  return node;
}

QueueNode* queue_put(Queue* q, void *data) {
  QueueNode* node = malloc(sizeof(struct QueueNode));

  if (node == NULL)
    return NULL;

  node->data = data;
  node->next = NULL;

  pthread_mutex_lock(&q->mutex);
    if (q->head == NULL)
      q->head = q->tail = node;
    else {
      q->tail->next = node;
      q->tail = node;
    }
    q->lenght++;
  pthread_mutex_unlock(&q->mutex);

  return node;
}

size_t queue_lenght(Queue* q) {
  size_t len;

  pthread_mutex_lock(&q->mutex);
  len = q->lenght;
  pthread_mutex_unlock(&q->mutex);
  return (len);
}

bool is_queue_empty(Queue* q) {
  return queue_lenght(q) == 0;
}
