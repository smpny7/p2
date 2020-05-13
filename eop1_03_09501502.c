#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

void cmd_quit(void) {
    exit(0);
}

void cmd_check(char cmd) {
    fprintf(stderr, "%%%c command is invoked with no arg\n", cmd);
}

void cmd_print(char cmd, char *param) {
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd, param);
}

void cmd_read(char cmd, char *param) {
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd, param);
}

void cmd_write(char cmd, char *param) {
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd, param);
}

void cmd_find(char cmd, char *param) {
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd, param);
}

void cmd_sort(char cmd, char *param) {
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd, param);
}

void exec_command(char cmd, char *param) {
    switch (cmd) {
        case 'Q': cmd_quit();   break;
        case 'C': cmd_check(cmd);  break;
        case 'P': cmd_print(cmd, param);  break;
        case 'R': cmd_read(cmd, param);   break;
        case 'W': cmd_write(cmd, param);  break;
        case 'F': cmd_find(cmd, param);   break;
        case 'S': cmd_sort(cmd, param);   break;
        default:
        printf("Unregistered Command Is Entered.\n");
    }
}

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
    subst(str, sep, '\0');
    for (i=0; i<max; i++) {
        ret[i] = str;
        str += strlen(str)+1;
        count++;
    }
    return count;
}

int get_line(char *line) {
    if (fgets(line, MAX_LINE_LEN + 1, stdin) == NULL || strlen(line) > MAX_LINE_LEN || *line == '\n') {
        return 0;
    } else {
        subst(line, '\n', '\0');
        return 1;
    }
}

void new_profile(char *line) {
    int i, max = 5;
    char *ret[80] = {0}, sep = ',';
    split(line, ret, sep, max);
    for (i = 0; i < max; i++) {
        printf(" >ret[%d] = '%s'\n", i, ret[i]);
    }
}

void parse_line(char *line) {
    char cmd, param[80] = {0};
    if (*line == '%') {
        cmd = *(line+1);
        strcpy(param, line+3);
        exec_command(cmd, param);
    } else {
        new_profile(line);
    }
}

int main(void) {
    char line[MAX_LINE_LEN + 1];
    while (get_line(line)) {
        parse_line(line);
    }
    return 0;
}
