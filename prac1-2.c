#include <stdio.h>

int main() {
  char s[4] = "ABCD";
  // char s[4] = {65, 66, 67, 68, 0};
  char *p;
  int i, x;

  p = s;
  while (*p != '\0') {
    p++;
  }
  // xは先頭から何文字目か示す
  x = p - s;

  p = s;
  for (i = 0; i < x + 1; i++) {  // ← (4)
    if ('A' <= s[i] && s[i] <= 'Z') {
      s[i] += ('a' - 'A');
    }
  }
  return 0;
}
