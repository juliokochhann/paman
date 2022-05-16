/*! 
 *  \file paman.h
 *  \copyright MIT License
 */
#ifndef PAMAN_H
#define PAMAN_H

/*! 
 *  Define ASCII range
*/
#define ASCII_MAX       128
#define CIPHER_KEY      ASCII_MAX+42        /*!< Define cipher key */
#define PASS_LEN        16                  /*!< Define Max. password length */
#define VERSION         "1.0.0-beta"        /*!< Define paman version */
#define CFG_DIR         "/.config/paman/"   /*!< Define config. directory */
#define DB_FILE         ".paman_database"   /*!< Define database filename */
#define NEWLINE         "\n"                /*!< Define newline escape sequence */

//#define DEBUG

char  cipher(char c);
char* cipher_string(char* str);
void  insert_credential(FILE* fp, char* str);
void  list_credentials(FILE* fp, int export);
int   read_database(FILE* fp, char** buf);
int   search_credential(FILE* fp, char* str);
char* rand_ps(void);

/*! 
 *  Prints program version on stdout
*/
#define print_version()     puts("paman version "VERSION)

#define print_help()        puts("usage: paman [-a <name>:<username>]\n \
            [-s <search-query>]\n \
            [-d] [-e] [-h] [-l] [-v]\n \
\n \
-a: Insert credential in the form of <name>:<username>\n \
-d: Delete the database file\n \
-e: Export the database file to plain text\n \
-h: Print this help\n \
-l: List all credentials\n \
-s: Search for credentials that contain <search-query>\n \
-v: Print program version")
            /*!< Prints program help on stdout */

#define exit_error(...)     {print_help(); fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE);}
            /*!< Prints error message on stderr and exits */

#endif