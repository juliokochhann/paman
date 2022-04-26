#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ASCII_MAX       128
#define CIPHER_KEY      ASCII_MAX+42
#define PASS_LEN        16
#define VERSION         "0.1.0"
#define FILENAME        ".paman_database"

char* cipher(char* buf);
void  put_file(FILE* fp, FILE* stream);
int   read_file(FILE* fp, char** buf);
void  insert(FILE* fp, char* str);
int   find(FILE* fp, char* str);
char* rand_ps(void);

#define print_version()     puts("paman version "VERSION)
#define print_help()        puts("usage: paman [-a <site>:<username>]\n \
            [-c <filename>]\n \
            [-q <search-query>]\n \
            [-e] [-h] [-l] [-r] [-v]")

int main(int argc, char* argv[])
{
    FILE* fp = fopen(FILENAME, "a+");
    assert(fp != NULL);

    int opt;
    opterr = 0;

    while ((opt = getopt(argc, argv, ":a:c:ehlq:rv")) != -1) {
        switch (opt) {
            case 'a':
                insert(fp, optarg);
                break;
            case 'c': ;
                FILE* f = fopen(optarg, "r+");
                assert(f != NULL);
                put_file(f, f);     // filechar = cipher(filechar)
                break;
            case 'e': ;
                FILE* of = fopen(FILENAME"_plain_text", "w");
                put_file(fp, of);
                break;
            case 'h':
                print_help();
                break;
            case 'l':
                put_file(fp, stdout);
                break;
            case 'q':
                find(fp, optarg);
                break;
            case 'r':
                printf("Do you want to permanently delete the database file? (Y/n): ");
                if (getchar() == 'Y') {
                    fclose(fp);

                    assert(!remove(FILENAME));

                    printf("database successfully deleted.\n");
                }
                break;
            case 'v':
                print_version();
                break;
            case ':':
                fprintf (stderr, "option '-%c' requires an argument.\n", optopt);
                print_help();
                exit(EXIT_FAILURE);
                break;
            case '?':
                fprintf (stderr, "unknown option '-%c'.\n", optopt);
                print_help();
                exit(EXIT_FAILURE);
                break;
        }
    }

    exit(EXIT_SUCCESS);
}

char* cipher(char* buf)
{
    char* temp = buf;

    while (*buf) {
        *buf ^= CIPHER_KEY;
        buf++;
    }

    return temp;
}

void put_file(FILE* fp, FILE* stream)
{
    char* file;

    read_file(fp, &file);

    cipher(file);

    fputs(file, stream);
}

int read_file(FILE* fp, char** buf)
{
    char c;
    int  n = 0;             // number of characters read
    int  buf_sz = 1024;

    assert(fp != NULL);

    (*buf) = (char *) malloc(buf_sz * sizeof(char));

    c = fgetc(fp);

    while (!feof(fp)) {
        if (n == buf_sz)
            (*buf) = (char *) realloc((*buf), (buf_sz *= 2) * sizeof(char));

        assert((*buf) != NULL);

        (*buf)[n++] = c;
        c = fgetc(fp);
    }

    (*buf)[n] = '\0';

    rewind(fp);
    
    return n;
}

void insert(FILE* fp, char* str)
{
    char* site   = strtok( str, ":" );
    char* username = strtok(NULL, "\0");
    char* ps = rand_ps();

    if (username == NULL) {
        print_help();
        exit(EXIT_FAILURE);
    }

    char* cred = (char *) malloc(strlen(site) + strlen(username) + PASS_LEN + 4);

    sprintf(cred, "%s %s", site, username);

    if ( find(fp, cred) ) {
        fprintf(stderr, "error: credential must be unique.\n");
        exit(EXIT_FAILURE);
    }

    sprintf(cred, "%s %s %s", site, username, ps);
    puts(cred);
    fprintf(fp, "%s\n", cipher(cred));
}

int find(FILE* fp, char* str)
{
    char* file;
    int   n = 0;        // number of matches found

    read_file(fp, &file);

    file = strtok(file, "\n");          // Split file at newlines

    while(file != NULL) {
        if (strstr(file, str)) {
            puts(file);
            n++;
        }
        
        file = strtok(NULL, "\n");
    }

    return n;
}

char* rand_ps(void)
{
    static char pswd[PASS_LEN+1];

    srand(time(0));     // Use current time as seed for random generator

    int c, i;
    for (i = 0; i < PASS_LEN; i++) {
        while (isgraph( (c = rand() % ASCII_MAX) ) == 0)
        ;
        pswd[i] = c;
    }

    pswd[i] = '\0';

    return pswd;
}