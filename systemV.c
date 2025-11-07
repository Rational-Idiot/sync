#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  char *str = mmap(NULL, 1024, PROT_READ | PROT_WRITE,
                   MAP_ANONYMOUS | MAP_SHARED, -1, 0);

  int pid = fork();
  if (pid == 0) {
    strcpy(str, "Varun is Gay\n");
    printf("Writer wrote: %s", str);
  } else {
    sleep(1);
    printf("The reader read: %s", str);
  }

  munmap(NULL, 1024);
  return 0;
}
