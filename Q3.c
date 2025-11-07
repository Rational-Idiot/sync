#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>

int cnt = 0;
sem_t write;
sem_t queue;
sem_t read;
int reader_cnt = 0;

void *writer(void *args) {
  long id = (long)args;
  sem_wait(&queue);
  sem_wait(&write);
  int old = cnt;
  cnt++;
  printf("Writer %ld: Incremented the vlaue of cnt from %d to %d\n", id,
         cnt - 1, cnt);
  sem_post(&write);
  sem_post(&queue);
  return NULL;
}

void *reader(void *args) {
  long id = (long)args;

  sem_wait(&queue);

  sem_wait(&read);
  reader_cnt++;
  if (reader_cnt == 1)
    sem_wait(&write);
  sem_post(&read);
  sem_post(&queue);

  printf("Reader %ld: The current value of cnt is %d\n", id, cnt);

  sem_wait(&read);
  reader_cnt--;
  if (reader_cnt == 0)
    sem_post(&write);
  sem_post(&read);

  return NULL;
}

int main() {
  sem_init(&write, 0, 1);
  sem_init(&queue, 0, 1);
  sem_init(&read, 0, 1);

  pthread_t readers[10], writers[15];
  for (long i = 0; i < 10; i++)
    pthread_create(&readers[i], NULL, reader, (void *)i);
  for (long i = 0; i < 15; i++)
    pthread_create(&writers[i], NULL, writer, (void *)i);

  for (int i = 0; i < 10; i++)
    pthread_join(readers[i], NULL);
  for (int i = 0; i < 15; i++)
    pthread_join(writers[i], NULL);

  sem_destroy(&write);
  sem_destroy(&queue);
  sem_destroy(&read);
}
