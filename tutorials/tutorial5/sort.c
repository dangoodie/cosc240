/*
 * Write a C function called sort that takes an array of integers as its first
 * parameter and the number of elements in the array as its second parameter.
 * The function must have the array sorted from smallest to largest by the time
 * the function completes. Your function should return 1 if the sort was
 * successful, or 0 otherwise.
 */

#include <stdio.h>

int sort(int *arr, int n) {
  int i, j, temp;
  for (i = 0; i < n; i++) {
    for (j = i + 1; j < n; j++) {
      if (arr[i] > arr[j]) {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
  }
  return 1;
}

/*
 * Create a struct called Measurement that has an id (which is an integer) and a
 * value (which is a double). Write two sort functions: one to sort an array of
 * Measurements by id, and another to sort them by value.
 */

struct Measurement {
  int id;
  double value;
};

int sortById(struct Measurement *arr, int n) {
  int i, j;
  struct Measurement temp;
  for (i = 0; i < n; i++) {
    for (j = i + 1; j < n; j++) {
      if (arr[i].id > arr[j].id) {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
  }
  return 1;
}

int sortByValue(struct Measurement *arr, int n) {
  int i, j;
  struct Measurement temp;
  for (i = 0; i < n; i++) {
    for (j = i + 1; j < n; j++) {
      if (arr[i].value > arr[j].value) {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
  }
  return 1;
}

int main() {
  int arr[] = {5, 3, 2, 1, 4};
  int n = sizeof(arr) / sizeof(arr[0]);
  printf("Array before sorting: ");
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  printf("Array after sorting: ");
  sort(arr, n);
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  printf("\n");

  struct Measurement measurements[] = {{2, 5.4}, {1, 2.3}, {3, 4.5}};
  printf("Measurements before sorting: \n");
  n = sizeof(measurements) / sizeof(measurements[0]);
  for (int i = 0; i < n; i++) {
    printf("id: %d value: %f\n", measurements[i].id, measurements[i].value);
  }
  printf("\n");

  printf("Measurements after sorting by id: \n");
  sortById(measurements, n);
  for (int i = 0; i < n; i++) {
    printf("id: %d value: %f\n", measurements[i].id, measurements[i].value);
  }
  printf("\n");
    
  printf("Measurements after sorting by value: \n");
  sortByValue(measurements, n);
  for (int i = 0; i < n; i++) {
    printf("id: %d value: %f\n", measurements[i].id, measurements[i].value);
  }

  return 0;
}
