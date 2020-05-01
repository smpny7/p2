#include <stdio.h>

int subst(char *str, char c1, char c2) {
  int diff = 0;
  char *p;

  p = str;

  while (*p != '\0') {
    if (*p == c1) {
      *p = c2;
      diff++;
    }
    p++;
  }

  return diff;
}

int main(void) {
  char c1 = 'A';
  char c2 = 'B';
  char s[80] = "ABCDE";

  printf("文字列: %s\n置換文字: %c -> %c\n\n", s, c1, c2);
  printf("実行結果: %s\n差分: %d\n", s, subst(s, c1, c2));
}
