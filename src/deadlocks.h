/* Header file for "deadlocks.c". Max Besley. April 2022.
*/

#include "structs.h"


/* Fulfils Tasks 3, 4 and 5. */
void handle_deadlocks(proc_t **processes, size_t n_procs, file_t **files, size_t n_files);

/* Takes a process-id as an argument and returns the index of that process in the `processes` array. */
int get_proc_index(proc_t **processes, size_t n_procs, int pid);

/* Takes a file-id as an argument and returns the index of that file in the `files` array. */
int get_file_index(file_t **files, size_t n_files, int fid);

/* Finds and returns the process-id (a non-negative integer) in `visited[]` that is smallest. */
int find_smallest_id(int visited[], int node_types[], int n_visited);

/* Determines whether `smallest_id` is part of the cycle that exists in the `visited` array. */
bool contained_in_cycle(int smallest_id, int visited[], int node_types[], int n_visited, int duplicate_id);

/* Returns either true or false, depending on whether `key` is already contained in `A[]`. */
bool already_in_array(int key, int A[], int n);

/* Insertion sort of an integer array into ascending order. */
void sort_int_array(int A[], int n);

/* A basic swapping function needed for `sort_int_array()`. */
void int_swap(int *p1, int *p2);

/* Prints out all elements of an integer array. */
void print_int_array(int A[], int n);
