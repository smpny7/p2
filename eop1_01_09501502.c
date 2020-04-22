#include <stdio.h>

int main() {
  char s[4] = {'A', 'B', 'C', '\0'};  // ← (1) & (2)
  char *p;
  int i, x;

  p = s;
  while (*p != '\0') {
    p++;
  }
  x = p - s;

  p = s;
  for (i = 0; i < x + 1; i++) {  // ← (4)
    if ('A' <= s[i] && s[i] <= 'Z') {
      s[i] += ('a' - 'A');
    }
  }
  return 0;
}