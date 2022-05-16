/*! 
 *  \brief     Password manager
 *  \details   Paman is a CLI tool to manage your passwords.
 *  \author    Julio Cesar Kochhann
 *  \version   1.0.0-beta
 *  \date      May 2022
 *  \copyright MIT License
 */
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>
#include "paman.h"

int main(int argc, char* argv[])
{
    char cfg_dir[128] = {0};

    #ifndef DEBUG
    strcpy(cfg_dir, getenv("HOME"));    
    strcat(cfg_dir, CFG_DIR);

    DIR* dr;

    while ( (dr = opendir(cfg_dir)) == NULL )
        if ( mkdir(cfg_dir, S_IRWXU | S_IRWXG | S_IRWXO) == -1 )
            exit(EXIT_FAILURE);

    closedir(dr);
    #endif

    strcat(cfg_dir, DB_FILE);

    FILE* fp = fopen(cfg_dir, "a+");
    assert(fp != NULL);

    int opt;
    opterr = 0;
    while ((opt = getopt(argc, argv, ":a:c:dehls:v")) != -1) {
        switch (opt) {
            case 'a':           /*!< Insert credential at the end of file fp (append) */
                insert_credential(fp, optarg);
                break;
            case 'd':           /*! Delete the database file */
                printf("Do you want to delete " DB_FILE "? (Y/n): ");

                if (getchar() == 'Y') {
                    fclose(fp);
                    assert( ! remove(cfg_dir) );
                    puts(DB_FILE " deleted.");
                }
                break;
            case 'e': ;         /*!< Export database to plain text */
                list_credentials(fp, true);
                break;
            case 'h':
                print_help();
                break;
            case 'l':           /*! List all credentials */
                list_credentials(fp, false);
                break;
            case 's':           /*! Search for credentials that contain \a optarg */
                if ( ! search_credential(fp, optarg) )
                    printf("%s not found." NEWLINE, optarg);
                break;
            case 'v':
                print_version();
                break;
            case ':':
                exit_error("option '-%c' requires an argument." NEWLINE, optopt)
                break;
            case '?':
                exit_error("unknown option '-%c'." NEWLINE, optopt)
                break;
        }
    }

    if (argc == 1)
        print_help();

    exit(EXIT_SUCCESS);
}