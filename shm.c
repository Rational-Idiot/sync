#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pid = fork();
  if (pid == 0) {
    key_t key = ftok("shm", 69);
    long shmid = shmget(key, 1024, 0666);

    long *str = (long *)shmat(shmid, NULL, 0);
    printf("\nThe reader saw: %ld", *str);
    shmdt(str);
  } else {
    key_t key = ftok("shm", 69);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    long *str = (long *)shmat(shmid, NULL, 0);

    *str = 420;
    printf("writer wrote: %d", *str);
    shmdt(str);
  }
}
