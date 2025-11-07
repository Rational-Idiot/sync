#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

sem_t read_mutex;
sem_t write_mutex;
sem_t priority;
int read_count = 0;

void *reader(void *args) {
  int64_t shmid = (int64_t)args;
  sem_wait(&priority);

  sem_wait(&read_mutex);
  read_count++;
  if (read_count == 1)
    sem_wait(&write_mutex);
  sem_post(&read_mutex);

  int64_t *str = (int64_t *)shmat(shmid, NULL, 0);
  printf("The reader saw: %ld\n", *str);
  shmdt(str);

  sem_wait(&read_mutex);
  read_count--;
  if (read_count == 0)
    sem_post(&write_mutex);
  sem_post(&read_mutex);

  sem_post(&priority);
  return NULL;
}

void *writer(void *args) {
  int64_t *arg = (int64_t *)args;
  int64_t shmid = arg[0];
  int64_t val = arg[1];
  sem_wait(&priority);
  sem_wait(&write_mutex);

  int64_t *str = (int64_t *)shmat(shmid, NULL, 0);

  *str = 420 + val;
  printf("writer wrote: %ld\n", *str);
  shmdt(str);

  sem_post(&write_mutex);
  sem_post(&priority);
  return NULL;
}

int main(int argc, char *argv[]) {
  sem_init(&read_mutex, 0, 1);
  sem_init(&write_mutex, 0, 1);
  sem_init(&priority, 0, 1);

  key_t key = ftok("shm", 69);
  int64_t shmid = shmget(key, 1024, 0666 | IPC_CREAT);

  pthread_t writers[10], readers[10];
  for (int64_t i = 0; i < 10; i++) {
    int64_t arg[2];
    arg[0] = shmid;
    arg[1] = i;
    pthread_create(&readers[i], NULL, reader, (void *)shmid);
    pthread_create(&writers[i], NULL, writer, arg);
  }

  for (int i = 0; i < 10; i++) {
    pthread_join(readers[i], NULL);
    pthread_join(writers[i], NULL);
  }

  sem_destroy(&read_mutex);
  sem_destroy(&write_mutex);
  sem_destroy(&priority);
  shmctl(shmid, IPC_RMID, NULL);
}
