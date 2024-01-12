/* Here we define all the structs we'll be using. Max Besley. April 2022.
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct {
    int id;                    // Unique file id number
    bool is_locked;            // Either true or false
    int locked_by;             // Contains the process-id of who is locking the file
    bool is_requested;         // Either true or false
    int *all_requests;         // Array of process-ids for processes that are requesting the file
    size_t num_requests;       // Buddy variable of `all_requests`
    size_t all_requests_size;  // Records how much memory is allocated to `all_requests`

    bool is_marked;            // Used in the deadlock detection algorithm
} file_t;

typedef struct {
    int id;                    // Unique process id number
    int locked_file;           // Contains file-id of the file being locked by the process
    int requested_file;        // Contains file-id of the file being requested by the process

    bool is_marked;
} proc_t;

#endif
