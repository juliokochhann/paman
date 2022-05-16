/*! 
 *  \file paman.c
 *  \copyright MIT License
 */
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "paman.h"

/*! \fn char cipher(char c)
 *  \brief Encrypt/Decrypt a character.
 *  \param c a character.
 *  \return void.
 */
char cipher(char c) { return c ^= CIPHER_KEY; }

/*! \fn void cipher_string(char* str)
 *  \brief Encrypt/Decrypt a string with \a CIPHER_KEY.
 *  \param str a string.
 *  \return a character pointer.
 */
char* cipher_string(char* str)
{
    for (int i = 0; i < strlen(str); i++)
        str[i] = cipher( str[i] );

    return str;
}

/*! \fn void insert_credential(FILE* fp, char* str)
 *  \brief Insert credential at the end of database (append).
 *  \param fp a FILE pointer.
 *  \param str a string.
 *  \exception null usernam.
 *  \exception credential not unique.
 *  \return void.
 */
void insert_credential(FILE* fp, char* str)
{
    char* name      = strtok( str, ":" );
    char* user_name = strtok(NULL, ":" );

    if (user_name == NULL)
        exit_error("error: null username." NEWLINE)

    size_t len = strlen(name) + strlen(user_name);
    char*  id  = (char *) malloc(len + PASS_LEN + 4);

    sprintf(id, "%s %s", name, user_name);

    if ( search_credential(fp, id) )
        exit_error("error: credential not unique." NEWLINE)

    char* ps = rand_ps();

    sprintf(id, "%s %s %s" NEWLINE, name, user_name, ps);
    cipher_string(id);
    fprintf(fp, "%s", id);
}

/*! \fn void list_credentials(FILE* fp, int export)
 *  \brief Print all credentials in the database to either stdout (export=false) or paman_database.txt (export=true).
 *  \param fp a FILE pointer.
 *  \param export an int.
 *  \return void.
 */
void list_credentials(FILE* fp, int export)
{
    char* file_str;

    read_database(fp, &file_str);

    FILE* of = export ? fopen("paman_database.txt", "a+") : stdout;
    assert(of != NULL);
    
    fputs(file_str, of);
    free(file_str);
}

/*! \fn int read_database(FILE* fp, char** buf)
 *  \brief Read database into buf.
 *  \param fp a FILE pointer.
 *  \param buf a pointer to a character pointer.
 *  \return an integer.
 */
int read_database(FILE* fp, char** buf)
{
    char c;
    int  n = 0;             // number of characters read
    int  buf_sz = 1024;

    assert(fp != NULL);

    (*buf) = (char *) malloc(buf_sz * sizeof(char));

    c = fgetc(fp);

    while ( !feof(fp) ) {
        if (n == buf_sz)
            (*buf) = (char *) realloc((*buf), (buf_sz *= 2) * sizeof(char));

        assert((*buf) != NULL);

        (*buf)[n++] = cipher(c);    // Decrypt character
        c = fgetc(fp);
    }

    (*buf)[n] = '\0';

    rewind(fp);
    
    return n;
}

/*! \fn int search_credential(FILE* fp, char* str)
 *  \brief Search for credentials in the database that contain \a str
 *  \param fp a FILE pointer.
 *  \param str a string.
 *  \return an integer.
 */
int search_credential(FILE* fp, char* str)
{
    char* file_str;
    char* temp;

    read_database(fp, &file_str);
    temp = file_str;

    file_str = strtok(file_str, NEWLINE);   // Split at newlines

    int n = 0;
    while( file_str != NULL ) {
        if (strstr(file_str, str)) {
            puts(file_str);
            n++;
        }

        file_str = strtok(NULL, NEWLINE);
    }

    free(temp);

    return n;
}

/*! \fn char* rand_ps(void)
 *  \brief Generate a string of <PASS_LEN> random characters.
 *  \return a character pointer.
 */
char* rand_ps(void)
{
    static char pswd[PASS_LEN+1];

    srand( time(0) );   // Use current time as seed for random generator

    int c, i;
    for (i = 0; i < PASS_LEN; i++) {
        while (isgraph( (c = rand() % ASCII_MAX) ) == 0)
        ;
        pswd[i] = c;
    }

    pswd[i] = '\0';

    return pswd;
}