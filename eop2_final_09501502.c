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
    char note[1024];
};

int profile_data_nitems = 0;
struct profile profile_data_store[10000];
FILE *fp;

int get_line(char *line);
void parse_line(char *line);

void swap(struct profile *a, struct profile *b)
{
    struct profile tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

void quicksort_id(int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        int x = profile_data_store[mid].id;
        int i = low;
        int j = high;
        while (i <= j)
        {
            while (profile_data_store[i].id < x)
                i += 1;
            while (profile_data_store[j].id > x)
                j -= 1;
            if (i <= j)
                swap(&profile_data_store[i++], &profile_data_store[j--]);
        }
        quicksort_id(low, j);
        quicksort_id(i, high);
    }
}

void quicksort_name(int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        char x[70];
        strcpy(x, profile_data_store[mid].name);
        int i = low;
        int j = high;
        while (i <= j)
        {
            while (strcmp(profile_data_store[i].name, x) < 0)
                i += 1;
            while (strcmp(profile_data_store[j].name, x) > 0)
                j -= 1;
            if (i <= j)
                swap(&profile_data_store[i++], &profile_data_store[j--]);
        }
        quicksort_name(low, j);
        quicksort_name(i, high);
    }
}

void quicksort_address(int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        char x[70];
        strcpy(x, profile_data_store[mid].address);
        int i = low;
        int j = high;
        while (i <= j)
        {
            while (strcmp(profile_data_store[i].address, x) < 0)
                i += 1;
            while (strcmp(profile_data_store[j].address, x) > 0)
                j -= 1;
            if (i <= j)
                swap(&profile_data_store[i++], &profile_data_store[j--]);
        }
        quicksort_address(low, j);
        quicksort_address(i, high);
    }
}

void quicksort_note(int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        char x[1024];
        strcpy(x, profile_data_store[mid].note);
        int i = low;
        int j = high;
        while (i <= j)
        {
            while (strcmp(profile_data_store[i].note, x) < 0)
                i += 1;
            while (strcmp(profile_data_store[j].note, x) > 0)
                j -= 1;
            if (i <= j)
                swap(&profile_data_store[i++], &profile_data_store[j--]);
        }
        quicksort_note(low, j);
        quicksort_note(i, high);
    }
}

int compare_date(struct date *d1, struct date *d2)
{
    if (d1->y != d2->y)
        return d1->y - d2->y;
    if (d1->m != d2->m)
        return d1->m - d2->m;
    return d1->d - d2->d;
}

void quicksort_birthday(int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        struct date x = profile_data_store[mid].birthday;
        int i = low;
        int j = high;
        while (i <= j)
        {
            while (compare_date(&profile_data_store[i].birthday, &x) < 0)
                i += 1;
            while (compare_date(&profile_data_store[j].birthday, &x) > 0)
                j -= 1;
            if (i <= j)
                swap(&profile_data_store[i++], &profile_data_store[j--]);
        }
        quicksort_birthday(low, j);
        quicksort_birthday(i, high);
    }
}

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
    if (num == 0) // 引数が0の場合
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
    else if (num > 0) // 引数が正の場合
    {
        if (num > profile_data_nitems) // 要素数よりも大きい値が入力された場合
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
    else if (num < 0) // 引数が負の場合
    {
        if (num < -profile_data_nitems) // 要素数よりも大きい値が入力された場合
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
    char line[MAX_LINE_LEN + 1];
    fp = fopen(param, "r");
    if (fp != NULL)
    {
        while (get_line(line))
        {
            parse_line(line);
        }
    }
    else
    {
        fprintf(stderr, "Enterd file cannot be opened.\n");
    }
    fclose(fp);
}

void cmd_write(char cmd, char *param)
{
    int i;
    fp = fopen(param, "w");
    if (fp != NULL)
    {
        for (i = 0; i < profile_data_nitems; i++)
        {
            fprintf(fp, "%d,%s,%04d-%02d-%02d,%s,%s\n", profile_data_store[i].id, profile_data_store[i].name, profile_data_store[i].birthday.y, profile_data_store[i].birthday.m, profile_data_store[i].birthday.d, profile_data_store[i].address, profile_data_store[i].note);
        }
    }
    else
    {
        fprintf(stderr, "Enterd file cannot be opened.\n");
    }
    fclose(fp);
}

void cmd_find(char cmd, char *param)
{
    int i;
    char id_tmp[9];
    char birthday_tmp[11];
    struct profile *p;
    for (i = 0; i < profile_data_nitems; i++)
    {
        p = &profile_data_store[i];
        sprintf(id_tmp, "%d", p->id);
        sprintf(birthday_tmp, "%04d-%02d-%02d", p->birthday.y, p->birthday.m, p->birthday.d);
        if (
            strcmp(id_tmp, param) == 0 ||
            strcmp(p->name, param) == 0 ||
            strcmp(birthday_tmp, param) == 0 ||
            strcmp(p->address, param) == 0 ||
            strcmp(p->note, param) == 0)
        {
            printf("Id    : %d\n", profile_data_store[i].id);
            printf("Name  : %s\n", profile_data_store[i].name);
            printf("Birth : %04d-%02d-%02d\n", profile_data_store[i].birthday.y, profile_data_store[i].birthday.m, profile_data_store[i].birthday.d);
            printf("Addr. : %s\n", profile_data_store[i].address);
            printf("Comm. : %s\n\n", profile_data_store[i].note);
        }
    }
}

void cmd_sort(char cmd, char *param)
{
    switch (atoi(param))
    {
    case 1:
        quicksort_id(0, profile_data_nitems - 1);
        break;
    case 2:
        quicksort_name(0, profile_data_nitems - 1);
        break;
    case 3:
        quicksort_birthday(0, profile_data_nitems - 1);
        break;
    case 4:
        quicksort_address(0, profile_data_nitems - 1);
        break;
    case 5:
        quicksort_note(0, profile_data_nitems - 1);
        break;
    default:
        break;
    }
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
        fprintf(stderr, "Unregistered Command Is Entered.\n");
        break;
    }
}

int subst(char *str, char c1, char c2)
{
    int diff = 0;
    char *p;

    p = str; // 文字の先頭にポインタをあわせる
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
    int count = 1;
    ret[0] = str;

    while (*str)
    {
        if (count >= max) // 規定値よりも多い時
            break;
        if (*str == sep)
        {
            *str = '\0'; // NULL終端に置き換え
            ret[count++] = str + 1;
        }
        str++;
    }

    return count;
}

int get_line(char *line)
{
    if (fp != NULL && fgets(line, MAX_LINE_LEN + 1, fp) != NULL)
    {
        subst(line, '\n', '\0');
        return 1;
    }
    if (fgets(line, MAX_LINE_LEN + 1, stdin) == NULL)
    {
        return 0;
    }
    else
    {
        subst(line, '\n', '\0');
        return 1;
    }
}

int new_profile(struct profile *profile_data_store, char *line)
{
    int max_line = 5, max_date = 3;
    char *ret[80] = {0}, *date[80] = {0}, sep_line = ',', sep_date = '-';

    if (split(line, ret, sep_line, max_line) != 5)
    {
        return -1;
    }

    split(line, ret, sep_line, max_line);
    split(ret[2], date, sep_date, max_date);

    profile_data_store->id = atoi(ret[0]);
    strcpy(profile_data_store->name, ret[1]);

    profile_data_store->birthday.y = atoi(date[0]);
    profile_data_store->birthday.m = atoi(date[1]);
    profile_data_store->birthday.d = atoi(date[2]);

    strcpy(profile_data_store->address, ret[3]);
    strcpy(profile_data_store->note, ret[4]);
    return 0;
}

void parse_line(char *line)
{
    if (*line == '%')
    {
        exec_command(line[1], &line[3]);
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
