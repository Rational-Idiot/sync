#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_mutex_t readers;

void *reader(void *attr) {
  pthread_mutex_lock(&lock);
  pthread_mutex_lock(&readers);
  FILE *fp = fopen("file.txt", "r");
  char *stream = (char *)malloc(100 * sizeof(char));
  printf("The contents of the file are as follow:\n");
  while (fgets(stream, 100, fp))
    printf("%s", stream);
  fclose(fp);
  pthread_mutex_unlock(&lock);
  return NULL;
}

void *writer(void *attr) {
  pthread_mutex_lock(&lock);
  FILE *fp = fopen("file.txt", "a");
  fprintf(fp, "\nOs is not that boring");
  fclose(fp);
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main() {
  pthread_t r1, w2, w1, w3;
  pthread_create(&r1, NULL, reader, NULL);
  pthread_create(&w2, NULL, writer, NULL);
  pthread_create(&w1, NULL, writer, NULL);
  pthread_create(&w3, NULL, writer, NULL);
  pthread_join(r1, NULL);
  pthread_join(w2, NULL);
  pthread_join(w1, NULL);
  pthread_join(w3, NULL);
}
