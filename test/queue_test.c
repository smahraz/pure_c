#include "queue.h"
#include <stdbool.h>
#include <stdlib.h>

bool test_len_and_free_func()
{
  Queue* q = queue_init();
  if (q == NULL)
    return true;
  for (int i=0; i < 10; i++){
    queue_put(q, malloc(sizeof(int)));
  }

  if (queue_lenght(q) != 10)
    return true;
  
  queue_free(q, &free);

  return false;
}

int main()
{
  if (test_len_and_free_func())
    return 1;
}
