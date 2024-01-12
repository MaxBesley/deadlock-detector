/* For printing the number of processes and files present to standard output.
   Max Besley. April 2022.
*/

#include "print_sys_stats.h"


void
print_sys_stats(int n_procs, int n_files) {
    // Easy, since all the work was already done in `read_data()`
    printf("Processes %d\n", n_procs);
    printf("Files %d\n", n_files);
    return;
}
