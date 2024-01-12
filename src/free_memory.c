/* For freeing dynamically allocated memory.
   Max Besley. April 2022.
*/

#include "free_memory.h"


void
free_data_structures(proc_t **processes, size_t n_procs, file_t **files, size_t n_files) {
    int i;

    // Free `processes` array
    for (i = 0; i < n_procs; i++) {
        free(processes[i]);
    }
    free(processes);

    // Free `files` array
    for (i = 0; i < n_files; i++) {
        free(files[i]->all_requests);
        free(files[i]);
    }
    free(files);

    return;
}
