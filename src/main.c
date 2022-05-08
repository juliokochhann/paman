/*! 
 *  \brief     Password manager
 *  \details   Paman is a CLI tool to manage your passwords.
 *  \author    Julio Cesar Kochhann
 *  \version   0.1.2
 *  \date      Apr 2022
 *  \copyright MIT License
 */
#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "paman.h"

int main(int argc, char* argv[])
{
    char config_path[128];

    strcpy(config_path, getenv("HOME"));    
    strcat(config_path, "/.config/paman/");

    DIR* dr;

    do {
        dr = opendir(config_path);

        if (dr != NULL)
            break;
            
        if ( mkdir(config_path, S_IRWXU | S_IRWXG | S_IRWXO) == -1 )
            exit(EXIT_FAILURE);
    }while (dr == NULL);

    closedir(dr);

    strcat(config_path, DATABASE);

    FILE* fp = fopen(config_path, "a+");
    assert(fp != NULL);

    int opt;
    opterr = 0;
    while ((opt = getopt(argc, argv, ":a:c:dehls:v")) != -1) {
        switch (opt) {
            case 'a':           /*!< Insert credential at the end of file fp (append) */
                insert(fp, optarg);
                break;
            case 'c': ;         /*! Convert file <optarg> to cipher/text */
                FILE* cf = fopen(optarg, "r+");
                assert(cf != NULL);
                put_file(cf, cf);
                break;
            case 'd':           /*! Delete the database file */
                printf("Do you want to delete " DATABASE "? (Y/n): ");
                if (getchar() == 'Y') {
                    fclose(fp);
                    assert(remove(config_path) == 0);
                    printf(DATABASE " deleted.\n");
                }
                break;
            case 'e': ;         /*!< Export the database file to plain text */
                strcat(config_path, "_plain.txt");
                FILE* of = fopen(config_path, "w");
                assert(of != NULL);
                put_file(fp, of);
                printf("%s\n", config_path);
                break;
            case 'h':
                print_help();
                break;
            case 'l':           /*! List all credentials */
                put_file(fp, stdout);
                break;
            case 's':           /*! Search for credentials that contain \a optarg */
                if (search(fp, optarg) == 0)
                    printf("%s not found.\n", optarg);
                break;
            case 'v':
                print_version();
                break;
            case ':':
                print_help();
                fprintf(stderr, "option '-%c' requires an argument.\n", optopt);
                exit(EXIT_FAILURE);
                break;
            case '?':
                print_help();
                fprintf(stderr, "unknown option '-%c'.\n", optopt);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (argc == 1)
        print_help();

    exit(EXIT_SUCCESS);
}