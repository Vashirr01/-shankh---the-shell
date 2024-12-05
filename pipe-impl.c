#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*GOAL: ls | wc*/

int main() {
  int fildes[2];
  pipe(fildes);

  int pid = fork();
  if (pid == -1) {
    perror("fork error") ;
  }
  else if (pid == 0) {
  dup2(fildes[1], STDOUT_FILENO);
    close(fildes[0]);
    close(fildes[1]);
    execlp("ls", "ls", NULL);
  }
  else{
  dup2(fildes[0], STDIN_FILENO);
    close(fildes[0]);
    close(fildes[1]);
    execlp("wc", "wc", NULL);
  }
}
