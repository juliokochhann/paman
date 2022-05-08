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
#define VERSION         "0.1.2"             /*!< Define paman version */
#define DATABASE        ".paman_database"   /*!< Define paman database file name */

char  cipher(char c);

void  translate(char* buf);

void  put_file(FILE* fp, FILE* stream);

int   read_file(FILE* fp, char** buf);

void  insert(FILE* fp, char* str);

int   search(FILE* fp, char* str);

char* rand_ps(void);

/*! 
 *  Prints program version on stdout
*/
#define print_version()     puts("paman version "VERSION)

#define print_help()        puts("usage: paman [-a <domain>:<username>]\n \
            [-c <filename>]\n \
            [-s <search-query>]\n \
            [-d] [-e] [-h] [-l] [-v]\n \
\n \
-a: Insert credential in the form of <domain>:<username>\n \
-c: Convert a file <filename> to cipher/text\n \
-d: Delete the database file\n \
-e: Export the database file to plain text\n \
-h: Print this help\n \
-l: List all credentials\n \
-s: Search for credentials that contain <search-query>\n \
-v: Print program version")
            /*!< Prints program help on stdout */

#endif