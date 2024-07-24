/*
 * Using a while loop, write a function called factorial that takes a single
 * parameter n and calculates n! (Note: n! = n * (n-1) * (n-2) * ... * 1)
 */

#include <stdio.h>

int main() {
  printf("Calculate factorial: ");
  int num = 0;
  scanf("%d", &num);

  if (num < 1) {
    printf("Number must be greater than 0!");
    return 0;
  }

  // compute factorial
  int factorial = num;
  unsigned long long int out = 1;
  while (num > 1) {
    out *= num;
    num--;
  }

  printf("The factorial of %d is %llu.\n", factorial, out);

  return 0;
}
