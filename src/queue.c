
#include "queue.h"
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>

Queue* queue_init(void){
  Queue* q = malloc(sizeof(struct Queue));
  
  if (q == NULL)
    return NULL;

  if (pthread_mutex_init(&q->mutex, NULL)){
    free(q);
    return NULL;
  }

  q->head = NULL;
  q->tail = NULL;
  return q;
}

void queue_free(Queue* q, void (*func)(void *)){
  while (q->head)
  {
    QueueNode* node = queue_get(q);
    if (func)
      func(node->data);
    free(node);
  }
  pthread_mutex_destroy(&q->mutex);
  free(q);
}

QueueNode* queue_get(Queue *q){
  QueueNode* node;

  pthread_mutex_lock(&q->mutex);
  node = q->head;
  if (node != NULL){
    q->head = node->next;
    if (q->tail == node)
      q->tail = NULL;
  }
  pthread_mutex_unlock(&q->mutex);
  return node;
}

QueueNode* queue_put(Queue* q, void *data){
  QueueNode* node = malloc(sizeof(struct QueueNode));

  if (node == NULL)
    return NULL;

  node->data = data;
  node->next = NULL;

  pthread_mutex_lock(&q->mutex);
    if (q->head == NULL)
      q->head = q->tail = node;
    else{
      q->tail->next = node;
      q->tail = node;
    }
  pthread_mutex_unlock(&q->mutex);

  return node;
}

size_t queue_len(Queue* q){
  size_t len = 0;

  pthread_mutex_lock(&q->mutex);
  for (QueueNode *node = q->head; node; node = node->next)
    len++;
  pthread_mutex_unlock(&q->mutex);
  return (len);
}
