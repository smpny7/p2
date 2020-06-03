#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_PROFILES 10000
#define MAX_LINE_LEN 1024

struct profile {}; // Stub struct
int    profile_data_nitems = 0;
struct profile profile_data_store[MAX_PROFILES];

/****************************************************************/
/* Function prototypes                                          */
/****************************************************************/
/**
 * Substitute C1 to C2 in string STR.
 * return: number of replacement
 */
int subst(char *str, char c1, char c2);

/**
 * Split STR using delimiter char SEP.
 * RET[] points each split element.
 * return: number of split elements
 */
int split(char *str, char *ret[], char sep, int max);

/**
 * Read at most the number of characters
 * specified by MAX_LINE_LEN and store them in the string LINE including '\n'.
 * LINE must have a room for MAX_LINE_LEN+1 chars (additional 1 is for \0).
 */
int get_line(char *line);
// Note: If you have defined another types of get_line();
//         e.g.) int get_line(FILE *fp, char *line)
//       Please define your get_line() as get_line_f(), and set the macro as folllowing:
//         #define get_line(x) get_line_f(stdin, x)
//         int get_line_f(FILE *fp, char *line) { /* Your function */ }

/**
 * Parse a single-line input from stdin.
 * if the first character of the LINE is '%', it dispatches the input to the command
 * workhorse, otherwise, CSV parser of Profile is invoked.
 */
void parse_line(char *line);

/****************************************************************/
/* Stub functions for the parse_line() function                 */
/****************************************************************/
struct _history {
    char cmd;
    char param[MAX_LINE_LEN];
} g_last_parse_result;

void exec_command(char cmd, char *param)
{
    //printf("EXEC_COMMAND('%c', \"%s\");\n", cmd, param);
    struct _history *h = &g_last_parse_result;
    h->cmd = cmd;
    strcpy(h->param, param);
}

struct profile *new_profile(struct profile *p, char *csv)
{
    //printf("NEW_PROFILE(\"%s\");\n", csv);

    struct _history *h = &g_last_parse_result;
    h->cmd = 0;
    strcpy(h->param, csv);

    return p;
}

void split_test(char *line)
{
    new_profile(NULL, line);
}

/****************************************************************/
/* Test-helper functions                                        */
/****************************************************************/
#define RESULT_PASS 0
#define RESULT_FAIL 1

static char g_repcntrl_buf[2048];
static char* _repcntrl(char* str) {
    char *p1 = str, *p2 = g_repcntrl_buf;
    g_repcntrl_buf[0] = '\0';
    while(p1 && *p1) {
        if(!iscntrl(*p1)) {
            *p2 = *p1;
        } else {
            switch(*p1) {
                case '\n': sprintf(p2, "\\n"); break;
                case '\t': sprintf(p2, "\\t"); break;
                case '\r': sprintf(p2, "\\r"); break;
                default:   sprintf(p2, "\\0x%02x", *p1);
            }
            p2 = p2 + strlen(p2) - 1;
        }
        p1++; p2++;
    }
    *p2 = '\0';
    return g_repcntrl_buf;
}

void check_subst(int expected1, char* expected2,  char *str, char c1, char c2)
{
    int num = -1;
    unsigned int result = RESULT_FAIL;
    if(iscntrl(c1) || iscntrl(c2)) {
        printf("  SUBST(\"%s\", 0x%02x, 0x%02x);\t--> ", _repcntrl(str), c1, c2);
    } else {
        printf("  SUBST(\"%s\", '%c', '%c');\t--> ", _repcntrl(str), c1, c2);
    }

    num = subst(str, c1, c2);

    result = (num != expected1) | (strcmp(str, expected2) != 0);

    if(result == RESULT_PASS) printf("PASS");
    else {
        printf("FAILED");
        printf("\t| [%d] ==> [%d]", expected1, num);
        printf("\t| [%s]", _repcntrl(expected2));
        printf("==> [%s]",_repcntrl(str));
    }
    printf("\n");
}

void check_split(char** expected_ret,   char *str, char sep, int nmax)
{
    unsigned int result = RESULT_PASS;
    int i, n, nmin;
    char *ret[64] = {0};

    int expected_n = 1;
    while(expected_ret[expected_n] != NULL) expected_n++;

    if(iscntrl(sep)) {
        printf("  SPLIT(\"%s\", RET, 0x%02x, %d);\t--> ", _repcntrl(str), sep, nmax);
    } else {
        printf("  SPlIT(\"%s\", RET, '%c', %d);\t--> ", _repcntrl(str), sep, nmax);
    }

    n = split(str, ret, sep, nmax);

    // check return values
    if(expected_n != n || nmax < n) {
        result = RESULT_FAIL;
    } else {
        for (i = 0; i < n; i++) {
            if(strcmp(expected_ret[i], ret[i]) != 0) {
                result = RESULT_FAIL;
                break;
            }
        }
    }

    if(result == RESULT_PASS) printf("PASS");
    else {
        printf("FAILED\n");
        printf("    | RETURN: [%d] ==> [%d]\n", expected_n, n);
        nmin = (n <= expected_n) ? n : expected_n;
        for (i = 0; i < nmin; i++) {
            printf("    | RET[%d]: [%s]\t", i, _repcntrl(expected_ret[i]));
            printf("==> [%s]\n", _repcntrl(ret[i]));
        }
        for (; i < n; i++)          printf("    | RET[%d]: [%s]\t==> [%s]\n", i, "(null)", _repcntrl(ret[i]));
        for (; i < expected_n; i++) printf("    | RET[%d]: [%s] => [%s]\n", i, _repcntrl(expected_ret[i]), "(null)");
        printf("    \\---------- ---------- ---------- ---------- ----------");
    }
    printf("\n");
}

#define RESET_GETLINE_BUF(x) (memset(x, '.', MAX_LINE_LEN), (x)[MAX_LINE_LEN] = '\0')
void check_get_line(int expected1, char* expected2)
{
    static int ncalled = 0;
    unsigned int result = RESULT_PASS;

    int ret;
    char str[MAX_LINE_LEN + 1];

    // Non-zero filled buffer will be help your debugging...
    memset(str, '.', MAX_LINE_LEN);
    str[MAX_LINE_LEN] = '\0';

    printf("  GET_LINE(LINE) [line: %d] --> ", ++ncalled);

    ret = get_line(str);

    if(ret != expected1) {
        result = RESULT_FAIL;
    } else if ((ret > 0) && (strcmp(str, expected2) != 0)){
        result = RESULT_FAIL;
    }

    if(result == RESULT_PASS) printf("PASS");
    else {
        printf("FAILED\n");
        printf("    | RETURN: [%d] ==> [%d]\n", expected1, ret);
        printf("    | LINE  : [%s]", _repcntrl(expected2));
        printf(" ==> [%s]\n", _repcntrl(str));
        printf("    \\---------- ---------- ---------- ---------- ----------");
    }
    printf("\n");
}

void check_parse_line(int expected1, char* expected2,  char* line)
{
    unsigned int result = RESULT_PASS;
    struct _history *h = &g_last_parse_result;

    printf("  PARSE_LINE(\"%s\")\t--> ", _repcntrl(line));

    parse_line(line);

    if(h->cmd != expected1) {
        result = RESULT_FAIL;
    } else if( strcmp(h->param, expected2) != 0) {
        result = RESULT_FAIL;
    }

    if(result == RESULT_PASS) printf("PASS");
    else {
        printf("FAILED\n");
        printf("    | CMD  : [0x%02x] ==> [0x%02x]\n", expected1, h->cmd);
        printf("    | PARAM: [%s]", _repcntrl(expected2));
        printf(" ==> [%s]\n", _repcntrl(h->param));
        printf("    \\---------- ---------- ---------- ---------- ----------");
    }
    printf("\n");
}

/****************************************************************/
/* Test functions                                               */
/****************************************************************/

void test_subst()
{
    struct _test_arguments {
        char input_str[MAX_LINE_LEN + 1];
        char input_c1;
        char input_c2;
        int  expected_return;
        char expected_str[MAX_LINE_LEN + 1];
    } args[] = {
        {"abcdefg",   'd', '*',  1, "abc*efg"},
        {"abcdefg",   'x', '*',  0, "abcdefg"},
        {"abcdefg ",  ' ', '*',  1, "abcdefg*"},
        {"ab,cd,efg", ',', '_',  2, "ab_cd_efg"},
        {"ab,,efg",   ',', '_',  2, "ab__efg"},
        {" LUCK",     ' ', 'P',  1, "PLUCK"},
        {"I have LF!\n",  '\n', '\0',  1, "I have LF!"},
        {"I have LF!\n",  '\n', '?',  1, "I have LF!?"},
    };

    printf("***** TEST SUBST *****\n");
    int i;
    for (i = 0; i < sizeof(args) / sizeof(args[0]); ++i) {
        check_subst(args[i].expected_return, args[i].expected_str,
                    args[i].input_str, args[i].input_c1, args[i].input_c2);
    }
}

void test_split()
{
    struct _test_arguments {
        char  input_line[MAX_LINE_LEN + 1];
        char  input_sep;
        int   input_max;
        char* expected_ret[MAX_LINE_LEN + 1];
    } args[] = {
        {"ab,cd,efg", ',', 3, {"ab", "cd", "efg", NULL}},
        {"ab,cd,efg", ',', 2, {"ab", "cd,efg", NULL}},
        {"ab,cd,efg", ',', 1, {"ab,cd,efg", NULL}},
        {"ab,cd,efg", ',', 4, {"ab", "cd", "efg", NULL}},
        {"ab,,efg",   ',', 3, {"ab", "", "efg", NULL}},
        {"ab,,efg",   ',', 2, {"ab", ",efg", NULL}},
        {"ab cd efg", ' ', 3, {"ab", "cd", "efg", NULL}},
        {"ab cd efg", ' ', 2, {"ab", "cd efg", NULL}},
        {"ab cd efg\n", ' ', 3, {"ab", "cd", "efg\n", NULL}},
        {"ab\ncd\nefg", '\n', 3, {"ab", "cd", "efg", NULL}},
    };

    printf("***** TEST SPLIT *****\n");
    int i;
    for (i = 0; i < sizeof(args) / sizeof(args[0]); ++i) {
        check_split(args[i].expected_ret,
                    args[i].input_line, args[i].input_sep, args[i].input_max);
    }

}

void test_get_line()
{
    char args[][MAX_LINE_LEN + 1] = {
        "Jupiter,Juno,Minerva",
        "Jupiter,Juno,Minerva,Apollo,Venus,Mars",
        "%P 123"
    };

    int i;
    FILE *fp;

    // Genrate a dummy datafile
    fp = fopen(".test1.txt", "wb");
    for (i = 0; i < sizeof(args) / sizeof(args[0]); ++i) {
        fprintf(fp, "%s\n", args[i]);
    }
    fclose(fp);

    // Attach the dummy datafile to STDIN
    fp = freopen(".test1.txt", "r", stdin);

    // Test
    printf("***** TEST GET_LINE *****\n");

    for (i = 0; i < sizeof(args) / sizeof(args[0]); ++i) {
        check_get_line(1, args[i]);
    }
    check_get_line(0, NULL); // Last line

    fclose(fp);
}

void test_parse_line()
{
    struct _test_arguments {
        char input_line[MAX_LINE_LEN + 1];
        int  expected_cmd;
        char expected_param[MAX_LINE_LEN + 1];
    } args[] = {
        {"%C", 'C', ""},
        {"Jupiter,Juno,Minerva", 0, "Jupiter,Juno,Minerva"},
        {"%P 123", 'P', "123"},
        {"Jupiter,Juno,Minerva,Apollo,Venus,Mars", 0, "Jupiter,Juno,Minerva,Apollo,Venus,Mars"},
        {"%X The command like string, but...", 'X', "The command like string, but..."},
        {"", 0, ""}
    };
    int i;

    printf("***** TEST PARSE_LINE *****\n");

    for (i = 0; i < sizeof(args) / sizeof(args[0]); ++i) {
        check_parse_line(args[i].expected_cmd, args[i].expected_param,
                         args[i].input_line);
    }
}
