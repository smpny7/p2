#include "eop-test1func.c"

int main()
{
    test_subst();
    test_split();
    test_get_line();
    test_parse_line();

    printf("Done.\n");

    return 0;
}

/**
 * Substitute C1 to C2 in string STR.
 * return: number of replacement
 */
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

/**
 * Split STR using delimiter char SEP.
 * RET[] points each split element.
 * return: number of split elements
 */
int split(char *str, char *ret[], char sep, int max)
{
    int count = 0;
    ret[count++] = str; // ret0番目に格納

    while (*str && count < max) // 文字があるかつmax値より小さいとき
    {
        if (*str == sep) // 文字がカンマだったとき
        {
            *str = '\0'; // NULL終端に置き換え
            ret[count++] = str + 1; // 次の文頭以降の文字を配列に格納
        }
        str++;
    }

    return count;
}

/**
 * Read at most the number of characters
 * specified by MAX_LINE_LEN and store them in the string LINE including '\n'.
 * LINE must have a room for MAX_LINE_LEN+1 chars (additional 1 is for \0).
 */
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

/**
 * Parse a single-line input from stdin.
 * if the first character of the LINE is '%', it dispatches the input to the command
 * workhorse, otherwise, CSV parser of Profile is invoked.
 */
void parse_line(char *line)
{
    // nop
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
