#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int cnt = 0;
pthread_mutex_t write_mutex;
pthread_mutex_t priority;
pthread_mutex_t read_mutex;
int reader_cnt = 0;

void *writer(void *args) {
  long id = (long)args;

  pthread_mutex_lock(&priority);

  pthread_mutex_lock(&write_mutex);
  int old = cnt;
  cnt++;
  printf("Writer %ld: Incremented the vlaue of cnt from %d to %d\n", id,
         cnt - 1, cnt);
  pthread_mutex_unlock(&write_mutex);

  pthread_mutex_unlock(&priority);
  return NULL;
}

void *reader(void *args) {
  long id = (long)args;

  pthread_mutex_lock(&priority);

  pthread_mutex_lock(&read_mutex);
  reader_cnt++;
  if (reader_cnt == 1)
    pthread_mutex_lock(&write_mutex);
  pthread_mutex_unlock(&read_mutex);

  pthread_mutex_unlock(&priority);

  printf("Reader %ld: The current value of cnt is %d\n", id, cnt);

  pthread_mutex_lock(&read_mutex);
  reader_cnt--;
  if (reader_cnt == 0)
    pthread_mutex_unlock(&write_mutex);
  pthread_mutex_unlock(&read_mutex);

  usleep(100000);
  return NULL;
}

int main() {
  pthread_mutex_init(&write_mutex, NULL);
  pthread_mutex_init(&priority, NULL);
  pthread_mutex_init(&read_mutex, NULL);

  pthread_t readers[10], writers[15];
  for (long i = 0; i < 10; i++)
    pthread_create(&readers[i], NULL, reader, (void *)i);
  for (long i = 0; i < 15; i++)
    pthread_create(&writers[i], NULL, writer, (void *)i);

  for (int i = 0; i < 10; i++)
    pthread_join(readers[i], NULL);
  for (int i = 0; i < 15; i++)
    pthread_join(writers[i], NULL);

  pthread_mutex_destroy(&write_mutex);
  pthread_mutex_destroy(&priority);
  pthread_mutex_destroy(&read_mutex);
}
