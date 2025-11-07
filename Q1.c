#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int global = 0;
sem_t lock;

void *threadfunc(void *args) {
  for (int i = 0; i < 10000; i++) {
    sem_wait(&lock);
    global++;
    sem_post(&lock);
  }
  return NULL;
}

int main() {
  pthread_t lmao, rofl;
  sem_init(&lock, 0, 1);
  pthread_create(&lmao, NULL, threadfunc, NULL);
  pthread_create(&rofl, NULL, threadfunc, NULL);

  pthread_join(lmao, NULL);
  pthread_join(rofl, NULL);
  sem_destroy(&lock);

  printf("The value of global is: %d", global);
}
