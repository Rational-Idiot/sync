#include <pthread.h>
#include <stdio.h>

int global = 0;
pthread_mutex_t lock;

void *pthread_func(void *attr) {
  for (int i = 0; i < 10000; i++) {
    pthread_mutex_lock(&lock);
    global++;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

int main() {
  pthread_t t1, t2;
  pthread_mutex_init(&lock, NULL);
  pthread_create(&t1, NULL, pthread_func, NULL);
  pthread_create(&t2, NULL, pthread_func, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("The value of global is: %d", global);
}
