/* For printing the minimum execution time required for all processes to finish.
   Max Besley. April 2022.
*/

#include "print_exec_time.h"


void
print_exec_time(proc_t **processes, int n_procs) {
    int all_reqd_files[n_procs];
    int i, freq_of_mode, exec_time;

    for (i = 0; i < n_procs; i++) {
        all_reqd_files[i] = processes[i]->requested_file;
    }
    // Note that `all_reqd_files` now contains the third
    // column of the inputted resource allocation file

    // Find the frequency of the most common element in `all_reqd_files`
    freq_of_mode = find_freq_of_mode(all_reqd_files, n_procs);

    // This formula will determine the (minimum) total execution time
    exec_time = 1 + freq_of_mode;

    // Print the result to standard output
    printf("Execution time %d\n", exec_time);

    return;
}


int
find_freq_of_mode(int *A, size_t size) {
    int i, j, current_freq, mode_freq = 0;

    // Iterate over each element of `A`
    for (i = 0; i < size; i++) {
        // Calculate the frequency of `A[i]`
        current_freq = 0;
        for (j = 0; j < size; j++) {
            if (A[j] == A[i]) {
                current_freq += 1;
            }
        }
        // Check whether `mode_freq` should be updated
        if (current_freq > mode_freq) {
            mode_freq = current_freq;
        }
    }

    // Return the frequency of the mode of `A`
    return mode_freq;
}
