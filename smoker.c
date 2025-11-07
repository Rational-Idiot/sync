#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t agent, smoker1, smoker2, smoker3;

void *agent_thread(void *arg) {
  for (int i = 0; i < 60; i++) {
    sem_wait(&agent);
    int choice = rand() % 3;

    if (choice == 0) {
      printf("Agent: Paper + Match\n");
      sem_post(&smoker1);
    } else if (choice == 1) {
      printf("Agent: Tobacco + Match\n");
      sem_post(&smoker2);
    } else {
      printf("Agent: Tobacco + Paper\n");
      sem_post(&smoker3);
    }
    sleep(1);
  }
  return NULL;
}

void *smoker(void *arg) {
  char *name = (char *)arg;
  for (int i = 0; i < 20; i++) {
    sem_wait((sem_t *)((name[0] == 'T')   ? &smoker1
                       : (name[0] == 'P') ? &smoker2
                                          : &smoker3));
    printf("%s smokes\n", name);
    sem_post(&agent);
    sleep(1);
  }
  return NULL;
}

int main() {
  pthread_t a, s1, s2, s3;

  sem_init(&agent, 0, 1);
  sem_init(&smoker1, 0, 0);
  sem_init(&smoker2, 0, 0);
  sem_init(&smoker3, 0, 0);

  pthread_create(&a, NULL, agent_thread, NULL);
  pthread_create(&s1, NULL, smoker, "Tobacco");
  pthread_create(&s2, NULL, smoker, "Paper");
  pthread_create(&s3, NULL, smoker, "Match");

  pthread_join(a, NULL);

  return 0;
}
