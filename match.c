#include <stdio.h>
#include <string.h>
#include <ctype.h>

void upper(char *string)
{
    while (*string)
    {
        *string = toupper(*string);
        string++;
    }
}

int match(char *string, char *find)
{
    int i;
    char *string_tmp, *find_tmp;

    string_tmp = string;
    find_tmp = find;

    printf("%s\n", string_tmp);

    for (i = 0; i <= strlen(string); i++)
    {
        printf("find_tmp先頭: 今は%cです\n", *find_tmp);
        if (!*find_tmp)
            return 1;
        if (*string_tmp == *find && *string_tmp != *find_tmp)
        {
            printf("\n<再起> %s\n", string_tmp);
            if (match(string_tmp, find))
                return 1;
        }
        printf("string_tmp: %s\n", string_tmp);
        if (*string_tmp == *find_tmp)
        {
            printf("1文字一致: %c と %c\n", *string_tmp, *find_tmp);
            find_tmp++;
        }
        else
        {
            find_tmp = find;
            printf("不一致: ポインタが戻されました / find_tmp先頭: 今は%cです\n", *find_tmp);
        }
        string_tmp++;
    }
    printf("処理終了\n\n");
    return 0;
}

int main(void)
{
    char string[1024] = "aIIsssueOKAKI";
    char find[1024] = "aIIsssueOKAKIa";

    upper(string);
    upper(find);

    if (match(string, find))
        printf("\n\n一致\n");
    else
        printf("\n\n不一致\n");
}
