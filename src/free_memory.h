/* Header file for "free_memory.c". Max Besley. April 2022.
*/

#include "structs.h"


/* Frees all the memory we have allocated for structs and dynamic arrays. */
void free_data_structures(proc_t **processes, size_t n_procs, file_t **files, size_t n_files);
