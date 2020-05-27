#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

struct date
{
    int y;
    int m;
    int d;
};

struct profile
{
    int id;
    char name[70];
    struct date birthday;
    char address[70];
    char note[80];
};

int profile_data_nitems = 0;
struct profile profile_data_store[10000];

void cmd_quit(void)
{
    exit(0);
}

void cmd_check(char cmd)
{
    printf("%d profile(s)\n", profile_data_nitems);
}

void cmd_print(char cmd, char *param)
{
    int count, num = atoi(param);
    if (num == 0)
    {
        count = 0;
        while (count < profile_data_nitems)
        {
            printf("Id    : %d\n", profile_data_store[count].id);
            printf("Name  : %s\n", profile_data_store[count].name);
            printf("Birth : %04d-%02d-%02d\n", profile_data_store[count].birthday.y, profile_data_store[count].birthday.m, profile_data_store[count].birthday.d);
            printf("Addr. : %s\n", profile_data_store[count].address);
            printf("Comm. : %s\n\n", profile_data_store[count].note);
            count++;
        }
    }
    else if (num > 0)
    {
        if (num > profile_data_nitems)
            num = profile_data_nitems;
        count = 0;
        while (count < num)
        {
            printf("Id    : %d\n", profile_data_store[count].id);
            printf("Name  : %s\n", profile_data_store[count].name);
            printf("Birth : %04d-%02d-%02d\n", profile_data_store[count].birthday.y, profile_data_store[count].birthday.m, profile_data_store[count].birthday.d);
            printf("Addr. : %s\n", profile_data_store[count].address);
            printf("Comm. : %s\n\n", profile_data_store[count].note);
            count++;
        }
    }
    else if (num < 0)
    {
        if (num < -profile_data_nitems)
            num = -profile_data_nitems;
        count = profile_data_nitems + num;
        while (count < profile_data_nitems)
        {
            printf("Id    : %d\n", profile_data_store[count].id);
            printf("Name  : %s\n", profile_data_store[count].name);
            printf("Birth : %04d-%02d-%02d\n", profile_data_store[count].birthday.y, profile_data_store[count].birthday.m, profile_data_store[count].birthday.d);
            printf("Addr. : %s\n", profile_data_store[count].address);
            printf("Comm. : %s\n\n", profile_data_store[count].note);
            count++;
        }
    }
}

void cmd_read(char cmd, char *param)
{
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd, param);
}

void cmd_write(char cmd, char *param)
{
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd, param);
}

void cmd_find(char cmd, char *param)
{
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd, param);
}

void cmd_sort(char cmd, char *param)
{
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd, param);
}

void exec_command(char cmd, char *param)
{
    switch (cmd)
    {
    case 'Q':
        cmd_quit();
        break;
    case 'C':
        cmd_check(cmd);
        break;
    case 'P':
        cmd_print(cmd, param);
        break;
    case 'R':
        cmd_read(cmd, param);
        break;
    case 'W':
        cmd_write(cmd, param);
        break;
    case 'F':
        cmd_find(cmd, param);
        break;
    case 'S':
        cmd_sort(cmd, param);
        break;
    default:
        printf("Unregistered Command Is Entered.\n");
    }
}

int subst(char *str, char c1, char c2)
{
    int diff = 0;
    char *p;

    p = str;
    while (*p != '\0')
    {
        if (*p == c1)
        {
            *p = c2;
            diff++;
        }
        p++;
    }
    return diff;
}

int split(char *str, char *ret[], char sep, int max)
{
    int i, count = 0;
    subst(str, sep, '\0');
    for (i = 0; i < max; i++)
    {
        ret[i] = str;
        str += strlen(str) + 1;
        count++;
    }
    return count;
}

int get_line(char *line)
{
    if (fgets(line, MAX_LINE_LEN + 1, stdin) == NULL || strlen(line) > MAX_LINE_LEN || *line == '\n')
    {
        return 0;
    }
    else
    {
        subst(line, '\n', '\0');
        return 1;
    }
}

void new_profile(struct profile *profile_data_store, char *line)
{
    int max_line = 5, max_date = 3;
    char *ret[80] = {0}, *date[80] = {0}, sep_line = ',', sep_date = '-';

    split(line, ret, sep_line, max_line);
    split(ret[2], date, sep_date, max_date);

    profile_data_store->id = atoi(ret[0]);
    strcpy(profile_data_store->name, ret[1]);

    profile_data_store->birthday.y = atoi(date[0]);
    profile_data_store->birthday.m = atoi(date[1]);
    profile_data_store->birthday.d = atoi(date[2]);

    strcpy(profile_data_store->address, ret[3]);
    strcpy(profile_data_store->note, ret[4]);
}

void parse_line(char *line)
{
    char cmd, param[80] = {0};
    if (*line == '%')
    {
        cmd = *(line + 1);
        strcpy(param, line + 3);
        exec_command(cmd, param);
    }
    else
    {
        new_profile(&profile_data_store[profile_data_nitems++], line);
    }
}

int main(void)
{
    char line[MAX_LINE_LEN + 1];
    while (get_line(line))
    {
        parse_line(line);
    }
    return 0;
}
