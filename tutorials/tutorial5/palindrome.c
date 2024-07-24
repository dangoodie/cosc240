/*
 * Write a C program that reads through each line in the file
 * /usr/share/dict/words and outputs any palindrome to a file named
 * palindromes.txt (i.e. palindromes.txt should end up as a list of all
 * palindromes in the file /usr/share/dict/words).
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const char *boolToString(bool booleanValue) {
  return booleanValue ? "True" : "False";
}

bool is_palindrome(char word[]);

int main(int argc, char *argv[]) {
  FILE *dict = fopen("/usr/share/dict/words", "r");
  if (dict == NULL) {
    printf("Could not open dictionary!\n");
    return 1;
  }

  FILE *pals = fopen("palindromes.txt", "w+");
  if (pals == NULL) {
    printf("Could not create palindromes.txt!\n");
    fclose(dict);
    return 1;
  }

  char word[256];

  while (fgets(word, sizeof(word), dict)) {
    word[strcspn(word, "\n")] = 0;

    if (is_palindrome(word)) {
      fprintf(pals, "%s\n", word);
    }
  }

  fclose(dict);
  fclose(pals);
  return 0;
}

bool is_palindrome(char word[]) {
  int len = strlen(word);

  if (len <= 1) {
    return true;
  }

  char first = tolower(word[0]);
  char last = tolower(word[len - 1]);

  if (first != last) {
    return false;
  }

  if (len == 2) {
    return true;
  }

  char substring[len - 1];
  strncpy(substring, word + 1, len - 2);
  substring[len - 2] = '\0';

  return is_palindrome(substring);
}
