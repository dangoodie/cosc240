/* Write a function named draw_grid that takes an integer named width and an
integer named height as its parameters. The program should draw a grid like the
following, with width -s across and height |s down. For example, draw_grid(3,4)
should print:

+---+
|   |
|   |
|   |
|   |
+---+

*/

#include <stdio.h>

void draw_grid(int width, int height);

int main() {
  int width = 0;
  int height = 0;

  printf("Enter width: ");
  scanf("%d", &width);
  printf("Enter height: ");
  scanf("%d", &height);

  draw_grid(width, height);

  return 0;
}

void draw_grid(int width, int height) {
  // check for valid dimensions
  if (width <= 0 || height <= 0) {
    printf("Invalid dimensions!\n");
    return;
  }

  // top header
  printf("+");
  for (int x = 0; x < width; x++) {
    printf("-");
  }
  printf("+\n");

  // body
  for (int x = 0; x < height; x++) {
      printf("|");
        for (int y = 0; y < width; y++) {
          printf(" ");
        }
        printf("|\n");
  }

  // bottom header
  printf("+");
  for (int x = 0; x < width; x++) {
    printf("-");
  }
  printf("+\n");
}
