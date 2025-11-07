#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define N 10
bool choosing[N];
int ticket[N];
int num[N];

void unlock(int i) { num[i] = 0; }

int maxel(int n, int *arr) {
  int res = 0;
  for (int i = 0; i < n; i++)
    if (res < arr[i])
      res = arr[i];
  return res;
}

void lock(int i) {
  choosing[i] = true;
  num[i] = maxel(N, num) + 1;
  choosing[i] = false;

  for (int p = 0; p < N; p++) {
    while (choosing[p])
      ;
    while (num[p] != 0 && ((num[p] < num[i]) || (num[p] == num[i] && p < i)))
      ;
  }
}

void *functhread(void *args) {
  long i = (long)args;
  for (int k = 0; k < 3; k++) {
    lock(i);
    printf("Thread %ld entered the CS\n", i);
    static int cnt = 0;
    int temp = cnt;
    usleep(10000);
    cnt = temp + 1;

    printf("Thread %ld updated the cnt to %d \n", i, cnt);
    unlock(i);
    usleep(10000);
  }
  return NULL;
}

int main() {
  pthread_t t[N];

  for (long i = 0; i < N; i++)
    pthread_create(&t[i], NULL, functhread, (void *)i);
  for (long i = 0; i < N; i++)
    pthread_join(t[i], NULL);

  return 0;
}
