#include <stdio.h>

int main() {
  int age = 0;

  printf("How old are you?: ");
  scanf("%d", &age);

  if (age < 18) {
    printf("You are too young to use this program.\n");
    return 0;
  }

  if (age % 2 == 0) { // Even age
    printf("Try again next year.\n");
    return 0;
  } else {
      age++;
      printf("You will be %d years old on your next birthday.\n", age);
  }

  return 0;
}
