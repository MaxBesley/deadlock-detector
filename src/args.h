/* Header file for "args.c". Max Besley. April 2022.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>


/* Handles the command line arguments that are stored in `argv[]` in `main()`. */
void read_cl_args(int argc, char **argv, bool *e_flag, bool *f_flag, bool *c_flag, char **filename);
