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

int get_line(char *line) {
    if (fgets(line, 1024, stdin) == NULL) {
        return 0;
    } else {
        subst(line, ',', '\0');
        // printf("%d", strlen(line));
        // line += 1024;
        // *line = 'A';
        return 1;
    }
}

int main(void) {
    int i, count = 1, max = 5;
    char line[1024] = {0};
    char *ret[5] = {0};
    char sep = ',';

    while (get_line(line)) {
        printf("Line%d\n", count++);
        split(line, ret, sep, max);
        for (i = 0; i < max; i++) {
            subst(ret[i], '\n', '\0');
            printf(" >ret[%d] = '%s'\n", i, ret[i]);
        }
    }
}
