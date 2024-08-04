#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main() {
  int x;
  pid_t child;
  x = 0;
  child = fork();
  if (child) {
    // In parent
    printf("Child's id is: %d\n", child);
  } else {
    // In child
    printf("Since I am the child, I got back: %d\n", child);
    x = 1;
  }
  printf("The value of x is: %d\n", x);
  return x;
}
