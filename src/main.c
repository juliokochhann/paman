/*! 
 *  \brief     Password manager
 *  \details   Paman is a CLI tool to manage your passwords.
 *  \author    Julio Cesar Kochhann
 *  \version   0.1.0
 *  \date      Apr 2022
 *  \copyright MIT License
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "paman.h"

int main(int argc, char* argv[])
{
    FILE* fp = fopen(FILENAME, "a+");
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
                printf("Do you want to delete " FILENAME "? (Y/n): ");
                if (getchar() == 'Y') {
                    fclose(fp);
                    assert(!remove(FILENAME));
                    printf(FILENAME " deleted.\n");
                }
                break;
            case 'e': ;         /*!< Export the database file to plain text */
                FILE* of = fopen(FILENAME "_plain.txt", "w");
                assert(of != NULL);
                put_file(fp, of);
                break;
            case 'h':
                print_help();
                break;
            case 'l':           /*! List all credentials */
                put_file(fp, stdout);
                break;
            case 's':           /*! Search for credentials that contain \a optarg */
                search(fp, optarg);
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