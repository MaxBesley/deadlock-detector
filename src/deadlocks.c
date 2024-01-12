/* Implements Tasks 3, 4 and 5. Detects deadlock(s), if they exist, and
   determines the process(es) that need to be terminated.
   Max Besley. April 2022.
*/

#include "deadlocks.h"


void
handle_deadlocks(proc_t **processes, size_t n_procs, file_t **files, size_t n_files) {
    int i, j, k, curr_node, curr_node_index, smallest_id, duplicate_id;
    bool node_is_proc, duplicate_found;
    // An array of ids of the nodes in the graph we have visited
    int visited[n_procs + n_files + 1];
    // If node_types[i] equals one then visited[i] is of type proc_t
    // and if node_types[i] equals zero then visited[i] is a file_t
    int node_types[n_procs + n_files + 1];
    // Buddy variable of `visited[]` and `node_types[]`
    int n_visited;
    // Stores the ids of the processes that need to be terminated
    int to_be_killed[n_procs];
    // Buddy variable of `to_be_killed[]`
    int n_to_be_killed = 0;

    for (i = 0; i < n_procs; i++) {
        // Initialise the empty lists/arrays with junk values
        for (j = 0; j < n_procs + n_files; j++) {
            visited[j] = -1;
            node_types[j] = -1;
        }
        // Set the buddy variable of the two arrays to zero
        n_visited = 0;
        // Mark all the nodes as yet unprocessed
        for (j = 0; j < n_procs; j++) {
            processes[j]->is_marked = false;
        }
        for (j = 0; j < n_files; j++) {
            files[j]->is_marked = false;
        }

        // Initialise the starting node
        curr_node = processes[i]->id;
        node_is_proc = true;

        // Iterate through the (sub)graph until either a deadlock is found or a dead end is reached
        while (true) {
            // Add the current node to the `visited` array
            visited[n_visited] = curr_node;
            node_types[n_visited++] = node_is_proc;

            // Check for duplicate nodes in `visited`
            // If a duplicate exists then store its id
            duplicate_found = false;
            duplicate_id = -1;
            for (j = 0; j < n_visited; j++) {
                for (k = j+1; k < n_visited; k++) {
                    // Check whether two nodes have the same id number and are of the same type
                    if (visited[j] == visited[k] && node_types[j] == node_types[k]) {
                        duplicate_found = true;
                        // Store the id of the duplicated node
                        duplicate_id = visited[j];
                    }
                }
            }

            // If there is a duplicate then a cycle/deadlock has been detected
            if (duplicate_found) {
                // Find the smallest process-id in the `visited` array
                smallest_id = find_smallest_id(visited, node_types, n_visited);
                // 1) Don't store the same process-id in `to_be_killed` multiple times
                // 2) Make sure that `smallest_id` is actually contained in the cycle
                if (!already_in_array(smallest_id, to_be_killed, n_to_be_killed) &&
                     contained_in_cycle(smallest_id, visited, node_types, n_visited, duplicate_id)) {
                    // To break a deadlock, process `smallest_id` should be killed
                    to_be_killed[n_to_be_killed++] = smallest_id;
                }
                // Exit the while loop
                break;
            }

            // We must handle nodes that are files and nodes that are processes differently
            if (node_is_proc) {
                curr_node_index = get_proc_index(processes, n_procs, curr_node);
                // Check whether the current node (a proc_t) has been examined yet
                if (processes[curr_node_index]->is_marked == false) {
                    // Mark the current node as already processed
                    processes[curr_node_index]->is_marked = true;
                    // Follow on to the new current node (which is a file_t)
                    curr_node = processes[curr_node_index]->requested_file;
                    node_is_proc = false;
                    // Now iterate through the while loop again
                    continue;
                } else {
                    // We have reached a dead end, so exit the while loop
                    break;
                }
            // This "else" branch is analogous to the "if" branch above
            } else {
                curr_node_index = get_file_index(files, n_files, curr_node);
                // Observe that if a file is locked then it has an "outgoing edge"
                if (files[curr_node_index]->is_marked == false && files[curr_node_index]->is_locked) {
                    files[curr_node_index]->is_marked = true;
                    curr_node = files[curr_node_index]->locked_by;
                    node_is_proc = true;
                    continue;
                } else {
                    break;
                }
            }
        }
    }

    // Print the appropriate result to standard output
    if (n_to_be_killed > 0) {
        printf("Deadlock detected\n");
        // Sort in ascending order the processes that should be killed
        sort_int_array(to_be_killed, n_to_be_killed);
        // Now print the processes that should be killed
        printf("Terminate ");
        print_int_array(to_be_killed, n_to_be_killed);
    } else {
        // There are no deadlocks and no processes to kill
        printf("No deadlocks\n");
    }

    return;
}


int
get_proc_index(proc_t **processes, size_t n_procs, int pid) {
    // A simple linear search
    for (int i = 0; i < n_procs; i++) {
        if (processes[i]->id == pid) {
            return i;
        }
    }
    return -1;
}


int
get_file_index(file_t **files, size_t n_files, int fid) {
    // A simple linear search
    for (int i = 0; i < n_files; i++) {
        if (files[i]->id == fid) {
            return i;
        }
    }
    return -1;
}


int
find_smallest_id(int visited[], int node_types[], int n_visited) {
    assert(n_visited > 0);
    int i, min_id;

    // Initialise the minimum
    min_id = visited[0];

    for (i = 1; i < n_visited; i++) {
        // Skip nodes that are files
        if (node_types[i] != 1) {
            continue;
        }

        // Check whether or not to update the minimum
        if (visited[i] < min_id) {
            min_id = visited[i];
        }
    }

    return min_id;
}


bool
contained_in_cycle(int smallest_id, int visited[], int node_types[], int n_visited, int duplicate_id) {
    assert(n_visited > 0);
    int i;
    bool inside_cycle;

    // Check if the first node is part of the cycle
    if (visited[0] == duplicate_id) {
        inside_cycle = true;
        // Just in case the first node is the smallest process-id
        if (visited[0] == smallest_id) {
            return true;
        }
    } else {
        inside_cycle = false;
        if (visited[0] == smallest_id) {
            return false;
        }
    }

    for (i = 1; i < n_visited; i++) {
        // Check if we hit a duplicated node
        if (visited[i] == duplicate_id && node_types[i] == 1) {
            // Flip the value of the boolean
            inside_cycle = !inside_cycle;
        }

        // Check if `smallest_id` has been found within the cycle
        if (inside_cycle && visited[i] == smallest_id && node_types[i] == 1) {
            return true;
        }
    }

    // Otherwise, return false
    return false;
}


bool
already_in_array(int key, int A[], int n) {
    // A simple linear search (which also works when `n` is zero)
    for (int i = 0; i < n; i++) {
        if (A[i] == key) {
            return true;
        }
    }
    return false;
}


void
sort_int_array(int A[], int n) {
    // Insertion sort (ascending order)
    for (int i = 1; i < n; i++) {
        for (int j = i-1; j >= 0 && A[j] > A[j+1]; j--) {
            int_swap(&A[j], &A[j+1]);
        }
    }
    return;
}


void
int_swap(int *p1, int *p2) {
    assert(p1 && p2);
    int tmp;
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    return;
}


void
print_int_array(int A[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d", A[i]);
        if (i != n-1) {
            printf(" ");
        }
    }
    printf("\n");
    return;
}
