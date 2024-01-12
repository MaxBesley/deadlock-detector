/* For reading the input data into the chosen data structures (arrays of pointers to structs).
   Max Besley. April 2022.
*/

#include "read_data.h"


void
read_data(char *filename, proc_t ***processes, size_t *n_procs, file_t ***files, size_t *n_files) {
    FILE *fp;
    int pid, fid1, fid2;

    // Open the resource allocation file
    assert((fp = fopen(filename, "r")));

    // Read in the input data one line at a time
    while ((fscanf(fp, "%d %d %d\n", &pid, &fid1, &fid2)) == 3) {
        // Update our two arrays using the data from the line we just read
        *processes = update_processes_array(*processes, n_procs, pid, fid1, fid2);
        *files = update_files_array(*files, n_files, pid, fid1, fid2);
    }

    fclose(fp);

    return;
}


proc_t**
update_processes_array(proc_t **processes, size_t *n_procs, int pid, int fid1, int fid2) {
    static size_t processes_arr_size;

    // Check whether the array has been allocated memory yet
    if (processes == NULL) {
        // Allocate memory for `processes` array
        processes = (proc_t**) malloc(INIT_ARRAY_SIZE * sizeof(proc_t*));
        assert(processes);

        // Keep track of how many array entries have been allocated
        processes_arr_size = INIT_ARRAY_SIZE;

        // Initally, the array has not been assigned anything
        *n_procs = 0;
    }

    // Check whether the array needs resizing (is there at least one empty slot? or not?)
    else if (*n_procs >= processes_arr_size) {
        processes_arr_size *= SCALE_FACTOR;
        processes = (proc_t**) realloc(processes, processes_arr_size * sizeof(proc_t*));
        assert(processes);
    }

    // Now, create and initialise a new proc_t and store it in the `processes` array
    processes[*n_procs] = create_new_proc_t(pid, fid1, fid2);
    *n_procs += 1;

    return processes;
}


proc_t*
create_new_proc_t(int pid, int fid1, int fid2) {
    proc_t *ptr;

    // Allocate memory for a new proc_t
    ptr = (proc_t*) malloc(sizeof(proc_t));
    assert(ptr);

    // Store the line of input data in this proc_t
    ptr->id = pid;
    ptr->locked_file = fid1;
    ptr->requested_file = fid2;

    // Return a pointer to this proc_t
    return ptr;
}


file_t**
update_files_array(file_t **files, size_t *n_files, int pid, int fid1, int fid2) {
    static size_t files_arr_size;

    // For initial allocation
    if (files == NULL) {
        files = (file_t**) malloc(INIT_ARRAY_SIZE * sizeof(file_t*));
        assert(files);
        files_arr_size = INIT_ARRAY_SIZE;
        *n_files = 0;
    }

    // For resizing, check whether or not there're at least N_FILES_PER_LINE empty slots
    if (*n_files + N_FILES_PER_LINE > files_arr_size) {
        files_arr_size *= SCALE_FACTOR;
        files = (file_t**) realloc(files, files_arr_size * sizeof(file_t*));
        assert(files);
    }

    // Insert the first file (the one being locked) into `files`
    insert_locked_file(files, n_files, pid, fid1);
    // Insert the second file (the one being requested) into `files`
    insert_requested_file(files, n_files, pid, fid2);

    return files;
}


void
insert_locked_file(file_t **files, size_t *n_files, int pid, int fid) {
    int i;

    // Do a linear search through `files` array
    for (i = 0; i < *n_files; i++) {
        // Check whether `fid` is already in the array
        if (files[i]->id == fid) {
            // `fid` already appears in `files`, so now update its lock information
            files[i]->is_locked = true;
            files[i]->locked_by = pid;
            return;
        }
    }

    /* If here then `fid` is not in `files`, so create a new file_t and insert */

    // Create a new file_t and store it in the `files` array
    files[*n_files] = create_new_file_t(fid);
    // Store the new file_t's lock information
    files[*n_files]->is_locked = true;
    files[*n_files]->locked_by = pid;
    // Increment the buddy variable
    *n_files += 1;

    return;
}


void
insert_requested_file(file_t **files, size_t *n_files, int pid, int fid) {
    int i;

    for (i = 0; i < *n_files; i++) {
        // Check if `fid` already appears in `files`
        if (files[i]->id == fid) {
            // Check if the `all_requests` array has been allocated space yet
            if (files[i]->all_requests == NULL) {
                // Allocate space for `all_requests` array
                files[i]->all_requests = (int*) malloc(INIT_ARRAY_SIZE * sizeof(int));
                assert(files[i]->all_requests);
                // Add initial values into `files[i]`
                files[i]->all_requests_size = INIT_ARRAY_SIZE;
                files[i]->is_requested = true;
                files[i]->num_requests = 0;
            } else if (files[i]->num_requests == files[i]->all_requests_size) {
                // Resize `all_requests`
                files[i]->all_requests_size *= SCALE_FACTOR;
                files[i]->all_requests = (int*) realloc(files[i]->all_requests,
                                                        files[i]->all_requests_size * sizeof(int));
                assert(files[i]->all_requests);
            }
            // Store the data into `all_requests` and exit the function
            files[i]->all_requests[files[i]->num_requests] = pid;
            files[i]->num_requests += 1;
            return;
        }
    }

    /* If here then `fid` is not in `files`, so create a new file_t and insert */

    files[*n_files] = create_new_file_t(fid);
    files[*n_files]->all_requests = (int*) malloc(INIT_ARRAY_SIZE * sizeof(int));
    assert(files[*n_files]->all_requests);
    files[*n_files]->all_requests_size = INIT_ARRAY_SIZE;
    files[*n_files]->is_requested = true;
    files[*n_files]->num_requests = 0;
    files[*n_files]->all_requests[files[i]->num_requests] = pid;
    files[*n_files]->num_requests += 1;
    *n_files += 1;

    return;
}


file_t*
create_new_file_t(int fid) {
    file_t *ptr;

    ptr = (file_t*) malloc(sizeof(file_t));
    assert(ptr);

    // Assign the file identifier
    ptr->id = fid;
    // Assign other initial values for safety purposes
    ptr->is_locked = false;
    ptr->locked_by = -1;
    ptr->is_requested = false;
    ptr->all_requests = NULL;
    ptr->num_requests = 0;
    ptr->all_requests_size = 0;

    return ptr;
}


void
print_data(proc_t **processes, size_t n_procs, file_t **files, size_t n_files) {
    int i;
    // Useful for testing/debugging purposes
    printf("\t***PRINTING FOR TESTING PURPOSES***\n\n");
    for (i = 0; i < n_procs; i++) {
        printf("\tprocesses[%d] contains %d %d %d\n", i, processes[i]->id,
            processes[i]->locked_file, processes[i]->requested_file);
    }
    printf("\n");
    for (i = 0; i < n_files; i++) {
        printf("\tfiles[%d]\n", i);
        printf("\tid = %d is_locked = %d locked_by = %d\n\tis_requested = %d\n\tnum_requests = %zu\n", files[i]->id,
            files[i]->is_locked, files[i]->locked_by, files[i]->is_requested, files[i]->num_requests);
        printf("\n");
    }
    printf("\t***PRINTING COMPLETE***\n");
    return;
}
