#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t file_sem;

void *reader(void *attr) {
    sem_wait(&file_sem);
    FILE *fp = fopen("file.txt", "r");
    char *stream = malloc(100 * sizeof(char));
    printf("The contents of the file are as follow:\n");
    while (fgets(stream, 100, fp))
        printf("%s", stream);
    fclose(fp);
    free(stream);
    sem_post(&file_sem);
    return NULL;
}

void *writer(void *attr) {
    sem_wait(&file_sem);
    FILE *fp = fopen("file.txt", "a");
    fprintf(fp, "\nOs is not that boring");
    fclose(fp);
    sem_post(&file_sem);
    return NULL;
}

int main() {
    pthread_t r1, w1, w2, w3;
    sem_init(&file_sem, 0, 1);

    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&w1, NULL, writer, NULL);
    pthread_create(&w2, NULL, writer, NULL);
    pthread_create(&w3, NULL, writer, NULL);

    pthread_join(r1, NULL);
    pthread_join(w1, NULL);
    pthread_join(w2, NULL);
    pthread_join(w3, NULL);

    sem_destroy(&file_sem);
    return 0;
}

