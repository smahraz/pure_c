#ifndef PURE_C_QUEUE
# define PURE_C_QUEUE
# include <pthread.h>


typedef struct QueueNode {
  void* data;
  struct QueueNode* next;
} QueueNode;


typedef struct Queue {

  QueueNode* head;
  QueueNode* tail;

  pthread_mutex_t mutex;

  size_t lenght;

} Queue;

Queue* queue_init(void);
QueueNode* queue_get(Queue *q);
void queue_free(Queue* q, void (*func)(void *));
QueueNode* queue_put(Queue* q, void* data);
size_t queue_lenght(Queue* q);

#endif 

