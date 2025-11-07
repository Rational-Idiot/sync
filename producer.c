#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

sem_t full, empty;
pthread_mutex_t lock;
int buff[50];
int in = 0, out = 0;

void *producer(void *args) {
  int64_t id = (int64_t)args;
  static int item = 0;
  while (1) {
    item++;
    sem_wait(&empty);
    pthread_mutex_lock(&lock);

    buff[in] = item;
    printf("Producer %ld: produced item %d\n", id, item);
    in = (in + 1) % 50;

    pthread_mutex_unlock(&lock);
    sem_post(&full);
    sleep(1);
  }
  pthread_exit(NULL);
}

void *consumer(void *args) {
  int64_t id = (int64_t)args;

  while (1) {
    sem_wait(&full);
    pthread_mutex_lock(&lock);

    int consumed = buff[out];
    printf("Consumer %ld: consumed item %d\n", id, consumed);
    out = (out + 1) % 50;

    pthread_mutex_unlock(&lock);
    sem_post(&empty);
    sleep(2);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_mutex_init(&lock, NULL);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, 50);

  pthread_t consumers[10], producers[10];
  for (int64_t i = 0; i < 10; i++) {
    pthread_create(&producers[i], NULL, producer, (void *)i);
    pthread_create(&consumers[i], NULL, consumer, (void *)i);
  }

  for (int64_t i = 0; i < 10; i++) {
    pthread_join(producers[i], NULL);
    pthread_join(consumers[i], NULL);
  }

  sem_destroy(&full);
  sem_destroy(&empty);
  pthread_mutex_destroy(&lock);
}
