#include <linux/limits.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_SIZE 100
#define PROMPT "> "
int quit = 0;
int rc;

void handler(int signal) {
  if (rc == 0) {
    quit = 1;
  }
  }


int main(){
  int contains_pipe = 0;
  int cmdpassed = 0;
  struct sigaction sa;
  sa.sa_handler = handler;

    char command [MAX_SIZE]; 
    char* cmd1_after_pipes;
    char* cmd2_after_pipes;
    char* argv[MAX_SIZE];
    char* argv2[MAX_SIZE];
  int argc = 0;
  int argc2 = 0;

    printf("🐚 shankh - the shell\n");

    while (1) {
    printf(PROMPT);
  sigaction(SIGINT, &sa, NULL);
        if (quit == 1) {
      printf("exiting shell\n");
        exit(0);
    }

    if((fgets(command , MAX_SIZE, stdin) == NULL) && ferror(stdin)) {
      perror("fgets error");
      
      exit(1);
    }

    if (feof(stdin)) {
    exit(0);
    }
    else {
      cmdpassed = 1;
    }
  for (int i = 0; i<MAX_SIZE; i++) {
        if (command[i] == '|') {
          contains_pipe = 1;  
        } 
      }
      argc= 0;
      argc2 = 0;
    if (cmdpassed == 1) {
      if (contains_pipe == 1) {
               cmd1_after_pipes = strtok(command, "|");
      cmd2_after_pipes = strtok(NULL, "|");

    argv[argc] = strtok(cmd1_after_pipes, " \n\t");
      while (argv[argc] != NULL) {
      argv[++argc] = strtok(NULL, " \n\t");
      }

    argv2[argc2] = strtok(cmd2_after_pipes, " \n\t");
      while (argv2[argc2] != NULL) {
      argv2[++argc2] = strtok(NULL, " \n\t");
      }
      int fildes[2];
      pipe(fildes);
      int rc = fork();
        if (rc == -1) {
          perror("fork error") ;
        }
        else if (rc == 0) {
        dup2(fildes[1], STDOUT_FILENO);
          close(fildes[0]);
          close(fildes[1]);
        if (0 != execvp(argv[0], argv)) {
       perror("exec error"); 
            exit(1);
         }
        }
      int rc2  = fork();
      if (rc2 == -1) {
          perror("fork error") ;
        }
      else if (rc2 == 0) {
        dup2(fildes[0], STDIN_FILENO);
          close(fildes[0]);
          close(fildes[1]);

       if (0 != execvp(argv2[0], argv2)) {
       perror("exec error"); 
            exit(1);
         }
        }
        close(fildes[0]);
        close(fildes[1]);
        waitpid(rc, NULL, 0);
        waitpid(rc2, NULL, 0);

        }
      else{
    argv[argc] = strtok(command, " \n\t");

   
      while (argv[argc] != NULL) {
      argv[++argc] = strtok(NULL, " \n\t");
      }

      if (0 == strcmp("quit", argv[0])) {
        exit(0);
      }
      if (0 == strcmp("help", argv[0])) {
        printf("there is no help figure shit out\n");
        continue;
      }

      rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
        if (0 != execvp(argv[0], argv)) {
       perror("exec error"); 
            exit(1);
    }
  }
    int wc = wait(NULL);
   
      }

      cmdpassed = 0;
      contains_pipe = 0;
    }
  }
    return 0;
  }
