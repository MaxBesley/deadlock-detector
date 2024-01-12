/* This is the entry point of the program. Max Besley. April 2022.
*/

#include "structs.h"
#include "args.h"
#include "read_data.h"
#include "print_sys_stats.h"
#include "print_exec_time.h"
#include "deadlocks.h"
#include "free_memory.h"


int
main(int argc, char **argv) {
    bool e_flag, f_flag, c_flag;
    char *filename = NULL;
    proc_t **processes = NULL;
    file_t **files = NULL;
    size_t n_procs;        // Buddy variable of `processes[]`
    size_t n_files;        // Buddy variable of `files[]`

    // Parse the command line arguments in `argv[]`
    read_cl_args(argc, argv, &e_flag, &f_flag, &c_flag, &filename);

    // Read the input data into our chosen data structures
    read_data(filename, &processes, &n_procs, &files, &n_files);

    if (c_flag == true) {
        // Task 6
        // ***NOT IMPLEMENTED OR ATTEMPTED***
    } else {
        // Task 1
        print_sys_stats(n_procs, n_files);

        // Task 2
        if (e_flag == true) {
            print_exec_time(processes, n_procs);
        }

        // Tasks 3, 4 and 5
        handle_deadlocks(processes, n_procs, files, n_files);
    }

    // Free the memory allocated to our data structures
    free_data_structures(processes, n_procs, files, n_files);

    return 0;
}
