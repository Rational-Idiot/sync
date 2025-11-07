#include <pthread.h>
#include <stdio.h>
#include <time.h>

int global = 0;

void *pthread_func(void *attr) {
  for (int i = 0; i < 10000; i++)
    global++;
}

int main() {
  pthread_t t1, t2;
  pthread_create(&t1, NULL, pthread_func, NULL);
  pthread_create(&t2, NULL, pthread_func, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("The value of global is: %d", global);
}
