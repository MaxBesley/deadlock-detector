/* Parses and processes the command line arguments appropriately. Max Besley. April 2022.
*/

#include "args.h"


void
read_cl_args(int argc, char **argv, bool *e_flag, bool *f_flag, bool *c_flag, char **filename) {
    int opt;

    // Before we parse the options, assume the flags are not present
    *e_flag = false;
    *f_flag = false;
    *c_flag = false;

    // Parse the command line arguments
    while ((opt = getopt (argc, argv, "cef:")) != -1) {
        switch (opt) {
            case 'e':
                *e_flag = true;
                break;
            case 'f':
                *f_flag = true;
                *filename = strdup(optarg);
                break;
            case 'c':
                *c_flag = true;
                break;
            case '?':
                printf("Error: command line arguments given incorrectly\n");
                exit(EXIT_FAILURE);
        }
    }

    // The "-f" flag and the filename are mandatory
    assert(*f_flag == true && *filename != NULL);

    return;
}
