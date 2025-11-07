#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int cnt = 0;
pthread_mutex_t write_mutex;
pthread_mutex_t queue;
pthread_mutex_t read_mutex;
int reader_cnt = 0;

void *writer(void *args) {
  long id = (long)args;

  pthread_mutex_lock(&queue);

  pthread_mutex_lock(&write_mutex);
  int old = cnt;
  cnt++;
  printf("Writer %ld: Incremented the vlaue of cnt from %d to %d\n", id,
         cnt - 1, cnt);
  pthread_mutex_unlock(&write_mutex);

  pthread_mutex_unlock(&queue);
  return NULL;
}

void *reader(void *args) {
  long id = (long)args;

  pthread_mutex_lock(&queue);

  pthread_mutex_lock(&read_mutex);
  reader_cnt++;
  if (reader_cnt == 1)
    pthread_mutex_lock(&write_mutex);
  pthread_mutex_unlock(&read_mutex);

  pthread_mutex_unlock(&queue);

  printf("Reader %ld: The current value of cnt is %d\n", id, cnt);

  pthread_mutex_lock(&read_mutex);
  reader_cnt--;
  if (reader_cnt == 0)
    pthread_mutex_unlock(&write_mutex);
  pthread_mutex_unlock(&read_mutex);

  return NULL;
}

int main() {
  pthread_mutex_init(&write_mutex, NULL);
  pthread_mutex_init(&queue, NULL);
  pthread_mutex_init(&read_mutex, NULL);

  pthread_t readers[10], writers[10];
  for (long i = 0; i < 10; i++) {
    pthread_create(&readers[i], NULL, reader, (void *)i);
    pthread_create(&writers[i], NULL, writer, (void *)i);
  }

  for (int i = 0; i < 10; i++) {
    pthread_join(readers[i], NULL);
    pthread_join(writers[i], NULL);
  }

  pthread_mutex_destroy(&write_mutex);
  pthread_mutex_destroy(&queue);
  pthread_mutex_destroy(&read_mutex);
}
