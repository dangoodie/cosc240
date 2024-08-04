#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
int main() {
  int x;
  int status;
  pid_t child;
  x = 0;
  child = fork();
  if (child) {
    // In parent
    printf("Child's id is: %d\n", child);
    if (-1 == waitpid(child, &status, 0)) {
      printf("No child process to wait for!\n");
    } else if (WIFEXITED(status)) {
        printf("Child exited with exit status : %d\n", WEXITSTATUS(status));
    } else {
      printf("Child exited abnormally!\n");
    }
  } else {
    // In child
    printf("Since I am the child, I got back: %d\n", child);
    x = 1;
  }
  printf("The value of x is: %d\n", x);
  return x;
}
