/* Header file for "read_data.c". Max Besley. April 2022.
*/

#include "structs.h"

#define N_FILES_PER_LINE 2    // The number of files per line of input
#define INIT_ARRAY_SIZE 1     // Determines how large our malloc'd arrays will be initially
#define SCALE_FACTOR 2        // Determines how much we multiply the dynamic array sizes when reallocing


/* Opens the input file, reads the data within (consisting of three integers) line by line and
   updates `processes` and `files` appropriately, closes the file and then returns to `main()`. */
void read_data(char *filename, proc_t ***processes, size_t *n_procs, file_t ***files, size_t *n_files);

/* Takes a single line of input and uses it to update (and add to) the `processes` array. */
proc_t** update_processes_array(proc_t **processes, size_t *n_procs, int pid, int fid1, int fid2);

/* Allocates memory for a new proc_t, initialises it and then returns a pointer to it. */
proc_t* create_new_proc_t(int pid, int fid1, int fid2);

/* Takes a single line of input and uses it to update (and add to) the `files` array. */
file_t** update_files_array(file_t **files, size_t *n_files, int pid, int fid1, int fid2);

/* Takes the file-id of a locked file and the process-id of the process
   doing the locking and then inserts that information into `files`. */
void insert_locked_file(file_t **files, size_t *n_files, int pid, int fid);

/* Takes the file-id of a requested file and the process-id of the process
   doing the requesting and then inserts that information into `files`. */
void insert_requested_file(file_t **files, size_t *n_files, int pid, int fid);

/* Allocates memory for a new file_t, initialises it appropriately and returns a pointer to it. */
file_t* create_new_file_t(int fid);

/* Prints out the data in our data structures. Useful for debugging. */
void print_data(proc_t **processes, size_t n_procs, file_t **files, size_t n_files);
