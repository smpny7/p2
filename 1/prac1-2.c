#include <stdio.h>

int main() {
  char s[4] = "ABC";
  // char s[4] = {65, 66, 67, 0};
  char *p;
  int i, x;

  p = s;
  while (*p != '\0') {
    p++;
  }

  // xは先頭から何文字目か(=文字数)を示す
  x = p - s;
  printf("x: 文字数 | x = %d\n", x);

  p = s;

  for (i = 0; i < x + 1; i++) {
    if ('A' <= s[i] && s[i] <= 'Z') {
      s[i] += ('a' - 'A');
    }
  }

  // while (*p != '\0') {
  //   if ('A' <= *p && *p <= 'Z') {
  //     *p += ('a' - 'A');
  //   }
  //   p++;
  // }

  // ループを実行した結果，英字大文字が小文字に変換される
  printf("s: 実行結果 | s = %s\n", s);

  return 0;
}
