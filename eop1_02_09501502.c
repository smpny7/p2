#include <stdio.h>
#include <string.h>

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

int split(char *str, char *ret[], char sep, int max) {
    int i, count = 0;
    for (i=0; i<max; i++) {
        ret[i] = str;
        str += strlen(str)+1;
        count++;
    }
    return count;
}

int main(void) {
    int i, max = 5;
    char s[] = "5100046,The Bridge,1845-11-2,14 Seafield Road Longman Inverness,SEN Unit 2.0 Open";
    subst(s, ',', '\0');
    char *ret[5] = {0};
    char sep = ',';

    printf("element: %d\n", split(s, ret, sep, max));

    for (i = 0; i < max; i++) {
        printf("ret[%d] = '%s'\n", i, ret[i]);
    }
}
